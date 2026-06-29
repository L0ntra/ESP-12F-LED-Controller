#pragma once
#include <cstdint>

class Adafruit_NeoPixel;

class LEDController {
public:
    LEDController();
    ~LEDController();

    void begin(uint8_t pin, uint16_t n_leds, uint16_t type = 0);
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void show();

private:
    uint8_t color_[3] = {0, 0, 0};
    Adafruit_NeoPixel* strip_ = nullptr;
};
