#include "iface_list.h"
#include "log.h"
#include "defines.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <ctype.h>
#include <sys/wait.h> // Cho WIFEXITED, WEXITSTATUS

// === Các hàm nội bộ (Static Helper Functions) ===

/**
 * @brief Giải phóng bộ nhớ của các chuỗi dữ liệu bên trong một Node.
 * Hàm này KHÔNG giải phóng chính Node đó.
 * @param node Con trỏ tới Node cần giải phóng dữ liệu chuỗi.
 */
static void _free_node_data(struct Node* node) {
    if (node == NULL) return;
    // free(NULL) là an toàn
    free(node->name);
    free(node->mac);
    free(node->ipv4);
    free(node->bcast);
    free(node->mask);
    free(node->ipv6);
    free(node->status);
    // Đặt các con trỏ về NULL sau khi free
    node->name = NULL;
    node->mac = NULL;
    node->ipv4 = NULL;
    node->bcast = NULL;
    node->mask = NULL;
    node->ipv6 = NULL;
    node->status = NULL;
}

/**
 * @brief Hàm nội bộ kiểm tra xem danh sách đã chứa node có tên cho trước chưa.
 * @param head Con trỏ tới node đầu tiên.
 * @param name Tên cần kiểm tra.
 * @return true nếu tên đã tồn tại, false nếu chưa.
 */
static bool _list_contains_name(const struct Node* head, const char* name) {
    const struct Node* current = head;
    while (current != NULL) {
        if (current->name != NULL && name != NULL && strcmp(current->name, name) == 0) {
            return true; // Tìm thấy trùng tên
        }
        current = current->next;
    }
    return false; // Không tìm thấy
}

/**
 * @brief Hàm nội bộ để tạo Node và sao chép dữ liệu chuỗi.
 * Xử lý cấp phát và kiểm tra lỗi strdup.
 * @return Con trỏ tới Node mới hoặc NULL nếu lỗi.
 */
static struct Node* _create_node_internal(const char* name, const char* mac,
                                          const char* ipv4, const char* bcast, const char* mask,
                                          const char* ipv6, const char* status) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        LOG(LOG_LVL_ERROR, "%s, %d: malloc failed for new Node!", __func__, __LINE__);
        return NULL;
    }

    // Khởi tạo các con trỏ là NULL trước khi strdup
    newNode->name = newNode->mac = newNode->ipv4 = newNode->bcast = newNode->mask = newNode->ipv6 = newNode->status = NULL;
    newNode->next = NULL;

    // Sao chép chuỗi, kiểm tra lỗi sau mỗi lần strdup
    newNode->name = strdup(name ? name : ""); // Dùng "" nếu input là NULL
    if (!newNode->name) goto cleanup;
    newNode->mac = strdup(mac ? mac : "");
    if (!newNode->mac) goto cleanup;
    newNode->ipv4 = strdup(ipv4 ? ipv4 : "");
    if (!newNode->ipv4) goto cleanup;
    newNode->bcast = strdup(bcast ? bcast : "");
    if (!newNode->bcast) goto cleanup;
    newNode->mask = strdup(mask ? mask : "");
    if (!newNode->mask) goto cleanup;
    newNode->ipv6 = strdup(ipv6 ? ipv6 : "");
    if (!newNode->ipv6) goto cleanup;
    newNode->status = strdup(status ? status : "UNKNOWN"); // Mặc định status nếu NULL
    if (!newNode->status) goto cleanup;

    LOG(LOG_LVL_DEBUG, "%s, %d: Created node for '%s' at %p.", __func__, __LINE__, newNode->name, newNode);
    return newNode;

cleanup: // Nhãn để nhảy đến khi có lỗi strdup
    LOG(LOG_LVL_ERROR, "%s, %d: strdup failed during node creation!", __func__, __LINE__);
    _free_node_data(newNode); // Giải phóng các chuỗi đã cấp phát thành công
    free(newNode);            // Giải phóng chính Node
    return NULL;
}

/**
 * @brief Hàm nội bộ để nối một Node đã được tạo sẵn vào cuối danh sách.
 * @param head_ref Con trỏ trỏ tới con trỏ head.
 * @param newNode Con trỏ tới Node mới cần thêm (phải khác NULL).
 */
static void _append(struct Node** head_ref, struct Node* newNode) {
    // newNode đã được kiểm tra khác NULL ở hàm gọi
    if (*head_ref == NULL) {
        *head_ref = newNode; // Node mới là node đầu tiên
    } else {
        struct Node* last = *head_ref;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = newNode; // Nối vào cuối
    }
}

