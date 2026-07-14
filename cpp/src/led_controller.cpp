#include "led_controller.h"
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

LEDController::LEDController() {}

LEDController::~LEDController() {
    delete strip_;
}

// begin allocates and initialises the NeoPixel strip. Any previously
// allocated strip is freed first to prevent leaks on re-initialisation.
// frequency is 400 for WS2811 or 800 for WS2812B.
void LEDController::begin(uint8_t pin, uint16_t n_leds, int frequency) {
    uint8_t r = color_[0], g = color_[1], b = color_[2];
    if (strip_) {
        uint16_t old_count = strip_->numPixels();
        if (n_leds < old_count) {
            strip_->fill(0, n_leds, 0);
        } else {
            strip_->fill(0);
        }
        strip_->show();
        // WS2811 needs >500us latch but the library only provides 280us.
        if (frequency == 400) delayMicroseconds(300);
    }
    delete strip_;
    strip_ = nullptr;
    uint16_t color_order = (frequency == 400) ? NEO_RGB : NEO_GRB;
    uint16_t type = color_order | (frequency == 400 ? NEO_KHZ400 : NEO_KHZ800);
    strip_ = new Adafruit_NeoPixel(n_leds, pin, type);
    strip_->begin();
    strip_->fill(strip_->Color(
        Adafruit_NeoPixel::gamma8(r),
        Adafruit_NeoPixel::gamma8(g),
        Adafruit_NeoPixel::gamma8(b)
    ));
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
    uint32_t c = strip_->Color(
        Adafruit_NeoPixel::gamma8(color_[0]),
        Adafruit_NeoPixel::gamma8(color_[1]),
        Adafruit_NeoPixel::gamma8(color_[2])
    );
    strip_->fill(c);
    strip_->show();
}
