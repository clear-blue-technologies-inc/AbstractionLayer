//AbstractionLayer Modules
#include "IpCellularClientModule.hpp"
#include "CellularModule.hpp"
#include "OperatingSystemModule.hpp"
//AbstractionLayer Applications
#include "Log.hpp"

/**
 * @brief Convert the IP client version to the Quectel context type
 * 
 * @param version The IP client version
 * @return The Quectel context type
 */
static ContextType ToQuectelContextType(const IpClientSettings::Version version) {
    switch (version) {
        case IpClientSettings::Version::IPv4:
            return ContextType::Ipv4;
        case IpClientSettings::Version::IPv6:
            return ContextType::Ipv6;
        default:
            return ContextType::Unknown;
    }
}

static std::string ToQuectelProtocol(const IpClientSettings::Protocol protocol) {
    switch (protocol) {
        case IpClientSettings::Protocol::Tcp:
            return "TCP";
        case IpClientSettings::Protocol::Udp:
            return "UDP";
        default:
            return "";
    }
}

static int8_t ToQuectelAccessMode(const CellularConfig::AccessMode accessMode) {
    switch (accessMode) {
        case CellularConfig::AccessMode::Buffer:
            return 0;
        case CellularConfig::AccessMode::DirectPush:
            return 1;
        case CellularConfig::AccessMode::Transparent:
            return 2;
        default:
            return -1;
    }
}

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

        if (ErrorType::LimitReached == _cellNetworkInterface->nextAvailableConnectionId(socket)) {
            CBT_LOGW(TAG, "No more sockets available");
            socket = -1;
            return ErrorType::LimitReached;
        }

        ErrorType error = _cellNetworkInterface->pdpContextIsActive(Cellular::_IpContext);
        if (ErrorType::Success != error) {
            error = _cellNetworkInterface->activatePdpContext(Cellular::_IpContext, socket, ToQuectelContextType(version), _cellNetworkInterface->accessPointName());
            if (ErrorType::Success != error) {
                socket = -1;
                return error;
            }
        }

        std::string quectelProtocol = ToQuectelProtocol(protocol);
        if (quectelProtocol.empty()) {
            CBT_LOGE(TAG, "Invalid protocol");
            socket = -1;
            return ErrorType::InvalidParameter;
        }

        std::string openSocketCommand("AT+QIOPEN=");
        openSocketCommand.append(std::to_string(Cellular::_IpContext));
        openSocketCommand.append(",").append(std::to_string(socket));
        openSocketCommand.append(",").append("\"").append(quectelProtocol).append("\"");
        openSocketCommand.append(",").append("\"").append(hostname).append("\"");
        openSocketCommand.append(",").append(std::to_string(port));
        openSocketCommand.append(",").append(std::to_string(0));
        openSocketCommand.append(",").append(std::to_string(ToQuectelAccessMode(_cellNetworkInterface->accessModeConst())));

        error = _cellNetworkInterface->sendCommand(openSocketCommand, 1000, 10);
        if (ErrorType::Success != error) {
            socket = -1;
            return error;
        }

        error = _cellNetworkInterface->receiveCommand(openSocketCommand, 1000, 10, "OK");
        if (ErrorType::Success != error) {
            socket = -1;
            return error;
        }

        CBT_LOGI(TAG, "Connected to %s", hostname.c_str());
        _status.connected = true;

        return ErrorType::Success;
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