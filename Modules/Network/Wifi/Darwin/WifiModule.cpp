//Modules
#include "WifiModule.hpp"
//C++
#include <cassert>
#include <cstdio>

ErrorType Wifi::init() {
    return ErrorType::NotAvailable;
}
ErrorType Wifi::networkUp() {
    return ErrorType::NotAvailable;
}
ErrorType Wifi::networkDown() {
    return ErrorType::NotAvailable;
}
ErrorType Wifi::txBlocking(const std::string &frame, const Milliseconds timeout) {
    return ErrorType::NotAvailable;
}
ErrorType Wifi::txNonBlocking(const std::shared_ptr<std::string> frame, std::function<void(const ErrorType error, const Bytes bytesWritten)> callback) {
    return ErrorType::NotAvailable;
}
ErrorType Wifi::rxBlocking(std::string &frameBuffer, const Milliseconds timeout) {
    return ErrorType::NotAvailable;
}
ErrorType Wifi::rxNonBlocking(std::shared_ptr<std::string> frameBuffer, std::function<void(const ErrorType error, std::shared_ptr<std::string> frameBuffer)> callbackr) {
    return ErrorType::NotAvailable;
}
ErrorType Wifi::getMacAddress(std::string &macAddress) {
    const Count macAddressSize = 17;
    //The command extracts just the mac address from the output.
    constexpr char command[] = "sh -c \"networksetup -getmacaddress Wi-Fi | cut -f 3 -d\' \' \"";
    ErrorType error = ErrorType::Failure;
    
    FILE* pipe = popen(command, "r");
    if (nullptr != pipe) {
        if (nullptr != fgets(macAddress.data(), macAddress.capacity(), pipe)) {
            macAddress.resize(macAddressSize);
            error = ErrorType::Success;
        }
        else {
            macAddress.clear();
            pclose(pipe);
            error = ErrorType::Failure;
        }
    }

    return error;
}

ErrorType Wifi::getSignalStrength(DecibelMilliWatts &signalStrength) {
    std::string signalNoiseRatioString(64, 0);
    
    //The command extracts just the mac address from the output.
    //You can use the same command to get the noise as well to calculate a ratio .
    constexpr char command[] = "sh -c \"system_profiler -detailLevel basic SPAirPortDataType | egrep -A 10 'Current Network Information' | egrep 'Signal / Noise:' | cut -d: -f2 | tr -d [[^:numeric:]] | cut -d ' ' -f2\"";
    ErrorType error = ErrorType::Failure;
    
    FILE* pipe = popen(command, "r");
    if (nullptr != pipe) {
        if (nullptr != fgets(signalNoiseRatioString.data(), signalNoiseRatioString.capacity(), pipe)) {
            error = ErrorType::Success;
        }
        else {
            signalNoiseRatioString.clear();
            pclose(pipe);
            error = ErrorType::Failure;
        }
    }

    sscanf(signalNoiseRatioString.data(), "%d", &signalStrength);

    _status.signalStrength = signalStrength;

    return error;
}

ErrorType Wifi::mainLoop() {
    return runNextEvent();
}