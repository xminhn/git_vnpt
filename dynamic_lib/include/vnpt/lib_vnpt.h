/*
 * @File        : lib_vnpt.h
 * @Description : Include the common functions header for all of vnpt applications
 * @Author      : HEC, VNPT-Technology
 */

#ifndef LIB_VNPT_H_
#define LIB_VNPT_H_

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <uci.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <malloc.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <netdb.h>
#include <syslog.h>
#include "json_endecode.h"
#include "topo_lock.h"
#include "cJSON.h"
#include "regex.h"

#define TEMP_BACKHAUL 					"Temporary_backhaul"
#define DEVICE_CONFIG_FILE				"/etc/config/apinfo"
#define TEMP_DEVICE_CONFIG_FILE         "/etc/config/apinfoTemp"
#define IS_MESH_CONFIG_FILE				"/etc/isMeshed"
#define MESH_MODE_FILE					"/etc/config/mode_mesh"
#define NETWORK_CONFIG_FILE 			"/etc/config/network"
#define TEMP_NETWORK_CONFIG_FILE        "/etc/config/networkTemp"
#define SYSTEM_FILE						"/etc/config/system"
#define TEMP_SYSTEM_FILE				"/etc/config/systemtemp"
#define ALARM_CONFIG_FILE				"/etc/config/alarm"
#define TEMP_ALARM_CONFIG_FILE			"/etc/config/tempAlarm"
#define WIRELESS_CONFIG_FILE			"/etc/config/wireless"
#define DHCP_CONFIG_FILE				"/etc/config/dhcp"
#define HOME_WIFI_CONFIG_FILE			"/etc/config/homewifi"
#define TEMP_HOME_WIFI_CONFIG_FILE 		"/etc/config/tempHomeWifi.conf"
#define LED_CONTROL_FILE				"/etc/config/onoffled"
#define FIREWALL_CONFIG_FILE			"/etc/config/firewall"
#define TEMP_FIREWALL_CONFIG_FILE		"/etc/config/firewallTemp"
#define FIRMWARE_LINK_DOWNLOAD_FILE		"/etc/config/fw_link"
#define GROUP_URL_RESTRICT_CONFIG 		"/etc/config/urlTimeRes.conf"
#define GROUP_URL_RESTRICT_FILE			"/etc/config/urlTimeRes"
#define GROUP_URL_RESTRICT_TMP_FILE		"/etc/config/urlTimeResTmp"
#define DDNS_CONFIG_FILE				"/etc/config/ddns"
#define CONFIG_BAS2_WIRELESS_FILE 		"/etc/config/conf.bas2.wireless"
#define CONFIG_BAS5_WIRELESS_FILE 		"/etc/config/conf.bas5.wireless"
#define CONFIG_RAD2_WIRELESS_FILE		"/etc/config/conf.rad2.wireless"
#define CONFIG_RAD5_WIRELESS_FILE		"/etc/config/conf.rad5.wireless"
#define CONFIG_REP2_WIRELESS_FILE		"/etc/config/conf.rep2.wireless"
#define CONFIG_REP5_WIRELESS_FILE		"/etc/config/conf.rep5.wireless"
#define URL_FILETER_LIST_FILE			"/etc/config/urlFilter"
#define CONFIG_WAN_NOT_AUTO_FILE        "/etc/config/notauto"
#define NODE_FW_INFO_FILE				"/tmp/nodeFWInfo"
#define DOWNLOAD_FW_STATUS_FILE			"/tmp/downloadFWStatus"
#define RESOLV_CONFIG_FILE				"/tmp/resolv.conf.auto"
#define NODE_MESH_INFO_FILE				"/tmp/nodeInfo"
#define TOPOLOGY_FILE					"/tmp/topology"
#define DHCP_LEASE_FILE					"/tmp/dhcp.leases"
#define CLIENT_INFO_FILE				"/tmp/client_info"
#define ASSOCIATED_CLIENT_INFO_FILE		"/tmp/associated_client"
#define REALTIME_CLIENT_INFO_FILE		"/var/run/clientInfo"
#define UMP_HANDLE_FILE					"/etc/umpHandleServ"
#define SERVER_SOCK_PATH                "/var/run/comm_ctrl_socket_server"
#define MESH_COMM_SERVER_SOCK_PATH      "/var/run/comm_ctrl_socket_server"
#define LIST_NEW_NODE_FILE              "/tmp/listNewNode"
#define LIST_ADD_NODE_FILE              "/tmp/listAllowNode"
#define HTTP_SERVICE					"http"
#define HTTPS_SERVICE					"https"

#define TMP_DIRECTORY_PATH				"/tmp/"
#define SPEEDTEST_SERVERS_URL			"speedtest-servers-static.php"
#define SPEEDTEST_CONFIG_URL			"speedtest-config.php"
#define SPEEDTEST_DOMAIN_NAME			"www.speedtest.net"
#define SPEEDTEST_SERVERS_DOMAIN_NAME	"c.speedtest.net"
#define MAX_WIFI_INTERFACE        		6  /*2 Backhaul + 2 main ssid + 2 repeater */
#define MAX_GUEST_INTERFACE             6  /*3 for wifi 2.4GHz and 3 for wifi 5GHz*/
#define MAX_RADIO_INTERFACE       		2
#define MAX_WIFI_IFACE_SECTION          10
#define MAX_EWAN_INTERFACE        		4
#define MAX_LAN_INTERFACE        		4
#define MAX_NUMBER_VLAN           		3
#define MAX_VLAN_ID           			6
#define MINIMUM_DDSN_LEN          		7
#define MAX_NUMBER_GROUP                8
#define MAX_NUMBER_MAC_IN_GROUP         4
#define MAX_NUMBER_URL_IN_GROUP         8
#define MAX_NTP_SERVER                  4
#define MAX_NUMBER_DDNS                 8
#define MAX_NUMBER_RULE_PORT_FORWARDING 8
#define MAX_NUMBER_RULE_FILEWALL        19
#define MAX_ZONE_FIREWALL               10
#define MAX_RULE_FORWARDING_FIREWALL    90
#define MAX_BUFF_LEN              		65536
#define MAX_NUMBER_PORT_FORWARDING      8
#define MAX_NUMBER_URL_FILTERING        8
#define MAX_NUMBER_CLIENT_FILTERING     8
#define CLIENTS_INFO_BUFF               14000
#define MAX_NUMBER_ROUTE				8
#define MAX_NUMBER_NETWORK				10

#define MAC_ADDR_STR_LEN          		18
#define IP_ADDR_LEN						16
#define IP6_ADDR_LEN					46
#define RECV_SOCK_TIMEOUT               10
#define RUN_CMD_NOR_TIMEOUT             2

//define option for set wifi
#define SET_REP_5GHZ                    0
#define SET_REP_2GHZ                    1
#define CHANGE_SSID                     2
#define NOT_IMPORTANT                   100

