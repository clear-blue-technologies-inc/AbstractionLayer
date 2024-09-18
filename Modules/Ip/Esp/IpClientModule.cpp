//Modules
#include "IpClientModule.hpp"
#include "WifiModule.hpp"
#include "OperatingSystemModule.hpp"
#include "Log.hpp"
//Lwip
#include "lwip/netdb.h"
#include "lwip/tcp.h"
#include "lwip/dns.h"
#include "lwip/ip_addr.h"
//C++
#include <cassert>
#include <cstring>

/*
 * I tried the example code for non-blocking sockets from ESP github, it didn't work.
 * getaddrinfo would return a new IP address every time you call it and none of them
 * worked for v2devshard. If you didn't call gethostbyname (which getaddrinfo is meant
 * to replace) getaddrinfo would fail to find an IP every time you call it. Lastly, if you
 * call fcntl and set O_NONBLOCK on the socket it would fail to connect every time so there
 * was quite a symphony of bugs there.
*/
ErrorType IpClient::connectTo(std::string hostname, Port port, IpClientSettings::Protocol protocol, IpClientSettings::Version version, Socket &sock, Milliseconds timeout) {

    auto connectCb = [&]() -> ErrorType {
        disconnect();

        if (version != IpClientSettings::Version::IPv4) {
            CBT_LOGE(TAG, "only IPv4 is implemented");
            return ErrorType::NotImplemented;
        }

        //While I think I have the code to do the rest of this in Ipv6, I don't know the code to do the DNS stuff in Ipv6.
        ip_addr_t ip_addr;
        ip_addr.type = IPADDR_TYPE_V4;
        uint8_t dns_server_ip[] = {8,8,8,8};
        IP4_ADDR(&ip_addr.u_addr.ip4, dns_server_ip[0], dns_server_ip[1], dns_server_ip[2], dns_server_ip[3]);
        dns_setserver(0, &ip_addr);
        struct hostent *hent = gethostbyname(hostname.c_str());
        if (NULL == hent) {
            CBT_LOGE(TAG, "couldn't get address for %s", hostname.c_str());
            return ErrorType::Failure;
        }
        struct in_addr **addr_list = (struct in_addr **)hent->h_addr_list;
        struct sockaddr_in dest_ip;
        dest_ip.sin_addr.s_addr = addr_list[0]->s_addr;
        dest_ip.sin_family = toPosixFamily(version);
        dest_ip.sin_port = htons(port);

        if (-1 == (_socket = socket(toPosixFamily(version), toPosixSocktype(protocol), IPPROTO_IP))) {
        CBT_LOGE(TAG, "couldn't create socket");
        return ErrorType::Failure;
        }

        if (-1 == connect(_socket, (struct sockaddr *)&dest_ip, sizeof(dest_ip))) {
        CBT_LOGE(TAG, "couldn't connect to %s (%s)", hostname.c_str(), inet_ntoa(*(struct in_addr *)hent->h_addr_list[0]));
        close(_socket);
        return ErrorType::Failure;
        }

        CBT_LOGI(TAG, "connection in progress");
        fd_set fdset;
        FD_ZERO(&fdset);
        FD_SET(_socket, &fdset);

        // Connection in progress -> have to wait until the connecting socket is marked as writable, i.e. connection completes
        int res = select(_socket+1, NULL, &fdset, NULL, NULL);
        if (res < 0) {
            CBT_LOGE(TAG, "Error during connection: select for socket to be writable %s", strerror(errno));
            return ErrorType::Failure;
        } else if (res == 0) {
            CBT_LOGE(TAG, "Connection timeout: select for socket to be writable %s", strerror(errno));
            return ErrorType::Failure;
        } else {
            int sockerr;
            socklen_t len = (socklen_t)sizeof(int);

            if (getsockopt(_socket, SOL_SOCKET, SO_ERROR, (void*)(&sockerr), &len) < 0) {
                CBT_LOGE(TAG, "Error when getting socket error using getsockopt() %s", strerror(errno));
                return ErrorType::Failure;
            }
            if (sockerr) {
                CBT_LOGE(TAG, "Connection error %d", sockerr);
                return ErrorType::Failure;
            }
        }

        CBT_LOGI(TAG, "Connected to %s (%s)", hostname.c_str(), inet_ntoa(*(struct in_addr *)hent->h_addr_list[0]));
        _status.connected = true;
        return ErrorType::Success;
    };

    std::unique_ptr<EventAbstraction> event = std::make_unique<EventQueue::Event<IpClient>>(std::bind(connectCb));
    if (ErrorType::Success != network().addEvent(event)) {
        return ErrorType::Failure;
    }

    {
    Milliseconds i;
    for (i = 0; i < timeout / 10 && !_status.connected; i++) {
        OperatingSystem::Instance().delay(10);
    }

    if (!_status.connected && (timeout / 10) == i) {
        return ErrorType::Timeout;
    }
    else if (!_status.connected) {
        return ErrorType::Failure;
    }
    else {
        return ErrorType::Success;
    }
    }
}

