#include "GpioModule.hpp"
//Common
#include "Log.hpp"
//ESP
#include "driver/gpio.h"

ErrorType Gpio::pinWrite(const GpioLogicLevel &logicLevel) {
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

ErrorType Gpio::pinRead(GpioLogicLevel &logicLevel) {
    if (1 == gpio_get_level(toEspPinNumber(pinNumber()))) {
        logicLevel = GpioLogicLevel::High;
    }
    else {
        logicLevel = GpioLogicLevel::Low;
    }

    return ErrorType::Success;
}

ErrorType Gpio::configure(const uint32_t *basePeripheralRegister, const PinNumber pinNumber, const GpioPinDirection direction, const GpioInterruptMode interruptMode, const bool pullUpEnable, const bool pullDownEnable) {
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

    if (pullDownEnable && pullUpEnable) {
        return ErrorType::InvalidParameter;
    }

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