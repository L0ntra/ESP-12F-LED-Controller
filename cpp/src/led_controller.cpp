#include "led_controller.h"
#include <Adafruit_NeoPixel.h>

LEDController::LEDController() {}

LEDController::~LEDController() {
    delete strip_;
}

// begin allocates and initialises the NeoPixel strip. Any previously
// allocated strip is freed first to prevent leaks on re-initialisation.
// frequency is 400 for WS2811 or 800 for WS2812B.
void LEDController::begin(uint8_t pin, uint16_t n_leds, int frequency) {
    if (strip_) {
        strip_->fill(0);
        strip_->show();
    }
    delete strip_;
    strip_ = nullptr;
    uint16_t color_order = (frequency == 400) ? NEO_RGB : NEO_GRB;
    uint16_t type = color_order | (frequency == 400 ? NEO_KHZ400 : NEO_KHZ800);
    strip_ = new Adafruit_NeoPixel(n_leds, pin, type);
    strip_->begin();
    strip_->show();
}

void LEDController::setColor(uint8_t r, uint8_t g, uint8_t b) {
    color_[0] = r;
    color_[1] = g;
    color_[2] = b;
}

// show writes the color to every pixel on the strip.
// This is a no-op if the strip pointer is null.
void LEDController::show() {
    if (!strip_) return;
    uint32_t c = strip_->Color(color_[0], color_[1], color_[2]);
    strip_->fill(c);
    strip_->show();
}
