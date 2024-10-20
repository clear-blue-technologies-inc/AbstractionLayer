#include "IpServerModule.hpp"

ErrorType IpServer::listenTo(IpServerSettings::Protocol protocol, IpServerSettings::Version version, Port port) {
    return ErrorType::NotImplemented;
}

ErrorType IpServer::acceptConnection(Socket &socket) {
    return ErrorType::NotImplemented;
}

ErrorType IpServer::closeConnection() {
    return ErrorType::NotImplemented;
}

ErrorType IpServer::sendBlocking(const std::string &data, const Milliseconds timeout) {
    return ErrorType::NotImplemented;
}

ErrorType IpServer::receiveBlocking(std::string &buffer, const Milliseconds timeout) {
    return ErrorType::NotImplemented;
}

ErrorType IpServer::sendNonBlocking(const std::shared_ptr<std::string> data, const Milliseconds timeout, std::function<void(const ErrorType error, const Bytes bytesWritten)> callback) {
    return ErrorType::NotImplemented;
}

ErrorType IpServer::receiveNonBlocking(std::shared_ptr<std::string> buffer, const Milliseconds timeout, std::function<void(const ErrorType error, std::shared_ptr<std::string> buffer)> callback) {
    return ErrorType::NotImplemented;
}