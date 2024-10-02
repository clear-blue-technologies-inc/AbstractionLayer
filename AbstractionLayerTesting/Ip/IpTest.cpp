//Modules
#include "OperatingSystemModule.hpp"
#include "WifiModule.hpp"
#include "IpClientModule.hpp"
#include "IpServerModule.hpp"
//Applications
#include "Log.hpp"

static const char TAG[] = "IpTest";
static const std::string globalDataToSend("Hello World!");

static constexpr Port ServerPort = 44000;

struct ClientNetwork {
    IpClient *client;
    Wifi *wifi;
}wifiNetworkClient;

struct ServerNetwork {
    IpServer *server;
    Wifi *wifi;
}wifiNetworkServer;

static void *startNetworkThread(void *arg) {
    Wifi *wifi = reinterpret_cast<Wifi *>(arg);
    while (true) {
        OperatingSystem::Instance().delay(1);
        wifi->mainLoop();
    }
    return nullptr;
}

static void *startServerThread(void *arg) {
    ErrorType error;

    error = wifiNetworkServer.server->listenTo(IpServerSettings::Protocol::Tcp, IpServerSettings::Version::IPv4, ServerPort);
    if (ErrorType::Success != error) {
        CBT_LOGE(TAG, "Failed to listen on port %d", ServerPort);
        assert(false);
    }

    while(true);

    return nullptr;
}

static void *startClientThread(void *arg) {
    Socket socket = -1;
    ErrorType error;
    constexpr Milliseconds timeout = 1000;

    error = wifiNetworkClient.client->connectTo("localhost", ServerPort, IpClientSettings::Protocol::Tcp, IpClientSettings::Version::IPv4, socket, timeout);
    if (ErrorType::Success != error) {
        CBT_LOGE(TAG, "Failed to connect to server");
        assert(false);
    }
    else {
        assert(-1 != socket);
        CBT_LOGI(TAG, "Connected to server");
    }

    while(true);

    return nullptr;
}

static int blockingReceiveTest() {
    ErrorType error;
    Bytes received = 0;
    constexpr Milliseconds timeout = 5000;
    auto buffer = std::make_shared<std::string>(64, 0);

    assert(ErrorType::Success == (error = wifiNetworkServer.server->receiveNonBlocking(buffer, timeout)));
    
    if (0 != buffer->compare(0, globalDataToSend.length(), globalDataToSend)) {
        CBT_LOGE(TAG, "Data did not match. Bytes received: %u, Received/Expected %s/%s", buffer->size(), buffer->c_str(), globalDataToSend.c_str());
        assert(false);
    }

    return EXIT_SUCCESS;
}

static int blockingSendTest() {
    ErrorType error;
    Socket sock = -1;
    constexpr Milliseconds timeout = 1000;

    while (true) {
        error = wifiNetworkServer.server->acceptConnection(sock);
        if (ErrorType::Success != error) {
            continue;
        }
        else {
            assert(-1 != sock);
            CBT_LOGI(TAG, "Accepted connection");
            break;
        }
        OperatingSystem::Instance().delay(1);
    }

    error = wifiNetworkClient.client->sendNonBlocking(std::make_shared<std::string>(globalDataToSend), timeout);
    if (ErrorType::Success != error) {
        assert(false);
    }

    return EXIT_SUCCESS;
}

static int runAllTests() {
    std::vector<std::function<int(void)>> tests = {
        blockingSendTest,
        blockingReceiveTest
    };

    for (auto test : tests) {
        if (EXIT_SUCCESS != test()) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
int main() {
    ErrorType error;

    OperatingSystem::Init();
    Logger::Init();
    Wifi testWifi;

    if (ErrorType::Success != (error = testWifi.init())) {
        if (ErrorType::NotAvailable != error) {
            CBT_LOGE(TAG, "Failed to initialize wifi");
            return EXIT_FAILURE;
        }
    }

    std::string macAddress(32, 0);
    testWifi.getMacAddress(macAddress);

    //Not setting the ssid or the password. This test is being done on a desktop which likely means
    //the wifi is already connected to some access point and if it isn't, then you should do that before
    //running this test. Also since this just uses whatever network interface the IP address is routed to, so any
    //wifi or ethernet test would actually look exactly the same.

    IpClient client;
    client.setNetwork(testWifi);
    wifiNetworkClient.client = &client;
    wifiNetworkClient.wifi = &testWifi;

    IpServer server;
    server.setNetwork(testWifi);
    wifiNetworkServer.server = &server;
    wifiNetworkServer.wifi = &testWifi;

    constexpr uint16_t kilobyte = 1024;
    Id networkId, clientId, serverId;

    OperatingSystem::Instance().createThread(OperatingSystemConfig::Priority::High, "networkThread", &testWifi, 4*kilobyte, startNetworkThread, networkId);
    //Creating a seperate thread for the client and server is not required. You just want the server running independently of the client
    //so that it can loop and accept connections.
    OperatingSystem::Instance().createThread(OperatingSystemConfig::Priority::Normal, "serverThread", nullptr, 4*kilobyte, startServerThread, networkId);
    OperatingSystem::Instance().createThread(OperatingSystemConfig::Priority::Low, "clientThread", nullptr, 4*kilobyte, startClientThread, networkId);

    int result = runAllTests();

    return result;
}