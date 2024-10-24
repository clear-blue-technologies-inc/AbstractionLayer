/***************************************************************************//**
* @author   Ben Haubrich
* @file     IpCellularClientModule.hpp
* @details  Cellular IP client for Quectel EC21A.
* @ingroup  QuectelEC21A Modules
*******************************************************************************/
#ifndef __IP_CELLULAR_CLIENT_MODULE_HPP__
#define __IP_CELLULAR_CLIENT_MODULE_HPP__

//AbstractionLayer
#include "IpClientAbstraction.hpp"

class Cellular;

class IpCellularClient : public IpClientAbstraction {

    public:
    IpCellularClient() : IpClientAbstraction() {}
    ~IpCellularClient() = default;

    ErrorType connectTo(std::string hostname, Port port, IpClientSettings::Protocol protocol, IpClientSettings::Version version, Socket &socket, Milliseconds timeout) override;
    ErrorType disconnect() override;
    ErrorType sendNonBlocking(const std::shared_ptr<std::string> data, const Milliseconds timeout, std::function<void(const ErrorType error, const Bytes bytesWritten)> callback = nullptr) override;
    ErrorType receiveNonBlocking(std::shared_ptr<std::string> buffer, const Milliseconds timeout, std::function<void(const ErrorType error, std::shared_ptr<std::string> buffer)> callback = nullptr) override;

    private:
    Cellular *_cellNetworkInterface = nullptr;

    ErrorType sendBlocking(const std::string &data, const Milliseconds timeout) override;
    ErrorType receiveBlocking(std::string &buffer, const Milliseconds timeout) override;

    /**
     * @brief block and poll for data.
     * @pre The access mode must be buffer access mode to poll for data.
     * @returns ErrorType::Success if there is data avilable.
     * @returns ErrorType::Timeout if the time ran out before any data was avilable
     * @returns ErrorType::PrerequisitesNotMet if the current access mode is not buffer access mode.
     */
    ErrorType pollForData(const Socket socket, const Milliseconds timeout);
};

#endif // __IP_CELLULAR_CLIENT_MODULE_HPP__