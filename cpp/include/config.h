#pragma once
#include <string>

struct Config {
    std::string ssid;
    std::string password;
    int color[3] = {255, 0, 0};
    int n_leds = 0;

    bool load();
    bool save();
};
