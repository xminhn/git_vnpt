#include "uci_handler.h" 
#include "log.h"         
#include <uci.h>         
#include <stdio.h>       
#include <string.h>     
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>

/**
 * @brief Doc cau hinh LAN tu UCI va ghi ra file text.
 * Cac ten option va gia tri option se duoc ghi nguyen ban.
 * @param lan Ten file de ghi du lieu ra.
 * @return int 0 neu thanh cong, cac ma loi khac neu that bai.
 */
int show_lan_config(const char *output_filename) {
    log_message(LOG_LVL_DEBUG, "show_lan_config: Bat dau xu ly, ghi ra file: %s", output_filename);

    FILE *outfile = NULL;
    struct uci_context *ctx = NULL;
    struct uci_package *pkg = NULL;
    struct uci_element *e_iter = NULL; // Doi ten bien de tranh nham lan
    struct uci_section *s_lan = NULL;
    int function_return_status = 0;

    outfile = fopen(output_filename, "w");
    if (!outfile) {
        log_message(LOG_LVL_ERROR, "show_lan_config: Khong the mo file %s de ghi.", output_filename);
        return -1;
    }

    ctx = uci_alloc_context();
    if (!ctx) {
        log_message(LOG_LVL_ERROR, "show_lan_config: Khong du bo nho cho UCI context.");
        fclose(outfile);
        return -1;
    }

    if (uci_load(ctx, "network", &pkg) != UCI_OK) {
        log_message(LOG_LVL_ERROR, "show_lan_config: Khong the tai package 'network'. Ma loi UCI: %d", ctx->err);
        uci_free_context(ctx);
        fclose(outfile);
        return -1;
    }

    uci_foreach_element(&pkg->sections, e_iter) {
        struct uci_section *current_s = uci_to_section(e_iter);
        if (strcmp(current_s->type, "interface") == 0 && strcmp(current_s->e.name, "lan") == 0) {
            s_lan = current_s;
            log_message(LOG_LVL_DEBUG, "show_lan_config: Tim thay section 'interface' ten 'lan'.");
            break;
        }
    }

    if (!s_lan) {
        log_message(LOG_LVL_WARN, "show_lan_config: Khong tim thay section 'interface' ten 'lan'.");
        function_return_status = -1; // Loi: Khong tim thay section
    } else {
        fprintf(outfile, "[interface.lan]\n");
        uci_foreach_element(&s_lan->options, e_iter) {
            struct uci_option *opt = uci_to_option(e_iter);
            const char *option_name = opt->e.name;
            const char *option_value_str = "";

            if (opt->type == UCI_TYPE_STRING) {
                option_value_str = opt->v.string;
                fprintf(outfile, "%s = %s\n", option_name, option_value_str);
                log_message(LOG_LVL_DEBUG, "show_lan_config:   %s = %s", option_name, option_value_str);
            } else if (opt->type == UCI_TYPE_LIST) {
                char list_buffer[1024] = {0};
                struct uci_element *list_item_iter;
                int first_in_list = 1;
                uci_foreach_element(&opt->v.list, list_item_iter) {
                    if (!first_in_list) {
                        strncat(list_buffer, " ", sizeof(list_buffer) - strlen(list_buffer) - 1);
                    }
                    strncat(list_buffer, list_item_iter->name, sizeof(list_buffer) - strlen(list_buffer) - 1);
                    first_in_list = 0;
                }
                fprintf(outfile, "%s = %s\n", option_name, list_buffer);
                log_message(LOG_LVL_DEBUG, "show_lan_config (list):   %s = %s", option_name, list_buffer);
            }
        }
    }

    if (pkg) {
        uci_unload(ctx, pkg);
    }
    uci_free_context(ctx);
    fclose(outfile);
    log_message(LOG_LVL_DEBUG, "show_lan_config: Da ghi xong du lieu ra file %s. Ket thuc ham.", output_filename);
    return function_return_status;
}


