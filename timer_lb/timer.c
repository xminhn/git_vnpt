#include "timer.h" // Include header của thư viện chúng ta (đã đổi tên)

#include <stdlib.h>     // Cho malloc, free, NULL
#include <stdio.h>      // Cho printf, perror, fprintf (cho debug/lỗi)
#include <string.h>     // Cho strdup, strcmp
#include <errno.h>      // Cho errno, strerror // Cần thiết khi gọi hàm hệ thống có thể lỗi
#include <math.h>       // Cho floor (để tách phần nguyên/thập phân)

#include <time.h>       // clock_gettime, struct timespec (cần include ở cả .h và .c)
#include <stdbool.h>    // bool
#include <stddef.h>     // NULL
#include <pthread.h>    // pthread_mutex_init, lock/unlock, destroy


// --- Hàm hỗ trợ nội bộ: Lấy thời điểm hiện tại từ đồng hồ đơn điệu ---
// Trả về true nếu thành công, false nếu lỗi
static bool get_current_monotonic_time(struct timespec* ts) {
    if (ts == NULL) return false;
    // clock_gettime với CLOCK_MONOTONIC lấy thời gian từ lúc boot, không bị thay đổi giờ hệ thống
    if (clock_gettime(CLOCK_MONOTONIC, ts) == -1) {
        // Ghi lỗi nếu không lấy được thời gian (rất hiếm gặp)
        perror("get_current_monotonic_time: clock_gettime failed");
        return false;
    }
    return true;
}


// --- Hàm hỗ trợ nội bộ: Cộng double giây vào struct timespec ---
// Hàm này sẽ tính ts = ts + seconds
static void timespec_add_double(struct timespec* ts, double seconds) {
    if (ts == NULL || seconds <= 0) return; // Chỉ cộng thời gian dương

    double whole_seconds = floor(seconds); // Phần nguyên giây
    double fractional_seconds = seconds - whole_seconds; // Phần thập phân giây

    // Chuyển phần thập phân sang nanoseconds (1 giây = 1e9 nanoseconds)
    long long nanoseconds = (long long)(fractional_seconds * 1e9);

    // Cộng phần nguyên giây vào tv_sec
    ts->tv_sec += (time_t)whole_seconds;

    // Cộng phần nanoseconds vào tv_nsec
    ts->tv_nsec += nanoseconds;

    // Xử lý trường hợp tv_nsec vượt quá 1 tỷ (tràn nanoseconds sang giây)
    if (ts->tv_nsec >= 1e9) {
        ts->tv_sec += ts->tv_nsec / (long)1e9; // Thêm số giây đầy đủ từ nanoseconds
        ts->tv_nsec %= (long)1e9; // Giữ lại phần dư nanoseconds
    }
    // Không cần xử lý tv_nsec âm vì chúng ta chỉ cộng thời gian dương với seconds >= 0
}


// --- Hàm hỗ trợ nội bộ: So sánh hai struct timespec ---
// Trả về <0 nếu ts1 < ts2, 0 nếu ts1 == ts2, >0 nếu ts1 > ts2
static int timespec_compare(const struct timespec* ts1, const struct timespec* ts2) {
    if (ts1->tv_sec != ts2->tv_sec) {
        return (ts1->tv_sec > ts2->tv_sec) ? 1 : -1; // So sánh phần giây
    }
    // Nếu phần giây bằng nhau, so sánh phần nanoseconds
    return (ts1->tv_nsec > ts2->tv_nsec) ? 1 : (ts1->tv_nsec < ts2->tv_nsec ? -1 : 0); // So sánh phần nanoseconds
}

// --- Hàm hỗ trợ nội bộ: Tìm TimerEntry trong danh sách theo tên ---
// Không lock/unlock mutex ở đây, việc này do hàm gọi đảm nhận
static TimerEntry* find_timer_entry_nolock(TimerManager* manager, const char* name) {
    if (manager == NULL || name == NULL) {
        return NULL;
    }

    TimerEntry* current = manager->head;
    while (current != NULL) {
        // So sánh tên (an toàn vì chúng ta dùng strdup để lưu trữ)
        if (current->name != NULL && strcmp(current->name, name) == 0) {
            return current; // Tìm thấy
        }
        current = current->next;
    }
    return NULL; // Không tìm thấy
}

