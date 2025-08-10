#include "WIfiAP.hpp"

WIfiAP::WIfiAP(std::string ssid, std::string password, uint8_t max_stations) : ssid(ssid), password(password), max_stations(max_stations)
{
    // Ensure the SSID and password are not empty
    if (this->ssid.empty())
    {
        this->ssid = "ESP32-AP";
    }
}

WIfiAP::start()
{
    // flash initialization
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize the event loop needed by Wi-Fi
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Initialize underlying TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_init());

    // Create default WIFI AP
    esp_netif_create_default_wifi_ap();

    // Initialize Wi-Fi with default config
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Configure Wi-Fi as Access Point
    wifi_config_t wifi_config = {};
    std::strcpy(reinterpret_cast<char *>(wifi_config.ap.ssid), this->ssid.c_str());
    std::strcpy(reinterpret_cast<char *>(wifi_config.ap.password), this->password.c_str());
    wifi_config.ap.ssid_len = 0;
    wifi_config.ap.max_connection = this->max_connections;
    wifi_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;

    if (this->password.length() == 0)
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;

    // Set Wi-Fi mode to AP and apply configuration
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    this->status = WIFI_STATUS_RUNNING;
}

WIfiAP::stop()
{
    ESP_ERROR_CHECK(esp_wifi_stop());
    this->status = WIFI_STATUS_STOPPED;
}

WIfiAP::getStatus()
{
    return this->status;
}

WIfiAP::getSSID() {
    return this->ssid;
}
WIfiAP::getPassword() {
    return this->password;
}