/**
 * @brief Hàm nội bộ để reset các buffer tạm dùng trong quá trình parse.
 */
static void _reset_temp_buffers(char *name, size_t name_size,
                                char *mac, size_t mac_size,
                                char *ipv4, size_t ipv4_size,
                                char *bcast, size_t bcast_size,
                                char *mask, size_t mask_size,
                                char *ipv6, size_t ipv6_size,
                                char *status, size_t status_size) {
    if (name_size > 0) name[0] = '\0';
    if (mac_size > 0) mac[0] = '\0';
    if (ipv4_size > 0) ipv4[0] = '\0';
    if (bcast_size > 0) bcast[0] = '\0';
    if (mask_size > 0) mask[0] = '\0';
    if (ipv6_size > 0) ipv6[0] = '\0';
    if (status_size > 0) strcpy(status, "DOWN"); // Reset status về DOWN
}


// === Các hàm công khai (Public Functions) ===

struct Node* create_node(const char* name, const char* mac,
                         const char* ipv4, const char* bcast, const char* mask,
                         const char* ipv6, const char* status) {
    // Hàm public này chỉ đơn giản gọi hàm internal
    return _create_node_internal(name, mac, ipv4, bcast, mask, ipv6, status);
}

void list_add(struct Node** head_ref, const char* name, const char* mac,
              const char* ipv4, const char* bcast, const char* mask,
              const char* ipv6, const char* status) {
    LOG(LOG_LVL_DEBUG, "%s, %d: Adding node '%s'.", __func__, __LINE__, name ? name : "NULL_NAME");
    struct Node* newNode = _create_node_internal(name, mac, ipv4, bcast, mask, ipv6, status);
    if (newNode != NULL) {
        _append(head_ref, newNode); // Gọi hàm helper để nối node
        LOG(LOG_LVL_DEBUG, "%s, %d: Node '%s' added successfully.", __func__, __LINE__, name ? name : "NULL_NAME");
    } else {
        LOG(LOG_LVL_ERROR, "%s, %d: Failed to create node for '%s', cannot add to list.", __func__, __LINE__, name ? name : "NULL_NAME");
    }
}

void list_print(const struct Node* head) {
    // Giữ nguyên logic nhưng có thể giảm bớt log nếu muốn
    LOG(LOG_LVL_DEBUG, "%s, %d: Printing interface list.", __func__, __LINE__);
    const struct Node* current = head;
    if (current == NULL) {
        printf("Interface list is empty.\n");
        return;
    }
    printf("\n--- Interface List ---\n");
    int count = 1;
    while (current != NULL) {
        printf("  [%d] Name  : %s\n", count, current->name ? current->name : "(N/A)");
        printf("      MAC   : %s\n", current->mac ? current->mac : "(N/A)");
        printf("      IPv4  : %s\n", current->ipv4 ? current->ipv4 : "(N/A)");
        printf("      Bcast : %s\n", current->bcast ? current->bcast : "(N/A)");
        printf("      Mask  : %s\n", current->mask ? current->mask : "(N/A)");
        printf("      IPv6  : %s\n", current->ipv6 ? current->ipv6 : "(N/A)");
        printf("      Status: %s\n", current->status ? current->status : "(N/A)");
        printf("      --------------------\n");
        current = current->next;
        count++;
    }
    printf("--- End of List ---\n\n");
}

void list_destroy(struct Node** head_ref) {
    LOG(LOG_LVL_DEBUG, "%s, %d: Destroying interface list.", __func__, __LINE__);
    struct Node* current = *head_ref;
    struct Node* next_node = NULL;
    while (current != NULL) {
        next_node = current->next;
        LOG(LOG_LVL_DEBUG, "%s, %d: Freeing node '%s' (%p).", __func__, __LINE__, current->name ? current->name : "N/A", current);
        _free_node_data(current); // Gọi helper để free chuỗi
        free(current);            // Free chính node
        current = next_node;
    }
    *head_ref = NULL;
    LOG(LOG_LVL_DEBUG, "%s, %d: List destroyed, head set to NULL.", __func__, __LINE__);
}

struct Node* list_search(const struct Node* head, const char* search_name) {
    // Logic tìm kiếm giữ nguyên, có thể giảm log
    LOG(LOG_LVL_DEBUG, "%s, %d: Searching for node '%s'.", __func__, __LINE__, search_name ? search_name : "NULL_NAME");
    const struct Node* current = head;
    while (current != NULL) {
        if (current->name != NULL && search_name != NULL && strcmp(current->name, search_name) == 0) {
            LOG(LOG_LVL_DEBUG, "%s, %d: Found node '%s' at %p.", __func__, __LINE__, search_name, current);
            return (struct Node*)current; // Vẫn cần ép kiểu bỏ const
        }
        current = current->next;
    }
    LOG(LOG_LVL_WARN, "%s, %d: Node '%s' not found.", __func__, __LINE__, search_name ? search_name : "NULL_NAME");
    return NULL;
}