//define option for edit, add, remove ewan
#define ADD_EWAN                        0
#define DEL_EWAN                        1
#define CHANGE_EWAN                     2

// Definition for database manager
#define TOPOLOGY_DTB_FILE_PATH          "/tmp/dtb_topology.json"
#define TOPOLOGY_DTB_TMP_FILE_PATH		"/tmp/dtb_topology_tmp"
#define CLIENTS_DTB_FILE_PATH           "/tmp/dtb_clients"
#define CLIENTS_DTB_TMP_FILE_PATH       "/tmp/dtb_clients_tmp"
#define TOPO_LINK_DTB_FILE_PATH         "/tmp/dtb_links"
#define TOPO_LINK_DTB_TMP_FILE_PATH     "/tmp/dtb_links_tmp"

// Definition for Mesh with ONT detect file
#define MESH_WITH_ONT_DETECT            "/tmp/detect_virtual_CAP"

#define VERDOR_FILE_PATH                "/WEBSERVER/www/oui"
#define SEPERATOR                       ':'
#define MINUS                           '-'

#define MAX_TOPO_NODE                   10
#define MAX_LINKS_NODE                  110
#define MAX_CLIENTS_NODE                100
#define MAX_NUM_CONN                    100
#define MAC_STR_LEN                     18

// Definion timeout for trace
#define TIMEOUT_SECONDS                 40

// Topo json names
#define TOPO_IEEE1905_MAC               "ieee1905Mac"
#define TOPO_DEV_TYPE                   "deviceType"
#define TOPO_IPADDR                     "ipAddress"
#define TOPO_WLAN2G_MAC                 "wlan2gMac"
#define TOPO_WLAN5G_MAC                 "wlan5gMac"
#define TOPO_ETH_MAC                    "etherMac"
#define TOPO_UPSTREAM_DEV               "upstreamDev"
#define TOPO_NODE_LINK                  "nodeLink"
#define TOPO_DEV_NAME                   "deviceName"
#define TOPO_DEV_LOCA                   "location"
#define TOPO_SERIAL                     "serial"
#define TOPO_MODEL_NUM                  "modelNum"

#define TOPO_MESH_OVER_ETH				"meshOverEth"
#define TOPO_RSSI						"rssi"
#define TOPO_WAN_CONN					"wanConnection"
#define TOPO_CONN_SPEED					"connectionSpeed"
#define TOPO_UL_SPEED					"ulSpeed"
#define TOPO_DL_SPEED					"dlSpeed"
#define TOPO_CHANNEL					"channel"

// Client json names
#define CLIE_MAC                        "mac"
#define CLIE_IPADDR                     "ipAddr"
#define CLIE_NAME                       "name"
#define CLIE_PAREN_MAC                  "parentMac"
#define CLIE_PAREN_SSID                 "parentSSID"
#define CLIE_LINK                       "link"
#define CLIE_STATUS                     "status"
#define CLIE_RSSI                       "rssi"
#define CLIE_TOTAL_UP                   "totalUp"
#define CLIE_TOTAL_DOWN                 "totalDown"
#define CLIE_TX_RATE                    "txRate"
#define CLIE_RX_RATE                    "rxRate"
#define CLIE_BAND                       "band"
#define CLIE_TOTAL_CONN                 "totalconn"
#define CLIE_CONN_GROUP                 "connections"
#define CLIE_CONN_NAME                  "protocol"
#define CLIE_CONN_NUMBER                "numberConn"
#define CLIE_CONN_TX_BYTES              "txBytes"
#define CLIE_CONN_RX_BYTES              "rxBytes"
#define CLIE_GROUP_NAME                 "groupName"

// Links json names
#define LINK_SOURCE                     "source"
#define LINK_TARGET                     "target"
#define LINK_TYPE                       "type"
#define LINK_STRENGTH                   "strength"

#ifdef VNPTT_EW12SX_PLATFORM
#define RADIO_2G_CFG_NAME "mt7603"
#define RADIO_5G_CFG_NAME "mt7663"
#elif defined(VNPTT_EW12ST_PLATFORM)
#define RADIO_2G_CFG_NAME "mt7615e2"
#define RADIO_5G_CFG_NAME "mt7615e5"
#else
#define RADIO_2G_CFG_NAME "wifi0"
#define RADIO_5G_CFG_NAME "wifi1"
#endif

#if defined(VNPTT_EW30SX_PLATFORM)
#define REPEATER_2G_IFNAME "ath0"
#define REPEATER_5G_IFNAME "ath1"
#else
#define REPEATER_2G_IFNAME "apclix0"
#define REPEATER_5G_IFNAME "apcli0"
#endif

#define MAX_ETHER_CLIENT_PER_NODE   20

// validate
#define defRoute "uncheck"
#define MAX_LEN_DEVICE_NAME 32
#define MIN_LEN_DEVICE_NAME 2
#define MAX_LEN_LOCATION 32
#define MIN_LEN_LOCATION 2
#define MAX_LEN_SSID 32
#define MIN_LEN_SSID 1
#define MAX_LEN_PASSPHRASE 63
#define MIN_LEN_PASSPHRASE 8
#define MAX_LEN_USERNAME 32
#define MIN_LEN_USERNAME 1
#define MAX_LEN_PASSWORD 32
#define MIN_LEN_PASSWORD 1
#define MAX_RANGE_MTU 9000
#define MIN_RANGE_MTU 1500
#define MAX_LEN_HOSTNAME 63
#define MIN_LEN_HOSTNAME 1
#define MAX_LEN_FW_ZONE 10
#define MIN_LEN_FW_ZONE 1
#define MAX_LEN_PORT_FORWARDING_NAME 32
#define MIN_LEN_PORT_FORWARDING_NAME 1
#define MAX_LEN_DDNS_ENTRY_NAME 32
#define MIN_LEN_DDNS_ENTRY_NAME 1
#define MAX_LEN_DDNS_USER 128
#define MIN_LEN_DDNS_USER 1
#define MAX_LEN_DDNS_PASSWORD 128
#define MIN_LEN_DDNS_PASSWORD 1
#define MIN_LEN_LEASETIME 20
#define MAX_LEN_LEASETIME 2147483647

//// Structures for database manager
//typedef enum {
//    ETHER = 0,
//    WLAN2G = 1,
//    WLAN5G = 2,
//    DUAL = 3
//} Link_Type_e;

typedef enum {
    CAP = 0,
    MRE = 1,
    NONE = 2,
    ONT = 3
} Device_Type_e;

typedef enum {
    OFF = 0,
    ON = 1
} Client_Status_e;

typedef enum {
    ADD_PACKAGE_VS_SECTION = 0,
    ADD_ONLY_SECTION       = 1,
    UPDATE_SECTION         = 2
} Option_Action_e;

