#include "doctest.h"
#include "LittleFS.h"
#include "../include/config.h"

TEST_CASE("Config.default_values") {
    Config c;
    CHECK_EQ(c.ssid, "");
    CHECK_EQ(c.password, "");
    CHECK_EQ(c.color[0], 255);
    CHECK_EQ(c.color[1], 0);
    CHECK_EQ(c.color[2], 0);
    CHECK_EQ(c.n_leds, 0);
}

TEST_CASE("Config.load_populates_fields") {
    LittleFS.clear();
    LittleFS.fileContent("/config.json") =
        "{\"ssid\":\"MyWiFi\",\"password\":\"secret123\","
        "\"color\":[100,150,200],\"n_leds\":60}";

    Config c;
    bool ok = c.load();

    CHECK(ok);
    CHECK_EQ(c.ssid, "MyWiFi");
    CHECK_EQ(c.password, "secret123");
    CHECK_EQ(c.color[0], 100);
    CHECK_EQ(c.color[1], 150);
    CHECK_EQ(c.color[2], 200);
    CHECK_EQ(c.n_leds, 60);
}

TEST_CASE("Config.load_missing_file_returns_false") {
    LittleFS.clear();
    Config c;
    bool ok = c.load();
    CHECK_FALSE(ok);
}

TEST_CASE("Config.save_roundtrip") {
    LittleFS.clear();
    Config c;
    c.ssid = "HomeNet";
    c.password = "pass123";
    c.color[0] = 10; c.color[1] = 20; c.color[2] = 30;
    c.n_leds = 144;

    c.save();

    Config c2;
    c2.load();
    CHECK_EQ(c2.ssid, "HomeNet");
    CHECK_EQ(c2.password, "pass123");
    CHECK_EQ(c2.color[0], 10);
    CHECK_EQ(c2.color[1], 20);
    CHECK_EQ(c2.color[2], 30);
    CHECK_EQ(c2.n_leds, 144);
}

TEST_CASE("Config.save_writes_valid_json") {
    LittleFS.clear();
    Config c;
    c.ssid = "Test";
    c.password = "pw";
    c.color[0] = 1; c.color[1] = 2; c.color[2] = 3;
    c.n_leds = 10;

    c.save();

    string json = LittleFS.fileContent("/config.json");
    CHECK(json.find("\"ssid\":\"Test\"") != string::npos);
    CHECK(json.find("\"password\":\"pw\"") != string::npos);
    CHECK(json.find("\"color\":[1,2,3]") != string::npos);
    CHECK(json.find("\"n_leds\":10") != string::npos);
}
