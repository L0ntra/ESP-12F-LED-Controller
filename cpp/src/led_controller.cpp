#include "led_controller.h"
#include <Adafruit_NeoPixel.h>

static Adafruit_NeoPixel* strip = nullptr;

void LEDController::begin(uint8_t pin, uint16_t n_leds) {
    n_leds_ = n_leds;
    strip = new Adafruit_NeoPixel(n_leds, pin, NEO_GRB + NEO_KHZ800);
    strip->begin();
    strip->show();
}

void LEDController::setColor(uint8_t r, uint8_t g, uint8_t b) {
    color_[0] = r;
    color_[1] = g;
    color_[2] = b;
}

void LEDController::setBrightness(float percent) {
    if (percent < 0.0f) percent = 0.0f;
    if (percent > 1.0f) percent = 1.0f;
    brightness_ = percent;
}

void LEDController::show() {
    if (!strip) return;
    uint32_t c = strip->Color(
        static_cast<uint8_t>(color_[0] * brightness_),
        static_cast<uint8_t>(color_[1] * brightness_),
        static_cast<uint8_t>(color_[2] * brightness_)
    );
    strip->fill(c);
    strip->show();
}