typedef enum
{
	METRIC_NONE = 0,
	METRIC_WAN = 10,
	METRIC_WAN1 = 20,
	METRIC_WAN2 = 30,
	METRIC_WAN3 = 40,
	METRIC_WWAN1 = 50,
	METRIC_WWAN = 60
} Metric_e;

typedef struct __attribute__((packed)) {
    char source[MAC_ADDR_STR_LEN];
    char target[MAC_ADDR_STR_LEN];
//    Link_Type_e iface;
    char iface[STR_LEN_8];
    char strength[STR_LEN_8];
}Links_File_Node_Info_s;

typedef struct __attribute__((packed)) {
    char ieee1905Mac[MAC_ADDR_STR_LEN];
    char ipAddress[STR_LEN_32];
    char wlan2gMac[MAC_ADDR_STR_LEN];
    char wlan5gMac[MAC_ADDR_STR_LEN];
    char etherMac[MAC_ADDR_STR_LEN];
    char upstreamDev[MAC_ADDR_STR_LEN];
    char deviceName[STR_LEN_64];
    char location[STR_LEN_64];
    char topoLevel[STR_LEN_8];
    char modelNumber[STR_LEN_32];
    char serial[STR_LEN_32];
    char rssi[STR_LEN_8];

    Device_Type_e deviceType;
    char nodeLink[STR_LEN_8];
    char clientEthernet[MAX_ETHER_CLIENT_PER_NODE][MAC_ADDR_STR_LEN];

    int meshOverEth;
    char wanConnection[STR_LEN_16];
    char connectionSpeed[STR_LEN_16];
    char ulSpeed[STR_LEN_16];
    char dlSpeed[STR_LEN_16];
    char channel[STR_LEN_16];
    char fwVersion[STR_LEN_32]; // for 12STv1 T0003
} Topo_File_Node_Info_s;

typedef struct __attribute__((packed)) {
    char protocol[STR_LEN_16];
    char numberConn[STR_LEN_32];
    char txBytes[STR_LEN_32];
    char rxBytes[STR_LEN_32];
} Conn_Info_s;

typedef struct __attribute__((packed)) {
    char mac[MAC_ADDR_STR_LEN];
    char band[STR_LEN_8];
    char ipAddr[STR_LEN_32];
    char name[STR_LEN_32];
    char parentMac[MAC_ADDR_STR_LEN];
    char parentSSID[STR_LEN_64];
    Client_Status_e status;
    char rssi[STR_LEN_8];
    char totalUp[STR_LEN_32];
    char totalDown[STR_LEN_32];
    char txRate[STR_LEN_8];
    char rxRate[STR_LEN_8];
    char groupName[STR_LEN_32];
    char totalConns[STR_LEN_8];
    char link[STR_LEN_8];
    char vendor[STR_LEN_64];
    Conn_Info_s connections[MAX_NUM_CONN];
    int  numConnections;
} Client_File_Node_Info_s;

typedef struct __attribute__((packed))
{
    char device[STR_LEN_16];
    char network[STR_LEN_8];
    char mode[STR_LEN_8];
    char ssid[STR_LEN_64];
    char encryption[STR_LEN_32];
    char key[STR_LEN_256];
    char disabled[STR_LEN_8];
    char bssid[MAC_ADDR_STR_LEN];
    char wds[STR_LEN_8];
    char disableCoext[STR_LEN_8];
    char section[STR_LEN_16];
    char aaaEnable[STR_LEN_4];
    char aaaMode[STR_LEN_4];
    char aaaServer[STR_LEN_4];
    char maxAssoc[STR_LEN_4];
    char hidden[STR_LEN_4];
    char macFilter[STR_LEN_8];
    char macList[STR_LEN_256];
    char sae[STR_LEN_4];
    char saePassword[STR_LEN_256];  // !TODO update length
    char owe[STR_LEN_4];
    char ieee80211w[STR_LEN_4];
} Wifi_Interface_s;

typedef struct __attribute__((packed))
{
    char bandwidth[STR_LEN_8];  // bw
    char channel[STR_LEN_32];
    char country[STR_LEN_8];
    char enableRepeater[STR_LEN_8];
    char hwMode[STR_LEN_8];    // wifimode
    char macAddress[MAC_ADDR_STR_LEN];
    char rst[STR_LEN_32];
    char status[STR_LEN_8];
    char txPower[STR_LEN_8];
    char device[STR_LEN_16];
    char disablecoext[STR_LEN_8];
    char channelOptimization[STR_LEN_4];
    char dfs[STR_LEN_4];
} Wifi_Radio_s;

typedef struct __attribute__((packed))
{
    char enableBridge[STR_LEN_8];
    char status[STR_LEN_8];
    char name[STR_LEN_16];
    char ifName[STR_LEN_32];  //  br-wan, ...
    char vlanName[STR_LEN_64]; // eth0 eth0.200
    char protocol[STR_LEN_8];
    char address[MAC_ADDR_STR_LEN];
    char ipv6Address[IP6_ADDR_LEN];
    char gateWay[IP_ADDR_LEN];
    char netMask[IP_ADDR_LEN];
    char priDNS[IP_ADDR_LEN];
    char secDNS[IP_ADDR_LEN];
    char server[IP_ADDR_LEN];
    char username[STR_LEN_32 + 1];
    char password[STR_LEN_32 + 1];
    char ipv6[STR_LEN_8];
    char metric[STR_LEN_8];
    char mtu[STR_LEN_8];
    char defaultRoute[STR_LEN_8];
    char type[STR_LEN_8];
    char wanTypeService[STR_LEN_16];
    char acceptRa[STR_LEN_16];
    char encap[STR_LEN_16];
    char section[STR_LEN_16];
    char broadcast[IP_ADDR_LEN];
    char ipv6GateWay[IP6_ADDR_LEN];
    char ipv6Type[STR_LEN_4];
    char ipStack[STR_LEN_4];
    char localEndpoint[IP_ADDR_LEN];
    char remoteEndpoint[IP_ADDR_LEN];
} Ewan_s;

typedef struct __attribute__((packed))
{
    char type[STR_LEN_8];
    char name[STR_LEN_16];
    char ifName[STR_LEN_32];
    char protocol[STR_LEN_8];
    char ipStack[STR_LEN_4];
    char netIfName[STR_LEN_32];
    char netState[STR_LEN_16];
} Network_s;

typedef struct __attribute__((packed))
{
    char ssid[STR_LEN_64];
    char key[STR_LEN_256];
    char encryption[STR_LEN_32];
} Main_Ssid_s;

