#include "WifiAP.hpp"

WifiAP::WifiAP(std::string ssid, std::string password, uint8_t max_stations)
{
    this->ssid = ssid.empty() ? "ESP32-AP" : ssid;
    this->password = password;
    this->max_stations = max_stations;
    // flash initialization is on caller side
}

void WifiAP::start()
{
    // Initialize underlying TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_init());

    // Create default WIFI AP
    esp_netif_create_default_wifi_ap();

    // Initialize Wi-Fi with default config
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Configure Wi-Fi as Access Point
    wifi_config_t wifi_config = {};
    strcpy(reinterpret_cast<char *>(wifi_config.ap.ssid), this->ssid.c_str());
    strcpy(reinterpret_cast<char *>(wifi_config.ap.password), this->password.c_str());
    wifi_config.ap.ssid_len = 0;
    wifi_config.ap.max_connection = this->max_stations;
    wifi_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;

    if (this->password.length() == 0)
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;

    // Set Wi-Fi mode to AP and apply configuration
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    this->status = WIFI_STATUS_RUNNING;

    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT,
        WIFI_EVENT_AP_STACONNECTED,
        &WifiAP::onConnect,
        this,
        nullptr));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT,
        WIFI_EVENT_AP_STADISCONNECTED,
        &WifiAP::onDisconnect,
        this,
        nullptr));
}

void WifiAP::stop()
{
    ESP_ERROR_CHECK(esp_wifi_stop());
    this->status = WIFI_STATUS_STOPPED;
}

WIFI_STATUS WifiAP::getStatus()
{
    return this->status;
}

std::string &WifiAP::getSSID()
{
    return this->ssid;
}
std::string &WifiAP::getPassword()
{
    return this->password;
}

uint8_t WifiAP::getMaxStations()
{
    return this->max_stations;
}

void WifiAP::onConnect(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    WifiAP *self = static_cast<WifiAP *>(arg);
    self->status = WIFI_STATUS_CONNECTED;
}

void WifiAP::onDisconnect(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    WifiAP *self = static_cast<WifiAP *>(arg);
    self->status = WIFI_STATUS_RUNNING;
}