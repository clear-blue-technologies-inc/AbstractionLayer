/***************************************************************************//**
* @author   Ben Haubrich
* @file     NetworkAbstraction.hpp
* @details  \b Synopsis: \n Interface for communication over the network.
* @ingroup  AbstractionLayer
*******************************************************************************/
#ifndef __NETWORK_ABSTRACTION_HPP__
#define __NETWORK_ABSTRACTION_HPP__

//Foundation
#include "IcCommunicationProtocol.hpp"
//C++
#include <string>

namespace NetworkTypes {
    
    /**
     * @enum Technology
     * @brief The technology of the network interface.
     * @note If you google wifi, 3g, or 4g, It will say they are all wireless communication technologies.
     *       That's where the name comes from.
    */
    enum class Technology {
        Unknown = 0, ///< Unknown
        Wifi,        ///< Wi-Fi
        Zigbee,      ///< ZigBee
        Ethernet,    ///< Ethernet
        Cellular,    ///< Cellular
    };

    /**
     * @struct NetworkStatus
     * @brief The status of the network interface.
    */
    struct Status {
        bool isUp;                           ///< True when the network is up and ready for use.
        Technology technology;               ///< The technology of the network interface.
        std::string manufacturerName;        ///< The manufacturer name of the network interface.
        DecibelMilliWatts signalStrength;    ///< The signal strength of the network interface.
    };
}

/**
 * @class NetworkAbstraction
 * @brief Interface for communication over the network.
*/
class NetworkAbstraction : public CommunicationProtocol {
    public:
    /**
     * @fn NetworkAbstraction
     * @sa NetworkFactory::Factory
    */
    NetworkAbstraction() = default;
    virtual ~NetworkAbstraction() = default;

    /// @brief Tag for logging
    static constexpr char TAG[] = "Network";

