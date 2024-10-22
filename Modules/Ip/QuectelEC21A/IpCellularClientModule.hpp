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
    IpCellularClient() : IpClientAbstraction() {};
    ~IpCellularClient() = default;

    ErrorType connectTo(std::string hostname, Port port, IpClientSettings::Protocol protocol, IpClientSettings::Version version, Socket &socket, Milliseconds timeout) override;
    ErrorType disconnect() override;
    ErrorType sendNonBlocking(const std::shared_ptr<std::string> data, const Milliseconds timeout, std::function<void(const ErrorType error, const Bytes bytesWritten)> callback = nullptr) override;
    ErrorType receiveNonBlocking(std::shared_ptr<std::string> buffer, const Milliseconds timeout, std::function<void(const ErrorType error, std::shared_ptr<std::string> buffer)> callback = nullptr) override;

    private:
    ErrorType sendBlocking(const std::string &data, const Milliseconds timeout) override;
    ErrorType receiveBlocking(std::string &buffer, const Milliseconds timeout) override;

    Cellular *_cellNetworkInterface = nullptr;
};

#endif // __IP_CELLULAR_CLIENT_MODULE_HPP__