// --- Hàm thay đổi IP và Netmask cho LAN ---
int set_lan_ip_config(const char *ipaddr, const char *netmask) {
    log_message(LOG_LVL_DEBUG, "set_lan_ip_config: Bat dau. IP: '%s', Netmask: '%s'", ipaddr, netmask);
    struct uci_context *ctx = NULL;
    struct uci_ptr ptr;
    int return_status = 0; // 0 = thanh cong, -1 = that bai

    if (!ipaddr || !netmask) {
        log_message(LOG_LVL_ERROR, "set_lan_ip_config: Dia chi IP hoac Netmask la NULL.");
        return -1;
    }

    ctx = uci_alloc_context();
    if (!ctx) {
        log_message(LOG_LVL_ERROR, "set_lan_ip_config: Khong du bo nho cho UCI context.");
        return -1;
    }

    char uci_path_spec[256];

    // Set ipaddr
    snprintf(uci_path_spec, sizeof(uci_path_spec), "network.lan.ipaddr=%s", ipaddr);
    if (uci_lookup_ptr(ctx, &ptr, uci_path_spec, true) != UCI_OK || uci_set(ctx, &ptr) != UCI_OK) {
        log_message(LOG_LVL_ERROR, "set_lan_ip_config: Loi khi set ipaddr '%s'. Ma loi UCI: %d", ipaddr, ctx->err);
        return_status = -1;
        goto cleanup_ip_config;
    }

    // Set netmask
    snprintf(uci_path_spec, sizeof(uci_path_spec), "network.lan.netmask=%s", netmask);
    if (uci_lookup_ptr(ctx, &ptr, uci_path_spec, true) != UCI_OK || uci_set(ctx, &ptr) != UCI_OK) {
        log_message(LOG_LVL_ERROR, "set_lan_ip_config: Loi khi set netmask '%s'. Ma loi UCI: %d", netmask, ctx->err);
        return_status = -1;
        goto cleanup_ip_config;
    }

    // Save va Commit package "network" (ptr.p se tro den package "network")
    if (ptr.p) { // Dam bao package da duoc load vao ptr
        if (uci_save(ctx, ptr.p) != UCI_OK || uci_commit(ctx, &ptr.p, false) != UCI_OK) {
            log_message(LOG_LVL_ERROR, "set_lan_ip_config: Loi khi save hoac commit package network. Ma loi UCI: %d", ctx->err);
            return_status = -1;
        } else {
            log_message(LOG_LVL_DEBUG, "set_lan_ip_config: Da commit thanh cong thay doi IP/Netmask.");
        }
    } else {
        log_message(LOG_LVL_ERROR, "set_lan_ip_config: Khong tim thay package 'network' de save/commit.");
        return_status = -1;
    }

cleanup_ip_config:
    uci_free_context(ctx);
    log_message(LOG_LVL_DEBUG, "set_lan_ip_config: Ket thuc ham voi status %d.", return_status);
    return return_status;
}


