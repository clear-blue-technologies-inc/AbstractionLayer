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

    std::string interface(16, 0);
    std::string host("google.com");
    std::string ipAddress("127.127.127.127");

    hostToIp(host, ipAddress);
    interfaceRoutedTo(ipAddress, interface);

    constexpr char command[] = "sh -c \"ip addr show %s | egrep \"link/ether\" | tr -s " " | cut -d' ' -f3\"";
    char commandWithInterface[sizeof(command) + host.size()];
    assert(snprintf(commandWithInterface, sizeof(commandWithInterface), command, host.data()) > 0);
    ErrorType error = ErrorType::Failure;
    
    FILE* pipe = popen(command, "r");
    if (nullptr != pipe) {
        if (nullptr != fgets(macAddress.data(), macAddress.capacity(), pipe)) {
            error = ErrorType::Success;
        }
        else {
            pclose(pipe);
            error = ErrorType::Failure;
        }
    }

    return error;
}

ErrorType Wifi::getSignalStrength(DecibelMilliWatts &signalStrength) {
    std::string signalNoiseRatioString(64, 0);
    
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

    return error;
}

ErrorType Wifi::hostToIp(const std::string &host, std::string ipAddress) {
    constexpr char command[] = "sh -c \"getent ahosts %s | awk '{print $1; exit}' | cut -d' ' -f1\"";
    char commandWithHostname[sizeof(command) + host.size()];
    assert(snprintf(commandWithHostname, sizeof(commandWithHostname), command, host.data()) > 0);
    ErrorType error = ErrorType::Failure;
    
    FILE* pipe = popen(command, "r");
    if (nullptr != pipe) {
        if (nullptr != fgets(ipAddress.data(), ipAddress.capacity(), pipe)) {
            error = ErrorType::Success;
        }
        else {
            pclose(pipe);
            error = ErrorType::Failure;
        }
    }

    return error;
}

ErrorType Wifi::interfaceRoutedTo(const std::string &ipAddress, std::string &interface) {
    constexpr char command[] = "sh -c \"ip route get %s | egrep dev | cut -d' ' -f3\"";
    char commandWithIpAddress[sizeof(command) + ipAddress.size()];
    assert(snprintf(commandWithIpAddress, sizeof(commandWithIpAddress), command, ipAddress.data()) > 0);
    ErrorType error = ErrorType::Failure;
    
    FILE* pipe = popen(command, "r");
    if (nullptr != pipe) {
        if (nullptr != fgets(interface.data(), interface.capacity(), pipe)) {
            error = ErrorType::Success;
        }
        else {
            pclose(pipe);
            error = ErrorType::Failure;
        }
    }

    return error;
}

ErrorType Wifi::mainLoop() {
    return runNextEvent();
}