/**************************************************************************//**
* @author Ben Haubrich                                        
* @file   CbtLog.hpp
* @details \b Synopsis: \n Pretty printed logs
* @ingroup FoundationCore
*******************************************************************************/
#ifndef __CBT_LOG_HPP__
#define __CBT_LOG_HPP__

//Modules
#include "LoggerModule.hpp"
//C
#include <inttypes.h>

#if CONFIG_LOG_COLORS
#define LOG_COLOR_BLACK   "30"
#define LOG_COLOR_RED     "31"
#define LOG_COLOR_GREEN   "32"
#define LOG_COLOR_BROWN   "33"
#define LOG_COLOR_BLUE    "34"
#define LOG_COLOR_PURPLE  "35"
#define LOG_COLOR_CYAN    "36"
#define LOG_COLOR(COLOR)  "\033[0;" COLOR "m"
#define LOG_BOLD(COLOR)   "\033[1;" COLOR "m"
#define LOG_RESET_COLOR   "\033[0m"
#define LOG_COLOR_E       LOG_COLOR(LOG_COLOR_RED)
#define LOG_COLOR_W       LOG_COLOR(LOG_COLOR_BROWN)
#define LOG_COLOR_I       LOG_COLOR(LOG_COLOR_GREEN)
#define LOG_COLOR_D
#define LOG_COLOR_V
#else //CONFIG_LOG_COLORS
/// @def LOG_COLOR_E
/// @brief The color for error messages
#define LOG_COLOR_E
/// @def LOG_COLOR_W
/// @brief The color for warnings
#define LOG_COLOR_W
/// @def LOG_COLOR_I
/// @brief The color for information messages
#define LOG_COLOR_I
/// @def LOG_COLOR_D
/// @brief The color for debugging messages
#define LOG_COLOR_D
/// @def LOG_COLOR_V
/// @brief The color for verbose messages
#define LOG_COLOR_V
/// @def LOG_RESET_COLOR
/// @brief Used to stop writing text that is coloured.
#define LOG_RESET_COLOR
#endif //CONFIG_LOG_COLORS

/**
 * @def LOG_FORMAT
 * @brief The format of a log message
 * @param letter The letter that represents the type of the log (warning, info, debug, etc.)
 * @param format The format of the log. Format is a printf-style format
 */
#define LOG_FORMAT(letter, format)  LOG_COLOR_ ## letter #letter " %s: " format LOG_RESET_COLOR "\n"

/**
 * @def CBT_LOG_LEVEL
 * @brief Print a formatted log message to the terminal.
 * @param[in] type The type of the log
 * @sa Fnd::LogType
 * @param[in] tag The tag of the log. Can be any string you like.
 * @param[in] format The format of the log. Format is a printf-style format string
 * @param[in] ... The arguments to the format string
 */
#define CBT_LOG_LEVEL(type, tag, format, ...) do {                             \
    if      (type == LogType::Error)   { Logger::Instance().log(type, tag, LOG_FORMAT(E, format), tag, ##__VA_ARGS__); } \
    else if (type == LogType::Warning) { Logger::Instance().log(type, tag, LOG_FORMAT(W, format), tag, ##__VA_ARGS__); } \
    else if (type == LogType::Info)    { Logger::Instance().log(type, tag, LOG_FORMAT(I, format), tag, ##__VA_ARGS__); } \
    else if (type == LogType::Debug)   { Logger::Instance().log(type, tag, LOG_FORMAT(D, format), tag, ##__VA_ARGS__); } \
} while(0)

/**
 * @def CBT_LOGE(tag, format, ...)
 * @brief Log an error
 * @param[in] tag The tag of the log
 * @param[in] format The format of the log. Format is a printf-style format string
 * @post Logs a message to stdout. If the terminal supports colored output, the message is printed in red.
 */
#define CBT_LOGE(tag, format, ... ) CBT_LOG_LEVEL(LogType::Error,   tag, format, ##__VA_ARGS__)

/**
 * @def CBT_LOGW(tag, format, ...)
 * @brief Log an error
 * @param[in] tag The tag of the log
 * @param[in] format The format of the log. Format is a printf-style format string
 * @note format is a printf-style format string
 * @post Logs a message to stdout. If the terminal supports colored output, the message is printed in yellow.
 */
#define CBT_LOGW(tag, format, ... ) CBT_LOG_LEVEL(LogType::Warning, tag, format, ##__VA_ARGS__)

/**
 * @def CBT_LOGI(tag, format, ...)
 * @brief Log an error
 * @param[in] tag The tag of the log
 * @param[in] format The format of the log. Format is a printf-style format string
 * @post Logs a message to stdout. If the terminal supports colored output, the message is printed in green
 */
#define CBT_LOGI(tag, format, ... ) CBT_LOG_LEVEL(LogType::Info,    tag, format, ##__VA_ARGS__)

/**
 * @def CBT_LOGD(tag, format, ...)
 * @brief Log an error
 * @param[in] tag The tag of the log
 * @param[in] format The format of the log. Format is a printf-style format string
 * @post Logs a message to stdout.
 */
#define CBT_LOGD(tag, format, ... ) CBT_LOG_LEVEL(LogType::Debug,   tag, format, ##__VA_ARGS__)

/**
 * @def CBT_LOG_BUFFER_HEXDUMP(tag, buffer, buff_len, level)
 * @brief Dump the memory contents of a buffer.
 * @param[in] tag The log tag. Can be whatever you want to help you identify where the log came from.
 * @param[in] buffer The buffer to dump
 * @param[in] buff_len The length of the buffer
 * @param[in] type The type of the log
*/
#define CBT_LOG_BUFFER_HEXDUMP(tag, buffer, buff_len, type) do { \
    Logger::Instance().logBuffer(type, tag, buffer, buff_len); \
} while(0);
#endif // __CBT_LOG_HPP__