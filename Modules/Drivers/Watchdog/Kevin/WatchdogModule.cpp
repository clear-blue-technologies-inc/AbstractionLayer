//Modules
#include "GpioModule.hpp"
#include "WatchdogModule.hpp"
//AbstractionLayer
#include "Types.hpp"

//C
#include <unistd.h>
#include <assert.h>

ErrorType Watchdog::startPetting() {
    const PinNumber io = 2;

    _stopPetting = false;
    _watchdogGpio = std::make_unique<Gpio>();
    assert(nullptr != _watchdogGpio);
    return _watchdogGpio->configure(nullptr, io, GpioPinDirection::DigitalOutput, GpioInterruptMode::Disabled, false, false);
}

ErrorType Watchdog::stopPetting() {
    _stopPetting = true;
    return ErrorType::Success;
}

ErrorType Watchdog::pet() {
    ErrorType error = ErrorType::Failure;
    assert(nullptr != _watchdogGpio);

    _watchdogGpio->pinWrite(GpioLogicLevel::Low);
    usleep(15E6);
    error = _watchdogGpio->pinWrite(GpioLogicLevel::High);
    usleep(1);

    return error;
}