#pragma once
#include <cstdint>
#include <cstring>
#include <vector>

#define NEO_GRB     0
#define NEO_KHZ800  0

struct MockPixelState {
    uint32_t color = 0;
};

extern uint32_t last_fill_color;

class Adafruit_NeoPixel {
public:
    Adafruit_NeoPixel(uint16_t n, uint8_t pin, uint16_t type) {}

    void begin() {}
    void show() {}

    void fill(uint32_t c, uint16_t first = 0, uint16_t count = 0) {
        fill_color_ = c;
        last_fill_color = c;
    }

    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
        return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
    }

    uint32_t getFillColor() const { return fill_color_; }

private:
    uint32_t fill_color_ = 0;
};