// --- Hàm thay đổi cấu hình DHCP cho LAN (bao gồm enable/disable) ---
int set_dhcp_lan_config(const char *start_ip, const char *limit, const char *leasetime, int dhcp_enabled_status) {
const char *log_start_ip = start_ip ? start_ip : "(khong doi)";
const char *log_limit = limit ? limit : "(khong doi)";
const char *log_leasetime = leasetime ? leasetime : "(khong doi)";

log_message(LOG_LVL_DEBUG, "set_dhcp_lan_config: Bat dau. StartIP='%s', Limit='%s', LeaseTime='%s', EnableStatus=%d",
            log_start_ip,
            log_limit,
            log_leasetime,
            dhcp_enabled_status);

    struct uci_context *ctx = NULL;
    struct uci_ptr ptr;
    int return_status = 0; // 0 = thanh cong, -1 = that bai
    int changes_made = 0;

    ctx = uci_alloc_context();
    if (!ctx) {
        log_message(LOG_LVL_ERROR, "set_dhcp_lan_config: Khong du bo nho cho UCI context.");
        return -1;
    }

    char uci_path_spec[256];

    // Xu ly trang thai bat/tat DHCP server
    if (dhcp_enabled_status == 1 || dhcp_enabled_status == 0) {
        const char *ignore_value = (dhcp_enabled_status == 1) ? "0" : "1";
        snprintf(uci_path_spec, sizeof(uci_path_spec), "dhcp.lan.ignore=%s", ignore_value);
        if (uci_lookup_ptr(ctx, &ptr, uci_path_spec, true) != UCI_OK || uci_set(ctx, &ptr) != UCI_OK) {
            log_message(LOG_LVL_ERROR, "set_dhcp_lan_config: Loi khi set 'dhcp.lan.ignore'. Ma loi UCI: %d", ctx->err);
            return_status = -1;
            goto cleanup_dhcp_config;
        }
        changes_made = 1;
    }

    // Set start_ip
    if (start_ip) {
        snprintf(uci_path_spec, sizeof(uci_path_spec), "dhcp.lan.start=%s", start_ip);
        if (uci_lookup_ptr(ctx, &ptr, uci_path_spec, true) != UCI_OK || uci_set(ctx, &ptr) != UCI_OK) {
            log_message(LOG_LVL_ERROR, "set_dhcp_lan_config: Loi khi set 'dhcp.lan.start'. Ma loi UCI: %d", ctx->err);
            return_status = -1;
            goto cleanup_dhcp_config;
        }
        changes_made = 1;
    }

    // Set limit
    if (limit) {
        snprintf(uci_path_spec, sizeof(uci_path_spec), "dhcp.lan.limit=%s", limit);
        if (uci_lookup_ptr(ctx, &ptr, uci_path_spec, true) != UCI_OK || uci_set(ctx, &ptr) != UCI_OK) {
            log_message(LOG_LVL_ERROR, "set_dhcp_lan_config: Loi khi set 'dhcp.lan.limit'. Ma loi UCI: %d", ctx->err);
            return_status = -1;
            goto cleanup_dhcp_config;
        }
        changes_made = 1;
    }

    // Set leasetime
    if (leasetime) {
        snprintf(uci_path_spec, sizeof(uci_path_spec), "dhcp.lan.leasetime=%s", leasetime);
        if (uci_lookup_ptr(ctx, &ptr, uci_path_spec, true) != UCI_OK || uci_set(ctx, &ptr) != UCI_OK) {
            log_message(LOG_LVL_ERROR, "set_dhcp_lan_config: Loi khi set 'dhcp.lan.leasetime'. Ma loi UCI: %d", ctx->err);
            return_status = -1;
            goto cleanup_dhcp_config;
        }
        changes_made = 1;
    }

    // Chi save va commit neu co thay doi va package da duoc load
    if (changes_made && ptr.p) {
        if (uci_save(ctx, ptr.p) != UCI_OK || uci_commit(ctx, &ptr.p, false) != UCI_OK) {
            log_message(LOG_LVL_ERROR, "set_dhcp_lan_config: Loi khi save hoac commit package dhcp. Ma loi UCI: %d", ctx->err);
            return_status = -1;
        } else {
            log_message(LOG_LVL_DEBUG, "set_dhcp_lan_config: Da commit thanh cong cac thay doi DHCP.");
        }
    } else if (changes_made && !ptr.p) {
        log_message(LOG_LVL_ERROR, "set_dhcp_lan_config: Co yeu cau thay doi nhung package 'dhcp' khong duoc load (ptr.p is NULL).");
        return_status = -1;
    } else {
        log_message(LOG_LVL_DEBUG, "set_dhcp_lan_config: Khong co thay doi nao duoc yeu cau cho DHCP.");
    }

cleanup_dhcp_config:
    uci_free_context(ctx);
    log_message(LOG_LVL_DEBUG, "set_dhcp_lan_config: Ket thuc ham voi status %d.", return_status);
    return return_status;
}

