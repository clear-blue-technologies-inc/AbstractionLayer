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

/*
 * In order to get the mac address, we use a common hostname like google and
 * run a test to see what interface is used when connecting to the common hostname.
 * The we use this hostname as our interface that we use to connect to the internet
 * and get the macAddress from that interface.
*/
ErrorType Wifi::getMacAddress(std::string &macAddress) {
    std::string interface(16, 0);
    std::string host("google.com");
    std::string ipAddress("127.127.127.127");

    hostToIp(host, ipAddress);
    interfaceRoutedTo(ipAddress, interface);

    const std::string command1("sh -c \"ip addr show ");
    const std::string command2("\" | egrep \"link/ether\" | tr -s ' ' | cut -d' ' -f3");
    std::string commandFinal(command1.size() + command2.size() + macAddress.size(), 0);
    commandFinal.assign(command1);
    commandFinal.append(interface);
    commandFinal.append(command2);

    ErrorType error = ErrorType::Failure;
    
    FILE* pipe = popen(commandFinal.c_str(), "r");
    if (nullptr != pipe) {
        size_t bytesRead = fread(macAddress.data(), sizeof(uint8_t), macAddress.capacity(), pipe);
        if (feof(pipe) || bytesRead == macAddress.capacity()) {
            error = ErrorType::Success;
            macAddress.resize(bytesRead);
            while (macAddress.back() == '\n') {
                macAddress.pop_back();
            }
        }
        else if (ferror(pipe)) {
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

ErrorType Wifi::hostToIp(const std::string &host, std::string &ipAddress) {
    const std::string command1("sh -c \"getent ahosts ");
    const std::string command2(" | awk '{print $1; exit}' | cut -d' ' -f1\"");
    std::string commandFinal(command1.size() + command2.size() + ipAddress.size(), 0);
    commandFinal.assign(command1);
    commandFinal.append(host);
    commandFinal.append(command2);

    ErrorType error = ErrorType::Failure;
    
    FILE* pipe = popen(commandFinal.c_str(), "r");
    if (nullptr != pipe) {
        const size_t bytesRead = fread(ipAddress.data(), sizeof(uint8_t), ipAddress.capacity(), pipe);
        if (feof(pipe) || bytesRead == ipAddress.capacity()) {
            error = ErrorType::Success;
            ipAddress.resize(bytesRead);
            while (ipAddress.back() == '\n') {
                ipAddress.pop_back();
            }
        }
        else if (ferror(pipe)) {
            pclose(pipe);
            error = ErrorType::Failure;
        }
    }

    return error;
}

ErrorType Wifi::interfaceRoutedTo(const std::string &ipAddress, std::string &interface) {
    const std::string command1("sh -c \"ip route get ");
    const std::string command2("\" | awk -F \"dev \" \'{print $2}\' | cut -d' ' -f1");
    std::string commandFinal(command1.size() + command2.size() + ipAddress.size(), 0);
    commandFinal.assign(command1);
    commandFinal.append(ipAddress);
    commandFinal.append(command2);

    ErrorType error = ErrorType::Failure;
    
    FILE* pipe = popen(commandFinal.c_str(), "r");
    if (nullptr != pipe) {
        const size_t bytesRead = fread(interface.data(), sizeof(uint8_t), interface.capacity(), pipe);
        if (feof(pipe) || bytesRead == interface.capacity()) {
            error = ErrorType::Success;
            interface.resize(bytesRead);
            while (interface.back() == '\n') {
                interface.pop_back();
            }
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