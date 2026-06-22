#include "doctest.h"
#include "../include/utils.h"

TEST_CASE("Utils.rgbToHex_red") {
    auto h = rgbToHex(255, 0, 0);
    CHECK_EQ(h, "#ff0000");
}

TEST_CASE("Utils.rgbToHex_green") {
    auto h = rgbToHex(0, 255, 0);
    CHECK_EQ(h, "#00ff00");
}

TEST_CASE("Utils.rgbToHex_blue") {
    auto h = rgbToHex(0, 0, 255);
    CHECK_EQ(h, "#0000ff");
}

TEST_CASE("Utils.rgbToHex_white") {
    auto h = rgbToHex(255, 255, 255);
    CHECK_EQ(h, "#ffffff");
}

TEST_CASE("Utils.rgbToHex_black") {
    auto h = rgbToHex(0, 0, 0);
    CHECK_EQ(h, "#000000");
}

TEST_CASE("Utils.rgbToHex_custom") {
    auto h = rgbToHex(171, 205, 239);
    CHECK_EQ(h, "#abcdef");
}

TEST_CASE("Utils.hexToRgb_red") {
    int r, g, b;
    hexToRgb("#ff0000", r, g, b);
    CHECK_EQ(r, 255);
    CHECK_EQ(g, 0);
    CHECK_EQ(b, 0);
}

TEST_CASE("Utils.hexToRgb_green") {
    int r, g, b;
    hexToRgb("#00ff00", r, g, b);
    CHECK_EQ(r, 0);
    CHECK_EQ(g, 255);
    CHECK_EQ(b, 0);
}

TEST_CASE("Utils.hexToRgb_blue") {
    int r, g, b;
    hexToRgb("#0000ff", r, g, b);
    CHECK_EQ(r, 0);
    CHECK_EQ(g, 0);
    CHECK_EQ(b, 255);
}

TEST_CASE("Utils.hexToRgb_no_hash") {
    int r, g, b;
    hexToRgb("ff0000", r, g, b);
    CHECK_EQ(r, 255);
    CHECK_EQ(g, 0);
    CHECK_EQ(b, 0);
}

TEST_CASE("Potentiometer.full_brightness") {
    float pct = readPotentiometer(0);
    CHECK(pct == doctest::Approx(1.0f).epsilon(0.01f));
}

TEST_CASE("Potentiometer.dead_zone") {
    float pct = readPotentiometer(5);
    CHECK(pct == doctest::Approx(1.0f).epsilon(0.01f));
}

TEST_CASE("Potentiometer.mid_point") {
    float pct = readPotentiometer(512);
    CHECK(pct == doctest::Approx(0.5f).epsilon(0.05f));
}

TEST_CASE("Potentiometer.zero_brightness") {
    float pct = readPotentiometer(1023);
    CHECK(pct == doctest::Approx(0.0f).epsilon(0.02f));
}
