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
