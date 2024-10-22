//AbstractionLayer Modules
#include "IpCellularClientModule.hpp"
#include "CellularModule.hpp"
#include "OperatingSystemModule.hpp"
//AbstractionLayer Applications
#include "Log.hpp"

ErrorType IpCellularClient::connectTo(std::string hostname, Port port, IpClientSettings::Protocol protocol, IpClientSettings::Version version, Socket &socket, Milliseconds timeout) {
    auto connectCb = [&]() -> ErrorType {
        _cellNetworkInterface = dynamic_cast<Cellular *>(&network());
        if (nullptr == _cellNetworkInterface) {
            CBT_LOGE(TAG, "Can't connect to network without a cellular interface");
            return ErrorType::NotSupported;
        }

        disconnect();

        if (version != IpClientSettings::Version::IPv4) {
            CBT_LOGE(TAG, "only IPv4 is implemented");
            return ErrorType::NotImplemented;
        }

        CBT_LOGI(TAG, "Connected to %s", hostname.c_str());
        _status.connected = false;

        return ErrorType::NotImplemented;
    };

    std::unique_ptr<EventAbstraction> event = std::make_unique<EventQueue::Event<IpCellularClient>>(std::bind(connectCb));
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
ErrorType IpCellularClient::disconnect() {
    return ErrorType::NotImplemented;
}
ErrorType IpCellularClient::sendBlocking(const std::string &data, const Milliseconds timeout) {
    return ErrorType::NotImplemented;
}
ErrorType IpCellularClient::receiveBlocking(std::string &buffer, const Milliseconds timeout) {
    return ErrorType::NotImplemented;
}
ErrorType IpCellularClient::sendNonBlocking(const std::shared_ptr<std::string> data, const Milliseconds timeout, std::function<void(const ErrorType error, const Bytes bytesWritten)> callback) {
    return ErrorType::NotImplemented;
}
ErrorType IpCellularClient::receiveNonBlocking(std::shared_ptr<std::string> buffer, const Milliseconds timeout, std::function<void(const ErrorType error, std::shared_ptr<std::string> buffer)> callback) {
    return ErrorType::NotImplemented;
}