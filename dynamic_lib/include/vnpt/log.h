/*
 * @File        : log.h
 * @Description :
 * @Author      : HEC, VNPT-Technology
 */

#ifndef LOG_H_
#define LOG_H_

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include <syslog.h>
#include "basic_defs.h"

#define LOG_MESSAGE_LEN     512
#define TIME_STR_LEN        20
#define HEADER_SIZE_MAX     25

typedef enum
{
    SYSLOG_ERROR = 0,
    SYSLOG_ALARM = 1,
    SYSLOG_NOTICE = 2
} Syslog_Level_e;

#define LOG(module, logMsg, args ...)                           LG_write_to_screen(module, 0, logMsg"\n", __FILE__, __LINE__, ## args);

#define ALARM(module, logMsg, args ...)                         LG_write_to_screen(module, 1, logMsg"\n", __FILE__, __LINE__, ## args);

#define SYS_LOG(module, syslogLevel, syslogMsg, args ...)                           \
    do                                                                              \
    {                                                                               \
        LG_write_to_syslog(module, syslogMsg, syslogLevel, ## args);                \
        LG_write_to_screen(module, 0, syslogMsg"\n", __FILE__, __LINE__, ## args);  \
    }                                                                               \
    while (0);

void LG_write_to_screen(const char *p_module, int flag, const char *p_format, const char *p_filePath, int fileLine, ...);

void LG_write_to_syslog(const char *p_module, const char *p_msgLog, Syslog_Level_e syslogLevel, ...);


#endif /* LOG_H_ */
