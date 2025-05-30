/*
 * @File        : lib_ota.h
 * @Description : Include the common functions header for update ota firmware feature of vnpt applications
 * @Author      : HEC, VNPT-Technology
 */

#ifndef LIB_OTA_H_
#define LIB_OTA_H_
#include <vnpt/lib_vnpt.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define FW_INFO_STATUS_FILE             "/tmp/fw_info_stt"
#define FW_DL_STATUS_FILE               "/tmp/fw_dl_stt"
#define CHECK_FILE_DOWNLOAD_INTERVAL    60

typedef enum
{
    FW_INFO_MAC = 0,
    FW_INFO_TYPE = 1,
    FW_INFO_LABEL = 2,
    FW_INFO_MODEL = 3,
    FW_INFO_HW_VER = 4,
    FW_INFO_SERIAL = 5,
    FW_INFO_CUR_FW_VER = 6,
    FW_INFO_CUR_FW_BUILD = 7,
    FW_INFO_NEW_FW_VER = 8,
    FW_INFO_NEW_FW_BUILD = 9,
    FW_INFO_FW_URL = 10,
    FW_INFO_MAX = 11
} Firmware_Info_Param_Index_e;

typedef enum
{
    FW_UPGRADE_MAC = 0,
    FW_UPGRADE_STATUS = 1,
    FW_UPGRADE_FWPATH = 2,
    FW_UPGRADE_MAX = 3
} Firmware_Upgrade_Param_Index_e;

typedef enum {
    DL_FAILURE = 0,
    DL_SUCCESS = 1
} Download_firmware_Status_e;

typedef struct __attribute__((packed)) {
    char mac[MAC_ADDR_STR_LEN];
    char type[STR_LEN_16];
    char label[STR_LEN_16];
    char model[STR_LEN_64];
    char hwVersion[STR_LEN_64];
    char currFWVersion[STR_LEN_64];
    char currFWBuildtime[STR_LEN_64];
    char serial[STR_LEN_64];
    char newFWVersion[STR_LEN_64];
    char newFWBuildtime[STR_LEN_64];
    char fwUrl[STR_LEN_256];
} Node_Firmware_Info_s;

typedef struct __attribute__((packed)) {
    char mac[MAC_ADDR_STR_LEN];
    Download_firmware_Status_e status;
} Node_Download_FW_status;


LV_Bool_e LV_get_newest_fw_info_from_ota_server(Node_Firmware_Info_s *p_nodeFWInfo);

LV_Bool_e LV_download_firmware_from_OTA_server(char *firmwareLink, char *p_fwPath);

LV_Bool_e LV_get_firmwares_info_from_file(Node_Firmware_Info_s *p_fwInfo, int *p_numNode);

LV_Bool_e LV_update_download_firmware_status_file(Node_Download_FW_status dlStatus);

LV_Bool_e LV_get_firmwares_download_status_from_file(Node_Download_FW_status *p_dlStatus, int *p_numNode);

LV_Bool_e LV_get_current_firmware_info();

LV_Bool_e LV_update_firmware_info_file(Node_Firmware_Info_s fwInfo);

#endif /* LIB_OTA_H_ */