bool list_delete(struct Node** head_ref, const char* delete_name) {
    LOG(LOG_LVL_DEBUG, "%s, %d: Attempting to delete node '%s'.", __func__, __LINE__, delete_name ? delete_name : "NULL_NAME");
    if (*head_ref == NULL || delete_name == NULL) {
        LOG(LOG_LVL_WARN, "%s, %d: List is empty or delete_name is NULL, cannot delete.", __func__, __LINE__);
        return false;
    }

    struct Node* temp = *head_ref;
    // Trường hợp xóa node đầu
    if (temp->name != NULL && strcmp(temp->name, delete_name) == 0) {
        *head_ref = temp->next;
        LOG(LOG_LVL_DEBUG, "%s, %d: Deleting head node '%s' (%p).", __func__, __LINE__, delete_name, temp);
        _free_node_data(temp);
        free(temp);
        return true;
    }

    // Trường hợp xóa node khác
    struct Node* previous = *head_ref;
    struct Node* current = (*head_ref)->next;
    while (current != NULL) {
        if (current->name != NULL && strcmp(current->name, delete_name) == 0) {
            previous->next = current->next; // Bỏ qua node current
            LOG(LOG_LVL_DEBUG, "%s, %d: Deleting node '%s' (%p).", __func__, __LINE__, delete_name, current);
            _free_node_data(current);
            free(current);
            return true;
        }
        previous = current;
        current = current->next;
    }

    LOG(LOG_LVL_WARN, "%s, %d: Node '%s' not found for deletion.", __func__, __LINE__, delete_name);
    return false;
}

char* load_data_from_file(const char* filename) {
    LOG(LOG_LVL_DEBUG, "%s, %d: Loading data from file '%s'.", __func__, __LINE__, filename ? filename : "NULL_FILENAME");
     FILE *fp = NULL;
     char *buffer = NULL;
     long file_size = 0;
     size_t bytes_read = 0;

     fp = fopen(filename, "rb");
     if (fp == NULL) {
         LOG(LOG_LVL_ERROR, "%s, %d: Cannot open file '%s'. Error: %s", __func__, __LINE__, filename, strerror(errno));
         return NULL;
     }

     if (fseek(fp, 0, SEEK_END) != 0) {
         LOG(LOG_LVL_ERROR, "%s, %d: Cannot fseek to end of file '%s'. Error: %s", __func__, __LINE__, filename, strerror(errno));
         fclose(fp);
         return NULL;
     }
     file_size = ftell(fp);
     if (file_size == -1) {
         LOG(LOG_LVL_ERROR, "%s, %d: Cannot ftell file size '%s'. Error: %s", __func__, __LINE__, filename, strerror(errno));
         fclose(fp);
         return NULL;
     }
     rewind(fp);

     buffer = (char*)malloc(file_size + 1);
     if (buffer == NULL) {
         LOG(LOG_LVL_ERROR, "%s, %d: Cannot allocate %ld bytes for buffer.", __func__, __LINE__, file_size + 1);
         fclose(fp);
         return NULL;
     }
     LOG(LOG_LVL_DEBUG, "%s, %d: Allocated buffer %ld bytes at %p.", __func__, __LINE__, file_size + 1, buffer);

     bytes_read = fread(buffer, 1, file_size, fp);
     if (bytes_read != (size_t)file_size) {
         if (ferror(fp)) {
              LOG(LOG_LVL_ERROR, "%s, %d: Error reading file '%s'. Error: %s", __func__, __LINE__, filename, strerror(errno));
         } else { // Bao gồm cả trường hợp feof sớm
              LOG(LOG_LVL_ERROR, "%s, %d: Incomplete read from file '%s' (read %zu / %ld bytes).", __func__, __LINE__, filename, bytes_read, file_size);
         }
         free(buffer);
         fclose(fp);
         return NULL;
     }
     buffer[bytes_read] = '\0';
     fclose(fp);
     LOG(LOG_LVL_DEBUG, "%s, %d: Successfully read file '%s' (%zu bytes).", __func__, __LINE__, filename, bytes_read);
     return buffer;
}

