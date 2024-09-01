/***************************************************************************//**
* @author   Ben Haubrich
* @file     UartAbstraction.hpp
* @details  Abstraction layer for UART communication.
* @ingroup  AbstractionLayer
*******************************************************************************/
#ifndef __UART_ABSTRACTION_HPP__
#define __UART_ABSTRACTION_HPP__

//AbstractionLayer
#include "Error.hpp"
#include "Types.hpp"
#include "IcCommunicationProtocol.hpp"
//C
#include <stdint.h>

/**
 * @namespace UartConfig
 * @brief Uart configuration types
*/
namespace UartConfig {

    /**
     * @enum FlowControl
     * @brief   UART flow control.
    */
    enum class FlowControl : uint8_t {
        Unknown = 0,
        Disable, ///< Disable flow control.
        Rts,     ///< Enable RTS flow control.
        Cts,     ///< Enable CTS flow control.
        CtsRts   ///< Enable both RTS and CTS flow control.
    };

    /**
     * @enum PeripheralNumber
     * @brief   UART peripheral number. Refers to the base address of the uart module being used.
    */
    enum class PeripheralNumber : uint8_t {
        Zero = 0, ///< Peripheral number 0.
        One,      ///< Peripheral number 1.
        Two,      ///< Peripheral number 2.
        Three,    ///< Peripheral number 3.
        Four,     ///< Peripheral number 4.
        Five,     ///< Peripheral number 5.
        Unknown   ///< Unknown and invalid peripheral number.
    };

    /**
     * @enum Line
     * @brief UART transmit and receive lines.
    */
    enum class Line : uint8_t {
        Unknown = 0, ///< Unknown and invalid line.
        Tx,          ///< Transmit line.
        Rx,          ///< Receive line.
        Both         ///< Transmit and receive line.
    };
}

/**
 * @class UartAbstraction
 * @brief An interface for a Uart peripheral.
*/
class UartAbstraction : public IcCommunicationProtocol{

    public:
    /// @brief Constructor.
    UartAbstraction() : IcCommunicationProtocol() {}
    /// @brief Destructor.
    virtual ~UartAbstraction() = default;

    /// @brief Tag for logging.
    static constexpr char TAG[] = "Uart";

    /**
     * @brief Initialize the UART.
     * @pre Configure the hardware first.
     * @sa setHardwareConfig()
     * @sa setDriverConfig()
     * @sa setFirmwareConfig()
     * @returns ErrorType::Success if the UART was initialized successfully.
     * @returns ErrorType::Failure otherwise
    */
    virtual ErrorType init() = 0;
    /**
     * @brief De-initialize the UART.
     * @returns ErrorType::Success if the UART was initialized successfully.
     * @returns ErrorType::Failure otherwise
    */
    virtual ErrorType deinit() = 0;
    /**
     * @brief Tranmit data
     * @details This is a blocking call. Control will not be returned until the data is transmitted or the timeout has been reached.
     * @param [in] data The data to transmit.
     * @param [in] timeout The amount of time to wait to send data.
     * @returns ErrorType::Success if the data was transmitted successfully.
     * @returns ErrorType::Failure otherwise
    */
    virtual ErrorType txBlocking(const std::string &data, const Milliseconds timeout) = 0;
    /**
     * @brief Receive data
     * @details This is a blocking call. Control will not be returned until the data is received or the timeout has been reached.
     * @param [in] buffer The data to transmit.
     * @param [in] timeout The amount of time to wait to send data.
     * @returns ErrorType::Success if the data was transmitted successfully.
     * @returns ErrorType::Failure otherwise
     * @post The amount of data received is equal to the size of the buffer (std::string::size()).
    */
    virtual ErrorType rxBlocking(std::string &buffer, const Milliseconds timeout) = 0;
    /**
     * @brief Tranmit data
     * @details This is a non-blocking call. Control is returned immediately to the caller and the callback is called once the data has been trasnmitted.
     * @param [in] data The data to transmit.
     * @param [in] callback The callback to call once the data has been transmitted.
     * @returns ErrorType::Success if the data was transmitted successfully.
     * @returns ErrorType::Failure if the terminating byte is not negative and the byte was not found in the message
    */
    virtual ErrorType txNonBlocking(const std::shared_ptr<std::string> data, std::function<void(const ErrorType error, const Bytes bytesWritten)> callback = nullptr) = 0;
    /**
     * @brief Receive data
     * @details This is a non-blocking call. Control is returned immediately to the caller and the callback is called once the data has been received.
     * @param [in] buffer The data to transmit.
     * @param [in] callback The callback to call once the data has been received.
     * @returns ErrorType::Success if the data was transmitted successfully.
     * @returns ErrorType::Failure otherwise
     * @post The amount of data received is equal to the size of the buffer (std::string::size()).
    */
    virtual ErrorType rxNonBlocking(std::shared_ptr<std::string> buffer, std::function<void(const ErrorType error, std::shared_ptr<std::string> buffer)> callback = nullptr) = 0;
    /**
     * @brief clear the receive buffer
     * @returns ErrorType::Success if the receive buffer was cleared successfully.
     * @returns ErrorType::Failure otherwise
     * @post The receive buffer is empty.
     */
    virtual ErrorType flushRxBuffer() = 0;
    /**
     * @brief Set hardware configuration parameters.
     * @param [in] txNumber The transmit pin number.
     * @param [in] rxNumber The receive pin number.
     * @param [in] rtsNumber The RTS pin number.
     * @param [in] ctsNumber The CTS pin number.
     * @param [in] peripheralNumber The UART peripheral number.
     * @returns ErrorType::Success if the hardware configuration was set successfully.
     * @returns ErrorType::InvalidParameter if the txNumber, rxNumber, rtsNumber, ctsNumber, or peripheralNumber are invalid for the underlying hardware implementation.
    */
    virtual ErrorType setHardwareConfig(PinNumber txNumber, PinNumber rxNumber, PinNumber rtsNumber, PinNumber ctsNumber, UartConfig::PeripheralNumber peripheralNumber) = 0;
    /**
     * @brief Set driver configuration parameters.
     * @param [in] baudRate The baud rate.
     * @param [in] dataBits The data bits.
     * @param [in] parity The parity bit.
     * @param [in] stopBits The stop bits.
     * @param [in] flowControl The flow control.
     * @returns ErrorType::Success if the driver configuration was set successfully.
     * @returns ErrorType::InvalidParameter if the baud rate, data bits, parity, stop bits, or flow control are invalid for the underlying hardware implementation.
    */
    virtual ErrorType setDriverConfig(uint32_t baudRate, uint8_t dataBits, char parity, uint8_t stopBits, UartConfig::FlowControl flowControl) = 0;
    /**
     * @brief Set firmware configuration parameters.
     * @param [in] receiveBufferSize The receive buffer size.
     * @param [in] transmitBufferSize The transmit buffer size.
     * @param [in] terminatingByte The terminating byte. Used for when you want to receive bytes until a certain character is read.
     * @return Always returns ErrorType::Success.
    */
    virtual ErrorType setFirmwareConfig(Bytes receiveBufferSize, Bytes transmitBufferSize, int8_t terminatingByte = -1) = 0;

