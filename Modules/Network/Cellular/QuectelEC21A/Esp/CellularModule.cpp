//AbstractionLayer Modules
#include "CellularModule.hpp"
#include "OperatingSystemModule.hpp"
//AbstractionLayer Applications
#include "log.hpp"
//C++
#include <cstring>

#define CELLULAR_MODULE_DEBUGGING_ON 0

ErrorType Cellular::init() {
    assert(nullptr != _ic);
    assert(-1 != _resetPin);
    assert(false == _accessPointName.empty());
    assert(CellularConfig::RadioAccessTechnology::Unknown != _radioAccessTechnology);
    assert(CellularConfig::AccessMode::Unknown != _accessMode);

    ErrorType error = ErrorType::Failure;

    _gpioReset = std::make_unique<Gpio>();
    assert(nullptr != _gpioReset);
    _gpioReset->configure(nullptr, _resetPin, GpioPinDirection::DigitalOutput, GpioInterruptMode::Disabled, false, false);

    CBT_LOGI(TAG, "Resetting modem. Waiting for at least 12 seconds for uart to become active.");
    error = reset();
    if (ErrorType::Success != error) {
        return error;
    }

    error = _ic->flushRxBuffer();
    if (ErrorType::Success != error) {
        return error;
    }

    std::string mfgName(32, 0);
    if (ErrorType::Success == getManufacturerName(mfgName)) {
        if (std::string::npos != mfgName.find("Quectel")) {
            _status.manufacturerName.assign("Quectel");
        }
    }
    else if (mfgName.empty()) {
        return ErrorType::PrerequisitesNotMet;
    }

    //Parsing of responses depends on echo mode being disabled.
    if (ErrorType::Success != echoMode(false)) {
        CBT_LOGE(TAG, "Failed to disable echo mode.");
        return error;
    }

    error = terminatingCharacter(_commandLineTerminationCharacter);
    if (ErrorType::Success != error) {
        return error;
    }

    error = responseFormattingCharacter(_responseFormattingCharacter);
    if (ErrorType::Success != error) {
        return error;
    }

    error = networkUp();
    if (ErrorType::Success != error) {
        return error;
    }

    return ErrorType::Success;
}

ErrorType Cellular::networkUp() {

    if (ErrorType::Success != simCardIsInserted()) {
        return ErrorType::Failure;
    }

    return ErrorType::NotImplemented;
}

ErrorType Cellular::networkDown() {
    return ErrorType::NotImplemented;
}

ErrorType Cellular::txBlocking(const std::string &frame, const Milliseconds timeout) {
    return ErrorType::NotImplemented;
}

ErrorType Cellular::txNonBlocking(const std::shared_ptr<std::string> frame, std::function<void(const ErrorType error, const Bytes bytesWritten)> callback) {
    return ErrorType::NotImplemented;
}

ErrorType Cellular::rxBlocking(std::string &frameBuffer, const Milliseconds timeout) {
    return ErrorType::NotImplemented;
}

ErrorType Cellular::rxNonBlocking(std::shared_ptr<std::string> frameBuffer, std::function<void(const ErrorType error, std::shared_ptr<std::string> frameBuffer)> callback) {
    return ErrorType::NotImplemented;
}

ErrorType Cellular::getMacAddress(std::string &macAddress) {
    return ErrorType::NotImplemented;
}

ErrorType Cellular::getSignalStrength(DecibelMilliWatts &signalStrength) {
    return ErrorType::NotImplemented;
}

ErrorType Cellular::mainLoop() {
    return ErrorType::NotImplemented;
}

//See Reset, Section 3.8, Quectel hardware design
ErrorType Cellular::reset() {
    assert(nullptr != _gpioReset);
    _gpioReset->pinWrite(GpioLogicLevel::High);
    OperatingSystem::Instance().delay(Milliseconds(151));
    _gpioReset->pinWrite(GpioLogicLevel::Low);

    //See Note 1, Pg. 43, Quectel hardware design.
    //Wait 12 seconds for the uart to become active.
    OperatingSystem::Instance().delay(Milliseconds(12000));

    return ErrorType::Success;
}

