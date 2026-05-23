#include "utils.h"
#include <cstdio>
#include <string>
using std::snprintf;
using std::stoi;

// rgbToHex converts three 0-255 RGB values into a hex colour string
// prefixed with '#', for example rgbToHex(255, 0, 0) returns "#ff0000".
string rgbToHex(int r, int g, int b) {
    char buf[8];
    snprintf(buf, sizeof(buf), "#%02x%02x%02x", r, g, b);
    return buf;
}

// hexToRgb parses a hex colour string (with or without leading '#') and
// writes the resulting 0-255 values into r, g, b. Values are left
// unchanged if fewer than 6 hex digits are available.
void hexToRgb(const string& hex, int& r, int& g, int& b) {
    string h = hex;
    if (!h.empty() && h[0] == '#') h = h.substr(1);
    if (h.size() >= 6) {
        r = stoi(h.substr(0, 2), nullptr, 16);
        g = stoi(h.substr(2, 2), nullptr, 16);
        b = stoi(h.substr(4, 2), nullptr, 16);
    }
}

// readPotentiometer converts a raw ADC reading (0-1023 on ESP8266) to a
// brightness factor in [0.0, 1.0]. The dead zone near the maximum ADC
// value maps to 0.0 so the LEDs can be fully turned off.
float readPotentiometer(int raw_adc) {
    int val = (raw_adc * 65535L) / 1024;
    float pct = 1.0f - ((val - 319) / 65535.0f);
    if (pct < 0.0f) pct = 0.0f;
    if (pct > 1.0f) pct = 1.0f;
    return pct;
}