    /**
    * @brief Initialize the interface.
    * @returns ErrorType::Success if the network interface was initialized and ready for clients to connect.
    * @returns ErrorType::NotImplemented if the network interface is not implemented.
    * @returns ErrorType::NotSupported if the network interface is not supported by the current hardware
    * @returns ErrorType::InvalidParameter if any of the required configuration variables have not been set before init was called.
    * @returns ErrorType::NotAvailable if the underlying implementation can't perform the operating (e.g. init wifi driver on Windows).
    * @attention Esp wifi You must set the WifiConfig::Mode before calling.
    * @sa setMode
    * @post May block for up to a maximum of 10 seconds to bring the interface up.
    * @post Clients are able to connect to a host after this function returns ErrorType::Success.
    */
        virtual ErrorType init() = 0;
    /**
     * @brief Bring up the network interface so that it is ready for use (e.g. IP connections)
     * @returns ErrorType::Success if the network interface was brought up successfully
     * @returns ErrorType::Failure if the network interface could not be brought up
     * @returns ErrorType::NotImplemented if not implemented
     * @returns ErrorType::NotAvailable if the underlying implementation can't perform the operating.
    */
    virtual ErrorType networkUp() = 0;
    /**
     * @brief Bring down the network interface.
     * @returns ErrorType::Success if the network interface was brought down successfully
     * @returns ErrorType::Failure if the network interface could not be brought down
     * @returns ErrorType::NotImplemented if not implemented
     * @returns ErrorType::NotAvailable if the underlying implementation can't perform the operating.
    */
    virtual ErrorType networkDown() = 0;
    /**
     * @brief Transmit a frame of data.
     * @param frame The frame of data to transmit
     * @param timeout The timeout in milliseconds to wait for the transmission to complete
     * @returns ErrorType::Success if the transmission was successful
     * @returns ErrorType::Failure if the transmission failed
     * @returns ErrorType::NotImplemented if not implemented
     * @returns ErrorType::NotAvailable if the underlying implementation can't perform the operating.
    */
    virtual ErrorType txBlocking(const std::string &frame, const Milliseconds timeout) = 0;
    /**
     * @sa txBlocking
     * @param frame The frame to transmit.
     * @param callback Function that is called when transmission is complete
     * @returns ErrorType::NotAvailable if the underlying implementation can't perform the operating.
     * @code 
     * //Function member signature:
     * void callback(ErrorType error, const Bytes bytesWritten) { return ErrorType::Success; }
     * //lambda signature:
     * auto callback = [](const ErrorType error, const Bytes bytesWritten) { return ErrorType::Success; }
     * @endcode
    */
    virtual ErrorType txNonBlocking(const std::shared_ptr<std::string> frame, std::function<void(const ErrorType error, const Bytes bytesWritten)> callback = nullptr) = 0;
    /**
     * @brief Receive a frame of data.
     * @param frameBuffer The buffer to store the received frame data.
     * @param timeout The timeout in milliseconds to wait for the transmission to complete
     * @returns ErrorType::Success if the frame was successfully received
     * @returns ErrorType::Failure if the frame was not received
     * @returns ErrorType::NotImplemented if not implemented
     * @returns ErrorType::NotAvailable if the underlying implementation can't perform the operating.
    */
    virtual ErrorType rxBlocking(std::string &frameBuffer, const Milliseconds timeout) = 0;
    /**
     * @sa rxBlocking
     * @param frameBuffer The buffer to store the received frame data.
     * @param callback Function that is called when the frame has been received
     * @returns ErrorType::NotAvailable if the underlying implementation can't perform the operating.
     * @code 
     * //Function member signature:
     * void callback(ErrorType error, std::shared_ptr<std::string> frameBuffer) { return ErrorType::Success; }
     * //lambda signature:
     * auto callback = [](const ErrorType error, std::shared_ptr<std::string> frameBuffer) { return ErrorType::Success; }
     * @endcode
    */
    virtual ErrorType rxNonBlocking(std::shared_ptr<std::string> frameBuffer, std::function<void(const ErrorType error, std::shared_ptr<std::string> frameBuffer)> callback = nullptr) = 0;
    /**
     * @brief Get the MAC address of this network interface.
     * @param[out] macAddress The MAC address of this network interface.
     * @returns ErrorType::Success if the MAC address was successfully retrieved
     * @returns ErrorType::Failure if the MAC address was not successfully retrieved
    */
    virtual ErrorType getMacAddress(std::string &macAddress) = 0;
    /**
    * @brief Get the signal strength of the network interface.
    * @param[out] signalStrength The signal strength of the network interface.
    * @returns ErrorType::Success if the signal strength was successfully retrieved
    * @returns ErrorType::Failure if the signal strength was not successfully retrieved
    * @returns ErrorType::NotImplemented if not implemented
    * @returns ErrorType::NotAvailable if the underlying implementation can't perform the operating.
    */
    virtual ErrorType getSignalStrength(DecibelMilliWatts &signalStrength) = 0;

    ErrorType sendBlocking(const std::string &data, const Milliseconds timeout) override {
        return txBlocking(data, timeout);
    }
    ErrorType sendNonBlocking(const std::shared_ptr<std::string> data, std::function<void(const ErrorType error, const Bytes bytesWritten)> callback = nullptr) override {
        return txNonBlocking(data, callback);
    }
    ErrorType receiveBlocking(std::string &buffer, const Milliseconds timeout) override {
        return rxBlocking(buffer, timeout);
    }
    ErrorType receiveNonBlocking(std::shared_ptr<std::string> buffer, std::function<void(const ErrorType error, std::shared_ptr<std::string> buffer)> callback = nullptr) {
        return rxNonBlocking(buffer, callback);
    }

    /// @brief The current status of the network interface as a const reference.
    const NetworkTypes::Status &statusConst() { return _status; }

    protected:
    /// @brief The current status of the network interface
    NetworkTypes::Status _status;
};

#endif // __NETWORK_ABSTRACTION_HPP__