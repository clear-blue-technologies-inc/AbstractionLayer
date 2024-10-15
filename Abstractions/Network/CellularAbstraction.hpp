/***************************************************************************//**
* @author   Ben Haubrich
* @file     CellularAbstraction.hpp
* @details  \b Synopsis: \n Interface for communication over the network.
* @ingroup  AbstractionLayer
*******************************************************************************/
#ifndef __CELLULAR_ABSTRACTION_HPP__
#define __CELLULAR_ABSTRACTION_HPP__

#include "NetworkAbstraction.hpp"

namespace CellularConfig {

    /**
     * @enum RadioAccessTechnology
     * @brief Cellular radio access technology.
    */
    enum class RadioAccessTechnology : uint8_t {
        Unknown = 0,
        Gsm,
        Cdma,
        Lte,
        NbIoT,
        GsmAndCdma,
        GsmAndLte,
        CdmaAndLte,
        GsmAndCdmaAndLte
    };

    /**
     * @enum AccessMode
     * @brief The access mode used to communicate with the cellular device.
     */
    enum class AccessMode : uint8_t {
        Unknown = 0,
        Transparent,
        Buffer,
        DirectPush
    };
}

/**
 * @class WifiAbstraction
 * @brief Interface for communication over the wifi.
*/
class CellularAbstraction : public NetworkAbstraction {
    public:
    /**
     * @fn CellularAbstraction
     * @sa CellularFactory::Factory
    */
    CellularAbstraction() = default;
    virtual ~CellularAbstraction() = default;

    /// @brief Tag for logging
    static constexpr char TAG[] = "Cellular";

    /**
     * @brief Reset the cellular device.
     * @returns The error type.
    */
    virtual ErrorType reset() = 0;
    /**
     * @brief Get the APN.
     * @returns The APN as a mutable reference.
    */
    std::string &accessPointName() { return _accessPointName; }
    /**
     * @brief Get the APN.
     * @returns The APN as a constant reference.
    */
    const std::string &accessPointNameConst() const { return _accessPointName; }
    /**
     * @brief Get the reset pin.
     * @returns The reset pin as a mutable reference.
    */
    PinNumber &resetPin() { return _resetPin; }
    /**
     * @brief Get the reset pin.
     * @returns The reset pin as a constant reference.
    */
    const PinNumber &resetPinConst() const { return _resetPin; }
    /**
     * @brief The IC peripheral used to communicate with the network device as a const reference.
     * @returns The IC peripheral as a constant reference.
    */
    const IcCommunicationProtocol &icConst() { return *_ic; }
    /**
     * @brief The IC peripheral used to communicate with the network device as a mutable pointer
     * @returns The IC peripheral as a mutable pointer.
    */
    std::unique_ptr<IcCommunicationProtocol> &ic() { return _ic; }
    /**
     * @brief Get the radio access technology.
     * @returns The radio access technology as a mutable reference.
    */
    CellularConfig::RadioAccessTechnology &radioAccessTechnology() { return _radioAccessTechnology; }
    /**
     * @brief Get the radio access technology.
     * @returns The radio access technology as a constant reference.
    */
    const CellularConfig::RadioAccessTechnology &radioAccessTechnologyConst() const { return _radioAccessTechnology; }
    /**
     * @brief Get the access mode.
     * @returns The access mode as a mutable reference.
    */
    CellularConfig::AccessMode &accessMode() { return _accessMode; }
    /**
     * @brief Get the access mode.
     * @returns The access mode as a constant reference.
    */
    const CellularConfig::AccessMode &accessModeConst() const { return _accessMode; }

    protected:
    /// @brief The Access Point Name (APN).
    std::string _accessPointName = std::string();
    /// @brief The pin number for the reset pin.
    PinNumber _resetPin = -1;
    /// @brief The IC peripheral used to communicate with the network device.
    std::unique_ptr<IcCommunicationProtocol> _ic;
    /// @brief The radio access technology.
    CellularConfig::RadioAccessTechnology _radioAccessTechnology = CellularConfig::RadioAccessTechnology::Unknown;
    /// @brief The access mode.
    CellularConfig::AccessMode _accessMode = CellularConfig::AccessMode::Unknown;
};

#endif