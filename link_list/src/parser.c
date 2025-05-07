#include "parser.h" // Header của chính nó
#include <stdio.h>  // Cho NULL (mặc dù stddef.h cũng có)
#include <string.h> // Cho strstr, strtok, strncpy, strlen, strcmp
#include <ctype.h>  // Cho isspace (có thể hữu ích)

// Hàm tách tên interface từ dòng đầu tiên
// Ví dụ dòng: "eth0      Link encap:Ethernet  HWaddr ..."
void parser_name(const char *line, char *name_buffer, size_t buffer_size) {
    if (buffer_size == 0) return; // Không có chỗ lưu

    // Tạo bản sao để strtok không làm hỏng chuỗi gốc
    char line_copy[256]; // Buffer tạm đủ lớn cho một dòng
    strncpy(line_copy, line, sizeof(line_copy) - 1);
    line_copy[sizeof(line_copy) - 1] = '\0';

    // Lấy token đầu tiên, phân tách bằng khoảng trắng
    char *token = strtok(line_copy, " \t"); // Dùng cả space và tab
    if (token) {
        strncpy(name_buffer, token, buffer_size - 1);
        name_buffer[buffer_size - 1] = '\0'; // Đảm bảo kết thúc null
    } else {
        name_buffer[0] = '\0'; // Trả về chuỗi rỗng nếu không tìm thấy
    }
}

// Hàm tách địa chỉ MAC
// Ví dụ dòng: "... HWaddr AA:BB:CC:11:22:33 ..."
void parser_mac(const char *line, char *mac_buffer, size_t buffer_size) {
    if (buffer_size == 0) return;

    const char *hwaddr_ptr = strstr(line, "HWaddr");
    if (hwaddr_ptr) {
        hwaddr_ptr += strlen("HWaddr"); // Di chuyển con trỏ qua chữ "HWaddr"

        // Bỏ qua các khoảng trắng phía sau "HWaddr"
        while (*hwaddr_ptr != '\0' && isspace((unsigned char)*hwaddr_ptr)) {
            hwaddr_ptr++;
        }

        // Tìm vị trí khoảng trắng tiếp theo sau địa chỉ MAC
        const char *end_ptr = hwaddr_ptr;
        while (*end_ptr != '\0' && !isspace((unsigned char)*end_ptr)) {
            end_ptr++;
        }

        // Tính độ dài địa chỉ MAC
        size_t mac_len = end_ptr - hwaddr_ptr;

        // Sao chép an toàn
        if (mac_len > 0) {
             // Chỉ copy số lượng ký tự tối đa cho phép trừ đi 1 cho null terminator
            size_t copy_len = (mac_len < buffer_size - 1) ? mac_len : (buffer_size - 1);
            strncpy(mac_buffer, hwaddr_ptr, copy_len);
            mac_buffer[copy_len] = '\0'; // Đảm bảo kết thúc null
        } else {
             mac_buffer[0] = '\0'; // Không tìm thấy MAC hợp lệ
        }

    } else {
        mac_buffer[0] = '\0'; // Không tìm thấy "HWaddr"
    }
}


// Hàm tách thông tin IPv4 (addr, bcast, mask)
// Ví dụ dòng: "inet addr:192.168.1.10  Bcast:192.168.1.255  Mask:255.255.255.0"
void parser_ipv4(const char *line, char *ipv4_buffer, size_t ipv4_size,
                   char *bcast_buffer, size_t bcast_size,
                   char *mask_buffer, size_t mask_size)
{
    // Khởi tạo buffer về rỗng
    if (ipv4_size > 0) ipv4_buffer[0] = '\0';
    if (bcast_size > 0) bcast_buffer[0] = '\0';
    if (mask_size > 0) mask_buffer[0] = '\0';

    const char *inet_ptr = strstr(line, "inet addr:");
    if (inet_ptr) {
        inet_ptr += strlen("inet addr:"); // Bỏ qua "inet addr:"

        // Tạo bản sao để dùng strtok
        char line_copy[256];
        strncpy(line_copy, inet_ptr, sizeof(line_copy) - 1);
        line_copy[sizeof(line_copy) - 1] = '\0';

        char *token = strtok(line_copy, " \t\n"); // Lấy phần đầu tiên (là địa chỉ IP)
        if (token && ipv4_size > 0) {
            strncpy(ipv4_buffer, token, ipv4_size - 1);
            ipv4_buffer[ipv4_size - 1] = '\0';
        }

        // Tiếp tục lấy các token còn lại trên dòng
        while ((token = strtok(NULL, " \t\n")) != NULL) {
            if (strncmp(token, "Bcast:", 6) == 0 && bcast_size > 0) {
                strncpy(bcast_buffer, token + 6, bcast_size - 1);
                bcast_buffer[bcast_size - 1] = '\0';
            } else if (strncmp(token, "Mask:", 5) == 0 && mask_size > 0) {
                strncpy(mask_buffer, token + 5, mask_size - 1);
                mask_buffer[mask_size - 1] = '\0';
            }
        }
    }
}


// Hàm tách địa chỉ IPv6
// Ví dụ dòng: "inet6 addr: fe80::1/64 Scope:Link"
void parser_ipv6(const char *line, char *ipv6_buffer, size_t buffer_size) {
     if (buffer_size == 0) return;

    const char *inet6_ptr = strstr(line, "inet6 addr:");
    if (inet6_ptr) {
        inet6_ptr += strlen("inet6 addr:"); // Bỏ qua "inet6 addr:"

         // Bỏ qua các khoảng trắng phía sau
        while (*inet6_ptr != '\0' && isspace((unsigned char)*inet6_ptr)) {
            inet6_ptr++;
        }

        // Tìm vị trí khoảng trắng hoặc '/' tiếp theo
        const char *end_ptr = inet6_ptr;
         // Tìm đến cuối chuỗi hoặc gặp khoảng trắng
        while (*end_ptr != '\0' && !isspace((unsigned char)*end_ptr)) {
            end_ptr++;
        }

        size_t ipv6_len = end_ptr - inet6_ptr;

        if (ipv6_len > 0) {
             size_t copy_len = (ipv6_len < buffer_size - 1) ? ipv6_len : (buffer_size - 1);
            strncpy(ipv6_buffer, inet6_ptr, copy_len);
            ipv6_buffer[copy_len] = '\0';
        } else {
             ipv6_buffer[0] = '\0';
        }
    } else {
        ipv6_buffer[0] = '\0';
    }
}

// Hàm xác định trạng thái UP/DOWN
// Dựa vào sự xuất hiện của chữ "UP" trên dòng chứa "MTU:"
// Ví dụ dòng: "UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1"
void parser_status(const char *line, char *status_buffer, size_t buffer_size) {
    if (buffer_size == 0) return;

    // Mặc định là DOWN trừ khi tìm thấy "UP" trên dòng có "MTU:"
    const char* default_status = "DOWN";
    if (strstr(line, "MTU:") != NULL && strstr(line, "UP") != NULL) {
        default_status = "UP";
    }

    strncpy(status_buffer, default_status, buffer_size - 1);
    status_buffer[buffer_size - 1] = '\0';
}