/***************************************************************************//**
* @author   Ben Haubrich
* @file     CbtGpioAbstraction.hpp
* @details  Abstraction layer for GPIO
* @ingroup  FoundationCore
*******************************************************************************/
#ifndef __CBT_GPIO_ABSTRACTION_HPP__
#define __CBT_GPIO_ABSTRACTION_HPP__

//Standard library
#include <stdint.h>
//Foundation
#include "CbtError.hpp"
#include "CbtTypes.hpp"

/**
 * @enum GpioPinDirection
 * @brief Pin direction
*/
enum class GpioPinDirection : uint8_t {
    DigitalUnknown = 0, ///< Unknown pin direction
    DigitalInput,       ///< Pin is digital input
    DigitalOutput       ///< Pin is digital output
};

/**
 * @enum GpioInterruptMode
 * @brief The state of the signal that will trigger an interrupt
*/
enum class GpioInterruptMode : uint8_t {
    Unknown,             ///< Unknown interrupt mode
    LowLevel,            ///< Logic low triggers an
    HighLevel,           ///< Logic high triggers an interrupt
    RisingEdge,          ///< The rising edge triggers an interrupt
    FallingEdge,         ///< The falling edge triggers an interrupt
    RisingOrFallingEdge, ///< Both rising and falling edge
    Disabled             ///< Interrupts are disabled
};

/**
 * @enum GpioLogicLevel
 * @brief Logic level
*/
enum class GpioLogicLevel : uint8_t {
    Unknown = 0, ///< Unknown logic level
    Low,        ///< Low logic level
    High        ///< High logic level
};

/**
 * @class GpioAbstraction
 * @brief Abstraction layer for GPIO pins.
*/
class GpioAbstraction {

    public:
    /**
     * @brief Constructor
    */
    GpioAbstraction() = default;
    /**
     * @brief Destructor
    */
    virtual ~GpioAbstraction() = default;

    /**
     * @brief write to the pin and set it's logic level.
     * @param logicLevel The logic level to write to the pin.
     * @returns ErrorType::Success if the pin was written.
     * @returns ErrorType::Failure if the pin was not written.
     * @returns ErrorType::NotImplemented if writing to the gpio pin is not implemented.
     * @returns ErrorType::NotAvailable if the system running foundation does not provide gpio pin writing.
     * @returns ErrorType::InvalidParameter if the logic level is not valid.
     * @post The pin should output a voltage level according to the logic level written.
    */
    virtual ErrorType pinWrite(GpioLogicLevel logicLevel) = 0;
    /**
     * @brief configure the gpio pin
     * @param basePeripheralRegister The base peripheral register of the gpio pin. Pointer to the hardware instance of the gpio bank that contains the pin.
     * @param pinNumber The pin number of the gpio pin.
     * @param direction The direction of the pin.
     * @param interruptMode The interrupt mode of the pin.
     * @param pullUpEnable Enable pull up resistor on the pin.
     * @param pullDownEnable Enable pull down resistor on the pin.
     * @returns ErrorType::Success if the pin was configured.
     * @returns ErrorType::Failure if the pin was not configured.
     * @returns ErrorType::Invalid parameter if any of the parameters are invalid.
     * @returns ErrorType::NotImplemented if configuring the gpio pin is not implemented.
    */
    virtual ErrorType configure(uint32_t *basePeripheralRegister, PinNumber pinNumber, GpioPinDirection direction, GpioInterruptMode interruptMode, bool pullUpEnable, bool pullDownEnable) = 0;

    /**
     * @brief Get the pointer to the base register of the gpio peripheral being used.
    */
    const uint32_t &basePeripheralRegister() const { return *_basePeripheralRegister; }
    /**
     * @brief Get the pin number of this gpio
    */
    PinNumber pinNumber() const { return _pinNumber; }
    /**
     * @brief Get the interrupt mode of the gpio pin.
    */
    GpioInterruptMode interruptMode() const { return _interruptMode; }
    /**
     * @brief Get the direction of the gpio pin.
    */
    GpioPinDirection direction() const { return _direction; }
    /**
     * @brief True if a pull-up resistor is enabled on this pin
    */
    bool pullUpEnabled() const { return _pullUpEnable; }
    /**
     * @brief True if a pull-down resistor is enabled on this pin.
    */
    bool pullDownEnabled() const { return _pullDownEnable; }


    protected:
    /// @brief The base peripheral register of the gpio pin.
    uint32_t *_basePeripheralRegister;
    /// @brief The pin number of the gpio pin.
    PinNumber _pinNumber;
    /// @brief The interrupt mode of the gpio pin.
    GpioInterruptMode _interruptMode;
    /// @brief The direction of the gpio pin.
    GpioPinDirection _direction;
    /// @brief True if a pull-up resistor is enabled on this pin.
    bool _pullUpEnable;
    /// @brief True if a pull-down resistor is enabled on this pin.
    bool _pullDownEnable;
};

#endif //__CBT_GPIO_ABSTRACTION_HPP__