/***************************************************************************//**
* @author   Ben Haubrich
* @file     WifiModule.hpp
* @details  Wifi for ESP32
* @ingroup  EspModules
*******************************************************************************/
#ifndef __WIFI_MODULE_HPP__
#define __WIFI_MODULE_HPP__

//Foundation
#include "NetworkAbstraction.hpp"
#include "EventQueue.hpp"
//ESP
#include "esp_wifi.h"

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

/**
 * @class Wifi
 * @brief Wifi for ESP32
*/
class Wifi : public NetworkAbstraction {

    public:
    Wifi() : NetworkAbstraction() {
        _status.isUp = false;
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

    ErrorType radioOn();
    ErrorType radioOff();

    WifiConfig::Mode mode() const { return _mode; }
    std::string ssid() const { return _ssid; }
    std::string password() const { return _password; }
    uint8_t channel() const { return _channel; }
    uint8_t maxConnections() const { return _maxConnections; }
    WifiConfig::AuthMode authMode() const { return _authMode; }

    /**
     * @brief Set the ssid for the selected mode.
     * @param[in] mode The wifi mode to set the SSID for
     * @param[in] ssid The ssid to set
     */
    ErrorType setSsid(WifiConfig::Mode mode, std::string ssid);
    /**
     * @brief Set the password for the selected mode.
     * @param[in] mode The wifi mode to set the password for
     * @param[in] password The password to set
    */
    ErrorType setPassword(WifiConfig::Mode mode, std::string password);
    ///@brief set the wifi channel.
    ErrorType setChannel(uint8_t channel) { _channel = channel; return ErrorType::Success; }
    ///@brief set the maximum number of wifi connections.
    ErrorType setMaxConnections(uint8_t maxConnections) { _maxConnections = maxConnections; return ErrorType::Success; }
    ///@brief set the wifi mode.
    ErrorType setMode(WifiConfig::Mode mode) { _mode = mode; return ErrorType::Success; }
    /**
     * @brief set the wifi authentication mode.
     * @param[in] authMode The authentication mode to set.
     * @sa WifiConfig::AuthMode
     */
    ErrorType setAuthMode(WifiConfig::AuthMode authMode) { _authMode = authMode; return ErrorType::Success; }

    private:
    WifiConfig::Mode _mode = WifiConfig::Mode::Unknown;
    std::string _ssid = std::string(32, '\0');
    std::string _password;
    uint8_t _channel = 0;
    uint8_t _maxConnections = 0;
    WifiConfig::AuthMode _authMode = WifiConfig::AuthMode::Unknown;

    wifi_auth_mode_t toEspAuthMode(WifiConfig::AuthMode authMode) {
        switch (authMode) {
            case WifiConfig::AuthMode::Open:
                return WIFI_AUTH_OPEN;
            case WifiConfig::AuthMode::Wep:
                return WIFI_AUTH_WEP;
            case WifiConfig::AuthMode::WpaPsk:
                return WIFI_AUTH_WPA_PSK;
            case WifiConfig::AuthMode::Wpa2Psk:
                return WIFI_AUTH_WPA2_PSK;
            default:
                return WIFI_AUTH_OPEN;
        }
    }

    wifi_mode_t toEspWifiMode(WifiConfig::Mode mode) {
        switch (mode) {
            case WifiConfig::Mode::AccessPoint:
                return WIFI_MODE_AP;
            case WifiConfig::Mode::Station:
                return WIFI_MODE_STA;
            case WifiConfig::Mode::AccessPointAndStation:
                return WIFI_MODE_APSTA;
            default:
                return WIFI_MODE_AP;
        }
    }

    ErrorType initAccessPoint();
    ErrorType initStation();

    ErrorType setStationSsid(std::string ssid);
    ErrorType setAccessPointSsid(std::string ssid);
};

#endif // __WIFI_MODULE_HPP__