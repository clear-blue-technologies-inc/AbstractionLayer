#include "WifiModule.hpp"

ErrorType Wifi::init() {
    return ErrorType::NotImplemented;
}
ErrorType Wifi::networkUp() {
    return ErrorType::NotImplemented;
}
ErrorType Wifi::networkDown() {
    return ErrorType::NotImplemented;
}
ErrorType Wifi::txBlocking(const std::string &frame, const Milliseconds timeout) {
    return ErrorType::NotImplemented;
}
ErrorType Wifi::txNonBlocking(const std::shared_ptr<std::string> frame, std::function<void(const ErrorType error, const Bytes bytesWritten)> callback) {
    return ErrorType::NotImplemented;
}
ErrorType Wifi::rxBlocking(std::string &frameBuffer, const Milliseconds timeout) {
    return ErrorType::NotImplemented;
}
ErrorType Wifi::rxNonBlocking(std::shared_ptr<std::string> frameBuffer, std::function<void(const ErrorType error, std::shared_ptr<std::string> frameBuffer)> callbackr) {
    return ErrorType::NotImplemented;
}
ErrorType Wifi::getMacAddress(std::string &macAddress) {
    return ErrorType::NotImplemented;
}

ErrorType Wifi::getSignalStrength(DecibelMilliWatts &signalStrength) {
    return ErrorType::NotImplemented;
}

ErrorType Wifi::mainLoop() {
    return ErrorType::NotImplemented;
}