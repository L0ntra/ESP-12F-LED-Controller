#include "led_controller.h"
#include <Adafruit_NeoPixel.h>

LEDController::LEDController() {}

LEDController::~LEDController() {
    delete static_cast<Adafruit_NeoPixel*>(strip_);
}

void LEDController::begin(uint8_t pin, uint16_t n_leds) {
    delete static_cast<Adafruit_NeoPixel*>(strip_);
    strip_ = nullptr;
    strip_ = new Adafruit_NeoPixel(n_leds, pin, NEO_GRB + NEO_KHZ800);
    static_cast<Adafruit_NeoPixel*>(strip_)->begin();
    static_cast<Adafruit_NeoPixel*>(strip_)->show();
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
    if (!strip_) return;
    auto* s = static_cast<Adafruit_NeoPixel*>(strip_);
    uint32_t c = s->Color(
        static_cast<uint8_t>(color_[0] * brightness_),
        static_cast<uint8_t>(color_[1] * brightness_),
        static_cast<uint8_t>(color_[2] * brightness_)
    );
    s->fill(c);
    s->show();
}
