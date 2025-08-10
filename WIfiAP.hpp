#pragma once
#include <string>
#include "esp_wifi.h"
#include "nvs_flash.h"

/**
 * c++ wrapper
 * 
 */

enum WIFI_STATUS {
    WIFI_STATUS_STOPPED = 0,
    WIFI_STATUS_RUNNING = 1,
    WIFI_STATUS_CONNECTED = 2
}


class WIfiAP
{
private:
    std::string ssid;
    std::string password;
    uint8_t max_stations;
    WIFI_STATUS status = WIFI_STATUS_STOPPED;
public:
    WIfiAP(std::string ssid, std::string password="");

    void start();

    void stop();

    WIFI_STATUS getStatus();

    std::string& getSSID();
    std::string& getPassword();
};