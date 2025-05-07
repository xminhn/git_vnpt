#include "log.h"
#include "defines.h"
#include "iface_list.h"
#include <stdlib.h>
#include <stdbool.h>

static void test_search(const struct Node* head, const char* name_to_find) {
    printf("Tim kiem '%s'...\n", name_to_find);
    struct Node* found_node = list_search(head, name_to_find); // Gọi hàm tìm kiếm
    if (found_node != NULL) {
        // Nếu tìm thấy, in ra một vài thông tin
        printf("Tim thay! Dia chi node: %p, MAC: %s, Status: %s\n",
               found_node,
               found_node->mac ? found_node->mac : "(N/A)",
               found_node->status ? found_node->status : "(N/A)");
    } else {
        printf("Khong tim thay '%s'.\n", name_to_find);
    }
}

int main() {
    LOG_set_level(LOG_LVL_DEBUG);
    LOG(LOG_LVL_DEBUG, "%s, %d: Chuong trinh bat dau.", __func__, __LINE__);

    struct Node* head = NULL;     // Khởi tạo danh sách rỗng
    bool success = false;

    // --- Bước 1: Chạy ifconfig và lưu vào data.txt ---
    printf("Dang chay lenh '%s' va luu vao '%s'...\n", PATH_CMD, PATH_DATA_FILE);
    success = run_command_and_save(PATH_CMD, PATH_DATA_FILE);

    if (!success) {
        printf("Loi khi chay lenh hoac luu file data.txt. Kiem tra log.\n");
        LOG(LOG_LVL_ERROR, "%s, %d: Buoc 1 that bai.", __func__, __LINE__);
        return 1; // Kết thúc với lỗi
    }
    printf("Da luu output vao '%s'.\n", PATH_DATA_FILE);

    // --- Bước 2: Đọc dữ liệu từ data.txt ---
    printf("Doc du lieu tu file: %s\n", PATH_DATA_FILE);
    char* file_buffer = load_data_from_file(PATH_DATA_FILE);

    if (file_buffer != NULL) {
        // --- Bước 3: Phân tích buffer và xây dựng danh sách ---
        printf("Phan tich du lieu...\n");
        parse_data_buffer(&head, file_buffer);

        // Giải phóng buffer ngay sau khi parse xong
        free(file_buffer);
        file_buffer = NULL;

        // --- Bước 4: Lưu danh sách đã phân tích vào my_iface_list.txt ---
        printf("Luu danh sach da phan tich vao '%s'...\n", PATH_MY_IFACE_LIST);
        save_list_to_file(head, PATH_MY_IFACE_LIST);
        printf("Da luu danh sach.\n");

        // --- Thử tìm kiếm (Sử dụng hàm trợ giúp) ---
        printf("\n--- Thu nghiem tim kiem ---\n");
        test_search(head, "ens33");    // Gọi hàm trợ giúp cho "lo"
        test_search(head, "wlan0"); // Gọi hàm trợ giúp cho "wlan0"

        // --- Bước 5: Dọn dẹp danh sách liên kết ---
        printf("\nGoi list_destroy de don dep bo nho...\n");
        list_destroy(&head);
        printf("Da don dep xong.\n");

    } else {
        printf("Khong the doc file du lieu '%s'. Kiem tra log.\n", PATH_DATA_FILE);
         LOG(LOG_LVL_ERROR, "%s, %d: Buoc 2 that bai.", __func__, __LINE__);
         return 1; // Kết thúc với lỗi
    }

    LOG(LOG_LVL_DEBUG, "%s, %d: Chuong trinh ket thuc thanh cong.", __func__, __LINE__);
    return 0;
}