typedef struct __attribute__((packed))
{
    char interface[STR_LEN_16];
    char url[STR_LEN_256];
    char domain[STR_LEN_128 + 1];
    char port[STR_LEN_16];
    char username[STR_LEN_32 + 1];
    char password[STR_LEN_32 + 1];
    char interval[STR_LEN_128];
    char enable[STR_LEN_8];
    char localSection[STR_LEN_16];
    char acsSection[STR_LEN_16];
} UMP_Server_s;

typedef struct __attribute__((packed))
{
    char accountingInterval[STR_LEN_128];
    char storedPeriods[STR_LEN_128];
    char commitInterval[STR_LEN_16];
    char refreshInterval[STR_LEN_16];
    char section[STR_LEN_16];
} Client_Monitoring_s;

typedef struct __attribute__((packed))
{
    char ruleDay[STR_LEN_64];
    char name[STR_LEN_64];
    char ruleStart[STR_LEN_16];
    char ruleStop[STR_LEN_16];
    char ruleMac[STR_LEN_256];
    char ruleURL[STR_LEN_4096];
    char section[STR_LEN_16];
} Access_Control_Config_s;

typedef struct __attribute__((packed))
{
    Access_Control_Config_s group[MAX_NUMBER_GROUP];
    int numGroup;
} Access_Control_Group_s;

typedef struct __attribute__((packed))
{
	char ifname[STR_LEN_64];
    char name[STR_LEN_8];
    char ssid[STR_LEN_64];
    char uptime[STR_LEN_32];
    char ipVersion[STR_LEN_8];
    char ipv4Addr[IP_ADDR_LEN];
    char netMask[IP_ADDR_LEN];
    char gateWay[IP_ADDR_LEN];
    char broadcast[IP_ADDR_LEN];
    char dhcpIgnore[STR_LEN_8];  /* dhcpv4 server*/
    char startIP[IP_ADDR_LEN];
    char numberIP[STR_LEN_16];
    char leaseTime[STR_LEN_32];
    char ipv6Adrr[STR_LEN_128];
    char dhcpv6Mode[STR_LEN_8];
    char ipv6Prefix[STR_LEN_64];
    char ipv6Length[STR_LEN_8];
    char ipv6PriDNS[IP6_ADDR_LEN];
    char ipv6SecondDNS[IP6_ADDR_LEN];
    char macAddress[MAC_ADDR_STR_LEN];
    char mtu[STR_LEN_8];
}  Lan_Information_s;

typedef struct __attribute__((packed))
{
    char sectionInterface[STR_LEN_16];
    char sectionPrefix[STR_LEN_16];
    char sectionRoute[STR_LEN_16];
    char sectionRddns[STR_LEN_16];
    char sectionDnssl[STR_LEN_16];
} Radvd_Section_s;

typedef struct __attribute__((packed))
{
    char hostName[STR_LEN_64];
    char timezone[STR_LEN_64];
    char listServer[STR_LEN_1024];
    char listServerOpt[MAX_NTP_SERVER][STR_LEN_128 + 1];
    char enabledLocal[STR_LEN_8];
    char enabledServer[STR_LEN_8];
    char currentTime[STR_LEN_64];
    char sectionSystem[STR_LEN_16];
} Time_Information_s;

typedef struct __attribute__((packed))
{
    char hostName[STR_LEN_64];
    char interface[STR_LEN_16];
    char serviceName[STR_LEN_128 + 1];
    char domain[STR_LEN_128 + 1];
    char userName[STR_LEN_128 + 1];
    char password[STR_LEN_128 + 1];
    char enable[STR_LEN_8];
    char section[STR_LEN_16];
} Ddns_Entry_s;

typedef struct __attribute__((packed))
{
    Ddns_Entry_s entry[MAX_NUMBER_DDNS];
    int numDDNS;
} DDNS_s;

typedef struct __attribute__((packed))
{
    char device[STR_LEN_64];
    char location[STR_LEN_64];
    char wanType[STR_LEN_16];			// wan connection in Topo_File_Node_Info_s
    char rssi[STR_LEN_16];
    char bitrate[STR_LEN_32];			// wan connection speed in Topo_File_Node_Info_s
    char macEthernet[MAC_STR_LEN];
    char ipAddress[IP_ADDR_LEN];
    char mode[STR_LEN_16];
    char mac24GHz[MAC_STR_LEN];
    char mac5GHz[MAC_STR_LEN];
    char wanmode[STR_LEN_16];
    char isMeshed[STR_LEN_16];
    char label[STR_LEN_16];
    char environment[STR_LEN_32];
    int meshOverEth;
    char ulSpeed[STR_LEN_16];
    char dlSpeed[STR_LEN_16];
    char channel[STR_LEN_16];
    char ssid [STR_LEN_64];
} Home_Information_s;

typedef struct __attribute__((packed))
{
    char device[STR_LEN_64];
    char modelNumber[STR_LEN_64];
    char serialNumber[STR_LEN_32];
    char location[STR_LEN_64];
    char firmwareVersion[STR_LEN_32];
    char buildtime[STR_LEN_64];
    char hardwareVersion[STR_LEN_32];
    char macAddress[MAC_STR_LEN];
    char operationMode[STR_LEN_32];
    char wanIP[IP_ADDR_LEN];
    char dns[STR_LEN_64];
    char wanIface[STR_LEN_32];
} Device_Information_s;

typedef struct __attribute__((packed))
{
    char type[STR_LEN_32];
    char iface[STR_LEN_32];
    char proto[STR_LEN_32];
    char status[STR_LEN_32];
    char ipv4[IP_ADDR_LEN];
    char ipv6[IP6_ADDR_LEN];
    char gateway[IP_ADDR_LEN];
    char priDNS[IP_ADDR_LEN];
    char secDNS[IP_ADDR_LEN];
} Network_Wan_Information_s;

typedef struct __attribute__((packed))
{
    char name[STR_LEN_128];
    char dest[STR_LEN_128];
    char input[STR_LEN_8];
    char output[STR_LEN_8];
    char forward[STR_LEN_8];
    char masq[STR_LEN_8];
    char mtuFix[STR_LEN_8];
    char network[STR_LEN_16];
    char section[STR_LEN_16];
} Zone_Section_Config_s;

typedef struct __attribute__((packed))
{
    char name[STR_LEN_128];
    char dest[STR_LEN_128];
    char section[STR_LEN_16];
} Zone_Forwarding_Config_s;

typedef struct __attribute__((packed))
{
    Zone_Section_Config_s entry[MAX_ZONE_FIREWALL];
    int numZone;
} Firewall_Zone_s;