    /// @brief Get the baud rate.
    uint32_t baudRate() const { return _baudRate; }
    /// @brief Get the data bits.
    uint32_t dataBits() const { return _dataBits; }
    /// @brief Get the stop bits.
    uint32_t stopBits() const { return _stopBits; }
    /// @brief Get the parity.
    uint32_t parity() const { return _parity; }
    /// @brief Get the flow control.
    UartConfig::FlowControl flowControl() const { return _flowControl; }
    /// @brief Get the tx pin number.
    PinNumber txNumber() const { return _txNumber; }
    /// @brief Get the rx pin number.
    PinNumber rxNumber() const { return _rxNumber; }
    /// @brief Get the rts pin number
    PinNumber rtsNumber() const { return _rtsNumber; }
    /// @brief Get the cts pin number
    PinNumber ctsNumber() const { return _ctsNumber; }
    /// @brief Get the peripheral number.
    UartConfig::PeripheralNumber peripheralNumber() const { return _peripheralNumber; }
    /// @brief Get the receive buffer size.
    Bytes receiveBufferSize() const { return _receiveBufferSize; }
    /// @brief Get the transmit buffer size.
    Bytes transmitBufferSize() const { return _transmitBufferSize; }
    /// @brief Get the terminating byte.
    int8_t terminatingByte() const { return _terminatingByte; }

    /// @brief When a pin is unused, set it to this value
    static constexpr PinNumber Unused = -1;

    protected:
    /// @brief Baud rate.
    uint32_t _baudRate = 115200;
    /// @brief Data bits.
    uint8_t _dataBits = 8;
    /// @brief Parity.
    char _parity = 'N';
    /// @brief Stop bits.
    uint8_t _stopBits = 1;
    /// @brief Flow control.
    UartConfig::FlowControl _flowControl = UartConfig::FlowControl::Disable;
    /// @brief Terminating byte. Default to not used.
    int8_t _terminatingByte = -1;

    /// @brief Peripheral number.
    UartConfig::PeripheralNumber _peripheralNumber = UartConfig::PeripheralNumber::Unknown;
    /// @brief tx pin number.
    PinNumber _txNumber = Unused;
    /// @brief rx pin number
    PinNumber _rxNumber = Unused;
    /// @brief rts pin number
    PinNumber _rtsNumber = Unused;
    /// @brief cts pin number
    PinNumber _ctsNumber = Unused;
    /// @brief Receive buffer size.
    Bytes _receiveBufferSize = 0;
    /// @brief Transmit buffer size.
    Bytes _transmitBufferSize = 0;
};

#endif // __UART_ABSTRACTION_HPP__
