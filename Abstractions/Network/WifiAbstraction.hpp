/***************************************************************************//**
* @author   Ben Haubrich
* @file     WifiAbstraction.hpp
* @details  \b Synopsis: \n Interface for communication over the network.
* @ingroup  AbstractionLayer
*******************************************************************************/
#ifndef __WIFI_ABSTRACTION_HPP__
#define __WIFI_ABSTRACTION_HPP__

#include "NetworkAbstraction.hpp"

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
 * @class WifiAbstraction
 * @brief Interface for communication over the wifi.
*/
class WifiAbstraction : public NetworkAbstraction {
    public:
    /**
     * @fn WifiAbstraction
     * @sa WifiFactory::Factory
    */
    WifiAbstraction() = default;
    virtual ~WifiAbstraction() = default;

    /// @brief Tag for logging
    static constexpr char TAG[] = "Wifi";

    /**
     * @brief Set the ssid for the selected mode.
     * @param[in] mode The wifi mode to set the SSID for
     * @param[in] ssid The ssid to set
     */
    virtual ErrorType setSsid(WifiConfig::Mode mode, std::string ssid) = 0;
    /**
     * @brief Set the password for the selected mode.
     * @param[in] mode The wifi mode to set the password for
     * @param[in] password The password to set
    */
    virtual ErrorType setPassword(WifiConfig::Mode mode, std::string password) = 0;
    /**
     * @brief set the wifi mode.
     * @param[in] mode The mode to set.
     * @sa WifiConfig::Mode
    */
    virtual ErrorType setMode(WifiConfig::Mode mode) = 0;
    /**
     * @brief set the wifi authentication mode.
     * @param[in] authMode The authentication mode to set.
     * @sa WifiConfig::AuthMode
     */
    virtual ErrorType setAuthMode(WifiConfig::AuthMode authMode) = 0;
};

#endif