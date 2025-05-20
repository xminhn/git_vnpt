// timer.h
#ifndef TIMER_H // Giữ include guard này để phân biệt với timer.h cơ bản nếu vẫn tồn tại
#define TIMER_H // Nên dùng một tên khác như MULTI_TIMER_H để tránh nhầm lẫn

#include <time.h>     // Cho struct timespec, clock_gettime, clockid_t
#include <stdbool.h>  // Cho kiểu bool (true, false)
#include <stddef.h>   // Cho hằng NULL
#include <pthread.h>  // Cho kiểu pthread_mutex_t (đảm bảo an toàn luồng)

/**
 * @brief Kiểu con trỏ hàm cho các hàm callback của timer.
 * * Scheduler (bộ quản lý timer) sẽ gọi một hàm có kiểu này khi một timer
 * đạt đến chu kỳ của nó.
 * * @param user_data Con trỏ void* chứa dữ liệu tùy chọn được truyền vào khi
 * thêm timer bằng hàm timer_add.
 */
typedef void (*TimerCallback)(void* user_data);

/**
 * @brief Cấu trúc biểu diễn một mục nhập timer đơn lẻ trong danh sách quản lý.
 * * Mỗi đối tượng TimerEntry lưu trữ thông tin và trạng thái của một timer cụ thể
 * đang được theo dõi bởi bộ quản lý.
 */
typedef struct TimerEntry {
    char* name;               /**< Tên duy nhất của timer. Được sử dụng để nhận diện
                                   khi xóa hoặc debug. Bộ nhớ cho tên được cấp phát
                                   và quản lý bởi thư viện. */
    struct timespec next_run_time; /**< Thời điểm tiếp theo (sử dụng đồng hồ đơn điệu
                                   CLOCK_MONOTONIC) mà timer này dự kiến sẽ kích hoạt. */
    double interval_seconds;  /**< Khoảng thời gian lặp lại (chu kỳ) của timer, tính
                                   bằng giây. */
    TimerCallback callback;   /**< Con trỏ tới hàm callback sẽ được gọi khi timer
                                   này kích hoạt. */
    void* user_data;          /**< Con trỏ void* tùy chọn chứa dữ liệu người dùng
                                   để truyền cho hàm callback khi nó được gọi. */

    struct TimerEntry* next;  /**< Con trỏ tới mục nhập TimerEntry tiếp theo trong
                                   danh sách liên kết. */
} TimerEntry;

/**
 * @brief Cấu trúc biểu diễn Bộ Quản lý Timer (Timer Manager).
 * * Đối tượng của cấu trúc này quản lý một tập hợp (danh sách liên kết) các
 * TimerEntry và cung cấp các hàm để thao tác với tập hợp đó.
 */
typedef struct TimerManager { 
    TimerEntry* head; /**< Con trỏ tới mục nhập TimerEntry đầu tiên trong danh sách
                           liên kết các timer đang được quản lý. NULL nếu danh sách rỗng. */
    pthread_mutex_t mutex; /**< Mutex (khóa) để đảm bảo an toàn khi truy cập
                               danh sách timer từ các luồng khác nhau. */
    int size;         /**< Số lượng timer hiện đang được quản lý trong danh sách. */
} TimerManager; 


// Khai báo các hàm công khai của Thư viện Quản lý Timer

/**
 * @brief Tạo và khởi tạo một Bộ Quản lý Timer mới.
 * * Đây là hàm đầu tiên cần được gọi khi sử dụng thư viện. Nó cấp phát bộ nhớ
 * cho bộ quản lý và chuẩn bị nó sẵn sàng.
 * * @return Con trỏ tới đối tượng TimerManager vừa tạo, hoặc NULL nếu có lỗi
 * (ví dụ: không đủ bộ nhớ hoặc lỗi khởi tạo mutex).
 */
TimerManager* timer_manager_create(void); 

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
 * tên là NULL, callback là NULL, interval <= 0, tên bị trùng, lỗi cấp phát bộ nhớ).
 */
bool timer_add(TimerManager* manager, const char* name, double interval_seconds, TimerCallback callback, void* user_data); 

/**
 * @brief Xóa (hủy đăng ký) một timer khỏi bộ quản lý dựa trên tên.
 * * Tìm timer có tên tương ứng trong danh sách, gỡ nó ra và giải phóng bộ nhớ
 * mà nó sử dụng.
 * * @param manager Con trỏ tới Bộ Quản lý Timer.
 * @param name Tên của timer cần xóa.
 * @return true nếu tìm thấy và xóa thành công, false nếu manager là NULL,
 * tên là NULL, hoặc không tìm thấy timer có tên đó.
 */
bool timer_remove(TimerManager* manager, const char* name); 

/**
 * @brief Cập nhật trạng thái của tất cả các timer trong bộ quản lý.
 * * Hàm này là động lực chính của thư viện. Nó cần được gọi lặp đi lặp lại
 * (ví dụ: trong vòng lặp chính của ứng dụng) để kiểm tra xem timer nào
 * đã đến lúc kích hoạt (dựa trên thời điểm hiện tại và next_run_time).
 * Nếu một timer kích hoạt, hàm callback tương ứng của nó sẽ được gọi,
 * và next_run_time của timer sẽ được cập nhật cho chu kỳ tiếp theo.
 * * @param manager Con trỏ tới Bộ Quản lý Timer.
 */
void timer_update(TimerManager* manager); 

/**
 * @brief Lấy số lượng timer hiện đang được quản lý.
 * * @param manager Con trỏ tới Bộ Quản lý Timer.
 * @return Số lượng timer đang được quản lý, hoặc -1 nếu manager là NULL.
 */
int timer_get_size(TimerManager* manager); 

/**
 * @brief Giải phóng toàn bộ bộ nhớ được cấp phát bởi Bộ Quản lý Timer.
 * * Hàm này cần được gọi khi không còn sử dụng bộ quản lý nữa để tránh rò rỉ
 * bộ nhớ. Nó sẽ giải phóng Bộ Quản lý Timer và tất cả các timer con bên trong nó.
 * * @param manager Con trỏ tới Bộ Quản lý Timer cần giải phóng.
 */
void timer_free(TimerManager* manager); 

#endif // TIMER_H