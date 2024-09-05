#include "GpioModule.hpp"
//Common
#include "Log.hpp"
//ESP
#include "driver/gpio.h"

ErrorType Gpio::pinWrite(GpioLogicLevel logicLevel) {
    if (GpioLogicLevel::High == logicLevel) {
        return toPlatformError(gpio_set_level(toEspPinNumber(pinNumber()), 1));
    }
    else if (GpioLogicLevel::Low == logicLevel) {
        return toPlatformError(gpio_set_level(toEspPinNumber(pinNumber()), 0));
    }
    else {
        return ErrorType::InvalidParameter;
    }

    return ErrorType::Success;
}

ErrorType Gpio::configure(uint32_t *basePeripheralRegister, PinNumber pinNumber, GpioPinDirection direction, GpioInterruptMode interruptMode, bool pullUpEnable, bool pullDownEnable) {
    gpio_config_t gpioConfig;

    switch (interruptMode) {
        case GpioInterruptMode::Disabled:
            gpioConfig.intr_type = GPIO_INTR_DISABLE;
            break;
        case GpioInterruptMode::FallingEdge:
            gpioConfig.intr_type = GPIO_INTR_NEGEDGE;
            break;
        case GpioInterruptMode::HighLevel:
            gpioConfig.intr_type = GPIO_INTR_HIGH_LEVEL;
            break;
        case GpioInterruptMode::LowLevel:
            gpioConfig.intr_type = GPIO_INTR_LOW_LEVEL;
            break;
        case GpioInterruptMode::RisingEdge:
            gpioConfig.intr_type = GPIO_INTR_POSEDGE;
            break;
        case GpioInterruptMode::RisingOrFallingEdge:
            gpioConfig.intr_type = GPIO_INTR_ANYEDGE;
            break;
        default:
            return ErrorType::InvalidParameter;
    }

    switch (direction) {
        case GpioPinDirection::DigitalInput:
            gpioConfig.mode = GPIO_MODE_INPUT;
            break;
        case GpioPinDirection::DigitalOutput:
            gpioConfig.mode = GPIO_MODE_OUTPUT;
            break;
        default:
            return ErrorType::InvalidParameter;
    }

    //TODO: Invalid pin number?
    gpioConfig.pin_bit_mask = ((uint32_t)1 << pinNumber);
    _pinNumber = pinNumber;

    if (pullDownEnable) {
        gpioConfig.pull_down_en = GPIO_PULLDOWN_ENABLE;
    }
    else {
        gpioConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    }

    if (pullUpEnable) {
        gpioConfig.pull_up_en = GPIO_PULLUP_ENABLE;
    }
    else {
        gpioConfig.pull_up_en = GPIO_PULLUP_DISABLE;
    }

    return toPlatformError(gpio_config(&gpioConfig));
}