ErrorType IpClient::disconnect() {
    if (_socket != -1) {
        shutdown(_socket, 0);
        close(_socket);
        _socket = -1;
    }

    return ErrorType::Success;
}

//TODO: Timeout is not implemented
ErrorType IpClient::sendBlocking(const std::string &data, const Milliseconds timeout) {
    assert(0 != _socket);

    if (-1 == send(_socket, data.data(), data.size(), 0)) {
        _status.connected = false;
        return toPlatformError(errno);
    }

    return ErrorType::Success;
}

ErrorType IpClient::receiveBlocking(std::string &buffer, const Milliseconds timeout) {
    assert(0 != _socket);

    ErrorType error = ErrorType::Failure;
    ssize_t bytesReceived = 0;

    struct timeval timeoutval = {
        .tv_sec = timeout / 1000,
        .tv_usec = 0
    };
    fd_set readfds;

    FD_ZERO(&readfds);
    FD_SET(_socket, &readfds);

    //Wait for input from the socket until the timeout
    {
    int ret;
    ret = select(_socket + 1, &readfds, NULL, NULL, &timeoutval);
    if (ret < 0) {
        return toPlatformError(errno);
    }
    }

    if (FD_ISSET(_socket, &readfds)) {
        if (-1 == (bytesReceived = recv(_socket, buffer.data(), buffer.size(), 0))) {
            error = toPlatformError(errno);
        }
        else if ((size_t)bytesReceived > buffer.size()) {
            error = ErrorType::PrerequisitesNotMet;
        }
        else {
            buffer.resize(bytesReceived);
            return ErrorType::Success;
        }
    }
    else {
        error = ErrorType::Timeout;
    }

    buffer.resize(0);
    _status.connected = false;

    return error;
}

ErrorType IpClient::sendNonBlocking(const std::shared_ptr<std::string> data, const Milliseconds timeout, std::function<void(const ErrorType error, const Bytes bytesWritten)> callback) {
    bool sent = false;

    auto tx = [this, callback, &sent](const std::shared_ptr<std::string> frame, const Milliseconds timeout) -> ErrorType {
        ErrorType error = ErrorType::Failure;

        if (nullptr == frame.get()) {
            return ErrorType::NoData;
        }

        error = sendBlocking(*frame, timeout);

        if (nullptr != callback) {
            callback(error, frame->size());
        }

        sent = true;
        return error;
    };

    std::unique_ptr<EventAbstraction> event = std::make_unique<EventQueue::Event<IpClient>>(std::bind(tx, data, timeout));
    ErrorType error = network().addEvent(event);
    if (ErrorType::Success != error) {
        return error;
    }

    //Block for the timeout specified if no callback is provided
    if (nullptr == callback) {
        Milliseconds i;
        for (i = 0; i < timeout / 10 && !sent; i++) {
            OperatingSystem::Instance().delay(10);
        }

        if (!sent && (timeout / 10) == i) {
            return ErrorType::Timeout;
        }
        else if (!sent) {
            return ErrorType::Failure;
        }
        else {
            return ErrorType::Success;
        }
    }

    return ErrorType::Success;
}

ErrorType IpClient::receiveNonBlocking(std::shared_ptr<std::string> buffer, const Milliseconds timeout, std::function<void(const ErrorType error, std::shared_ptr<std::string> buffer)> callback) {
    bool received = false;

    auto rx = [this, callback, &received](const std::shared_ptr<std::string> buffer, const Milliseconds timeout) -> ErrorType {
        ErrorType error = ErrorType::Failure;

        if (nullptr == buffer.get()) {
            return ErrorType::NoData;
        }

        error = receiveBlocking(*buffer, timeout);

        if (nullptr != callback) {
            callback(error, buffer);
        }

        received = true;
        return error;
    };

    std::unique_ptr<EventAbstraction> event = std::make_unique<EventQueue::Event<IpClient>>(std::bind(rx, buffer, timeout));
    ErrorType error = network().addEvent(event);
    if (ErrorType::Success != error) {
        return error;
    }

    //Block for the timeout specified if no callback is provided
    if (nullptr == callback) {
        Milliseconds i;
        for (i = 0; i < timeout / 10 && !received; i++) {
            OperatingSystem::Instance().delay(10);
        }

        if (!received && (timeout / 10) == i) {
            return ErrorType::Timeout;
        }
        else if (!received) {
            return ErrorType::Failure;
        }
        else {
            return ErrorType::Success;
        }
    }

    return ErrorType::Success;
}