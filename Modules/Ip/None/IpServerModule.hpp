#ifndef __IP_SERVER_MODULE_HPP__
#define __IP_SERVER_MODULE_HPP__

#include "IpServerAbstraction.hpp"

class IpServer : public IpServerAbstraction {

    public:
    ErrorType listenTo(IpServerSettings::Protocol protocol, IpServerSettings::Version version, Port port) override;
    ErrorType acceptConnection(Socket &socket) override;
    ErrorType closeConnection() override;
    ErrorType sendBlocking(const std::string &data, const Milliseconds timeout) override;
    ErrorType receiveBlocking(std::string &buffer, const Milliseconds timeout) override;
    ErrorType sendNonBlocking(const std::shared_ptr<std::string> data, const Milliseconds timeout, std::function<void(const ErrorType error, const Bytes bytesWritten)> callback = nullptr) override;
    ErrorType receiveNonBlocking(std::shared_ptr<std::string> buffer, const Milliseconds timeout, std::function<void(const ErrorType error, std::shared_ptr<std::string> buffer)> callback = nullptr) override;
};

#endif // __IP_SERVER_MODULE_HPP__