typedef struct __attribute__((packed))
{
    char mac[MAC_STR_LEN];
    char location[STR_LEN_64];
    char device[STR_LEN_64];
    char modelNumber[STR_LEN_128];
    char serial[STR_LEN_32];
    char etherMac[STR_LEN_64];
    char label[STR_LEN_16];
    char account[STR_LEN_256];
    char umpInterface[STR_LEN_32];
    char umpUser[STR_LEN_32 + 1];
    char umpPassword[STR_LEN_32 + 1];
    char umpPerEnable[STR_LEN_128];
    char umpPerInterval[STR_LEN_32];
    char umpUrl[STR_LEN_128 + 1];
    char alarmReboot[STR_LEN_64];
    char alarmChangeFW[STR_LEN_64];
    char alarmCpu[STR_LEN_64];
    char alarmRam[STR_LEN_64];
    char alarmDisk[STR_LEN_64];
    char alarmDownloadFW[STR_LEN_64];
    char alarmApplyFW[STR_LEN_64];
    char alarmWanIp[STR_LEN_64];
    char cpuThreshold[STR_LEN_64];
    char ramThreshold[STR_LEN_64];
    char diskThreshold[STR_LEN_64];
    char section[STR_LEN_32];
    int  removedNode;
    char ieee1905Mac[MAC_STR_LEN];
} MRE_Extend_Config_s;

typedef struct __attribute__((packed))
{ 
    MRE_Extend_Config_s entry[MAX_TOPO_NODE - 1];
    int numberMRE; 
} MRE_Extend_Info_s;

typedef struct __attribute__((packed))
{
    Zone_Forwarding_Config_s entry[MAX_RULE_FORWARDING_FIREWALL];
    int numRuleFw;
} Firewall_Forwarding_s;

typedef struct __attribute__((packed))
{
    char name[STR_LEN_32 + 1];
    char protocol[STR_LEN_16];
    char protocolTMP[STR_LEN_16];
    char externalZone[STR_LEN_16];
    char externalPort[STR_LEN_8];
    char internalZone[STR_LEN_16];
    char internalPort[STR_LEN_8];
    char internalIP[IP_ADDR_LEN];
    char enable[STR_LEN_8];
    char section[STR_LEN_16];
} Port_Forwarding_Entry_s;

typedef struct __attribute__((packed))
{
    char device[STR_LEN_16];
    char network[STR_LEN_8];
    char ssid[STR_LEN_64];
    char encryption[STR_LEN_32];
    char key[STR_LEN_256];
    char disabled[STR_LEN_8];
    char wds[STR_LEN_4];
    char isGuest[STR_LEN_4];
    char section[STR_LEN_16];
    char aaaEnable[STR_LEN_4];
    char aaaMode[STR_LEN_4];
    char aaaServer[STR_LEN_4];
    char maxAssoc[STR_LEN_4];
    char hidden[STR_LEN_4];
    char macFilter[STR_LEN_8];
    char macList[STR_LEN_256];
    char sae[STR_LEN_4];
    char saePassword[STR_LEN_256];  // !TODO update length
    char owe[STR_LEN_4];
    char ieee80211w[STR_LEN_4];
} Guest_Paramters_s;

typedef struct __attribute__((packed))
{
    char device[STR_LEN_16];
    char network[STR_LEN_8];
    char ssid[STR_LEN_64];
    char mode[STR_LEN_8];
    char section[STR_LEN_16];
    char guestSsid[STR_LEN_4];
    char disabled[STR_LEN_4];
    char ifname[STR_LEN_16];
} Wifi_Iface_Section_s;

typedef struct __attribute__((packed))
{
    char option[STR_LEN_128 + 1];
    char value[STR_LEN_128 + 1];
} Json_Entries_s;

typedef struct __attribute__((packed))
{
    Port_Forwarding_Entry_s entry[MAX_NUMBER_RULE_PORT_FORWARDING];
    int numPort;
} Port_Forwarding_s;

typedef struct __attribute__((packed))
{
    Port_Forwarding_Entry_s entry[MAX_NUMBER_RULE_FILEWALL];
    int numPort;
} Rule_Forwarding_s;

static const char *gNodeFWServerInfo[] =
{
    "jwt",
    "devType",
    "mac",
    "label",
    "model",
    "hwVer",
    "fwVer",
    "buildtime",
    "serial"
};

static const char *gOtaAuthenInfo[] =
{
    "id",
    "publickey",
    "signature"
};

typedef struct __attribute__((packed))
{
	char cfg[STR_LEN_16];
	char interface[STR_LEN_16];
	char target[IP_ADDR_LEN];
	char netmask[IP_ADDR_LEN];
	char gateway[IP_ADDR_LEN];
	char metric[STR_LEN_16];
	char mtu[STR_LEN_8];
}Route_Section_s;

typedef struct __attribute__((packed))
{
	char serverIP[IP_ADDR_LEN];
	char port[STR_LEN_8];
	char interface[STR_LEN_16];
	char interimInterval[STR_LEN_8];
	char cpPage1[STR_LEN_256];
    char cpPage2[STR_LEN_256];
	char sesstionTime[STR_LEN_4];
}SC_Section_s;

typedef struct __attribute__((packed))
{
	char serverIP[IP_ADDR_LEN];
	char authPort[STR_LEN_8];
	char accPort[STR_LEN_8];
    char coaPort[STR_LEN_8];
    char interimInterval[STR_LEN_8];
	char interface[STR_LEN_16];
	char secretKey[STR_LEN_32];
}AAA_Section_s;

typedef struct 
{
    const char *p_hwMode;
    const char *p_wifiMode;
    const char *p_band;    // "2G" or "5G"
}Wifi_Mode_Mtk_s; // struct convert mtk wifiMode 

typedef struct
{
    const char *p_bw;   // HT mode
    const char *p_mtkBw;
}Bandwidth_Mtk_s;

typedef struct
{
    const char *p_dbm;
    const char *p_per; // percentage
}TxPower_Mtk_s;

typedef struct __attribute__((packed))
{
	char  ssid[64];
    char  bssid[32];
    int   encryption;
    int   encryption_wpa;
    char  signal_level[5];
    int   isVnptTechnology;
}scanned_ssid_struct_lib;

int LV_is_ipv4_address_valid(char *p_ipAddress);

LV_Bool_e LV_get_wifi_interface(Wifi_Interface_s *p_wifiEntry);

void      LV_apply_wifi(const int keepCfg , const int option);
/*#define SET_REP_5GHZ                    0
 *#define SET_REP_2GHZ                    1
 *#define CHANGE_SSID                     2
*/
LV_Bool_e LV_set_wifi_interface(const Wifi_Interface_s *p_wifiEntry, const int option);

void LV_init_struct_wifi_interface(Wifi_Interface_s *p_wifiEntry, int numberEntry);


/*
 * Desc: Get json value by key
 */
LV_Bool_e LV_get_value_by_key(char *p_jsonString, const char *p_key, char *p_value);

/*
 * Desc: Check MAC value
 */
LV_Bool_e LV_is_mac_valid(char *p_macAddress);

/*
 * Desc: Convert from lower string to upper string
 */
void LV_lower_to_upper_string(char s[]);

/*
 * Desc: Convert from upper string to lower string
 */
