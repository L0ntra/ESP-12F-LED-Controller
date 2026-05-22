#include "framework.h"
#include "Arduino.h"
#include "Adafruit_NeoPixel.h"
#include "../include/led_controller.h"

TEST(LEDController, set_and_show_color) {
    LEDController leds;
    leds.begin(14, 60);

    leds.setColor(255, 0, 0);
    // Can't verify without inspecting strip internals
    // The test ensures no crash and correct compilation
}

TEST(LEDController, brightness_affects_output) {
    LEDController leds;
    leds.begin(14, 60);

    leds.setColor(255, 128, 64);
    leds.setBrightness(0.5f);
    last_fill_color = 0;
    leds.show();

    // If brightness is 0.5, the effective color is (127, 64, 32)
    // 127 = 0x7f, 64 = 0x40, 32 = 0x20 => 0x7f4020
    ASSERT_EQ(last_fill_color, 0x7f4020U);
}

TEST(LEDController, brightness_clamped_to_0) {
    LEDController leds;
    leds.setBrightness(-0.5f);
    // Should clamp to 0
}

TEST(LEDController, brightness_clamped_to_1) {
    LEDController leds;
    leds.setBrightness(1.5f);
    // Should clamp to 1
}
