//AbstractionLayer Modules
#include "CellularModule.hpp"
#include "OperatingSystemModule.hpp"
//AbstractionLayer Applications
#include "log.hpp"

ErrorType Cellular::init() {
    assert(nullptr != _ic);
    assert(-1 != _resetPin);
    assert(false == _accessPointName.empty());
    assert(CellularConfig::RadioAccessTechnology::Unknown != _radioAccessTechnology);
    assert(CellularConfig::AccessMode::Unknown != _accessMode);

    _gpioReset = std::make_unique<Gpio>();
    assert(nullptr != _gpioReset);
    _gpioReset->configure(nullptr, _resetPin, GpioPinDirection::DigitalOutput, GpioInterruptMode::Disabled, false, false);

    CBT_LOGI(TAG, "Resetting modem. Waiting for at least 12 seconds for uart to become active.");
    reset();

    _ic->flushRxBuffer();

    std::string mfgName(32, 0);
    if (ErrorType::Success == getManufacturerName(mfgName)) {
        if (std::string::npos == mfgName.find("Quectel")) {
            CBT_LOGE(TAG, "No Quectel modem detected");
            //The pre-requisite being that a Quectel modem must be installed.
            return ErrorType::PrerequisitesNotMet;
        }
        else {
            _status.manufacturerName.assign("Quectel");
        }
    }


    return ErrorType::Success;
}

ErrorType Cellular::networkUp() {
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

ErrorType Cellular::getManufacturerName(std::string &mfgName) {
    constexpr int maxSendRetries = 10;
    constexpr Milliseconds timeout = 500;

    ErrorType error = ErrorType::Failure;
    int retries = 0;
    bool foundResponse = false;
    std::string commandBuffer(256, 0);

    mfgName.clear();
    mfgName.resize(mfgName.capacity());

    do {
        commandBuffer.assign("ATI\r\n");
        error = _ic->txBlocking(commandBuffer, timeout);
        if (ErrorType::Success == error) {
            error = _ic->rxBlocking(mfgName, timeout);
            if (ErrorType::Success == error) {
                return error;
            }
        }
    } while (++retries < maxSendRetries && mfgName.empty());

    return error;
}
