#ifndef UCI_HANDLER_H  // Day la include guard, tranh viec include nhieu lan
#define UCI_HANDLER_H

#include <stddef.h>

/**
 * @brief Ghi cấu hình LAN (UCI network.lan) hiện tại ra file.
 * @param output_filename Tên file để ghi cấu hình.
 * @return 0 nếu thành công, -1 nếu có lỗi.
 */
int show_lan_config(const char *output_filename);

/**
 * @brief Đặt IP tĩnh và netmask cho LAN (UCI network.lan).
 * @param ipaddr Địa chỉ IP mới.
 * @param netmask Netmask mới.
 * @return 0 nếu thành công, -1 nếu có lỗi.
 */
int set_lan_ip_config(const char *ipaddr, const char *netmask);

/**
 * @brief Cấu hình DHCP server cho LAN (UCI dhcp.lan).
 * @param start_ip IP bắt đầu của dải DHCP (NULL nếu không đổi).
 * @param limit Số lượng IP tối đa (NULL nếu không đổi).
 * @param leasetime Thời gian thuê IP (NULL nếu không đổi).
 * @param dhcp_enabled_status 1 để bật DHCP, 0 để tắt.
 * @return 0 nếu thành công, -1 nếu có lỗi.
 */
int set_dhcp_lan_config(const char *start_ip, const char *limit, const char *leasetime, int dhcp_enabled_status);

/**
 * @brief Áp dụng các thay đổi mạng bằng cách restart service network và dnsmasq.
 * @return 0 nếu các lệnh restart được gọi thành công (system() trả về 0), 
 * -1 nếu có lỗi khi gọi lệnh.
 */
int apply_network_changes(void);

/**
 * @brief Kiểm tra IP/Netmask hiện tại của br-lan có khớp với giá trị mong đợi.
 * @param expected_ip_addr IP mong đợi.
 * @param expected_netmask Netmask mong đợi.
 * @return 0 nếu khớp, -1 nếu không khớp hoặc có lỗi.
 */
int check_lan_ip_config_validity(const char *expected_ip_addr, const char *expected_netmask);

/**
 * @brief Theo dõi file lease DHCP để xác nhận IP được cấp trong dải mới.
 * In kết quả ra stdout và log.
 * @param configured_start_ip_str IP bắt đầu của dải DHCP đã cấu hình.
 * @param configured_limit Số lượng IP trong dải DHCP đã cấu hình.
 * @param configured_leasetime_seconds Thời gian lease (giây) đã cấu hình.
 */
void check_dhcp_leases_with_validation(const char *configured_start_ip_str,
                                      int configured_limit,
                                      long configured_leasetime_seconds);

#endif // UCI_HANDLER_H