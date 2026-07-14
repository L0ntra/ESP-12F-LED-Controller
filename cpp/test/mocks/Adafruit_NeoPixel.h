#pragma once
#include <cstdint>

#define NEO_GRB     0
#define NEO_RGB     0
#define NEO_KHZ800  0
#define NEO_KHZ400  0x0100

class Adafruit_NeoPixel {
public:
    Adafruit_NeoPixel(uint16_t n, uint8_t /*pin*/, uint16_t /*type*/) : numLEDs_(n) {}

    void begin() {}
    void show() {}
    uint16_t numPixels() const { return numLEDs_; }

    void fill(uint32_t /*c*/, uint16_t /*first*/ = 0, uint16_t /*count*/ = 0) {
    }

    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
        return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
    }

private:
    uint16_t numLEDs_;
};
