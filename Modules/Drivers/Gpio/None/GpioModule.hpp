//AbstractionLayer
#include "GpioAbstraction.hpp"
//C++
#include <cstdint>

class Gpio : public GpioAbstraction {

    public:
    Gpio() : GpioAbstraction() {}
    ~Gpio() = default;

    ErrorType pinWrite(GpioLogicLevel logicLevel) override;
    ErrorType configure(uint32_t *basePeripheralRegister, PinNumber pinNumber, GpioPinDirection direction, GpioInterruptMode interruptMode, bool pullUpEnable, bool pullDownEnable) override;
};