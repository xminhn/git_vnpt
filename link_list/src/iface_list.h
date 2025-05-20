#ifndef IFACE_LIST_H
#define IFACE_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // Cho kiểu bool

// Định nghĩa cấu trúc struct Node một cách bình thường
struct Node {
    char* name;
    char* mac;
    char* ipv4;
    char* bcast;
    char* mask;
    char* ipv6;
    char* status;
    // Bên trong struct, dùng tên đầy đủ 'struct Node*' cho con trỏ next
    struct Node* next;
};

// SAU KHI struct đã được định nghĩa xong, mới dùng typedef
// để tạo tên gọi tắt "Node" cho "struct Node"
typedef struct Node Node; // Giờ đây có thể dùng Node thay cho struct Node

// --- Khai báo các hàm công khai (Public API) của thư viện ---

/**
 * @brief Tạo một Node mới chứa thông tin interface.
 * Hàm này cấp phát động bộ nhớ cho Node và các chuỗi bên trong.
 * @param name Tên interface.
 * @param mac Địa chỉ MAC.
 * @param ipv4 Địa chỉ IPv4.
 * @param bcast Địa chỉ Broadcast IPv4.
 * @param mask Subnet mask IPv4.
 * @param ipv6 Địa chỉ IPv6.
 * @param status Trạng thái (UP/DOWN).
 * @return Con trỏ tới Node mới được tạo, hoặc NULL nếu có lỗi cấp phát bộ nhớ.
 * @note Người gọi không cần giải phóng node này trực tiếp, nên dùng list_destroy.
 */
struct Node* create_node(const char* name, const char* mac,
                         const char* ipv4, const char* bcast, const char* mask,
                         const char* ipv6, const char* status);

/**
 * @brief Thêm một interface mới vào cuối danh sách liên kết.
 * Hàm này sẽ gọi create_node bên trong.
 * @param head_ref Con trỏ trỏ tới con trỏ head của danh sách (để có thể thay đổi head).
 * @param name Tên interface.
 * @param mac Địa chỉ MAC.
 * @param ipv4 Địa chỉ IPv4.
 * @param bcast Địa chỉ Broadcast IPv4.
 * @param mask Subnet mask IPv4.
 * @param ipv6 Địa chỉ IPv6.
 * @param status Trạng thái (UP/DOWN).
 */
void list_add(struct Node** head_ref, const char* name, const char* mac,
              const char* ipv4, const char* bcast, const char* mask,
              const char* ipv6, const char* status);

/**
 * @brief In thông tin của tất cả các node trong danh sách ra màn hình.
 * @param head Con trỏ tới node đầu tiên của danh sách.
 */
void list_print(const struct Node* head);

/**
 * @brief Giải phóng toàn bộ bộ nhớ đã cấp phát cho danh sách liên kết.
 * Bao gồm bộ nhớ của các Node và các chuỗi dữ liệu bên trong.
 * Đặt head_ref về NULL sau khi hoàn tất.
 * @param head_ref Con trỏ trỏ tới con trỏ head của danh sách.
 */
void list_destroy(struct Node** head_ref);

/**
 * @brief Tìm kiếm một node trong danh sách dựa vào tên interface.
 * @param head Con trỏ tới node đầu tiên của danh sách.
 * @param search_name Tên interface cần tìm.
 * @return Con trỏ tới node tìm thấy, hoặc NULL nếu không tìm thấy.
 */
// struct Node* list_search(const struct Node* head, const char* search_name);

/**
 * @brief Xóa một node khỏi danh sách dựa vào tên interface.
 * Hàm sẽ giải phóng bộ nhớ của node bị xóa và các chuỗi bên trong nó.
 * @param head_ref Con trỏ trỏ tới con trỏ head của danh sách.
 * @param delete_name Tên interface cần xóa.
 * @return true nếu xóa thành công, false nếu không tìm thấy node hoặc danh sách rỗng.
 */
bool list_delete(struct Node** head_ref, const char* delete_name);

/**
 * @brief Đọc toàn bộ nội dung của một file vào một buffer được cấp phát động.
 * @param filename Đường dẫn tới file cần đọc.
 * @return Con trỏ tới buffer chứa nội dung file (đã kết thúc bằng null),
 * hoặc NULL nếu có lỗi.
 * @note Người gọi hàm này chịu trách nhiệm giải phóng buffer trả về bằng free().
 */
char* load_data_from_file(const char* filename);

/**
 * @brief Chạy một lệnh hệ thống và lưu output chuẩn của nó vào một file.
 * @param command Lệnh cần thực thi.
 * @param output_filename Đường dẫn tới file để lưu output.
 * @return true nếu lệnh chạy và ghi file thành công, false nếu có lỗi.
 */
bool run_command_and_save(const char* command, const char* output_filename);

/**
 * @brief Phân tích một buffer chứa dữ liệu output của lệnh ifconfig (hoặc tương tự),
 * tạo các Node tương ứng và thêm chúng vào danh sách liên kết.
 * @param head_ref Con trỏ trỏ tới con trỏ head của danh sách.
 * @param data_buffer Buffer chứa dữ liệu cần phân tích.
 */
void parse_data_buffer(struct Node** head_ref, char* data_buffer);

/**
 * @brief Lưu thông tin từ danh sách liên kết vào một file text.
 * @param head Con trỏ tới node đầu tiên của danh sách.
 * @param output_filename Đường dẫn tới file để lưu danh sách.
 */
void save_list_to_file(const struct Node* head, const char* output_filename);

/**
 * @brief Tìm kiếm một node trong danh sách dựa vào tên interface.
 * @param head Con trỏ tới node đầu tiên của danh sách.
 * @param search_name Tên interface cần tìm.
 * @return Con trỏ tới node tìm thấy, hoặc NULL nếu không tìm thấy.
 */
struct Node* list_search(const struct Node* head, const char* search_name);
#endif // IFACE_LIST_H
