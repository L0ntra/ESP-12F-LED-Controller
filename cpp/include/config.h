#pragma once
#include <string>
using std::string;

struct StripConfig {
    int color[3] = {255, 0, 0};
    int n_leds = 0;
};

struct Config {
    string ssid;
    string password;
    string device_name;
    StripConfig strips[4];

    bool load();
    bool save();
};
