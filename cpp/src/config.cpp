#include "config.h"
#include <cstdio>
#include <memory>
#include <LittleFS.h>
#include <ArduinoJson.h>

static const char* CONFIG_PATH = "/config.json";

bool Config::load() {
    if (!LittleFS.begin()) return false;
    File f = LittleFS.open(CONFIG_PATH, "r");
    if (!f) return false;

    size_t sz = f.size();
    auto buf = std::unique_ptr<char[]>(new char[sz + 1]);
    f.readBytes(buf.get(), sz);
    buf[sz] = '\0';
    f.close();

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, buf.get());
    if (err) return false;

    ssid = doc["ssid"] | "";
    password = doc["password"] | "";
    color[0] = doc["color"][0] | 255;
    color[1] = doc["color"][1] | 0;
    color[2] = doc["color"][2] | 0;
    n_leds = doc["n_leds"] | 0;
    return true;
}

bool Config::save() {
    JsonDocument doc;
    doc["ssid"] = ssid;
    doc["password"] = password;
    auto col = doc["color"].to<JsonArray>();
    col.add(color[0]);
    col.add(color[1]);
    col.add(color[2]);
    doc["n_leds"] = n_leds;

    File f = LittleFS.open(CONFIG_PATH, "w");
    if (!f) return false;
    serializeJson(doc, f);
    f.close();
    return true;
}