// --- Hàm hỗ trợ nội bộ: Tìm TimerEntry và nút trước đó (để hỗ trợ xóa) ---
// Không lock/unlock mutex ở đây, việc này do hàm gọi đảm nhận
// Trả về con trỏ tới nút TRƯỚC NÓ nếu tìm thấy và nó không phải head
// Trả về NULL nếu không tìm thấy hoặc nút cần tìm là head
static TimerEntry* find_timer_entry_with_previous_nolock(TimerManager* manager, const char* name, TimerEntry** found_entry) {
     if (manager == NULL || name == NULL || found_entry == NULL) {
        *found_entry = NULL;
        return NULL; // prev là NULL
    }

    TimerEntry* current = manager->head;
    TimerEntry* prev = NULL;

    while (current != NULL) {
         if (current->name != NULL && strcmp(current->name, name) == 0) {
            *found_entry = current; // Đã tìm thấy nút
            return prev;            // Trả về nút trước nó
        }
        prev = current;
        current = current->next;
    }

    *found_entry = NULL; // Không tìm thấy
    return NULL;         // prev là NULL
}

/**
 * @brief Tạo và khởi tạo một Bộ Quản lý Timer mới.
 * * Đây là hàm đầu tiên cần được gọi khi sử dụng thư viện. Nó cấp phát bộ nhớ
 * cho bộ quản lý và chuẩn bị nó sẵn sàng.
 * * @return Con trỏ tới đối tượng TimerManager vừa tạo, hoặc NULL nếu có lỗi
 * (ví dụ: không đủ bộ nhớ hoặc lỗi khởi tạo mutex).
 */
TimerManager* timer_manager_create(void) { 
    // 1. Cấp phát bộ nhớ cho cấu trúc TimerManager
    TimerManager* manager = (TimerManager*)malloc(sizeof(TimerManager));

    // 2. Kiểm tra lỗi cấp phát bộ nhớ
    if (manager == NULL) {
        perror("timer_manager_create: Failed to allocate memory for TimerManager"); // <--- Đổi tên hàm trong log
        return NULL;
    }

    // 3. Khởi tạo các thành viên của cấu trúc TimerManager vừa tạo
    manager->head = NULL; // Danh sách các timer con ban đầu rỗng
    manager->size = 0;    // <--- KHỞI TẠO SIZE VỀ 0

    // Khởi tạo mutex
    int rc = pthread_mutex_init(&manager->mutex, NULL);
    if (rc != 0) {
        errno = rc;
        perror("timer_manager_create: Failed to initialize mutex"); // <--- Đổi tên hàm trong log
        free(manager);
        return NULL;
    }

    // 4. Trả về con trỏ tới Bộ Quản lý Timer vừa tạo
    return manager;
}

/**
 * @brief Thêm (đăng ký) một timer mới vào bộ quản lý.
 * * Tạo một mục nhập timer mới, lưu thông tin (tên, chu kỳ, callback, dữ liệu)
 * và thêm nó vào danh sách các timer được quản lý. Timer sẽ được bắt đầu
 * đếm ngay sau khi thêm thành công.
 * * @param manager Con trỏ tới Bộ Quản lý Timer (được tạo bởi timer_manager_create).
 * @param name Chuỗi ký tự là tên duy nhất cho timer này. Tên sẽ được sao chép
 * bởi thư viện.
 * @param interval_seconds Chu kỳ (khoảng thời gian lặp lại) của timer, tính bằng giây.
 * Phải lớn hơn 0.
 * @param callback Hàm callback sẽ được gọi khi timer này kích hoạt. Không được là NULL.
 * @param user_data Dữ liệu tùy chọn (con trỏ void*) để truyền cho hàm callback. Có thể là NULL.
 * @return true nếu timer được thêm thành công, false nếu có lỗi (ví dụ: manager là NULL,
 * tên là NULL, callback là NULL, interval <= 0, tên bị trùng, lỗi cấp phát bộ nhớ, lỗi lấy thời gian).
 */