bool run_command_and_save(const char* command, const char* output_filename) {
    LOG(LOG_LVL_DEBUG, "%s, %d: Running command '%s', saving to '%s'.", __func__, __LINE__, command, output_filename);
     FILE *cmd_pipe = NULL;
     FILE *out_file = NULL;
     int character;
     int pclose_status;

     cmd_pipe = popen(PATH_CMD, "r");
     if (cmd_pipe == NULL) {
         LOG(LOG_LVL_ERROR, "%s, %d: popen failed for command '%s'. Error: %s", __func__, __LINE__, command, strerror(errno));
         return false;
     }
     LOG(LOG_LVL_DEBUG, "%s, %d: Pipe opened for command '%s'.", __func__, __LINE__, command);

     out_file = fopen(output_filename, "w");
     if (out_file == NULL) {
         LOG(LOG_LVL_ERROR, "%s, %d: Cannot open file '%s' for writing. Error: %s", __func__, __LINE__, output_filename, strerror(errno));
         pclose(cmd_pipe);
         return false;
     }
      LOG(LOG_LVL_DEBUG, "%s, %d: Output file '%s' opened for writing.", __func__, __LINE__, output_filename);

     while ((character = fgetc(cmd_pipe)) != EOF) {
         if (fputc(character, out_file) == EOF) {
             LOG(LOG_LVL_ERROR, "%s, %d: Error writing to file '%s'. Error: %s", __func__, __LINE__, output_filename, strerror(errno));
             fclose(out_file);
             pclose(cmd_pipe);
             return false;
         }
     }

     if (ferror(cmd_pipe)) {
          LOG(LOG_LVL_ERROR, "%s, %d: Error reading from command pipe '%s'.", __func__, __LINE__, command);
          fclose(out_file);
          pclose(cmd_pipe);
          return false;
     }

     LOG(LOG_LVL_DEBUG, "%s, %d: Finished reading command output.", __func__, __LINE__);

     if (fclose(out_file) != 0) {
          LOG(LOG_LVL_ERROR, "%s, %d: Error closing output file '%s'. Error: %s", __func__, __LINE__, output_filename, strerror(errno));
          pclose(cmd_pipe);
          return false;
     }

     pclose_status = pclose(cmd_pipe);
     if (pclose_status == -1) {
         LOG(LOG_LVL_ERROR, "%s, %d: Error closing command pipe '%s'. Error: %s", __func__, __LINE__, command, strerror(errno));
         return false;
     } else {
          if (WIFEXITED(pclose_status)) {
              int exit_status = WEXITSTATUS(pclose_status);
              if (exit_status == 0) {
                 LOG(LOG_LVL_DEBUG, "%s, %d: Command '%s' executed successfully.", __func__, __LINE__, command);
              } else {
                  LOG(LOG_LVL_WARN, "%s, %d: Command '%s' exited with status %d.", __func__, __LINE__, command, exit_status);
              }
          } else {
               LOG(LOG_LVL_WARN, "%s, %d: Command '%s' terminated abnormally (pclose status: %d).", __func__, __LINE__, command, pclose_status);
          }
     }
     LOG(LOG_LVL_DEBUG, "%s, %d: Command execution and saving to '%s' completed.", __func__, __LINE__, command, output_filename);
     return true;
}

