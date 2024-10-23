//Common
#include "Log.hpp"
//Modules
#include "WifiModule.hpp"
#include "OperatingSystemModule.hpp"
//C++
#include <cstring>
//ESP
#include "esp_wifi_types.h"
#include "esp_mac.h" //For setting the Access Point ssid

#ifdef __cplusplus
extern "C" {
#endif
static void WifiEventHandler(void *eventHandleArg, esp_event_base_t event_base, int32_t event_id, void *event_data);
#ifdef __cplusplus
}
#endif

ErrorType Wifi::init() {
    esp_err_t err;
    ErrorType error;

    if (WifiConfig::Mode::Unknown == mode()) {
        return ErrorType::InvalidParameter;
    }
    if (ErrorType::Success != (error = networkUp())) {
        return error;
    }
    if (ESP_OK != (err = esp_event_loop_create_default())) {
        bool isCriticalErrror = !(err = ESP_ERR_INVALID_STATE);
        if (isCriticalErrror) {
            return toPlatformError(err);
        }
    }

    err = esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_START, &WifiEventHandler, this);
    if (ESP_OK != err) {
        return toPlatformError(err);
    }
    err = esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &WifiEventHandler, this);
    if (ESP_OK != err) {
        return toPlatformError(err);
    }
    err = esp_event_handler_register(IP_EVENT, WIFI_EVENT_STA_CONNECTED, &WifiEventHandler, this);
    if (ESP_OK != err) {
        return toPlatformError(err);
    }
    err = esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &WifiEventHandler, this);
    if (ESP_OK != err) {
        return toPlatformError(err);
    }

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    if (ESP_OK != (err = esp_wifi_init(&cfg))) {
        return toPlatformError(err);
    }

    esp_wifi_set_mode(toEspWifiMode(mode()));
    switch (mode()) {
        case WifiConfig::Mode::AccessPoint: {
            error = initAccessPoint();
            break;
        }
        case WifiConfig::Mode::Station: {
            error = initStation();
            break;
        }
        case WifiConfig::Mode::AccessPointAndStation: {
            error = initStation();
            if (ErrorType::Success == error) {
                error = initAccessPoint();
            }
            break;
        }
        default: {
            error = ErrorType::InvalidParameter;
            break;
        }
    }

    if (ErrorType::Success != error) {
        return error;
    }

    error = radioOn();
    if (ErrorType::Success != error) {
        return error;
    }

    //Wait for the interface to be up
    {
    int i = 0;
    ErrorType error;
    const Milliseconds oneHundred = 100;
    bool interfaceIsNotUp = _ipAddress.empty();
    bool weHaveNotRetried100Times = i < 100;
    CBT_LOGI(TAG, "Waiting for interface to come up");
    while (interfaceIsNotUp && weHaveNotRetried100Times) {
        OperatingSystem::Instance().delay(oneHundred);
        i++;
        interfaceIsNotUp = !_ipAddress.empty();
        weHaveNotRetried100Times = i < 100;
    }

    if (interfaceIsNotUp) {
        _status.isUp = false;
        error =  ErrorType::Timeout;
    }
    else {
        _status.isUp = true;
        error = ErrorType::Success;
    }

    return error;
    }
}

ErrorType Wifi::initAccessPoint() {
    esp_netif_create_default_wifi_ap();

    return ErrorType::Success;
}
ErrorType Wifi::initStation() {
    esp_netif_create_default_wifi_sta();

    return ErrorType::Success;
}

ErrorType Wifi::networkUp() {
    esp_err_t err;

    err = esp_netif_init();
    if (ESP_OK != err) {
        return toPlatformError(err);
    }

    return ErrorType::Success;
}

ErrorType Wifi::networkDown() {
    ErrorType error = toPlatformError(esp_wifi_stop());
    _ipAddress.clear();
    _status.isUp = false;
    return error;
}

ErrorType Wifi::radioOn() {
    return toPlatformError(esp_wifi_start());
}

ErrorType Wifi::radioOff() {
    return toPlatformError(esp_wifi_deinit());
}

ErrorType Wifi::txBlocking(const std::string &frame, const Milliseconds timeout) {
    return ErrorType::NotImplemented;
}
ErrorType Wifi::txNonBlocking(const std::shared_ptr<std::string> frame, std::function<void(const ErrorType error, const Bytes bytesWritten)> callbackr) {
    return ErrorType::NotImplemented;
}
ErrorType Wifi::rxBlocking(std::string &frameBuffer, const Milliseconds timeout) {
    return ErrorType::NotImplemented;
}
ErrorType Wifi::rxNonBlocking(std::shared_ptr<std::string> frameBuffer, std::function<void(const ErrorType error, std::shared_ptr<std::string> frameBuffer)> callback) {
    return ErrorType::NotImplemented;
}

ErrorType Wifi::getMacAddress(std::string &macAddress) {
    uint8_t macAddressByteArray[6];
    constexpr Count macAddressStringSize = 17;
    esp_err_t err;
    assert(macAddress.size() >= macAddressStringSize);

    err = esp_read_mac(macAddressByteArray, ESP_MAC_WIFI_STA);
    if (ESP_OK != err) {
        return toPlatformError(err);
    }

    assert(snprintf(macAddress.data(), macAddress.size(), MACSTR, MAC2STR(macAddressByteArray)) > 0);
    macAddress.resize(macAddressStringSize);

    return ErrorType::Success;
}

