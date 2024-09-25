#ifndef __CBT_UART_HPP__
#define __CBT_UART_HPP__

//AbstractionLayer
#include "UartAbstraction.hpp"
//Protobuf
#include "api_fc.pb.h"
#include "api_cf.pb.h"
#include "apiF0tx.pb.h"

class Uart : public UartAbstraction {
    public:
    Uart() : UartAbstraction() {
        _uartBuffer.reserve(sizeof(CF));
    }
    ~Uart() = default;
    ErrorType init() override;
    ErrorType deinit() override;
    ErrorType txBlocking(const std::string &data, const Milliseconds timeout) override;
    ErrorType rxBlocking(std::string &buffer, const Milliseconds timeout) override;
    ErrorType txNonBlocking(const std::shared_ptr<std::string> data, std::function<void(const ErrorType error, const Bytes bytesWritten)> callback = nullptr) override;
    ErrorType rxNonBlocking(std::shared_ptr<std::string> buffer, std::function<void(const ErrorType error, std::shared_ptr<std::string> buffer)> callback = nullptr) override;
    ErrorType flushRxBuffer() override;

    ErrorType setHardwareConfig(PinNumber txNumber, PinNumber rxNumber, PinNumber rtsNumber, PinNumber ctsNumber, UartConfig::PeripheralNumber peripheralNumber) override;
    ErrorType setDriverConfig(uint32_t baudRate, uint8_t dataBits, char parity, uint8_t stopBits, UartConfig::FlowControl flowControl) override;
    ErrorType setFirmwareConfig(Bytes receiveBufferSize, Bytes transmitBufferSize, int8_t terminatingByte) override;

    private:
    std::string _uartBuffer;
    Bytes _uartBufferBytesRead = 0;
    Id _messageRef = 0;

    ErrorType cleonToFoundationDataReply(uint8_t qref);
    ErrorType cleonToFoundationAcknowledge(uint8_t qref);
};

#endif // __CBT_UART_HPP__