#include "led_controller.h"
#include <Adafruit_NeoPixel.h>

LEDController::LEDController() {}

LEDController::~LEDController() {
    delete strip_;
}

// begin allocates and initialises the NeoPixel strip. Any previously
// allocated strip is freed first to prevent leaks on re-initialisation.
void LEDController::begin(uint8_t pin, uint16_t n_leds) {
    delete strip_;
    strip_ = nullptr;
    strip_ = new Adafruit_NeoPixel(n_leds, pin, NEO_GRB + NEO_KHZ800);
    strip_->begin();
    strip_->show();
}

void LEDController::setColor(uint8_t r, uint8_t g, uint8_t b) {
    color_[0] = r;
    color_[1] = g;
    color_[2] = b;
}

// setBrightness clamps the value to [0.0, 1.0] and stores it.
void LEDController::setBrightness(float percent) {
    if (percent < 0.0f) percent = 0.0f;
    if (percent > 1.0f) percent = 1.0f;
    brightness_ = percent;
}

// show writes the brightness-adjusted color to every pixel on the strip.
// This is a no-op if the strip pointer is null.
void LEDController::show() {
    if (!strip_) return;
    uint32_t c = strip_->Color(
        static_cast<uint8_t>(color_[0] * brightness_),
        static_cast<uint8_t>(color_[1] * brightness_),
        static_cast<uint8_t>(color_[2] * brightness_)
    );
    strip_->fill(c);
    strip_->show();
}
