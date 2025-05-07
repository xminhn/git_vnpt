#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

// Hàm tách tên interface từ dòng đầu tiên
void parser_name(const char *line, char *name_buffer, size_t buffer_size);

// Hàm tách địa chỉ MAC
void parser_mac(const char *line, char *mac_buffer, size_t buffer_size);

// Hàm tách thông tin IPv4 (addr, bcast, mask)
void parser_ipv4(const char *line, char *ipv4_buffer, size_t ipv4_size,
                 char *bcast_buffer, size_t bcast_size,
                 char *mask_buffer, size_t mask_size);

// Hàm tách địa chỉ IPv6
void parser_ipv6(const char *line, char *ipv6_buffer, size_t buffer_size);

// Hàm xác định trạng thái UP/DOWN
void parser_status(const char *line, char *status_buffer, size_t buffer_size);

#endif // PARSER_H