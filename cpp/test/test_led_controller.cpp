#include "doctest.h"
#include "Arduino.h"
#include "Adafruit_NeoPixel.h"
#include "../include/led_controller.h"

TEST_CASE("LEDController.set_and_show_color") {
    LEDController leds;
    leds.begin(14, 60);

    leds.setColor(255, 0, 0);
}