void LV_upper_to_lower_string(char s[]);

LV_Bool_e LV_parse_option_value(char *p_line, const char *p_name, char *p_value);

LV_Bool_e LV_run_command(const char *p_cmd, int timeout);

LV_Bool_e LV_run_command_and_get_result(const char *p_cmd, char *p_value);

void  LV_get_ethernet_status(char *p_ethStatus);

void  LV_get_harware_version(char *p_hwVersion);

void  LV_get_interface_gateway(char *p_iface);

LV_Bool_e Lv_create_socket_interaction_mesh_comm(int *p_sockFd, struct sockaddr_un *p_sevrAddr, struct sockaddr_un *p_clientAddr, int sendTimeout, int recvTimeout, const char *appName);

void LV_close_socket_interaction_mesh_comm(int sockFd, char *clientSunPath);

LV_Bool_e LV_wait_child_pro(pid_t pid, int nSec);

LV_Bool_e LV_check_exist_file(char *p_fileName);

void LV_do_upgrade_firmware(char *firmwarePath);

LV_Bool_e LV_get_ipv4_address_from_host(char *p_host, const char *p_service, struct sockaddr_in *p_addr);

/*
 * Desc: Write clients info into clients database file
 */
LV_Bool_e LV_write_clients_info_dtb_file(Client_File_Node_Info_s *p_newClieNode, int numActClieNode);

/*
 * Desc: Write links info into topology database file
 */
LV_Bool_e LV_write_links_info_dtb_file(Links_File_Node_Info_s *p_linksFileNode, int numLinksNode);

/*
 * Desc: Write topology info into topology database file
 */
LV_Bool_e LV_write_topo_info_dtb_file(Topo_File_Node_Info_s *p_topoFileNode, int numTopoNode);

/*
 * Desc: Read topology info from topology database file
 */
LV_Bool_e LV_read_topology_database(Topo_File_Node_Info_s *p_topoNode, int *p_totalNodeNum);

/*
 * Desc: Read clients info from database file
 */
LV_Bool_e LV_read_clients_info_database(Client_File_Node_Info_s *p_clientNode, int *p_totalClieNum);

/*
 * Desc: Read links info from database file
 */
LV_Bool_e LV_read_links_database(Links_File_Node_Info_s *p_linkNode, int *p_totalLinkNum);

/*
 * Desc: Get the number of topology node in the database
 */
LV_Bool_e LV_get_number_of_topo_nodes(int *p_totalNodeNum);

/*
 * Desc: Get the number of clients node in the database
 */
LV_Bool_e LV_get_number_of_client_nodes(int *p_totalClieNum);

/*
 * Desc: Get topology node info by mac address
 */
LV_Bool_e LV_read_topology_info_by_mac(Topo_File_Node_Info_s *p_topoNode, char *p_macAddr);

LV_Bool_e LV_get_cap_ip_address(char *p_ipAddr);

LV_Bool_e LV_get_number_online_clients(int *p_onlineClieNum);

void LV_convert_encryption_to_number(const char *p_encrytion, const char *p_sae, const char *p_owe, int *p_number);

void LV_convert_number_to_encryption(char *p_encrytion, char *p_sae, char *p_owe, int number);

/*
 *Desc: This function not include uci commit
 */
void LV_uci_set_config(const char *p_file, const char *p_section, const char *p_name, const char *p_value);

/*
 *Desc: This function not include uci commit
 */
void LV_uci_delete_config(const char *p_file, const char *p_section, const char *p_name);

/*
 *Desc: Desc: efficient function to get data: both list type and string type
 */
LV_Bool_e LV_uci_get_option_value(const char *p_path, char *p_value);

/*
 * Desc: efficient function to set data: both add new section and set value for section
 * In case want create new section please set p_option = NULL
 * Example: __LV_uci_set_option_value_for_section("network", "wan100", NULL, "service");
 * Included uci commit
 */
LV_Bool_e LV_uci_set_option_value_for_section(const char *p_package, const char *p_section, const char *p_option, const char *p_value);

/*
 *  Magical function for delete section
 *  Included uci commit
 */
LV_Bool_e LV_uci_delete_section_name(const char *p_configFile, char *p_sectionName);

/*
 *Desc:  Included uci commit
 */
LV_Bool_e LV_uci_delete_section_unname(const char *p_configFile, const char *p_keyName, char *p_keyValue, const char *p_type);

LV_Bool_e LV_check_config_is_pending();

void      LV_uci_commit_package(const char *p_package);

LV_Bool_e LV_get_radio(Wifi_Radio_s *p_radioEntry);

void      LV_apply_radio(int option);

/*
 *Desc: result = 1 for restart wifi; result = 0 do nothing; result = 2 pending
 */
LV_Bool_e LV_set_radio(const Wifi_Radio_s *p_radioEntry);

void      LV_init_struct_radio(Wifi_Radio_s *p_radioEntry, int numberEntry);

void LV_apply_dhcp6c(const int dhcp6C, const char *p_wanName);

void LV_apply_ewan(const char *p_wanName, const int option);

LV_Bool_e LV_get_ewan(Ewan_s *p_ewanEntry);

LV_Bool_e LV_get_network(Network_s *p_networkEntry);

LV_Bool_e LV_add_ewan(const char *p_vlanID);

LV_Bool_e LV_edit_ewan(Ewan_s *p_ewanEntry);

LV_Bool_e LV_remove_ewan(char *p_ewanSection);

void LV_init_struct_ewan(Ewan_s *p_ewanEntry, int numberEntry);

LV_Bool_e LV_get_client_monitoring_setting(Client_Monitoring_s *p_clientMonitor);

LV_Bool_e LV_set_client_monitoring_setting(const Client_Monitoring_s *p_clientMonitor);

LV_Bool_e LV_get_UMP_server(UMP_Server_s  *p_umpServer);

LV_Bool_e LV_set_UMP_server(const UMP_Server_s  *p_umpServer);

/*
 *Desc: Function remove and set group is legacy from delete_named_section and LV_set_parental_control 
 */
LV_Bool_e LV_get_parental_control(Access_Control_Group_s  *p_groupEntry);

LV_Bool_e LV_get_group_section(char *p_section, char *p_name);

LV_Bool_e LV_add_parental_control(const Access_Control_Config_s  *p_groupEntry);

LV_Bool_e LV_edit_parental_control(const Access_Control_Config_s  *p_groupEntry, char *p_oldRuleDay, char *p_oldRuleStart, char *p_oldRuleStop);

LV_Bool_e LV_delete_parental_control(char *p_groupName);

LV_Bool_e LV_add_mac_to_group(const Access_Control_Config_s  *p_groupEntry);

LV_Bool_e LV_remove_mac_in_group(const Access_Control_Config_s  *p_groupEntry);

