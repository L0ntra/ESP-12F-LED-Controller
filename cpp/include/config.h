#pragma once
#include <string>
using std::string;

struct Config {
    string ssid;
    string password;
    int color[3] = {255, 0, 0};
    int n_leds = 0;

    bool load();
    bool save();
};
