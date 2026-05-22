#include "framework.h"
#include "LittleFS.h"
#include "../src/config.cpp"

TEST(Config, default_values) {
    Config c;
    ASSERT_STREQ(c.ssid, "");
    ASSERT_STREQ(c.password, "");
    ASSERT_EQ(c.color[0], 255);
    ASSERT_EQ(c.color[1], 0);
    ASSERT_EQ(c.color[2], 0);
    ASSERT_EQ(c.n_leds, 0);
}

TEST(Config, load_populates_fields) {
    LittleFS.clear();
    LittleFS.fileContent("/config.json") =
        "{\"ssid\":\"MyWiFi\",\"password\":\"secret123\","
        "\"color\":[100,150,200],\"n_leds\":60}";

    Config c;
    bool ok = c.load();

    ASSERT_TRUE(ok);
    ASSERT_STREQ(c.ssid, "MyWiFi");
    ASSERT_STREQ(c.password, "secret123");
    ASSERT_EQ(c.color[0], 100);
    ASSERT_EQ(c.color[1], 150);
    ASSERT_EQ(c.color[2], 200);
    ASSERT_EQ(c.n_leds, 60);
}

TEST(Config, load_missing_file_returns_false) {
    LittleFS.clear();
    Config c;
    bool ok = c.load();
    ASSERT_FALSE(ok);
}

TEST(Config, save_roundtrip) {
    LittleFS.clear();
    Config c;
    c.ssid = "HomeNet";
    c.password = "pass123";
    c.color[0] = 10; c.color[1] = 20; c.color[2] = 30;
    c.n_leds = 144;

    c.save();

    Config c2;
    c2.load();
    ASSERT_STREQ(c2.ssid, "HomeNet");
    ASSERT_STREQ(c2.password, "pass123");
    ASSERT_EQ(c2.color[0], 10);
    ASSERT_EQ(c2.color[1], 20);
    ASSERT_EQ(c2.color[2], 30);
    ASSERT_EQ(c2.n_leds, 144);
}

TEST(Config, save_writes_valid_json) {
    LittleFS.clear();
    Config c;
    c.ssid = "Test";
    c.password = "pw";
    c.color[0] = 1; c.color[1] = 2; c.color[2] = 3;
    c.n_leds = 10;

    c.save();

    string json = LittleFS.fileContent("/config.json");
    ASSERT_TRUE(json.find("\"ssid\":\"Test\"") != string::npos);
    ASSERT_TRUE(json.find("\"password\":\"pw\"") != string::npos);
    ASSERT_TRUE(json.find("\"color\":[1,2,3]") != string::npos);
    ASSERT_TRUE(json.find("\"n_leds\":10") != string::npos);
}
