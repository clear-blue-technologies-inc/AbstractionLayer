#ifndef __LOGGER_MODULE_HPP__
#define __LOGGER_MODULE_HPP__

//Abstractions
#include "LoggingAbstraction.hpp"
//Utilities
#include "Global.hpp"
#include "EventQueue.hpp"

class Logger : public LoggingAbstraction, public Global<Logger>, public EventQueue {
    public:
    Logger() : LoggingAbstraction(), EventQueue() {}
    virtual ~Logger() = default;

    virtual ErrorType log(const LogType type, const char *tag, const char *format, ...) override;
    virtual ErrorType logBuffer(const LogType logType, const char *tag, const char *buffer, Bytes length) override;
};

#endif // __LOGGER_MODULE_HPP__