// Trong main_app.c
#include "log.h"
#include "uci_handler.h"
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main(void) {
    init_logger();
    set_log_file("log.txt");
    set_log_level(LOG_LVL_DEBUG);

    log_message(LOG_LVL_DEBUG, "main_app: Logger da duoc khoi tao va cau hinh.");
    printf("main_app: Chuong trinh bat dau. Kiem tra file log.txt va file output .txt.\n");

    const char *lan = "lan.txt";
    int result_txt = show_lan_config(lan); // Trả về 0 hoặc -1

    if (result_txt == 0) {
        log_message(LOG_LVL_DEBUG, "main_app: Da ghi cau hinh LAN ra %s thanh cong.", lan);
        printf("main_app: Da ghi cau hinh LAN ra %s thanh cong.\n", lan);
    } else {
        // result_txt sẽ là -1 nếu có lỗi
        log_message(LOG_LVL_ERROR, "main_app: Gap loi khi ghi cau hinh LAN ra %s. Ma loi: %d", lan, result_txt);
        printf("main_app: Gap loi khi ghi cau hinh LAN ra %s. Ma loi: %d\n", lan, result_txt);
    }

    const char *new_ip = "192.168.88.1";
    const char *new_mask = "255.255.255.0";
    const char *new_dhcp_start = "192.168.88.100";
    const char *new_dhcp_limit = "86";
    const char *new_dhcp_leasetime = "43200";
    int dhcp_enable_state = 1;
    int final_result = 0; // Khởi tạo là thành công

    // Buoc 1: Dat cau hinh IP
    // set_lan_ip_config trả về 0 (thành công) hoặc -1 (lỗi)
    int ip_set_result = set_lan_ip_config(new_ip, new_mask);
    if (ip_set_result != 0) {
        log_message(LOG_LVL_ERROR, "main_app: Dat IP/Netmask LAN vao file config THAT BAI! Ma loi: %d", ip_set_result);
        final_result = -1;
    } else {
        log_message(LOG_LVL_DEBUG, "main_app: Dat IP/Netmask LAN vao file config thanh cong!");
    }

    // Buoc 2: Dat cau hinh DHCP (chi thuc hien neu buoc 1 thanh cong)
    if (final_result == 0) {
        // set_dhcp_lan_config trả về 0 (thành công) hoặc -1 (lỗi)
        int dhcp_set_result = set_dhcp_lan_config(new_dhcp_start, new_dhcp_limit, new_dhcp_leasetime, dhcp_enable_state);
        if (dhcp_set_result != 0) {
            log_message(LOG_LVL_ERROR, "main_app: Dat cau hinh DHCP LAN vao file config THAT BAI! Ma loi: %d", dhcp_set_result);
            final_result = -1; // Gán lỗi chung
        } else {
            log_message(LOG_LVL_DEBUG, "main_app: Dat cau hinh DHCP LAN vao file config thanh cong!");
        }
    }

    // Buoc 3: Ap dung tat ca cac thay doi (chi thuc hien neu cac buoc truoc khong loi)
    if (final_result == 0) {
        log_message(LOG_LVL_DEBUG, "main_app: Dang ap dung cac thay doi cau hinh...");
        // apply_network_changes trả về 0 (thành công) hoặc -1 (lỗi)
        int apply_result = apply_network_changes();
        if (apply_result != 0) {
            log_message(LOG_LVL_ERROR, "main_app: Ap dung thay doi cau hinh THAT BAI! Ma loi: %d", apply_result);
            printf("main_app: Ap dung thay doi cau hinh THAT BAI! Ma loi: %d\n", apply_result);
            final_result = -1; // Gán lỗi chung
        } else {
            log_message(LOG_LVL_DEBUG, "main_app: Ap dung thay doi cau hinh thanh cong!");
            printf("main_app: Ap dung thay doi cau hinh thanh cong %s\n", new_ip);
        }
    }

    // Thông báo kết quả chung của quá trình cấu hình
    if (final_result == -1) {
        log_message(LOG_LVL_ERROR, "main_app: Mot hoac nhieu buoc cau hinh (IP, DHCP, Apply) da that bai.");
        printf("main_app: Mot hoac nhieu buoc cau hinh (IP, DHCP, Apply) da that bai. Kiem tra log de biet chi tiet.\n");
    }

    log_message(LOG_LVL_DEBUG, "main_app: Kiem tra tinh hop le cua cau hinh LAN IP/Netmask hien tai...");
    // check_lan_ip_config_validity trả về 0 (hợp lệ) hoặc -1 (không hợp lệ/lỗi)
    int validity_check_result = check_lan_ip_config_validity(new_ip, new_mask);

    if (validity_check_result == 0) {
        log_message(LOG_LVL_DEBUG, "main_app: >>> KIEM TRA LAN IP: Cau hinh DANG CHAY hop le!");
        printf("main_app: >>> KIEM TRA LAN IP: Cau hinh DANG CHAY hop le!\n");
    } else { // validity_check_result là -1
        log_message(LOG_LVL_ERROR, "main_app: >>> KIEM TRA LAN IP: Cau hinh DANG CHAY KHONG hop le hoac khong the kiem tra (ma loi: %d).", validity_check_result);
        printf("main_app: >>> KIEM TRA LAN IP: Cau hinh DANG CHAY KHONG hop le hoac khong the kiem tra (ma loi: %d).\n", validity_check_result);
    }

    const char *dhcp_start_da_set = new_dhcp_start;
    int dhcp_limit_da_set = atoi(new_dhcp_limit);
    long dhcp_leasetime_seconds_da_set = atol(new_dhcp_leasetime);

    log_message(LOG_LVL_DEBUG, "main_app: Kiem tra chi tiet danh sach IP da cap phat tu DHCP...");
    printf("\nmain_app: Kiem tra chi tiet danh sach IP da cap phat tu DHCP...\n");
    check_dhcp_leases_with_validation(dhcp_start_da_set,
                                  dhcp_limit_da_set,
                                  dhcp_leasetime_seconds_da_set);

    cleanup_logger();
    log_message(LOG_LVL_DEBUG, "main_app: Chuong trinh ket thuc.");
    printf("main_app: Chuong trinh ket thuc.\n");

    return 0;
}