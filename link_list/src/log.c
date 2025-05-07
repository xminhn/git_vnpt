#include "log.h"
#include "time.h"
#include <string.h>
#include "defines.h"
#include <sys/stat.h>

const char *log_level_strings[] =
    {
        "NONE",
        "ERROR",
        "WARN",
        "DEBUG"};

unsigned char log_run_level;
char buffer[1000];
int is_first_log = 1;
int log_enable = 0;

void LOG_set_level(int level)
{
    if (level >= LOG_LVL_NONE && level <= LOG_LVL_DEBUG)
    {
        log_run_level = level;
    }
    else
    {
        log_run_level = 1;
    }
}

void LOG_printf_info(int level, const char *format, ...)
{
    if (level <= log_run_level)
    {
        static long current_position = 0;
        FILE *log_file;
        va_list args;
        time_t now;
        struct tm *time_info;
        char time_buffer[20];
        struct stat log_stat;
        long header_size = 0;

        if (stat(LOG_FILE_PATH, &log_stat) == 0)
        {
            FILE *read_file = fopen(LOG_FILE_PATH, "r");
            if (read_file)
            {
                if (fscanf(read_file, "current_position = %ld\n", &current_position) != 1)
                {
                    printf("Error reading file!\n");
                }
                fscanf(read_file, "current_position = %ld\n", &current_position);

                header_size = ftell(read_file);
                fclose(read_file);
            }

            if (log_stat.st_size >= MAX_LOG_SIZE)
            {
                current_position = header_size;
            }
        }
        else
        {
            current_position = 0;
        }

        time(&now);
        time_info = localtime(&now);
        strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", time_info);

        log_file = fopen(LOG_FILE_PATH, "r+");
        if (log_file == NULL)
        {
            log_file = fopen(LOG_FILE_PATH, "w");
            if (log_file == NULL)
            {
                perror("Failed to open log file");
                exit(EXIT_FAILURE);
            }
            fprintf(log_file, "current_position = %ld\n", header_size);
            fflush(log_file);
            current_position = header_size;
        }

        char buffer[1000];
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        int msg_size = snprintf(NULL, 0, "[%s] [%s] %s\n", time_buffer, log_level_strings[level], buffer);
        va_end(args);

        if (current_position + msg_size >= MAX_LOG_SIZE)
        {
            current_position = header_size;
        }

        fseek(log_file, current_position, SEEK_SET);
        char buffer_temp[1000] = {0};
        fgets(buffer_temp, sizeof(buffer_temp), log_file);
        int msg_size_old = strlen(buffer_temp);
        fseek(log_file, current_position, SEEK_SET);
        for (int i = 0; i < msg_size_old; i++)
        {
            fputc(' ', log_file);
        }
        fflush(log_file);

        fseek(log_file, current_position, SEEK_SET);
        fprintf(log_file, "[%s] [%s] %s\n", time_buffer, log_level_strings[level], buffer);
        fflush(log_file);

        current_position = ftell(log_file);

        fseek(log_file, 0, SEEK_SET);
        fprintf(log_file, "current_position = %ld\n", current_position);
        fflush(log_file);
        fclose(log_file);
    }
}