// Hàm phân tích buffer dữ liệu và thêm các interface vào danh sách liên kết
void parse_data_buffer(struct Node** head_ref, char* data_buffer) {
    LOG(LOG_LVL_DEBUG, "%s, %d: Bat dau phan tich data buffer.", __func__, __LINE__);
    if (data_buffer == NULL) {
        LOG(LOG_LVL_ERROR, "%s, %d: Data buffer la NULL.", __func__, __LINE__);
        return;
    }

    char name_buf[256], mac_buf[256], ipv4_buf[256], bcast_buf[256], mask_buf[256], ipv6_buf[256], status_buf[256];

    char *line;
    char *saveptr;
    char* buffer_copy = strdup(data_buffer);
     if (buffer_copy == NULL) {
         LOG(LOG_LVL_ERROR, "%s, %d: Khong the strdup data_buffer for parsing.", __func__, __LINE__);
         return;
     }

    _reset_temp_buffers(name_buf, sizeof(name_buf), mac_buf, sizeof(mac_buf),
                        ipv4_buf, sizeof(ipv4_buf), bcast_buf, sizeof(bcast_buf),
                        mask_buf, sizeof(mask_buf), ipv6_buf, sizeof(ipv6_buf),
                        status_buf, sizeof(status_buf));

    line = strtok_r(buffer_copy, "\n", &saveptr);
    while (line != NULL) {
        if (!isspace((unsigned char)line[0]) && strstr(line, "Link encap:")) {
            if (name_buf[0] != '\0') { // Nếu có interface cũ, thêm vào list (nếu chưa có)
                if (!_list_contains_name(*head_ref, name_buf)) { // <-- KIỂM TRA TRÙNG LẶP
                    LOG(LOG_LVL_DEBUG, "%s, %d: Hoan tat phan tich cho '%s', them vao list.", __func__, __LINE__, name_buf);
                    list_add(head_ref, name_buf, mac_buf, ipv4_buf, bcast_buf, mask_buf, ipv6_buf, status_buf);
                } else {
                    LOG(LOG_LVL_WARN, "%s, %d: Interface '%s' da ton tai, bo qua them.", __func__, __LINE__, name_buf);
                }
            }
            // Reset cho interface mới
            _reset_temp_buffers(name_buf, sizeof(name_buf), mac_buf, sizeof(mac_buf),
                                ipv4_buf, sizeof(ipv4_buf), bcast_buf, sizeof(bcast_buf),
                                mask_buf, sizeof(mask_buf), ipv6_buf, sizeof(ipv6_buf),
                                status_buf, sizeof(status_buf));

            parser_name(line, name_buf, sizeof(name_buf));
            parser_mac(line, mac_buf, sizeof(mac_buf));

        } else if (name_buf[0] != '\0') { // Chỉ parse dòng con nếu đã có tên
            if (strstr(line, "inet addr:")) {
                parser_ipv4(line, ipv4_buf, sizeof(ipv4_buf), bcast_buf, sizeof(bcast_buf), mask_buf, sizeof(mask_buf));
            } else if (strstr(line, "inet6 addr:")) {
                parser_ipv6(line, ipv6_buf, sizeof(ipv6_buf));
            } else if (strstr(line, "MTU:")) {
                parser_status(line, status_buf, sizeof(status_buf));
            }
        }
        line = strtok_r(NULL, "\n", &saveptr);
    }

    // Thêm interface cuối cùng (nếu có và chưa tồn tại)
    if (name_buf[0] != '\0') {
         if (!_list_contains_name(*head_ref, name_buf)) { // <-- KIỂM TRA TRÙNG LẶP
            LOG(LOG_LVL_DEBUG, "%s, %d: Hoan tat phan tich cho interface cuoi cung '%s', them vao list.", __func__, __LINE__, name_buf);
            list_add(head_ref, name_buf, mac_buf, ipv4_buf, bcast_buf, mask_buf, ipv6_buf, status_buf);
        } else {
             LOG(LOG_LVL_WARN, "%s, %d: Interface cuoi cung '%s' da ton tai, bo qua them.", __func__, __LINE__, name_buf);
        }
    }

    free(buffer_copy); // Giải phóng bản sao
    LOG(LOG_LVL_DEBUG, "%s, %d: Phan tich data buffer hoan tat.", __func__, __LINE__);
}

void save_list_to_file(const struct Node* head, const char* output_filename) {
    LOG(LOG_LVL_DEBUG, "%s, %d: Saving list to file '%s'.", __func__, __LINE__, output_filename ? output_filename : "NULL_FILENAME");
     FILE *fp = NULL;
     const struct Node* current = head;

     fp = fopen(output_filename, "w");
     if (fp == NULL) {
         LOG(LOG_LVL_ERROR, "%s, %d: Cannot open file '%s' for writing. Error: %s", __func__, __LINE__, output_filename, strerror(errno));
         return;
     }
     LOG(LOG_LVL_DEBUG, "%s, %d: Output file '%s' opened for writing.", __func__, __LINE__, output_filename);

     while (current != NULL) {
         fprintf(fp, "Name: %s\n", current->name ? current->name : "");
         fprintf(fp, "MAC: %s\n", current->mac ? current->mac : "");
         fprintf(fp, "IPv4: %s\n", current->ipv4 ? current->ipv4 : "");
         fprintf(fp, "Bcast: %s\n", current->bcast ? current->bcast : "");
         fprintf(fp, "Mask: %s\n", current->mask ? current->mask : "");
         fprintf(fp, "IPv6: %s\n", current->ipv6 ? current->ipv6 : "");
         fprintf(fp, "Status: %s\n", current->status ? current->status : "");
         fprintf(fp, "--------------------------\n");
         current = current->next;
     }

     if (fclose(fp) != 0) {
          LOG(LOG_LVL_ERROR, "%s, %d: Error closing output file '%s'. Error: %s", __func__, __LINE__, output_filename, strerror(errno));
     } else {
         LOG(LOG_LVL_DEBUG, "%s, %d: List saved to file '%s' successfully.", __func__, __LINE__, output_filename);
     }
}


