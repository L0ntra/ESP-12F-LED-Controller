#pragma once
#include <cstdint>
#include <string>
using std::string;

enum wl_status_t : uint8_t {
    WL_NO_SHIELD = 0,
    WL_IDLE_STATUS = 1,
    WL_NO_SSID_AVAIL = 2,
    WL_SCAN_COMPLETED = 3,
    WL_CONNECTED = 4,
    WL_CONNECT_FAILED = 5,
    WL_CONNECTION_LOST = 6,
    WL_DISCONNECTED = 7
};

enum wl_mode_t : uint8_t {
    WIFI_OFF = 0,
    WIFI_STA = 1,
    WIFI_AP = 2,
    WIFI_AP_STA = 3
};

class IPAddress {
    uint8_t addr_[4] = {0,0,0,0};
public:
    IPAddress() = default;
    IPAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d) : addr_{a,b,c,d} {}
    string toString() const {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d.%d.%d.%d", addr_[0], addr_[1], addr_[2], addr_[3]);
        return buf;
    }
};

class WiFiClass {
public:
    wl_status_t status_val = WL_DISCONNECTED;
    string ssid;
    string password;
    wl_mode_t mode_val = WIFI_OFF;

    void mode(wl_mode_t m) { mode_val = m; }
    void begin(const char* s, const char* p) {
        ssid = s ? s : "";
        password = p ? p : "";
    }
    wl_status_t status() { return status_val; }
    IPAddress localIP() { return IPAddress(192,168,1,100); }
};

extern WiFiClass WiFi;