bool timer_add(TimerManager* manager, const char* name, double interval_seconds, TimerCallback callback, void* user_data) {
    // 1. Kiểm tra các tham số đầu vào cơ bản
    if (manager == NULL || name == NULL || callback == NULL || interval_seconds <= 0) {
        fprintf(stderr, "timer_add Error: Invalid input parameters (manager, name, callback, or interval <= 0).\n");
        return false;
    }

    // 2. Khóa mutex để bảo vệ danh sách
    pthread_mutex_lock(&manager->mutex);

    // 3. Kiểm tra tên timer đã tồn tại chưa (sử dụng hàm hỗ trợ _nolock sau khi khóa)
    if (find_timer_entry_nolock(manager, name) != NULL) {
        fprintf(stderr, "timer_add Error: Timer with name '%s' already exists.\n", name);
        // Mở khóa mutex trước khi thoát hàm
        pthread_mutex_unlock(&manager->mutex);
        return false; // Tên trùng, thoát
    }

    // 4. Cấp phát bộ nhớ cho mục nhập TimerEntry mới
    TimerEntry* newEntry = (TimerEntry*)malloc(sizeof(TimerEntry));

    // Kiểm tra lỗi cấp phát TimerEntry
    if (newEntry == NULL) {
        perror("timer_add Error: Failed to allocate memory for TimerEntry");
        pthread_mutex_unlock(&manager->mutex); // Mở khóa mutex
        return false; // Lỗi cấp phát, thoát
    }

    // 5. Cấp phát bộ nhớ và sao chép tên timer (strdup)
    newEntry->name = strdup(name);

    // Kiểm tra lỗi cấp phát tên
    if (newEntry->name == NULL) {
        perror("timer_add Error: Failed to allocate memory for timer name copy");
        free(newEntry); // Giải phóng TimerEntry đã cấp phát
        pthread_mutex_unlock(&manager->mutex); // Mở khóa mutex
        return false; // Lỗi cấp phát tên, thoát
    }

    // 6. Tính thời điểm chạy lần đầu tiên (Initial next_run_time)
    // Lấy thời điểm hiện tại từ đồng hồ đơn điệu
    if (!get_current_monotonic_time(&newEntry->next_run_time)) {
        // get_current_monotonic_time đã in lỗi, chỉ cần dọn dẹp ở đây
        fprintf(stderr, "timer_add Error: Failed to get current monotonic time for initial run time.\n");
        free(newEntry->name); // Giải phóng tên đã sao chép
        free(newEntry);       // Giải phóng TimerEntry
        pthread_mutex_unlock(&manager->mutex); // Mở khóa mutex
        return false; // Lỗi lấy thời gian, thoát
    }
    // Cộng chu kỳ vào thời điểm hiện tại để có thời điểm chạy lần đầu
    timespec_add_double(&newEntry->next_run_time, interval_seconds);

    // 7. Thiết lập các thành viên còn lại của TimerEntry
    newEntry->interval_seconds = interval_seconds;
    newEntry->callback = callback;
    newEntry->user_data = user_data;

    // 8. Thêm mục nhập mới vào đầu danh sách liên kết của bộ quản lý
    newEntry->next = manager->head;
    manager->head = newEntry;

    // 9. Cập nhật số lượng timer
    manager->size++;

    // 10. Mở khóa mutex sau khi hoàn tất thao tác với danh sách
    pthread_mutex_unlock(&manager->mutex);

    // 11. Báo hiệu thêm thành công
    return true;
}

/**
 * @brief Xóa (hủy đăng ký) một timer khỏi bộ quản lý dựa trên tên.
 * * Tìm timer có tên tương ứng trong danh sách, gỡ nó ra và giải phóng bộ nhớ
 * mà nó sử dụng.
 * * @param manager Con trỏ tới Bộ Quản lý Timer.
 * @param name Tên của timer cần xóa.
 * @return true nếu tìm thấy và xóa thành công, false nếu manager là NULL,
 * tên là NULL, hoặc không tìm thấy timer có tên đó.
 */
