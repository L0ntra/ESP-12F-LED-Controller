#include "utils.h"
#include <cstdio>
#include <cstdlib>
#include <string>
using std::snprintf;
using std::stoi;

string rgbToHex(int r, int g, int b) {
    char buf[8];
    snprintf(buf, sizeof(buf), "#%02x%02x%02x", r, g, b);
    return buf;
}

void hexToRgb(const string& hex, int& r, int& g, int& b) {
    string h = hex;
    if (!h.empty() && h[0] == '#') h = h.substr(1);
    if (h.size() >= 6) {
        r = stoi(h.substr(0, 2), nullptr, 16);
        g = stoi(h.substr(2, 2), nullptr, 16);
        b = stoi(h.substr(4, 2), nullptr, 16);
    }
}

float readPotentiometer(int raw_adc) {
    int val = (raw_adc * 65535L) / 1024;
    float pct = 1.0f - ((val - 319) / 65535.0f);
    if (pct < 0.0f) pct = 0.0f;
    if (pct > 1.0f) pct = 1.0f;
    return pct;
}
