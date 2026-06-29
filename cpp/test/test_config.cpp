#include "doctest.h"
#include "LittleFS.h"
#include "../include/config.h"

TEST_CASE("Config.default_values") {
    Config c;
    CHECK_EQ(c.ssid, "");
    CHECK_EQ(c.password, "");
    CHECK_EQ(c.device_name, "");
    for (int i = 0; i < 4; i++) {
        CHECK_EQ(c.strips[i].color[0], 255);
        CHECK_EQ(c.strips[i].color[1], 0);
        CHECK_EQ(c.strips[i].color[2], 0);
        CHECK_EQ(c.strips[i].n_leds, 0);
    }
}

TEST_CASE("Config.load_populates_fields") {
    LittleFS.clear();
    LittleFS.fileContent("/config.json") =
        "{\"ssid\":\"MyWiFi\",\"password\":\"secret123\","
        "\"device_name\":\"my-controller\","
        "\"strips\":["
        "{\"color\":[100,150,200],\"n_leds\":60},"
        "{\"color\":[10,20,30],\"n_leds\":30},"
        "{\"color\":[200,100,50],\"n_leds\":144},"
        "{\"color\":[0,255,128],\"n_leds\":10}"
        "]}";

    Config c;
    bool ok = c.load();

    CHECK(ok);
    CHECK_EQ(c.ssid, "MyWiFi");
    CHECK_EQ(c.password, "secret123");
    CHECK_EQ(c.device_name, "my-controller");

    CHECK_EQ(c.strips[0].color[0], 100);
    CHECK_EQ(c.strips[0].color[1], 150);
    CHECK_EQ(c.strips[0].color[2], 200);
    CHECK_EQ(c.strips[0].n_leds, 60);

    CHECK_EQ(c.strips[1].color[0], 10);
    CHECK_EQ(c.strips[1].color[1], 20);
    CHECK_EQ(c.strips[1].color[2], 30);
    CHECK_EQ(c.strips[1].n_leds, 30);

    CHECK_EQ(c.strips[2].color[0], 200);
    CHECK_EQ(c.strips[2].color[1], 100);
    CHECK_EQ(c.strips[2].color[2], 50);
    CHECK_EQ(c.strips[2].n_leds, 144);

    CHECK_EQ(c.strips[3].color[0], 0);
    CHECK_EQ(c.strips[3].color[1], 255);
    CHECK_EQ(c.strips[3].color[2], 128);
    CHECK_EQ(c.strips[3].n_leds, 10);
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
    c.device_name = "living-room";

    c.strips[0].color[0] = 10; c.strips[0].color[1] = 20; c.strips[0].color[2] = 30;
    c.strips[0].n_leds = 144;

    c.strips[1].color[0] = 50; c.strips[1].color[1] = 60; c.strips[1].color[2] = 70;
    c.strips[1].n_leds = 60;

    c.strips[2].color[0] = 80; c.strips[2].color[1] = 90; c.strips[2].color[2] = 100;
    c.strips[2].n_leds = 30;

    c.strips[3].color[0] = 110; c.strips[3].color[1] = 120; c.strips[3].color[2] = 130;
    c.strips[3].n_leds = 10;

    c.save();

    Config c2;
    c2.load();
    CHECK_EQ(c2.ssid, "HomeNet");
    CHECK_EQ(c2.password, "pass123");
    CHECK_EQ(c2.device_name, "living-room");

    for (int i = 0; i < 4; i++) {
        CHECK_EQ(c2.strips[i].color[0], c.strips[i].color[0]);
        CHECK_EQ(c2.strips[i].color[1], c.strips[i].color[1]);
        CHECK_EQ(c2.strips[i].color[2], c.strips[i].color[2]);
        CHECK_EQ(c2.strips[i].n_leds, c.strips[i].n_leds);
    }
}

TEST_CASE("Config.save_writes_valid_json") {
    LittleFS.clear();
    Config c;
    c.ssid = "Test";
    c.password = "pw";
    c.device_name = "test-device";

    c.strips[0].color[0] = 1; c.strips[0].color[1] = 2; c.strips[0].color[2] = 3;
    c.strips[0].n_leds = 10;

    c.save();

    string json = LittleFS.fileContent("/config.json");
    CHECK(json.find("\"ssid\":\"Test\"") != string::npos);
    CHECK(json.find("\"password\":\"pw\"") != string::npos);
    CHECK(json.find("\"device_name\":\"test-device\"") != string::npos);
    CHECK(json.find("\"color\":[1,2,3]") != string::npos);
    CHECK(json.find("\"n_leds\":10") != string::npos);
}