bool timer_remove(TimerManager* manager, const char* name) {
    // 1. Kiểm tra các tham số đầu vào cơ bản
    if (manager == NULL || name == NULL) {
        fprintf(stderr, "timer_remove Error: Invalid input parameters (manager or name is NULL).\n");
        return false;
    }

    // 2. Khóa mutex để bảo vệ danh sách
    pthread_mutex_lock(&manager->mutex);

    // 3. Tìm TimerEntry cần xóa và nút đứng trước nó
    TimerEntry* found_entry = NULL; // Con trỏ sẽ trỏ tới nút tìm thấy
    // Gọi hàm hỗ trợ để tìm nút cần xóa (found_entry) và nút trước nó (prev_entry)
    TimerEntry* prev_entry = find_timer_entry_with_previous_nolock(manager, name, &found_entry);

    // 4. Kiểm tra xem timer có được tìm thấy không
    if (found_entry == NULL) {
        // Không tìm thấy timer có tên này
        // fprintf(stderr, "timer_remove Warning: Timer with name '%s' not found.\n", name); // Debug log
        pthread_mutex_unlock(&manager->mutex); // Mở khóa mutex trước khi thoát
        return false; // Báo hiệu xóa thất bại vì không tìm thấy
    }

    // 5. Xóa nút khỏi danh sách liên kết
    // Dùng kết quả tìm kiếm để gỡ nút found_entry ra khỏi chuỗi
    if (prev_entry == NULL) {
        // Trường hợp 1: Nút cần xóa là nút đầu tiên (head)
        // Cập nhật head để nó trỏ đến nút kế tiếp
        manager->head = found_entry->next;
    } else {
        // Trường hợp 2: Nút cần xóa nằm ở giữa hoặc cuối danh sách
        // Cập nhật con trỏ 'next' của nút TRƯỚC nó để bỏ qua nút hiện tại
        prev_entry->next = found_entry->next;
    }

    // 6. Cập nhật số lượng timer
    manager->size--; // Giảm biến đếm size xuống 1

    // 7. Giải phóng bộ nhớ của các thành phần của TimerEntry đã xóa
    // Giải phóng tên đã sao chép bởi strdup
    free(found_entry->name);
    // Giải phóng chính cấu trúc TimerEntry
    free(found_entry);

    // printf("Timer '%s' removed. Total timers: %d\n", name, manager->size); // Debug log

    // 8. Mở khóa mutex sau khi hoàn tất thao tác với danh sách
    pthread_mutex_unlock(&manager->mutex);

    // 9. Báo hiệu xóa thành công
    return true;
}

/**
 * @brief Cập nhật trạng thái của tất cả các timer trong bộ quản lý.
 * * Hàm này là động lực chính của thư viện. Nó cần được gọi lặp đi lặp lại
 * (ví dụ: trong vòng lặp chính của ứng dụng) để kiểm tra xem timer nào
 * đã đến lúc kích hoạt (dựa trên thời điểm hiện tại và next_run_time).
 * Nếu một timer kích hoạt, hàm callback tương ứng của nó sẽ được gọi,
 * và next_run_time của timer sẽ được cập nhật cho chu kỳ tiếp theo.
 * * @param manager Con trỏ tới Bộ Quản lý Timer.
 */
void timer_update(TimerManager* manager) { // <--- Triển khai hàm timer_update
    // 1. Kiểm tra tham số đầu vào cơ bản
    if (manager == NULL) {
        // Không in lỗi ra stderr liên tục, chỉ return nếu manager là NULL
        return;
    }

    // 2. Lấy thời điểm hiện tại từ đồng hồ đơn điệu
    struct timespec current_time;
    // Nếu không lấy được thời gian hiện tại, không thể kiểm tra timer nào, chỉ return
    // Hàm get_current_monotonic_time đã in lỗi nếu cần.
    if (!get_current_monotonic_time(&current_time)) {
        return;
    }

    // 3. Khóa mutex để bảo vệ danh sách khi duyệt
    pthread_mutex_lock(&manager->mutex);

    // 4. Duyệt qua tất cả các TimerEntry trong danh sách
    TimerEntry* current = manager->head;
    while (current != NULL) {
        // 5. So sánh thời điểm hiện tại với thời điểm chạy tiếp theo của timer
        // timespec_compare trả về >= 0 nếu current_time >= current->next_run_time
        if (timespec_compare(&current_time, &current->next_run_time) >= 0) {
            // --- Timer này ĐẾN LÚC kích hoạt! ---

            // 6. Gọi hàm callback tương ứng
            // Đảm bảo callback không phải là NULL (nên được kiểm tra trong add, nhưng kiểm tra lại an toàn hơn)
            if (current->callback != NULL) {
                current->callback(current->user_data);
            }
            // else { /* Có thể log lỗi nếu callback là NULL */ }

            // 7. Tính toán thời điểm chạy LẦN TIẾP THEO
            // Lặp đi lặp lại cộng interval cho đến khi next_run_time VƯỢT QUA thời điểm hiện tại
            // Điều này xử lý trường hợp hàm update gọi BỊ TRỄ, khiến timer lỡ nhiều chu kỳ
            while (timespec_compare(&current_time, &current->next_run_time) >= 0) {
                 timespec_add_double(&current->next_run_time, current->interval_seconds);
            }
            // Ví dụ: interval 5s, due at 10s. Update gọi ở 16s (lỡ).
            // current_time = 16s, next_run_time = 10s. 16 >= 10 -> trigger.
            // Lần 1 trong while: timespec_add_double(&next_run_time, 5) -> next_run_time = 15s.
            // Lần 2 trong while: timespec_compare(16s, 15s) >= 0 -> true. timespec_add_double(&next_run_time, 5) -> next_run_time = 20s.
            // Lần 3 trong while: timespec_compare(16s, 20s) >= 0 -> false. Thoát while.
            // Next run time được đặt đúng là 20s, không phải 16s + 5s = 21s.

        }
        // Else: Timer này chưa đến lúc kích hoạt, không làm gì cả

        // 8. Di chuyển tới TimerEntry tiếp theo trong danh sách
        current = current->next;
    }

    // 9. Mở khóa mutex sau khi duyệt xong danh sách
    pthread_mutex_unlock(&manager->mutex);

    // Không có giá trị trả về (void)
}

