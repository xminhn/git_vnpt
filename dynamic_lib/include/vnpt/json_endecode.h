/*
 * @File        : json_endecode.h
 * @Description : Include the functions header for parser json string
 * @Author      : HEC, VNPT-Technology
 */

#ifndef JSON_ENDECODE_H_
#define JSON_ENDECODE_H_

#define JSN_FMT_CMA             "\"%s\":\"%s\","
#define JSN_FMT_NCMA            "\"%s\":\"%s\""

#include "basic_defs.h"
#include "log.h"

#define FW_REPORT_INFO_MAX         10
#define OTA_AUTHEN_INFO_MAX        4
#define DEVICE_INFO_MAX            5
#define RADIO_INFO_MAX             4
#define MESH_SSID_MAX              2
#define GUEST_SSID_MAX             4
#define REPEATER_MAX               6
#define DHCP_MAX                   5
#define VLAN_MAX                   3
#define GROUP_MAX                  5
#define TOPOLOGY_MAX               6
#define NODE_INFO_MAX              15
#define INTERNET_QUALITY_MAX       12
#define CLIENT_INFO_MAX            11
#define NUM_CLIENT_MAX             100
#define TRAFFIC_INFO_MAX           50
#define SSID_NEIGHBOR_MAX          3
#define DDNS_INFO_MAX              6
#define PORT_FORWARDING_INFO_MAX   8
#define FW_INFO_RESPONSE_MAX       6
#define FW_STATUS_RESPONSE_MAX     3
#define UPGRADE_FW_RESPONSE_MAX    3
#define SYNC_STATUS_INFO_MAX       2

typedef enum
{
    PAGE_NONE = 0,
    PAGE_DEVICE_INFO = 1,
    PAGE_INTERNET_SETTING_INFO = 2,
    PAGE_RADIO_INFO = 3,
    PAGE_MESH_SSID_INFO = 4,
    PAGE_GUEST_SSID_INFO = 5,
    PAGE_REPEATER_INFO = 6,
    PAGE_DHCP_INFO = 7,
    PAGE_VLAN_INFO = 8,
    PAGE_WIFI_GROUP_INFO = 9,
    PAGE_CLIENT_FILTERING_INFO = 10,
    PAGE_URL_FILTERING_INFO = 11,
    PAGE_ACCOUNT_INFO = 12,
    PAGE_WAN_STATUS_INFO = 13,
    PAGE_CREATE_MESH_WAN_ETHER = 14,
    PAGE_CREATE_MESH_WAN_REP = 15,
    PAGE_JOIN_MESH = 16,
    PAGE_MESH_NODE = 17,
    PAGE_NODE_INFO = 18,
    PAGE_TOPOLOGY_INFO = 19,
    PAGE_TOPOLOGY_NODE_INFO = 20,
    PAGE_INTERNET_QUALITY_INFO = 21,
    PAGE_CLIENT_INFO = 22,
    PAGE_TRAFFIC_NET_LIST_INFO = 23,
    PAGE_TRAFFIC_STATISTIC  = 24,
    PAGE_CPU_LOAD_STATISTIC = 25,
    PAGE_CONNECTION_STATISTIC = 26,
    PAGE_SPEED_RE_CAP = 27,
    PAGE_SPEED_CAP_TO_INTERNET = 28,
    PAGE_SSID_NEIGHBOR = 29,
    PAGE_PING_INFO = 30,
    PAGE_TRACE_INFO = 31,
    PAGE_DDNS_INFO = 32,
    PAGE_PORT_FORWARDING_INFO = 33,
    PAGE_UPGRADE_FW_INFO = 34,
	PAGE_CHECK_ADD_NODE = 35,
	PAGE_ADD_NEW_NODE = 36,
	PAGE_SCAN_NEW_NODE = 37,
	PAGE_ADD_LIST_NODE = 38,
	PAGE_INTERNET_SPEED = 39
} Page_Type_e;

typedef enum
{
    ACTION_NONE = 0,
    ACTION_VIEW = 1,
    ACTION_ADD = 2,
    ACTION_EDIT = 3,
    ACTION_DELETE = 4,
    ACTION_JOIN = 5,
    ACTION_RESET = 6,
    ACTION_REBOOT = 7,
    ACTION_UPGRADE = 8,
    ACTION_GET_FW_INFO = 9,
    ACTION_GET_SYNC_STATUS = 10
} Action_Type_e;

typedef struct __attribute__((packed))
{
    Page_Type_e page;
    Action_Type_e action;
} Control_Info_s;

typedef struct __attribute__((packed))
{
    char pageName[NORMAL_STR_LEN];
    Page_Type_e pageType;
} Page_Info_s;

typedef struct __attribute__((packed))
{
    char actionName[NORMAL_STR_LEN];
    Action_Type_e actionType;
} Action_Info_s;

typedef struct __attribute__((packed))
{
    char option[STR_LEN_64];
    char value[STR_LEN_256];
} Uci_Config_Value_s;

#define DEVICE_INFO             "deviceInfo"
#define INTERNET_SETTING_INFO   "internetSetting"
#define RADIO_INFO              "radio"
#define MESH_SSID_INFO          "meshSSID"
#define GUEST_SSID_INFO         "guestSSID"
#define REPEATER_INFO           "repeater"
#define DHCP_INFO               "dhcp"
#define VLAN_INFO               "vlan"
#define WIFI_GROUP_INFO         "wifiGroup"
#define CLIENT_FILTERING_INFO   "clientFiltering"
#define URL_FILTERING_INFO      "urlFiltering"
#define ACCOUNT_INFO            "account"
#define WAN_STATUS_INFO         "wanStatus"
#define CREATE_MESH_INFO        "createMesh"
#define JOIN_MESH_INFO          "joinMesh"
#define MESH_NODE_INFO          "meshNode"
#define NODE_INFO               "node"
#define TOPOLOGY_INFO           "topology"
#define TOPOLOGY_NODE_INFO      "topologyNode"
#define INTERNET_QUALITY_INFO   "internetQuality"
#define CLIENT_INFO             "clientInfo"
#define UPGRADE_FW_INFO         "upgradeFwInfo"


void JS_create_json_string(char *p_jsonString, char values[][MAX_ELEMENT_LENGTH], const char **p_keys, int numPair, LV_Bool_e maskBraces);

void JS_create_json_string_for_list(char *p_jsonString, char values[][MAX_ELEMENT_LENGTH], const char **p_keys, int numPair, char *p_listName, int numObject);

LV_Bool_e JS_get_value_by_key(char *p_jsonString, const char *p_key, char *p_value);

LV_Bool_e JS_get_list_value_by_key(char *p_jsonString, char *p_key, char p_value[][MAX_ELEMENT_LENGTH], int *p_numClient);

LV_Bool_e JS_decode_request_msg(char *p_request, const char **p_keys, char values[][MAX_ELEMENT_LENGTH], int numElement);

#endif /* JSON_ENDECODE_H_ */