int apply_network_changes(void) {
    log_message(LOG_LVL_DEBUG, "apply_network_changes: Bat dau ap dung thay doi cau hinh mang va DHCP...");
    int success_status = 0

    // Ap dung cau hinh mang
    log_message(LOG_LVL_DEBUG, "apply_network_changes: Goi lenh restart dich vu network...");
    if (system("/etc/init.d/network restart") != 0) {
        log_message(LOG_LVL_WARN, "apply_network_changes: Lenh '/etc/init.d/network restart' co the da khong thanh cong (kiem tra log he thong).");
        success_status = -1; 
    } else {
        log_message(LOG_LVL_DEBUG, "apply_network_changes: Lenh restart network da duoc goi.");
    }

    // Ap dung cau hinh DHCP
    // Luon goi restart dnsmasq
    log_message(LOG_LVL_DEBUG, "apply_network_changes: Goi lenh restart dich vu DHCP (dnsmasq)...");
    if (system("/etc/init.d/dnsmasq restart") != 0) {
        log_message(LOG_LVL_WARN, "apply_network_changes: Lenh '/etc/init.d/dnsmasq restart' co the da khong thanh cong (kiem tra log he thong).");
        success_status = -1;
    } else {
        log_message(LOG_LVL_DEBUG, "apply_network_changes: Lenh restart dnsmasq da duoc goi.");
    }

    if (success_status == -1) {
        log_message(LOG_LVL_ERROR, "apply_network_changes: Mot hoac ca hai lenh restart da that bai.");
    }
    return success_status;
}

int check_lan_ip_config_validity(const char *expected_ip_addr, const char *expected_netmask) {
    log_message(LOG_LVL_DEBUG, "check_lan_ip_config_validity: Bat dau kiem tra tinh hop le cua IP/Netmask LAN.");
    log_message(LOG_LVL_DEBUG, "check_lan_ip_config_validity: IP Mong doi: [%s], Netmask Mong doi: [%s]",
                expected_ip_addr ? expected_ip_addr : "NULL",
                expected_netmask ? expected_netmask : "NULL");

    if (!expected_ip_addr || !expected_netmask) {
        log_message(LOG_LVL_ERROR, "check_lan_ip_config_validity: Gia tri IP hoac Netmask mong doi la NULL.");
        return -1;
    }

    char current_ip_addr[40];
    char current_netmask[40];
    current_ip_addr[0] = '\0';
    current_netmask[0] = '\0';

    FILE *fp = NULL;
    char line_buffer[256];
    int found_ip = 0;
    int found_mask = 0;

    fp = popen("ifconfig br-lan", "r");
    if (fp == NULL) {
        log_message(LOG_LVL_ERROR, "check_lan_ip_config_validity: Khong the chay lenh 'ifconfig br-lan'.");
        return -1;
    }

    int line_count = 0;
    while (fgets(line_buffer, sizeof(line_buffer), fp) != NULL) {
        line_count++;
        char *ptr;
        char *value_start;
        char *value_end;

        if (strstr(line_buffer, "inet addr:") && strstr(line_buffer, "Mask:")) {
            if (!found_ip) {
                ptr = strstr(line_buffer, "inet addr:");
                if (ptr) {
                    value_start = ptr + strlen("inet addr:");
                    while (*value_start == ' ' && *value_start != '\0') { value_start++; }
                    value_end = value_start;
                    while (*value_end != '\0' && *value_end != ' ') { value_end++; }
                    if (value_end > value_start) {
                        size_t len_to_copy = value_end - value_start;
                        if (len_to_copy < sizeof(current_ip_addr)) {
                            strncpy(current_ip_addr, value_start, len_to_copy);
                            current_ip_addr[len_to_copy] = '\0';
                            found_ip = 1;
                        }
                    }
                }
            }

            if (!found_mask) {
                ptr = strstr(line_buffer, "Mask:");
                if (ptr) {
                    value_start = ptr + strlen("Mask:");
                    while (*value_start == ' ' && *value_start != '\0') { value_start++; }
                    value_end = value_start;
                    while (*value_end != '\0' && *value_end != '\n' && *value_end != '\r') {
                        if (!isdigit((unsigned char)*value_end) && *value_end != '.') { break; }
                        value_end++;
                    }
                    if (value_end > value_start) {
                        size_t len_to_copy = value_end - value_start;
                        if (len_to_copy < sizeof(current_netmask)) {
                            strncpy(current_netmask, value_start, len_to_copy);
                            current_netmask[len_to_copy] = '\0';
                            if (strlen(current_netmask) > 0) {
                                found_mask = 1;
                            }
                        }
                    }
                }
            }
        }
        if (found_ip && found_mask) {
            break;
        }
    }
    pclose(fp);

    if (found_ip && found_mask) {
        log_message(LOG_LVL_DEBUG, "check_lan_ip_config_validity: IP hien tai doc duoc: [%s]", current_ip_addr);
        log_message(LOG_LVL_DEBUG, "check_lan_ip_config_validity: Netmask hien tai doc duoc: [%s]", current_netmask);

        if (strcmp(current_ip_addr, expected_ip_addr) == 0 && strcmp(current_netmask, expected_netmask) == 0) {
            log_message(LOG_LVL_DEBUG, "check_lan_ip_config_validity: >>> LAN IP config la HOP LE (khop voi mong doi).");
            return 0; // Thanh cong, hop le
        } else {
            log_message(LOG_LVL_WARN, "check_lan_ip_config_validity: >>> LAN IP config KHONG HOP LE (khong khop voi mong doi).");
            if (strcmp(current_ip_addr, expected_ip_addr) != 0) {
                log_message(LOG_LVL_WARN, "check_lan_ip_config_validity: IP Doc duoc: %s, Mong doi: %s", current_ip_addr, expected_ip_addr);
            }
            if (strcmp(current_netmask, expected_netmask) != 0) {
                log_message(LOG_LVL_WARN, "check_lan_ip_config_validity: Netmask Doc duoc: %s, Mong doi: %s", current_netmask, expected_netmask);
            }
            return -1;
        }
    } else {
        log_message(LOG_LVL_WARN, "check_lan_ip_config_validity: Khong tim thay du thong tin IP/Netmask tu ifconfig.");
        return -1;
    }
    // return -1;
}

