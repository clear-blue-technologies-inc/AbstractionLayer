/***************************************************************************//**
* @author   Ben Haubrich
* @file     LoggingAbstraction.hpp
* @details  Logging for Foundation core softwarez.
* @ingroup  AbstractionLayer
*******************************************************************************/
#ifndef __LOGGING_ABSTRACTION_HPP__
#define __LOGGING_ABSTRACTION_HPP__

//AbstractionLayer
#include "Error.hpp"
#include "Types.hpp"
//C++
#include <string>

/**
 * @enum LogType
 * @brief Types of logging messages. Used with CBT_LOGx maacros to print to the terminal in a colour matching the
 *        log type.
*/
enum class LogType : uint8_t {
    Unknown = 0, ///< Unknown log type.
    Info = 1,    ///< Informational messages that would make sense to a user.
    Warning,     ///< Warning messages that would can be catered towards users and developers.
    Debug,       ///< Debugging message that are for developers only.
    Error        ///< Error messages that can be catered towards users or developers.
};

/**
 * @class LoggingAbstraction
 * @brief A logging abstraction for Foundation core software. Can be implemented by any logging framework
*/
class LoggingAbstraction {

    public:
    /// @brief Default constructor.
    LoggingAbstraction() = default;
    /// @brief Default destructor.
    virtual ~LoggingAbstraction() = default;

    /**
     * @brief Prints a message to the terminal.
     * @param logType The type of log message.
     * @sa LogType
     * @param tag The tag portion of the log message.
     * @param format Printf-style format string for the message.
     * @verbatim Message structure: <time> <tag> <message> @endverbatim
    */
    virtual ErrorType log(const LogType logType, const char *tag, const char *format, ...) = 0;
    /**
     * @brief Prints a message to the terminal.
     * @details The message is printed in a hexdump format.
     * @param logType The type of log message.
     * @sa LogType
     * @param tag The tag portion of the log message.
     * @param data The buffer to be printed in hexdump format.
     * @param length The length of the buffer.
     * @verbatim Message structure: <time> <tag> <message> @endverbatim
    */
    virtual ErrorType logBuffer(const LogType logType, const char *tag, const char *data, Bytes length) = 0;
};

#endif // __LOGGING_ABSTRACTION_HPP__