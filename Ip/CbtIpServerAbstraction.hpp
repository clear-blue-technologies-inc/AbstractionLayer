/***************************************************************************//**
* @author   Ben Haubrich
* @file     CbtIpServerAbstraction.hpp
* @details  \b Synopsis: \n Interface for creating a server on any network
* @ingroup  FoundationCore
*******************************************************************************/
#ifndef __CBT_IP_SERVER_ABSTRACTION_HPP__
#define __CBT_IP_SERVER_ABSTRACTION_HPP__

//Foundation
#include "CbtTypes.hpp"
#include "CbtError.hpp"
//C++
#include <memory>
#include <string>
#include <functional>

/**
 * @struct ServerStatus
 * @brief Server status
*/
struct ServerStatus {
    bool listening; ///< True when the server is listening for connections.
};

namespace IpServerSettings {

    /**
     * @enum Version
     * @brief The version to use for the IP connection
    */
    enum class Version : uint8_t {
        Unknown = 0, ///< Unknown
        IPv4,        ///< Internet Protocol Version 4
        IPv6         ///< Internet Protocol Version 6
    };

    /**
     * @enum Protocol
     * @brief The protocol to use for the IP connection
    */
    enum class Protocol : uint8_t {
        Unknown = 0, ///< Unknown
        Tcp,         ///< Transmission Control Protocol
        Udp          ///< User Datagram Protocol
    };
}

class NetworkAbstraction;


/**
 * @class IpServerAbstraction
 * @brief Creates a server on any network
 */
class IpServerAbstraction {

    public:
    /// @brief Constructor
    IpServerAbstraction() { _status.listening = false; };
    /// @brief Destructor
    virtual ~IpServerAbstraction() = default;

    /**
     * @brief Listen for connections on a port
     * @param[in] protocol The protocol to use for the connection
     * @sa IpServerSettings::Protocol
     * @param[in] version The version to use for the connection
     * @param[in] port The port to listen to
     * @sa IpServerSettings::Version
    */
    virtual ErrorType listenTo(IpServerSettings::Protocol protocol, IpServerSettings::Version version, Port port) = 0;
    /**
     * @brief Accept a connection from a client connecting to the socket given
     * @param[in] socket The socket to accept the connection on
     * @returns Fnd::ErrorType::Success on success
    */
    virtual ErrorType acceptConnection(Socket &socket) = 0;
    /**
     * @brief Close the connection
     * @returns Fnd::ErrorType::Success on success
    */
    virtual ErrorType closeConnection() = 0;
    /**
     * @brief Send data to a socket
     * @pre data should be appropriately sized with the correct amount of bytes you want to send, i.e data.resize()
     * @param[in] data The data to send
     * @param[in] timeout The amount of time to wait for a response
     * @returns Fnd::ErrorType::Success on success
     * @returns Fnd::ErrorType::Failure on failure
     * @returns Fnd::ErrorType::NotImplemented if not implemented
     * @returns Fnd::ErrorType::Timeout if a timeout occurred
    */
    virtual ErrorType sendBlocking(const std::string &data, const Milliseconds timeout) = 0;
    /**
     * @brief Receive data from a socket
     * @pre buffer should be appropriately sized with the correct amount of bytes you want to receive, i.e buffer.resize()
     * @param[out] buffer The buffer to receive data into
     * @param[in] timeout The amount of time to wait for a response
     * @returns Fnd::ErrorType::Success on success
     * @returns Fnd::ErrorType::Failure on failure
     * @returns Fnd::ErrorType::NotImplemented if not implemented
     * @returns Fnd::ErrorType::Timeout if a timeout occurred
     * @post The number of bytes received is equal to the size of the buffer (i.e buffer.size())
    */
    virtual ErrorType receiveBlocking(std::string &buffer, const Milliseconds timeout) = 0;
    /**
     * @brief Receive data from this client
     * @pre data should be appropriately sized with the correct amount of bytes you want to send, i.e buffer.resize()
     * @param[in] data The data to send
     * @param[in] callback The callback to call when the data is received
     * @code
     * //TODO: add an example of a callback and how to give it to this function
     * @endcode
     * @returns Fnd::ErrorType::Success on success
     * @returns Fnd::ErrorType::Failure on failure
     * @returns Fnd::ErrorType::NotImplemented if not implemented
     * @returns Fnd::ErrorType::Timeout if a timeout occurred
    */
    virtual ErrorType sendNonBlocking(const std::shared_ptr<std::string> data, const Milliseconds timeout, std::function<void(const ErrorType error, const Bytes bytesWritten)> callback = nullptr) = 0;
    /**
     * @brief Receive data from this client
     * @pre buffer should be appropriately sized with the correct amount of bytes you want to receive, i.e buffer.resize()
     * @param[out] buffer The buffer to receive data into
     * @param[in] callback The callback to call when the data is received
     * @code
     * //TODO: add an example of a callback and how to give it to this function
     * @endcode
     * @returns Fnd::ErrorType::Success on success
     * @returns Fnd::ErrorType::Failure on failure
     * @returns Fnd::ErrorType::NotImplemented if not implemented
     * @returns Fnd::ErrorType::Timeout if a timeout occurred
    */
    virtual ErrorType receiveNonBlocking(std::shared_ptr<std::string> buffer, const Milliseconds timeout, std::function<void(const ErrorType error, std::shared_ptr<std::string> buffer)> callback = nullptr) = 0;

    /**
     * @brief Get the socket
    */
    Socket getSocket() const { return _socket; }
    /**
     * @brief Get the protocol
    */
    IpServerSettings::Protocol protocol() const { return _protocol; }
    /**
     * @brief Get the version
    */
    IpServerSettings::Version version() const { return _version; }
    /**
     * @brief Get the port
    */
    Port port() const { return _port; }
    /**
     * @brief Get the network abstraction
    */
    NetworkAbstraction &network() const { return *_network; }
    /**
     * @brief Get the status of the server
    */
    ServerStatus status() const { return _status; }
    /**
     * @brief Set the network abstraction
    */
    ErrorType setNetwork(NetworkAbstraction &network) { _network = &network; return ErrorType::Success; }

    protected:
    /// @brief The socket
    Socket _socket = -1;
    /// @brief The protocol
    IpServerSettings::Protocol _protocol = IpServerSettings::Protocol::Unknown;
    /// @brief The IP version
    IpServerSettings::Version _version = IpServerSettings::Version::Unknown;
    /// @brief The port
    Port _port = 0;
    /// @brief The status of the server
    ServerStatus _status;

    private:
    /// @brief The network abstraction that this server communicates on.
    NetworkAbstraction *_network = nullptr;

};

#endif //__CBT_IP_SERVER_ABSTRACTION_HPP__