void PRINTF(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

// void LOG_printf_info(int level, const char *format, ...)
// {
//     if (level <= log_run_level)
//     {
//         FILE *log_file;
//         va_list args;
//         time_t now;
//         struct tm *time_info;
//         char time_buffer[20];
//         struct stat log_stat;

//         if (stat(LOG_FILE_PATH, &log_stat) == 0 && log_stat.st_size >= MAX_LOG_SIZE)
//         {
//             is_first_log = 1;
//         }

//         time(&now);
//         time_info = localtime(&now);
//         strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", time_info);

//         if (is_first_log)
//         {
//             log_file = open_file(LOG_FILE_PATH, "w");
//             is_first_log = 0;
//         }
//         else
//         {
//             log_file = open_file(LOG_FILE_PATH, "a");
//         }

//         if (log_file == NULL)
//         {
//             perror("Failed to open log file");
//             exit(EXIT_FAILURE);
//         }

//         va_start(args, format);
//         vsnprintf(buffer, sizeof(buffer), format, args);
//         fprintf(log_file, "[%s] [%s] %s", time_buffer, log_level_strings[level], buffer);
//         fflush(log_file);
//         va_end(args);
//         fclose(log_file);
//     }
// }

// /**
//  * @file log.c
//  * @brief Implementation of the logging system
//  * 
//  * Provides functions for logging messages with different severity levels
//  * to both file and standard error output.
//  */
// #include "log.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <time.h>
// #include <stdarg.h>
// #include <pthread.h>
// #include "defines.h"

// /**
//  * @brief Maximum size of a single log line
//  */
// #define MAX_LOG_LINE_SIZE 2048

// /**
//  * @brief Default logger configuration
//  */
// LoggerConfig logger_config = {
//     .log_file_path = "application.log",
//     .log_level = LOG_LVL_DEBUG
// };

// /**
//  * @brief Mutex for thread-safe logging
//  */
// static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

// /**
//  * @brief String representations of log levels
//  */
// static const char *log_level_names[] = {
//     "NONE",
//     "ERROR",
//     "WARN",
//     "DEBUG"
// };

// void init_logger(void) {
//     FILE *log_file = fopen(logger_config.log_file_path, "a");
//     if (log_file) {
//         fclose(log_file);
//     } else {
//         fprintf(stderr, "Cannot open log file %s. Using stderr for logging.\n", logger_config.log_file_path);
//     }
// }

// void cleanup_logger(void) {
// }

// void set_log_level(int level) {
//     if (level >= LOG_LVL_NONE && level <= LOG_LVL_DEBUG) {
//         logger_config.log_level = level;
//     }
// }

// void set_log_file(const char *file_path) {
//     if (file_path) {
//         strncpy(logger_config.log_file_path, file_path, sizeof(logger_config.log_file_path) - 1);
//         logger_config.log_file_path[sizeof(logger_config.log_file_path) - 1] = '\0';
        
//         FILE *log_file = fopen(logger_config.log_file_path, "a");
//         if (log_file) {
//             fclose(log_file);
//         } else {
//             fprintf(stderr, "Cannot open new log file %s. Using previous log file.\n", file_path);
//         }
//     }
// }

// void log_message(int level, const char *format, ...) {
//     if (level <= LOG_LVL_NONE || level > LOG_LVL_DEBUG || level > logger_config.log_level) {
//         return;
//     }
    
//  // get the current time
//  time_t now = time(NULL);
//  struct tm tm_now;
//  localtime_r(&now, &tm_now);
 
//  char time_str[20];
//  strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", &tm_now);
 
//  // log message buffer
//  char log_buffer[MAX_LOG_LINE_SIZE];
 
//  // Header with timestamp and log level
//  int header_len = snprintf(log_buffer, sizeof(log_buffer), "[%s] %s: ", 
//                           time_str, log_level_names[level]);
 
//  if (header_len < 0 || (size_t)header_len >= sizeof(log_buffer)) {
//      return; 
//  }
 
//  // log content
//  va_list args;
//  va_start(args, format);
//  int content_len = vsnprintf(log_buffer + header_len, sizeof(log_buffer) - header_len, 
//                            format, args);
//  va_end(args);
 
//  if (content_len < 0) {
//      return; // 
//  }
 
// // Ensure there is a newline character at the end
//  size_t total_len = header_len + content_len;
//  if (total_len < sizeof(log_buffer) - 2) {
//      if (log_buffer[total_len - 1] != '\n') {
//          log_buffer[total_len] = '\n';
//          log_buffer[total_len + 1] = '\0';
//      }
//  } else {
//    // Add a newline character for overly long strings
//      log_buffer[sizeof(log_buffer) - 2] = '\n';
//      log_buffer[sizeof(log_buffer) - 1] = '\0';
//  }
 
//  pthread_mutex_lock(&log_mutex);
 
// // Write log to file
//  FILE *log_file = fopen(logger_config.log_file_path, "a");
//  if (log_file) {
//      fputs(log_buffer, log_file);
//      fclose(log_file);
//  } else {
//      fputs(log_buffer, stderr);
//  }
 
//  pthread_mutex_unlock(&log_mutex);
// }