ErrorType Cellular::sendCommand(const std::string &atCommand, const Milliseconds timeout, const Count maxRetries) {
    std::string commandBuffer(atCommand.size() + sizeof(_commandLineTerminationCharacter), 0);
    ErrorType error = ErrorType::Failure;
    Count retries = 0;

    do {
        commandBuffer.assign(atCommand + _commandLineTerminationCharacter);
        error = _ic->txBlocking(commandBuffer, timeout);
#if CELLULAR_MODULE_DEBUGGING_ON
        CBT_LOGI(TAG, "Sending...");
        CBT_LOG_BUFFER_HEXDUMP(TAG, commandBuffer.data(), commandBuffer.size(), LogType::Info);
#endif
        if (ErrorType::Success == error) {
            return error;
        }
        else {
            retries++;
        }
    } while (retries < maxRetries);

    if (retries >= maxRetries) {
        error = ErrorType::Timeout;
    }
    else {
        error = ErrorType::Success;
    }

    return error;
}

ErrorType Cellular::receiveCommand(std::string &responseBuffer, const Milliseconds timeout, const Count maxRetries) {
    ErrorType error = ErrorType::Failure;
    Count retries = 0;

    responseBuffer.clear();
    responseBuffer.resize(0);
    std::string bytesRead(16, 0);
    bool endOfResponse = false;

    do {
        bytesRead.clear();
        bytesRead.resize(bytesRead.capacity());

        error = _ic->rxBlocking(bytesRead, timeout);
#if CELLULAR_MODULE_DEBUGGING_ON
        CBT_LOGI(TAG, "Receiving...");
#endif
        if (ErrorType::Success != error) {
            retries++;
        }
        else {
#if CELLULAR_MODULE_DEBUGGING_ON
            CBT_LOGI(TAG, "Partial response...");
            CBT_LOG_BUFFER_HEXDUMP(TAG, bytesRead.data(), bytesRead.size(), LogType::Info);
#endif
            responseBuffer.append(bytesRead);
        }

        endOfResponse = (responseBuffer.back() == _responseFormattingCharacter);

    } while (retries < maxRetries && !endOfResponse);

#if CELLULAR_MODULE_DEBUGGING_ON
    CBT_LOGI(TAG, "Full response...");
    CBT_LOG_BUFFER_HEXDUMP(TAG, responseBuffer.data(), responseBuffer.size(), LogType::Info);
#endif

    if (retries >= maxRetries) {
        error = ErrorType::Timeout;
    }
    else if (responseBuffer.back() != _responseFormattingCharacter) {
        //You can try increasing the buffer size in case it wasn't big enough to read all the data until the response formatting character was found.
        CBT_LOGE(TAG, "Response formatting character not found.");
        error = ErrorType::Failure;
    }
    else {
        error = ErrorType::Success;
    }

    return error;
}

ErrorType Cellular::getManufacturerName(std::string &mfgName) {
    ErrorType error;
    error = sendCommand("ATI", 1000, 10);
    if (ErrorType::Success == error) {
        error = receiveCommand(mfgName, 1000, 10);
    }

    if (std::string::npos != mfgName.find("OK")) {
        error = ErrorType::Success;
    }
    else {
        error = ErrorType::Failure;
    }

    return error;
}

