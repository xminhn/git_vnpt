#ifndef __LOG_H__
#define __LOG_H__
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

extern unsigned char log_run_level;
extern const char *log_level_strings[];

#define LOG(level, fmt, ...) LOG_printf_info(level, fmt, __VA_ARGS__)
#define LOG_LVL_NONE 0
#define LOG_LVL_ERROR 1
#define LOG_LVL_WARN 2
#define LOG_LVL_DEBUG 3

/* Describes: This function is responsible for setting the logging level for the program
 *
 * Parameter:
 *   level: log value want to set
 * 
 *  Output:
 *    If the value of level is valid, log_run_level will be assigned that value, 
 *    otherwise the default value will be LOG_LVL_ERROR
 */
void LOG_set_level(int level);

/* Describes: This function allows application print log in file log
 *
 * Parameter:
 *   level: Log level, used to determine the severity of the log message.
 *          This level is compared to the global variable log_run_level to decide whether to log or not.
 *   format: The log format string, similar to the format string in printf or snprintf. 
 *           It can contain variable parameters that you want to log.
 *   ...: Variadic arguments, used to pass values ​​to the format string.
 *
 *  Output:
 *    Write a message to the log file
 */
void LOG_printf_info(int level, const char *format, ...);

/* Describes: This function is a custom implementation for formatted output, similar to the standard printf.
 *            It processes a format string and a variable list of arguments, and writes the formatted 
 *            message to the console or a log file (depending on implementation).
 *
 * Parameters:
 *   format: A string containing text and format specifiers (e.g., %d, %s, %f). 
 *             These specifiers will be replaced by the corresponding values from the variadic arguments.
 *   ...: Variadic arguments that correspond to the placeholders in the format string.
 *
 * Output:
 *   Prints the formatted message to the console.
 *   Depending on the implementation, it can also log the message to a file or other output stream.
 */
void PRINTF(const char *format, ...);

#endif /*__LOG_H__*/


// #ifndef LOG_H
// #define LOG_H

// #include <stdarg.h>

// #define BUFFER_SIZE 1024

// /**
//  * @brief Log levels for controlling verbosity of log messages
//  */
// enum {
//     LOG_LVL_NONE = 0,   /**< No logging */
//     LOG_LVL_ERROR,      /**< Only error messages */
//     LOG_LVL_WARN,       /**< Error and warning messages */
//     LOG_LVL_DEBUG       /**< All messages (errors, warnings, and debug info) */
// };

// /**
//  * @brief Configuration structure for the logging system
//  */
// typedef struct {
//     char log_file_path[256];  /**< Path to the log file */
//     unsigned char log_level;  /**< Current logging level */
// } LoggerConfig;

// /**
//  * @brief Global logger configuration
//  */
// extern LoggerConfig logger_config;

// /**
//  * @brief Initialize the logging system
//  * 
//  * Sets up the log file and prepares the logging system for use.
//  */
// void init_logger(void);

// /**
//  * @brief Clean up resources used by the logging system
//  */
// void cleanup_logger(void);

// /**
//  * @brief Set the logging verbosity level
//  * 
//  * @param level New log level (LOG_LVL_NONE, LOG_LVL_ERROR, LOG_LVL_WARN, or LOG_LVL_DEBUG)
//  */
// void set_log_level(int level);

// /**
//  * @brief Change the log file path
//  * 
//  * @param file_path Path to the new log file
//  */
// void set_log_file(const char *file_path);

// /**
//  * @brief Log a message with the specified level
//  * 
//  * If the current log_level is set to a value lower than the specified level,
//  * the message will not be logged.
//  * 
//  * @param level Logging level for this message
//  * @param format Printf-style format string
//  * @param ... Variable arguments for the format string
//  */
// void log_message(int level, const char *format, ...);

// #endif