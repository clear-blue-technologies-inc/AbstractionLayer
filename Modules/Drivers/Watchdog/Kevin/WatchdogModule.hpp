#ifndef __WATCHDOG_MODULE_HPP__
#define __WATCHDOG_MODULE_HPP__

//Foundation
#include "WatchdogAbstraction.hpp"
//C++
#include <memory>

class Gpio;

class Watchdog : public WatchdogAbstraction{
    
    public:
    Watchdog() : WatchdogAbstraction() {}
    ~Watchdog() = default;

    ErrorType startPetting() override;
    ErrorType stopPetting() override;
    ErrorType pet() override;

    private:
    bool _stopPetting = true;
    std::unique_ptr<Gpio> _watchdogGpio = nullptr;
};

#endif // __WATCHDOG_MODULE_HPP__