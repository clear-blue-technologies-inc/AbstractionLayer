#include "GpioModule.hpp"

ErrorType Gpio::pinWrite(GpioLogicLevel logicLevel) {
    return ErrorType::NotImplemented;
}

ErrorType Gpio::configure(uint32_t *basePeripheralRegister, PinNumber pinNumber, GpioPinDirection direction, GpioInterruptMode interruptMode, bool pullUpEnable, bool pullDownEnable) {
    return ErrorType::NotImplemented;
}