static unsigned long ip_str_to_long(const char *ip_str) {
    unsigned int p1, p2, p3, p4;
    if (sscanf(ip_str, "%u.%u.%u.%u", &p1, &p2, &p3, &p4) == 4) {
        return (p1 << 24) | (p2 << 16) | (p3 << 8) | p4;
    }
    return 0; // Loi hoac khong hop le
}

void check_dhcp_leases_with_validation(const char *configured_start_ip_str,
                                       int configured_limit,
                                       long configured_leasetime_seconds) {

    const char *lease_file_path_internal = "/tmp/dhcp.leases";
    log_message(LOG_LVL_DEBUG, "check_dhcp_leases_validation: Bat dau kiem tra file lease: [%s] (kiem tra lap lai trong 3 phut)", lease_file_path_internal);
    printf("--- Kiem tra cac IP dang duoc thue (tu file: %s) ---\n", lease_file_path_internal);

    // 1. Kiểm tra và chuyển đổi các tham số cấu hình DHCP mong muốn (giữ nguyên)
    if (!configured_start_ip_str || strlen(configured_start_ip_str) == 0 ||
        configured_limit <= 0 || configured_leasetime_seconds <= 0) {
        log_message(LOG_LVL_ERROR, "check_dhcp_leases_validation: Mot hoac nhieu tham so cau hinh DHCP khong hop le (StartIP: '%s', Limit: %d, Leasetime: %ld).",
                    configured_start_ip_str ? configured_start_ip_str : "NULL", configured_limit, configured_leasetime_seconds);
        printf("Loi: Thong so cau hinh DHCP truyen vao khong hop le.\n");
        return;
    }

    unsigned long conf_start_ip_num = ip_str_to_long(configured_start_ip_str);
    if (conf_start_ip_num == 0) {
        log_message(LOG_LVL_ERROR, "check_dhcp_leases_validation: Khong the chuyen doi configured_start_ip_str [%s] sang dang so.", configured_start_ip_str);
        printf("Loi: configured_start_ip_str [%s] khong hop le.\n", configured_start_ip_str);
        return;
    }

    unsigned long conf_end_ip_numeric = 0;
    unsigned int start_p[4];
    if (sscanf(configured_start_ip_str, "%u.%u.%u.%u", &start_p[0], &start_p[1], &start_p[2], &start_p[3]) == 4) {
        unsigned int end_octet4 = start_p[3] + configured_limit - 1;
        if (end_octet4 >= start_p[3] && end_octet4 <= 255) {
            char end_ip_temp_str[20];
            snprintf(end_ip_temp_str, sizeof(end_ip_temp_str), "%u.%u.%u.%u", start_p[0], start_p[1], start_p[2], end_octet4);
            conf_end_ip_numeric = ip_str_to_long(end_ip_temp_str);
        } else {
            log_message(LOG_LVL_WARN, "check_dhcp_leases_validation: Tinh toan End IP phuc tap. Kiem tra 'Trong dai?' co the khong chinh xac.");
        }
    } else {
        log_message(LOG_LVL_ERROR, "check_dhcp_leases_validation: Khong the phan tich configured_start_ip_str [%s] de tinh End IP.", configured_start_ip_str);
    }

    printf("Cau hinh DHCP mong muon: Start IP = %s, Limit = %d, Leasetime = %lds\n",
           configured_start_ip_str, configured_limit, configured_leasetime_seconds);

    // 2. Vòng lặp kiểm tra trong 3 phút (180 giây)
    time_t start_time_polling = time(NULL);
    time_t current_time_polling;
    int found_ip_in_new_range = 0; // Cờ báo đã tìm thấy IP trong dải mới chưa
    const int max_polling_duration = 180; // Tối đa 3 phút
    const int polling_interval = 5;       // Kiểm tra mỗi 5 giây

    do {
        log_message(LOG_LVL_DEBUG, "check_dhcp_leases_validation: Mo file lease [%s] de kiem tra.", lease_file_path_internal);
        FILE *lf = fopen(lease_file_path_internal, "r");
        if (!lf) {
            log_message(LOG_LVL_ERROR, "check_dhcp_leases_validation: Khong the mo file lease: [%s] trong vong lap.", lease_file_path_internal);
            sleep(polling_interval); // Đợi trước khi thử lại
            current_time_polling = time(NULL);
            continue;
            printf("Loi: Khong the mo file lease: [%s]. Kiem tra se dung lai.\n", lease_file_path_internal);
            return; // Quyết định dừng hẳn nếu không mở được file
        }

        printf("\n--- Thoi gian hien tai: %lds ke tu khi bat dau polling ---\n", (long)(time(NULL) - start_time_polling));
        printf("%-20s %-18s %-15s %-18s %-12s %s\n",
               "MAC Address", "IP Address", "Hostname", "Thoi gian con lai", "Trong dai moi?", "Lease hop le?");

        char line_buffer[256];
        time_t current_time_epoch = time(NULL);

        while (fgets(line_buffer, sizeof(line_buffer), lf)) {
            long expiry_time_from_file;
            char mac_addr[20] = "";
            char ip_addr_str[20] = "";
            char hostname[64] = "";
            int items_scanned = sscanf(line_buffer, "%ld %19s %19s %63s", 
                                       &expiry_time_from_file, mac_addr, ip_addr_str, hostname);
            
            if (items_scanned < 3) { // Cần ít nhất expiry, MAC, IP
                log_message(LOG_LVL_DEBUG, "check_dhcp_leases_validation: Dong khong du truong (<3 fields): [%s]", line_buffer);
                continue; // Bỏ qua dòng không hợp lệ
            }
            if (items_scanned < 4) strcpy(hostname, "*");

            long remaining_time_sec = expiry_time_from_file - current_time_epoch;
            char remaining_str[64];
            char in_range_str[15] = "No"; 
            char valid_lease_str[5] = "No";

            if (remaining_time_sec > 0) {
                // (Giữ nguyên logic tính remaining_str và valid_lease_str)
                long hours = remaining_time_sec / 3600;
                long minutes = (remaining_time_sec % 3600) / 60;
                long seconds = remaining_time_sec % 60;
                snprintf(remaining_str, sizeof(remaining_str), "%02ldh:%02ldm:%02lds", hours, minutes, seconds);
                if (remaining_time_sec <= configured_leasetime_seconds) {
                    strncpy(valid_lease_str, "Yes", sizeof(valid_lease_str)-1);
                    valid_lease_str[sizeof(valid_lease_str)-1] = '\0';
                } else {
                    snprintf(valid_lease_str, sizeof(valid_lease_str), ">Cfg");
                    valid_lease_str[sizeof(valid_lease_str)-1] = '\0';
                }
            } else {
                snprintf(remaining_str, sizeof(remaining_str), "Da het han");
            }

            // Kiểm tra IP có nằm trong dải mới không
            if (conf_start_ip_num != 0 && conf_end_ip_numeric != 0) {
                unsigned long current_leased_ip_numeric = ip_str_to_long(ip_addr_str);
                if (current_leased_ip_numeric != 0 && // Đảm bảo chuyển đổi IP thành công
                    current_leased_ip_numeric >= conf_start_ip_num &&
                    current_leased_ip_numeric <= conf_end_ip_numeric) {
                    strncpy(in_range_str, "Yes", sizeof(in_range_str)-1);
                     in_range_str[sizeof(in_range_str)-1] = '\0';
                    found_ip_in_new_range = 1; // Đánh dấu đã tìm thấy
                } else if (current_leased_ip_numeric != 0) {
                    // IP không nằm trong dải mới, có thể là dải cũ
                    strncpy(in_range_str, "No", sizeof(in_range_str)-1);
                     in_range_str[sizeof(in_range_str)-1] = '\0';
                } else {
                    // Không chuyển đổi được IP hiện tại
                     strncpy(in_range_str, "Invalid IP", sizeof(in_range_str)-1);
                      in_range_str[sizeof(in_range_str)-1] = '\0';
                }
            } else if (conf_start_ip_num != 0) {
                strncpy(in_range_str, "N/A (EndIP Err)", sizeof(in_range_str)-1);
                 in_range_str[sizeof(in_range_str)-1] = '\0';
            }

            printf("%-20s %-18s %-15s %-18s %-12s %s\n",
                   mac_addr, ip_addr_str, hostname, remaining_str, in_range_str, valid_lease_str);
            log_message(LOG_LVL_DEBUG, "check_dhcp_leases_validation: Lease: IP=%s, MAC=%s, ConLai=%s, TrongDaiMoi=%s, LeaseHopLe=%s",
                        ip_addr_str, mac_addr, remaining_str, in_range_str, valid_lease_str);
        }
        fclose(lf);

        if (found_ip_in_new_range) {
            printf(">>> Da tim thay it nhat mot thiet bi trong dai IP moi (%s)!\n", configured_start_ip_str);
            log_message(LOG_LVL_DEBUG, "check_dhcp_leases_validation: Da tim thay IP trong dai moi. Ket thuc kiem tra.");
            break; // Thoát khỏi vòng lặp do-while
        }

        current_time_polling = time(NULL);
        if (difftime(current_time_polling, start_time_polling) >= max_polling_duration) {
            log_message(LOG_LVL_WARN, "check_dhcp_leases_validation: Da het thoi gian kiem tra (3 phut), khong tim thay IP trong dai moi.");
            break; // Hết giờ, thoát khỏi vòng lặp do-while
        }

        log_message(LOG_LVL_DEBUG, "check_dhcp_leases_validation: Chua tim thay IP trong dai moi. Doi %d giay...", polling_interval);
        printf("--- Chua tim thay IP trong dai moi. Doi %d giay de kiem tra lai ---\n", polling_interval);
        sleep(polling_interval); // Đợi 5 giây

    } while (1); // Vòng lặp sẽ được thoát bởi 'break'

    // 3. Thông báo kết quả cuối cùng
    if (found_ip_in_new_range) {
        printf(">>> Kiem tra DHCP: Cau hinh DHCP da duoc cap nhat.\n");
    } else {
        printf(">>> Kiem tra DHCP:KHONG tim thay thiet bi nao nhan IP trong dai %s. Co the cau hinh DHCP CHUA cap nhat thanh cong hoac khong co thiet bi ket noi.\n", configured_start_ip_str);
        log_message(LOG_LVL_WARN, "check_dhcp_leases_validation: Ket thuc kiem tra, khong co IP trong dai moi duoc tim thay.");
    }

    printf("--- Ket thuc kiem tra danh sach IP thue ---\n");
}
