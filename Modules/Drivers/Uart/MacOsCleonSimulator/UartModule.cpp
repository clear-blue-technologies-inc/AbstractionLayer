//Modules
#include "UartModule.hpp"
//Posix
#include <unistd.h>
//Modules
#include "SerializationModule.hpp"
#include "CyclicRedundancyCheckModule.hpp"
//C++
#include <random>

ErrorType Uart::init() {
    return ErrorType::Success;
}

ErrorType Uart::deinit() {
    return ErrorType::Success;
}

ErrorType Uart::txBlocking(const std::string &data, const Milliseconds timeout) {
    const uint8_t apiCall = data.at(0);
    uint8_t qref = data.at(1);

    _uartBufferBytesRead = 0;

    switch (apiCall) {
        case 0xFC:
            if (ErrorType::Success != cleonToFoundationDataReply(qref)) {
                return ErrorType::Failure;
            }
            break;
        case 0xF0:
            if (ErrorType::Success != cleonToFoundationAcknowledge(qref)) {
                return ErrorType::Failure;
            }
            break;
        default:
            return ErrorType::NotSupported;
    }

    return ErrorType::Success;
}

ErrorType Uart::txNonBlocking(const std::shared_ptr<std::string> data, std::function<void(const ErrorType error, const Bytes bytesWritten)> callback) {
    return ErrorType::NotImplemented;
}

ErrorType Uart::rxBlocking(std::string &buffer, const Milliseconds timeout) {
    buffer.assign(_uartBuffer.substr(_uartBufferBytesRead, buffer.size()));
    _uartBufferBytesRead += buffer.size();

    return ErrorType::Success;
}

ErrorType Uart::rxNonBlocking(std::shared_ptr<std::string> buffer, std::function<void(const ErrorType error, std::shared_ptr<std::string> buffer)> callback) {
    return ErrorType::NotImplemented;
}

ErrorType Uart::flushRxBuffer() {
    return ErrorType::NotAvailable;
}

ErrorType Uart::setHardwareConfig(int32_t txPinNumber, int32_t rxPinNumber, int32_t rtsPinNumber, int32_t ctsPinNumber, UartConfig::PeripheralNumber peripheralNumber) {
    return ErrorType::NotAvailable;
}

ErrorType Uart::setDriverConfig(uint32_t baudRate, uint8_t dataBits, char parity, uint8_t stopBits, UartConfig::FlowControl flowControl) {
    return ErrorType::NotAvailable;
}

ErrorType Uart::setFirmwareConfig(Bytes receiveBufferSize, Bytes transmitBufferSize, int8_t terminatingByte) {
    _receiveBufferSize = receiveBufferSize;
    _transmitBufferSize = transmitBufferSize;
    _terminatingByte = terminatingByte;

    return ErrorType::Success;
}

ErrorType Uart::cleonToFoundationDataReply(uint8_t qref) {
    Serializer serializer;
    Bytes maxSize;
    serializer.maxSerializedSize(maxSize, SerializationType::CleonToFoundationDataReply);
    std::string serializedDataReply(maxSize, 0);

    _uartBuffer = std::string(_uartBuffer.capacity(), 0);
    CF *cleonDataReply = reinterpret_cast<CF*>(_uartBuffer.data());

    cleonDataReply->has_data1 = true;
    cleonDataReply->data1.fields_count = 0;

    constexpr int messageRef = 280;
    cleonDataReply->data1.fields[2].has_field_id = true;
    cleonDataReply->data1.fields[2].field_id = messageRef;
    cleonDataReply->data1.fields[2].has_uint_value = true;
    cleonDataReply->data1.fields[2].uint_value = _messageRef++;
    cleonDataReply->data1.fields_count++;

    constexpr int timeSinceCreation = 300;
    cleonDataReply->data1.fields[3].has_field_id = true;
    cleonDataReply->data1.fields[3].field_id = timeSinceCreation;
    cleonDataReply->data1.fields[3].has_uint_value = true;
    cleonDataReply->data1.fields[3].uint_value = Seconds(1);
    cleonDataReply->data1.fields_count++;

    constexpr int bitfieldStatuses1 = 0;
    cleonDataReply->data1.fields[0].has_field_id = true;
    cleonDataReply->data1.fields[0].field_id = bitfieldStatuses1;
    cleonDataReply->data1.fields[0].has_uint_value = true;
    cleonDataReply->data1.fields_count++;

    constexpr int bitfieldStatuses2 = 1;
    cleonDataReply->data1.fields[1].has_field_id = true;
    cleonDataReply->data1.fields[1].field_id = bitfieldStatuses2;
    cleonDataReply->data1.fields[1].has_uint_value = true;
    cleonDataReply->data1.fields_count++;
    
    {
    int fieldId = 30;
    for (int i = 4; i < 25; i++) {
        cleonDataReply->data1.fields[i].has_field_id = true;
        cleonDataReply->data1.fields[i].field_id = fieldId;
        cleonDataReply->data1.fields[i].has_float_value = true;
        fieldId += 10;
        cleonDataReply->data1.fields_count++;
    }
    }

    if (ErrorType::Success != serializer.serialize(_uartBuffer, serializedDataReply, SerializationType::CleonToFoundationDataReply)) {
        return ErrorType::Failure;
    }

    std::string header(4,0);
    header.at(0) = 0xCF;
    header.at(1) = qref;
    header.at(2) = serializedDataReply.size() & 0x00FF;
    header.at(3) = (serializedDataReply.size() & 0xFF00) >> 8;
    _uartBuffer.assign(header);

    _uartBuffer.append(serializedDataReply);

    uint32_t crcValue;
    Crc::crc32LittleEndian(0xFFFFFFFF, _uartBuffer, crcValue);
    for (Bytes i = 0; i < sizeof(crcValue); i++) {
        _uartBuffer.push_back((uint8_t)((crcValue & (0xFF << 8*i)) >> 8*i));
    }

    return ErrorType::Success;
}

ErrorType Uart::cleonToFoundationAcknowledge(uint8_t qref) {
    _uartBuffer.push_back(0x41);
    _uartBuffer.push_back(qref);
    _uartBuffer.push_back(0x1);
    _uartBuffer.push_back(0x0);
    _uartBuffer.push_back(0xF0);

    uint32_t crcValue;
    Crc::crc32LittleEndian(0xFFFFFFFF, _uartBuffer, crcValue);
    for (Bytes i = 0; i < sizeof(crcValue); i++) {
        _uartBuffer.push_back((uint8_t)((crcValue & (0xFF << 8*i)) >> 8*i));
    }

    return ErrorType::Success;
}