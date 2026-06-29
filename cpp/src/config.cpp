#include "config.h"
#include <memory>
#include <LittleFS.h>
#include <ArduinoJson.h>
using std::unique_ptr;

static const char* CONFIG_PATH = "/config.json";

// load reads the config file from LittleFS and populates the fields.
// Returns false if the filesystem, file, or JSON is unreadable.
bool Config::load() {
    if (!LittleFS.begin()) return false;
    File f = LittleFS.open(CONFIG_PATH, "r");
    if (!f) return false;

    size_t sz = f.size();
    auto buf = unique_ptr<char[]>(new char[sz + 1]);
    f.readBytes(buf.get(), sz);
    buf[sz] = '\0';
    f.close();

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, buf.get());
    if (err) return false;

    ssid = doc["ssid"] | "";
    password = doc["password"] | "";
    device_name = doc["device_name"] | "esp-led-controller";

    JsonArray strips_arr = doc["strips"].as<JsonArray>();
    for (size_t i = 0; i < 4 && i < strips_arr.size(); i++) {
        JsonObject s = strips_arr[i];
        strips[i].color[0] = s["color"][0] | 255;
        strips[i].color[1] = s["color"][1] | 0;
        strips[i].color[2] = s["color"][2] | 0;
        strips[i].n_leds = s["n_leds"] | 0;
    }
    return true;
}

// save writes the current config fields to the config file on LittleFS.
// Returns false if the filesystem or file cannot be opened.
bool Config::save() {
    if (!LittleFS.begin()) return false;
    JsonDocument doc;
    doc["ssid"] = ssid;
    doc["password"] = password;
    doc["device_name"] = device_name;

    JsonArray strips_arr = doc["strips"].to<JsonArray>();
    for (int i = 0; i < 4; i++) {
        JsonObject s = strips_arr.add<JsonObject>();
        auto col = s["color"].to<JsonArray>();
        col.add(strips[i].color[0]);
        col.add(strips[i].color[1]);
        col.add(strips[i].color[2]);
        s["n_leds"] = strips[i].n_leds;
    }

    File f = LittleFS.open(CONFIG_PATH, "w");
    if (!f) return false;
    serializeJson(doc, f);
    f.close();
    return true;
}