LV_Bool_e LV_add_url_to_group(const Access_Control_Config_s  *p_groupEntry);

LV_Bool_e LV_remove_url_in_group(const Access_Control_Config_s  *p_groupEntry);

void      LV_apply_edit_lan(const Lan_Information_s *p_oldLanInfo, const Lan_Information_s *p_lanInfo, const int oldConfig);

/*
 *Desc: Function get all information of interface br-lan
 */
LV_Bool_e LV_get_network_lan_config(Lan_Information_s *p_lanInfo, char *p_sonBridge, int *p_enableRepeater);

/*
 *Desc: Function set all information of interface br-lan
 */
LV_Bool_e LV_set_network_lan_config(const Lan_Information_s *p_lanInfo);

/*
 *Desc: Function get ntp server
 */
LV_Bool_e LV_get_time_server( Time_Information_s *p_timeServer);

/*
 *Desc: Function set ntp server
 */
LV_Bool_e LV_set_time_server(const Time_Information_s *p_timeServer);

/*
 *Desc: Function set ntp server
 */
LV_Bool_e LV_get_ddns(DDNS_s *p_ddnsEntry);

LV_Bool_e LV_add_ddns(DDNS_s *p_ddnsEntry);

LV_Bool_e LV_remove_ddns(char *p_ddnsSection);

LV_Bool_e LV_get_port_forwarding(Port_Forwarding_s *p_portForward);

LV_Bool_e LV_set_port_forwarding(const Port_Forwarding_Entry_s *p_portForward);

LV_Bool_e LV_remove_port_forwarding(char *p_name);

LV_Bool_e LV_edit_port_forwarding(const Port_Forwarding_Entry_s *p_portForward);

LV_Bool_e LV_get_rule_firewall(Rule_Forwarding_s *p_portForward);

LV_Bool_e LV_set_rule_firewall(const Port_Forwarding_Entry_s *p_portForward);

LV_Bool_e LV_edit_rule_firewall(const Port_Forwarding_Entry_s *p_portForward);

LV_Bool_e LV_get_vlan(char vlanInfo[][STR_LEN_16]);

/*
 * Function remove vlan interface
 * action = 1 : restart service 
 * action = 0 : don't restart service
 */
void  LV_apply_vlan(int action);

void  LV_print_status(char *p_status, const int value);

LV_Bool_e LV_remove_vlan(const char *p_vlanID, int *p_action);

void __LV_get_option_vlan(int vlanInfo[]);

LV_Bool_e LV_add_vlan(const char *p_vlanID);

LV_Bool_e LV_edit_vlan(const char *p_oldVlan, const char *p_newVlan, int *p_action);

LV_Bool_e LV_get_home_wifi(Home_Information_s *p_homeInfo);

LV_Bool_e LV_get_device_info(Device_Information_s *p_deviceInfo);

LV_Bool_e LV_get_wan_network_info(Network_Wan_Information_s *p_networkInfo);

LV_Bool_e LV_get_firewall(Firewall_Zone_s *p_zone, Firewall_Forwarding_s *p_forwarding, char *p_listNetwork, int *p_numNet);

/*
 * action 0: mean remove both dest and source in forwarding rule
 * action 1: mean remove only source in forwarding rule
 */
LV_Bool_e LV_detele_firewall(const char *p_zoneName, const int action);

LV_Bool_e LV_add_firewall(const Firewall_Zone_s *p_zone, const Firewall_Forwarding_s *p_forwarding);

/*
 * function support editwan edit firewall
 *  action: 1 - add p_network to p_zonename
 *          0 - rm p_network from p_zonename
*/
LV_Bool_e LV_edit_firewall_zone_network(const char *p_zonename, const char *p_network, int action);

LV_Bool_e LV_join_to_mesh(const char *p_ssid, const char *p_password, const int option);

LV_Bool_e LV_update_mesh_config(const char* p_ssid, const char *p_password, const char *p_encryption, const int option);

LV_Bool_e LV_create_mesh(const char *p_ssid, const char *p_password, const int isBridge, const int meshOverEth);

LV_Bool_e LV_get_extend_mre_info(MRE_Extend_Info_s *p_entry, char *p_path);

LV_Bool_e LV_add_new_mre_info(MRE_Extend_Config_s *p_newReInfo, const int option);

LV_Bool_e LV_remove_mre_info(const MRE_Extend_Info_s *p_removedReInfo);

LV_Bool_e LV_update_mre_ap_info(char *p_mac, Uci_Config_Value_s *p_values, int numberValue, Option_Action_e option, char *p_ieee1905Mac);

// also use this function for get SSID_NAME in getHomeInfo
LV_Bool_e LV_get_backup_wifi_interface(Main_Ssid_s *p_mesh, char *p_path);

//for proccessing case charater `
void LV_processing_special_charater(char *p_input, const char charater, const int length);

void LV_apply_ddns(int option, char *p_entryName);

void LV_apply_firewall();

void LV_get_ipaddr_of_interface(char *p_iface, char *p_ipAddr, char *p_netmask);

void LV_get_ipv6addr_of_interface(char *p_iface, char *p_ipAddr);

int LV_get_gateway(char *p_iface, char *p_gateway);

int LV_update_dhcp6c(char *p_netName);

int LV_is_ipv4_address_valid(char *p_ipAddress);

LV_Bool_e LV_get_value_from_file(char *p_fileName, char *p_value);

LV_Bool_e LV_get_section_ddns(char *p_section, char *p_name);

LV_Bool_e LV_edit_ddns(Ddns_Entry_s *p_ddnsEntry);

Metric_e LV_GetMetricOfInterface(char *p_iface);

LV_Bool_e LV_get_guest_config(Guest_Paramters_s *p_guest_para);

// add a guest section config
LV_Bool_e LV_add_guest_config(Guest_Paramters_s guest_para);

// set a guest section config in home mode
LV_Bool_e LV_set_guest_config(Guest_Paramters_s guest_para);

// set a guest section config in enterprise mode
LV_Bool_e LV_set_guest_enterprise(Guest_Paramters_s guest_para);

void LV_apply_guest();

LV_Bool_e LV_get_ifname_Temporary_backhaul(char *p_ifname);

LV_Bool_e LV_get_environment_mode(char *p_mode);

LV_Bool_e LV_get_device_name_and_location(char *p_deviceName, char *p_location);

int LV_do_systemcall(const char *cmd, int cmdLength);

int LV_parse_json_entries(char *p_request, Json_Entries_s *p_entries, const char *p_optionName);

LV_Bool_e LV_check_mesh_mode(Device_Type_e *p_mode);

int LV_get_mesh_over_eth();

LV_Bool_e __LV_topo_get_wan_rate(int *p_isSta, char *p_iface, char *p_rssi, char *p_wanSpeed, char *p_ulSpeed, char *p_dlSpeed);

