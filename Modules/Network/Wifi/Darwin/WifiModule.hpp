/***************************************************************************//**
* @author   Ben Haubrich
* @file     WifiModule.hpp
* @details  Wifi for posix compliant systems.
* @ingroup  PosixModules
*******************************************************************************/
#ifndef __WIFI_MODULE_HPP__
#define __WIFI_MODULE_HPP__

//AbstractionLayer
#include "WifiAbstraction.hpp"
#include "Error.hpp"
//C++
#include <memory>
#include <string>

class Wifi : public WifiAbstraction {

    public:
    Wifi() : WifiAbstraction() {
        //Wifi on a posix system is always up.
        _status.isUp = true;
        _status.technology = NetworkTypes::Technology::Wifi;
    }
    ~Wifi() = default;

    ErrorType init() override;
    ErrorType networkUp() override;
    ErrorType networkDown() override;
    ErrorType txBlocking(const std::string &frame, const Milliseconds timeout) override;
    ErrorType txNonBlocking(const std::shared_ptr<std::string> frame, std::function<void(const ErrorType error, const Bytes bytesWritten)> callback = nullptr) override;
    ErrorType rxBlocking(std::string &frameBuffer, const Milliseconds timeout) override;
    ErrorType rxNonBlocking(std::shared_ptr<std::string> frameBuffer, std::function<void(const ErrorType error, std::shared_ptr<std::string> frameBuffer)> callback = nullptr) override;
    ErrorType getMacAddress(std::string &macAddress) override;
    ErrorType getSignalStrength(DecibelMilliWatts &signalStrength) override;

    ErrorType mainLoop() override;

    ErrorType setSsid(WifiConfig::Mode mode, std::string ssid) override { return ErrorType::NotAvailable; }
    ErrorType setPassword(WifiConfig::Mode mode, std::string password) override { return ErrorType::NotAvailable; }
    ErrorType setMode(WifiConfig::Mode mode) override { return ErrorType::NotAvailable; }
    ErrorType setAuthMode(WifiConfig::AuthMode authMode) override { return ErrorType::NotAvailable; }
};

#endif // __CBT_WIFI_HPP__