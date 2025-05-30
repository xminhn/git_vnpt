/*
 * @File        : basic_defs.h
 * @Description :
 * @Author      : HEC, VNPT-Technology
 */
#ifndef BASIC_DEFS_H_
#define BASIC_DEFS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>
#include <malloc.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <sys/stat.h>

typedef unsigned int        uint;
typedef unsigned long       ulong;
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;
typedef signed char         int8;
typedef signed short        int16;
typedef signed int          int32;
typedef signed long long    int64;
typedef float               float32;
typedef double              float64;
typedef int                 Socket_fd;

#define FREE_OBJECT(obj_name)       {if(obj_name != NULL) {free(obj_name); obj_name = NULL;}}

#define MA_CREATE_STRING(str_name, str_length)              { str_name = (char*) (malloc(sizeof(char) * ((str_length)+1)));}
#define MA_CREATE_STRING_FROM_STRING(str_name, str)     { if(str != NULL){MA_CREATE_STRING(str_name, strlen(str)); strcpy((str_name), str);}}

#ifndef MAX
#define MAX(a,b) ( (a) > (b) ? (a) : (b) )
#endif

#define MAX_BUFF_LEN                65536
#define MAC_ADDR_STR_LEN            18
#define LONG_STR_LEN                256
#define MEDIUM_STR_LEN              128
#define MAX_STR_LEN                 1024
#define NORMAL_STR_LEN              64
#define SHORT_STR_LEN               16
#define IP_ADDR_LEN                 16
#define IP6_ADDR_LEN                46
#define MAX_NUM_CONNECTION          10
#define CONNECTION_TIMEOUT          120
#define NOT_SOCKET                  -1
#define MAX_BROADCAST_CONNECTION    2
#define MAX_TCP_LISTEN              10
#define MAX_ELEMENT_LENGTH          256
#define MAX_ELEMENT_COUNT           100
#define MAX_LINE_IN_SECTION_CONFIG  10
#define LOG_HEADER_SIZE_MAX         25
#define TIME_STR_LEN                20
#define MAX_HEX_STR_LEN             2

#define STR_LEN_4096                4096
#define STR_LEN_2048                2048
#define STR_LEN_1024                1024
#define STR_LEN_512                 512
#define STR_LEN_256                 256
#define STR_LEN_128                 128
#define STR_LEN_64                  64
#define STR_LEN_32                  32
#define STR_LEN_16                  16
#define STR_LEN_8                   8
#define STR_LEN_4                   4
#define STR_LEN_2                   2
#define STR_LEN_20                  20

typedef char MacAddr[MAC_ADDR_STR_LEN];

typedef enum
{
    MA_FALSE = 0,
    MA_TRUE = 1
} Bool_e;

typedef enum
{
    LV_FALSE   = 0,
    LV_TRUE    = 1,
    LV_PENDING = 2
} LV_Bool_e;

#endif /* BASIC_DEFS_H_ */