LV_Bool_e LV_topo_get_wanspeed(char *p_mode, char *p_wanIface, char *p_wantype, char *p_rssi, char *p_wanSpeed, char *p_ulSpeed, char *p_dlSpeed);

LV_Bool_e LV_topo_get_channel(char *p_channel);

int LV_increase_mac(char *mac, int offset, int increase);

LV_Bool_e __LV_get_iface_channel(char *p_iface, char *p_ifaceChan);

/* argument: Route_Section_s *p_routeInfo - point to an array route info */
LV_Bool_e LV_get_route_info(Route_Section_s *p_routeInfo);

/* add/set a section config : Route_Section_s *p_routeInfo */
LV_Bool_e LV_config_route(Route_Section_s *p_routeInfo);

/* argument: SC_Section_s *p_scInfo - point to an sc section info */
LV_Bool_e LV_get_sc(SC_Section_s *p_scInfo);

/* set a section config : SC_Section_s *p_scInfo */
LV_Bool_e LV_edit_sc(SC_Section_s *p_scInfo);

/* argument: AAA_Section_s *p_aaaInfo - point to an aaa server section info */
LV_Bool_e LV_get_aaa(AAA_Section_s *p_aaaInfo);

/* set a section config : AAA_Section_s *p_aaaInfo */
LV_Bool_e LV_edit_aaa(AAA_Section_s *p_aaaInfo);

/* get all network interface except lan and loopback interface */
LV_Bool_e LV_get_network_iface(char *p_networkIfaces);

/* set 'option network p_network' to section name in p_listWifiCfg
    eg. p_listWifiCfg: "cfg34556 cfg43215"
    if p_listWifiCfg="" <=> remove p_network in wireless
 */
LV_Bool_e LV_set_wireless_network(char *p_listWifiCfg, char *p_network);

/*
    find and replace p_word in p_string (words and spaces) - remove head, tail space,
    p_newWord == NULL; find p_word only
*/
LV_Bool_e LV_string_find_replace_word(char *p_string, char *p_word, char *p_newWord);

/*
    Get all wifi-iface section with: ssid name, device, mode, guest_ssid, 

*/
LV_Bool_e LV_get_wifi_iface_section(Wifi_Iface_Section_s *p_wifiSection);

/*
 	Remove element p_removeValue in value of option p_option.
 		p_sectionName == NULL -> search in all section type p_sectionType
		elements in value of p_option are separated by space
*/
LV_Bool_e LV_clean_option(char *p_configFile, char *p_sectionType, char *p_sectionName, char *p_option, char *p_removeValue);

/*
    Convert mtk value in number to mode hwmode, bandwidth
*/
LV_Bool_e LV_mtk_2_hwmode(char *p_bandwidth, char *p_hwMode, char *p_band);

/*
    Convert mode hwmode, bandwidth, standard to mtk value in number
*/
LV_Bool_e LV_hwmode_2_mtk(char *p_bandwidth, char *p_hwMode, char *p_band);

/*
    Convert txpower
*/
LV_Bool_e LV_dbm_2_per(char *p_txpower);

LV_Bool_e LV_per_2_dbm(char *p_txpower);

char *json_common_get_string(cJSON *js_obj, char *key);

LV_Bool_e topo_parse(char *buf, Topo_File_Node_Info_s *p_node, int *p_nNode);

int LV_set_value_to_file(char *p_fileName, char *p_value);

		
/* get all network interface except lan and loopback interface */
LV_Bool_e LV_get_network_iface(char *p_networkIfaces);
/* set 'option network p_network' to section name in p_listWifiCfg
    eg. p_listWifiCfg: "cfg34556 cfg43215"
    if p_listWifiCfg="" <=> remove p_network in wireless
 */
LV_Bool_e LV_set_wireless_network(char *p_listWifiCfg, char *p_network);
/*
    find and replace p_word in p_string (words and spaces) - remove head, tail space,
    p_newWord == NULL; find p_word only
*/
LV_Bool_e LV_string_find_replace_word(char *p_string, char *p_word, char *p_newWord);
/*
    Get all wifi-iface section with: ssid name, device, mode, guest_ssid, 
*/
LV_Bool_e LV_get_wifi_iface_section(Wifi_Iface_Section_s *p_wifiSection);
/*
 	Remove element p_removeValue in value of option p_option.
 		p_sectionName == NULL -> search in all section type p_sectionType
		elements in value of p_option are separated by space
*/
LV_Bool_e LV_clean_option(char *p_configFile, char *p_sectionType, char *p_sectionName, char *p_option, char *p_removeValue);
/*
    Convert mtk value in number to mode hwmode, bandwidth
*/
LV_Bool_e LV_mtk_2_hwmode(char *p_bandwidth, char *p_hwMode, char *p_band);
/*
    Convert mode hwmode, bandwidth, standard to mtk value in number
*/
LV_Bool_e LV_hwmode_2_mtk(char *p_bandwidth, char *p_hwMode, char *p_band);
/*
    Convert txpower
*/
LV_Bool_e LV_dbm_2_per(char *p_txpower);
LV_Bool_e LV_per_2_dbm(char *p_txpower);
char *json_common_get_string(cJSON *js_obj, char *key);
LV_Bool_e topo_parse(char *buf, Topo_File_Node_Info_s *p_node, int *p_nNode);
int LV_set_value_to_file(char *p_fileName, char *p_value);

/*
return: 0 if fail
        1 if success
*/
int LV_validate_string(char *p_value);
int LV_validate_no_space(char *p_value);
int LV_validate_all_space(char *p_value);
/*
 minLen <= length of p_value <= maxLen
*/
int LV_validate_length(char *p_value, int minLen, int maxLen);
/*
    format ipaddr ver4: x.x.x.x
    eg. 255.255.255.0
*/
int LV_validate_ipaddr4_netmask(char *p_value);
int LV_validate_mac(char *p_value);
/*
    validate min <= value <= max
*/
int LV_validate_range(char *value, int min, long long max);
int LV_validate_url(char *p_value);
int LV_is_ipv6_address_valid(char *p_ipAddress);
int LV_is_ipv4_address_valid(char *p_ipAddress);
int LV_validate_channel(char *p_value, int channel);
int LV_validate_bandwidth(char *p_value, int band);
int LV_validate_def(char *p_value, int maxLength);
int LV_validate_enable(char *p_value);
int LV_validate_country(char *p_value);
int LV_validate_channel_opt(char *p_value);
int LV_validate_wirelessSecurity(char *p_value);
int LV_validate_service(char *p_value);
int LV_validate_ip_stack(char *p_value);
int LV_validate_timeZone(char *p_value);
int LV_validate_firewall(char *p_value);
int LV_validate_standard(char *p_value, int band);
int LV_validate_subnet(char *p_value);




#endif /* LIB_VNPT_H_ */
