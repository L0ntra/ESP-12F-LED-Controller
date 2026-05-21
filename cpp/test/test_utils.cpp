#include "framework.h"
#include "../include/utils.h"

TEST(Utils, rgbToHex_red) {
    auto h = rgbToHex(255, 0, 0);
    ASSERT_STREQ(h, "#ff0000");
}

TEST(Utils, rgbToHex_green) {
    auto h = rgbToHex(0, 255, 0);
    ASSERT_STREQ(h, "#00ff00");
}

TEST(Utils, rgbToHex_blue) {
    auto h = rgbToHex(0, 0, 255);
    ASSERT_STREQ(h, "#0000ff");
}

TEST(Utils, rgbToHex_white) {
    auto h = rgbToHex(255, 255, 255);
    ASSERT_STREQ(h, "#ffffff");
}

TEST(Utils, rgbToHex_black) {
    auto h = rgbToHex(0, 0, 0);
    ASSERT_STREQ(h, "#000000");
}

TEST(Utils, rgbToHex_custom) {
    auto h = rgbToHex(171, 205, 239);
    ASSERT_STREQ(h, "#abcdef");
}

TEST(Utils, hexToRgb_red) {
    int r, g, b;
    hexToRgb("#ff0000", r, g, b);
    ASSERT_EQ(r, 255);
    ASSERT_EQ(g, 0);
    ASSERT_EQ(b, 0);
}

TEST(Utils, hexToRgb_green) {
    int r, g, b;
    hexToRgb("#00ff00", r, g, b);
    ASSERT_EQ(r, 0);
    ASSERT_EQ(g, 255);
    ASSERT_EQ(b, 0);
}

TEST(Utils, hexToRgb_blue) {
    int r, g, b;
    hexToRgb("#0000ff", r, g, b);
    ASSERT_EQ(r, 0);
    ASSERT_EQ(g, 0);
    ASSERT_EQ(b, 255);
}

TEST(Utils, hexToRgb_no_hash) {
    int r, g, b;
    hexToRgb("ff0000", r, g, b);
    ASSERT_EQ(r, 255);
    ASSERT_EQ(g, 0);
    ASSERT_EQ(b, 0);
}

TEST(Utils, hexToRgb_roundtrip) {
    const int expected[][3] = {
        {255, 0, 0}, {0, 255, 0}, {0, 0, 255},
        {128, 128, 128}, {12, 34, 56}, {255, 255, 255}
    };
    for (auto& c : expected) {
        std::string hex = rgbToHex(c[0], c[1], c[2]);
        int r, g, b;
        hexToRgb(hex, r, g, b);
        ASSERT_EQ(r, c[0]);
        ASSERT_EQ(g, c[1]);
        ASSERT_EQ(b, c[2]);
    }
}

TEST(Potentiometer, full_brightness) {
    float pct = readPotentiometer(0);
    ASSERT_NEAR(pct, 1.0f, 0.01f);
}

TEST(Potentiometer, dead_zone) {
    float pct = readPotentiometer(5);
    ASSERT_NEAR(pct, 1.0f, 0.01f);
}

TEST(Potentiometer, mid_point) {
    float pct = readPotentiometer(512);
    ASSERT_NEAR(pct, 0.5f, 0.05f);
}

TEST(Potentiometer, zero_brightness) {
    float pct = readPotentiometer(1023);
    ASSERT_NEAR(pct, 0.0f, 0.02f);
}

TEST(Potentiometer, clamped_above_1) {
    float pct = readPotentiometer(0);
    ASSERT_TRUE(pct >= 0.0f);
    ASSERT_TRUE(pct <= 1.0f);
}

TEST(Potentiometer, clamped_below_0) {
    float pct = readPotentiometer(1023);
    ASSERT_TRUE(pct >= 0.0f);
    ASSERT_TRUE(pct <= 1.0f);
}
