#ifndef __LOGGER_MODULE_HPP__
#define __LOGGER_MODULE_HPP__

#include "LoggingAbstraction.hpp"
#include "Global.hpp"

class Logger : public LoggingAbstraction, public Global<Logger>{
    public:
    Logger() : LoggingAbstraction() {}
    virtual ~Logger() = default;

    virtual ErrorType log(const LogType type, const char *tag, const char *format, ...) override;
    virtual ErrorType logBuffer(const LogType logType, const char *tag, const char *buffer, Bytes length) override;
};

#endif // __LOGGER_MODULE_HPP__