ErrorType Cellular::simCardIsInserted() {
    std::string commandBuffer(64, 0);
    constexpr int numRetries = 10;
    constexpr Milliseconds timeout = 1000;
    ErrorType error = ErrorType::Failure;

    error = sendCommand("AT+QSIMSTAT?", timeout, numRetries);
    if (ErrorType::Success != error) {
        return error;
    }

    std::string &responseBuffer = commandBuffer;
    error = receiveCommand(responseBuffer, timeout, numRetries);
    if (ErrorType::Success != error) {
        return error;
    }

    if (ErrorType::Success == error) {
        if (std::string::npos != responseBuffer.find("OK")) {
            error = ErrorType::Success;
        }
        else {
            error = ErrorType::Failure;
        }
    }

    responseBuffer.erase(std::remove(responseBuffer.begin(), responseBuffer.end(), '\r'), responseBuffer.end());
    responseBuffer.erase(std::remove(responseBuffer.begin(), responseBuffer.end(), '\n'), responseBuffer.end());
    responseBuffer.replace(responseBuffer.find("OK"), 2, "");
    responseBuffer.replace(responseBuffer.find("+QSIMSTAT:"), 2, "");

    const char *delim = ",";
    char *save_ptr = responseBuffer.data();
    char *token = strtok_r(save_ptr, delim, &save_ptr);

    bool cardDetectedUrcEnabled = strtoul(token, nullptr, 10);
    token = strtok_r(save_ptr, delim, &save_ptr);
    bool isInserted = strtoul(token, nullptr, 10);

    if (isInserted) {
        error = ErrorType::Success;
    }
    else {
        error = ErrorType::Failure;
    }

    return error;
}

ErrorType Cellular::echoMode(const bool enable) {
    ErrorType error = ErrorType::Failure;
    std::string commandBuffer(64, 0);
    commandBuffer.assign("ATE");
    commandBuffer.append(enable ? "1" : "0");
    error = sendCommand(commandBuffer, 1000, 10);

    std::string &responseBuffer = commandBuffer;

    if (ErrorType::Success == error) {
        error = receiveCommand(responseBuffer, 1000, 10);
    }

    if (ErrorType::Success == error) {
        if (std::string::npos != responseBuffer.find("OK")) {
            error = ErrorType::Success;
        }
        else {
            error = ErrorType::Failure;
        }
    }

    return error;
}

ErrorType Cellular::terminatingCharacter(char &terminatingCharacter) {
    std::string responseBuffer(16, 0);
    ErrorType error;
    error = sendCommand("ATS3?", 1000, 10);
    if (ErrorType::Success == error) {
        error = receiveCommand(responseBuffer, 1000, 10);
    }
    else {
        CBT_LOGW(TAG, "Error sending to <command:ATS3?>");
    }

    if (ErrorType::Success == error) {
        if (std::string::npos != responseBuffer.find("OK")) {
            error = ErrorType::Success;
        }
        else {
            error = ErrorType::Failure;
        }
    }

    responseBuffer.erase(std::remove(responseBuffer.begin(), responseBuffer.end(), '\r'), responseBuffer.end());
    responseBuffer.erase(std::remove(responseBuffer.begin(), responseBuffer.end(), '\n'), responseBuffer.end());
    responseBuffer.replace(responseBuffer.find("OK"), 2, "");

    terminatingCharacter = strtoul(responseBuffer.c_str(), nullptr, 10);

    return error;
}

ErrorType Cellular::responseFormattingCharacter(char &responseFormattingCharacter) {
    std::string responseBuffer(16, 0);
    ErrorType error;
    error = sendCommand("ATS4?", 500, 10);
    if (ErrorType::Success == error) {
        error = receiveCommand(responseBuffer, 500, 10);
    }
    else {
        CBT_LOGW(TAG, "Error sending to <command:ATS4?>");
    }

    if (ErrorType::Success == error) {
        if (std::string::npos != responseBuffer.find("OK")) {
            error = ErrorType::Success;
        }
        else {
            error = ErrorType::Failure;
        }
    }

    responseBuffer.erase(std::remove(responseBuffer.begin(), responseBuffer.end(), '\r'), responseBuffer.end());
    responseBuffer.erase(std::remove(responseBuffer.begin(), responseBuffer.end(), '\n'), responseBuffer.end());
    responseBuffer.replace(responseBuffer.find("OK"), 2, "");

    responseFormattingCharacter = strtoul(responseBuffer.c_str(), nullptr, 10);

    return error;
}