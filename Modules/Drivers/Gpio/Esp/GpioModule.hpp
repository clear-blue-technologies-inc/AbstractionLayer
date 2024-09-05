//Foundation
#include "GpioAbstraction.hpp"
//Standard library
#include <stdint.h>

#include "hal/gpio_types.h"

class Gpio : public GpioAbstraction {

    public:
    Gpio() : GpioAbstraction() {}
    virtual ~Gpio() = default;

    ErrorType pinWrite(GpioLogicLevel logicLevel) override;
    ErrorType configure(uint32_t *basePeripheralRegister, PinNumber pinNumber, GpioPinDirection direction, GpioInterruptMode interruptMode, bool pullUpEnable, bool pullDownEnable) override;

    gpio_num_t toEspPinNumber(PinNumber pinNumber) {
        return static_cast<gpio_num_t>(pinNumber);
    }
};