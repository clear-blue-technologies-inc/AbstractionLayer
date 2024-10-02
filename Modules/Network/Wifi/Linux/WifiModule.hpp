/***************************************************************************//**
* @author   Ben Haubrich
* @file     WifiModule.hpp
* @details  Wifi for posix compliant systems.
* @ingroup  PosixModules
*******************************************************************************/
#ifndef __WIFI_MODULE_HPP__
#define __WIFI_MODULE_HPP__

//AbstractionLayer
#include "NetworkAbstraction.hpp"
#include "Error.hpp"
//C++
#include <memory>
#include <string>

namespace WifiConfig {

    /**
     * @enum Mode
     * @brief Wifi mode that effictively says whether you are connecting to wifi or being connected to by some other device.
    */
    enum class Mode : uint8_t {
        Unknown = 0,
        AccessPoint, ///< Access Point. You can connect to these from a station.
        Station,     ///< Station. You can connect these to an access point.
        AccessPointAndStation ///< Access Point and Station.
    };

    /**
     * @enum AuthMode
     * @brief Wifi authentication mode.
    */
    enum class AuthMode : uint8_t {
        Unknown = 0, ///< Unknown
        Open,        ///< Open
        Wep,         ///< WEP
        WpaPsk,      ///< WPA-PSK
        Wpa2Psk      ///< WPA2-PSK
    };
}

class Wifi : public NetworkAbstraction {

    public:
    Wifi() : NetworkAbstraction() {
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

    /**
     * @brief Set the ssid for the selected mode.
     * @param[in] mode The wifi mode to set the SSID for
     * @param[in] ssid The ssid to set
     */
    ErrorType setSsid(WifiConfig::Mode mode, std::string ssid) { return ErrorType::NotAvailable; }
    /**
     * @brief Set the password for the selected mode.
     * @param[in] mode The wifi mode to set the password for
     * @param[in] password The password to set
    */
    ErrorType setPassword(WifiConfig::Mode mode, std::string password) { return ErrorType::NotAvailable; }
    ///@brief set the wifi mode.
    ErrorType setMode(WifiConfig::Mode mode) { return ErrorType::NotAvailable; }
    /**
     * @brief set the wifi authentication mode.
     * @param[in] authMode The authentication mode to set.
     * @sa WifiConfig::AuthMode
     */
    ErrorType setAuthMode(WifiConfig::AuthMode authMode) { return ErrorType::NotAvailable; }

    /**
     * @brief Given a hostname, get it's IP address
     * @param[in] host The hostname to get the IP address of
     * @param[out] The IP address of the host
     * @returns ErrorType::Success if the IP address was returned.
     * @returns ErrorType::Failure otherwise.
     */
    ErrorType hostToIp(const std::string &host, std::string &ipAddress);
    /**
     * @brief Given a destination IP address, get the interface that the outgoing packet was routed to to reach the destination.
     * @param[in] ipAddress The destination IP address
     * @param[out] interface The interface that the packet was routed through to reach the destination
     * @returns ErrorType::Success if the interface was returned
     * @returns ErrorType::Failure if the interface was not returned.
     */
    ErrorType interfaceRoutedTo(const std::string &ipAddress, std::string &interface);
};

#endif // __CBT_WIFI_HPP__