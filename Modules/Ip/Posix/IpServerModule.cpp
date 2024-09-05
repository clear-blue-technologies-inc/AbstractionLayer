#include "IpServerModule.hpp"

//Posix
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
//Stdlib
#include <unistd.h>
//C++
#include <cassert>
#include <cstring>

ErrorType IpServer::listenTo(IpServerSettings::Protocol protocol, IpServerSettings::Version version, Port port) {
    Socket sock = -1;
    struct addrinfo hints;
    struct addrinfo *servinfo = nullptr;
    struct addrinfo *p = nullptr;
    char portString[] = "65535";

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = toPosixFamily(version);
    hints.ai_socktype = toPosixSocktype(protocol);
    hints.ai_flags = AI_PASSIVE;

    assert(snprintf(portString, sizeof(portString), "%u", port) > 0);

    if (0 != getaddrinfo(nullptr, portString, &hints, &servinfo)) {
        return toPlatformError(errno);
    }

    for (p = servinfo; p != nullptr; p = p->ai_next) {
        if (-1 == (sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol))) {
            continue;
        }
        
        int enable = 1;
        if (-1 == setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable))) {
            return toPlatformError(errno);
        }

        if (-1 == bind(sock, p->ai_addr, p->ai_addrlen)) {
            close(sock);
            continue;
        }

        break;
    }

    //For more connections, create another instance of this class.
    _status.listening = true;
    if (-1 == listen(sock, 1)) {
        _status.listening = false;
        return toPlatformError(errno);
    }

    freeaddrinfo(servinfo);

    //Socket is still invalid. The socket we just had is only for listening for connections.
    //The socket we get from accept can be used to send and received which is the one we want
    //to return to the user.
    _socket = sock;
    _protocol = protocol;
    _version = version;
    _port = port;

    return ErrorType::Success;
}
ErrorType IpServer::acceptConnection(Socket &socket) {
    struct sockaddr_storage clientAddress;
    socklen_t receiveSocketSize;

    if (-1 == (socket = accept(_socket, (struct sockaddr *)&clientAddress, &receiveSocketSize))) {
        return toPlatformError(errno);
    }

        //Overwrite the socket we used to listen for connections with the one that will be used to send and received
        //Since we only accept one connection per class.
        _socket = socket;
    return ErrorType::Success;
}

ErrorType IpServer::closeConnection() {
    return ErrorType::NotImplemented;
}

ErrorType IpServer::sendBlocking(const std::string &data, const Milliseconds timeout) {
    ssize_t bytesSent = 0;
    if (-1 == (bytesSent = send(_socket, data.data(), data.size(), 0))) {
        return toPlatformError(errno);
    }

    return ErrorType::Success;
}

ErrorType IpServer::receiveBlocking(std::string &buffer, const Milliseconds timeout) {
    ssize_t bytesReceived = 0;

    if (-1 == (bytesReceived = (recv(_socket, buffer.data(), buffer.size(), 0)))) {
        return toPlatformError(errno);
    }

    buffer.resize(bytesReceived);

    return ErrorType::Success;
}
ErrorType IpServer::sendNonBlocking(const std::shared_ptr<std::string> data, const Milliseconds timeout, std::function<void(const ErrorType error, const Bytes bytesWritten)> callback) {
    return ErrorType::NotImplemented;
}
ErrorType IpServer::receiveNonBlocking(std::shared_ptr<std::string> buffer, const Milliseconds timeout, std::function<void(const ErrorType error, std::shared_ptr<std::string> buffer)> callback) {
    return ErrorType::NotImplemented;
}