/**
 * @brief Lấy số lượng timer hiện đang được quản lý.
 * * @param manager Con trỏ tới Bộ Quản lý Timer.
 * @return Số lượng timer đang được quản lý, hoặc -1 nếu manager là NULL.
 */
int timer_get_size(TimerManager* manager) { // <--- Triển khai hàm timer_get_size
    // 1. Kiểm tra tham số đầu vào cơ bản
    if (manager == NULL) {
        // fprintf(stderr, "timer_get_size Error: Manager is NULL.\n"); // Có thể thêm log lỗi nếu cần
        return -1; // Trả về -1 nếu manager không hợp lệ
    }

    // 2. Khóa mutex để đọc giá trị size một cách an toàn
    pthread_mutex_lock(&manager->mutex);

    // 3. Lấy giá trị của biến size
    int current_size = manager->size;

    // 4. Mở khóa mutex sau khi đọc xong
    pthread_mutex_unlock(&manager->mutex);

    // 5. Trả về giá trị size đã đọc
    return current_size;
}

/**
 * @brief Giải phóng toàn bộ bộ nhớ được cấp phát bởi Bộ Quản lý Timer.
 * * Hàm này cần được gọi khi không còn sử dụng bộ quản lý nữa để tránh rò rỉ
 * bộ nhớ. Nó sẽ giải phóng Bộ Quản lý Timer và tất cả các timer con bên trong nó.
 * * @param manager Con trỏ tới Bộ Quản lý Timer cần giải phóng.
 */
void timer_free(TimerManager* manager) { // <--- Triển khai hàm timer_free
    // 1. Kiểm tra tham số đầu vào cơ bản
    if (manager == NULL) {
        // Không làm gì nếu manager là NULL
        return;
    }

    // 2. Khóa mutex để đảm bảo không có luồng nào khác đang truy cập danh sách
    // trong khi chúng ta giải phóng nó.
    pthread_mutex_lock(&manager->mutex);

    // 3. Duyệt qua danh sách liên kết và giải phóng từng TimerEntry một
    TimerEntry* current = manager->head;
    TimerEntry* next_entry; // Con trỏ tạm để lưu địa chỉ nút kế tiếp

    while (current != NULL) {
        // 4. Lưu con trỏ tới nút kế tiếp TRƯỚC khi giải phóng nút hiện tại
        next_entry = current->next;

        // 5. Giải phóng bộ nhớ của các thành phần bên trong TimerEntry hiện tại
        // Giải phóng tên (được cấp phát bởi strdup trong timer_add)
        free(current->name);
        // Giải phóng chính cấu trúc TimerEntry
        free(current);

        // 6. Di chuyển tới TimerEntry kế tiếp bằng con trỏ đã lưu
        current = next_entry;
    }

    // 7. Danh sách liên kết đã rỗng và các nút đã được giải phóng.
    // Bây giờ, hủy bỏ mutex.
    pthread_mutex_unlock(&manager->mutex); // Mở khóa trước khi destroy (yêu cầu của pthread)
    pthread_mutex_destroy(&manager->mutex);


    // 8. Cuối cùng, giải phóng bộ nhớ của cấu trúc TimerManager
    free(manager);

    // Sau lệnh free(manager), con trỏ 'manager' không còn hợp lệ nữa.
    // size không cần reset vì toàn bộ bộ nhớ đã được giải phóng.
    // head cũng không cần đặt lại NULL vì toàn bộ cấu trúc manager đã được giải phóng.
}