ErrorType Wifi::getSignalStrength(DecibelMilliWatts &signalStrength) {
    int signalStrengthRssi;
    esp_wifi_sta_get_rssi(&signalStrengthRssi);
    signalStrength = DecibelMilliWatts(signalStrengthRssi);

    _status.signalStrength = signalStrength;

    return ErrorType::Success;
}

ErrorType Wifi::mainLoop() {
    ErrorType error;

    if (ErrorType::NoData == (error = runNextEvent())) {
        return error;
    }
    else if (ErrorType::Success == error) {
        return error;
    }

    return ErrorType::Failure;
}

ErrorType Wifi::setSsid(WifiConfig::Mode mode, std::string ssid) {
    wifi_config_t wifiConfig;
    esp_err_t err;

    if (ssid.length() > sizeof(wifiConfig.sta.ssid) - 1) {
        return ErrorType::InvalidParameter;
    }

    if (WifiConfig::Mode::Station == mode) {
        setStationSsid(ssid);
    }
    else if (WifiConfig::Mode::AccessPoint == mode) {
        setAccessPointSsid(ssid);
    }
    else if (WifiConfig::Mode::AccessPointAndStation == mode) {
        return ErrorType::InvalidParameter;
    }

    return ErrorType::Success; 
}

ErrorType Wifi::setStationSsid(std::string ssid) {
    esp_err_t err;
    wifi_config_t wifiConfig;

    err = esp_wifi_get_config(WIFI_IF_STA, &wifiConfig);
    if (ESP_OK != err) {
        return toPlatformError(err);
    }

    memcpy(wifiConfig.sta.ssid, ssid.c_str(), ssid.length());
    wifiConfig.sta.bssid_set = false;
    wifiConfig.sta.threshold.authmode = toEspAuthMode(authMode());
    wifiConfig.sta.pmf_cfg.capable = false;
    wifiConfig.sta.pmf_cfg.required = false;

    esp_wifi_set_config(WIFI_IF_STA, &wifiConfig);
    if (ESP_OK != err) {
        return toPlatformError(err);
    }

    return toPlatformError(err);
}

ErrorType Wifi::setAccessPointSsid(std::string ssid) {
    esp_err_t err;
    wifi_config_t wifiConfig;

    err = esp_wifi_get_config(WIFI_IF_AP, &wifiConfig);
    if (ESP_OK != err) {
        return toPlatformError(err);
    }

    memcpy(wifiConfig.ap.ssid, ssid.c_str(), ssid.length());

    err = esp_wifi_set_config(WIFI_IF_AP, &wifiConfig);
    if (ESP_OK != err) {
        return toPlatformError(err);
    }

    return toPlatformError(err);
}

ErrorType Wifi::setPassword(WifiConfig::Mode mode, std::string password) {
    wifi_config_t wifiConfig;
    esp_err_t err;

    if (password.length() > sizeof(wifiConfig.sta.password) - 1) {
        return ErrorType::InvalidParameter;
    }

    if (WifiConfig::Mode::Station == mode) {
        err = esp_wifi_get_config(WIFI_IF_STA, &wifiConfig);
        if (ESP_OK != err) {
            return toPlatformError(err);
        }

        memcpy(wifiConfig.sta.password, password.c_str(), password.length());

        err = esp_wifi_set_config(WIFI_IF_STA, &wifiConfig);
        if (ESP_OK != err) {
            return toPlatformError(err);
        }
    }

    if (WifiConfig::Mode::AccessPoint == mode) {
        err = esp_wifi_get_config(WIFI_IF_AP, &wifiConfig);
        if (ESP_OK != err) {
            return toPlatformError(err);
        }

        memcpy(wifiConfig.ap.password, password.c_str(), password.length());

        err = esp_wifi_set_config(WIFI_IF_AP, &wifiConfig);
        if (ESP_OK != err) {
            return toPlatformError(err);
        }
    }
    else {
        return ErrorType::InvalidParameter;
    }

    return ErrorType::Success;
}

#ifdef __cplusplus
extern "C" {
#endif

static void WifiEventHandler(void *arg, esp_event_base_t eventBase, int32_t eventId, void *eventData) {
    static int s_retry_num = 0;
    constexpr unsigned int wifiConnectedBit = BIT0;
    constexpr unsigned int wifiFailBit = BIT1;

    Wifi *self = reinterpret_cast<Wifi *>(arg);

    if (eventBase == WIFI_EVENT && eventId == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *) eventData;
        CBT_LOGI(Wifi::TAG, "Station " "MACSTR" " left, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
    else if (eventBase == WIFI_EVENT && eventId == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *) eventData;
        CBT_LOGI(Wifi::TAG, "Station " "MACSTR" " left, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
    else if (eventBase == WIFI_EVENT && eventId == WIFI_EVENT_STA_CONNECTED) {
        CBT_LOGI(Wifi::TAG, "Station connected");
    }
    else if (eventBase == WIFI_EVENT && eventId == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
        CBT_LOGI(Wifi::TAG, "Station started");
    }
    else if (eventBase == IP_EVENT && eventId == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *) eventData;
        CBT_LOGI(Wifi::TAG, "Got IP:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
    }

    s_retry_num++;
}

#ifdef __cplusplus
}
#endif
