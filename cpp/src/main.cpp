#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <cstdio>
using std::snprintf;

#include "config.h"
#include "led_controller.h"
#include "webserver.h"
#include "utils.h"

// Pin assignments: D1=GPIO5, D5=GPIO14, D6=GPIO12, D7=GPIO13
constexpr uint8_t PIN_LED_BUILTIN = 2;
constexpr uint8_t PIN_STRIP_1 = 5;
constexpr uint8_t PIN_STRIP_2 = 14;
constexpr uint8_t PIN_STRIP_3 = 12;
constexpr uint8_t PIN_STRIP_4 = 13;
static const uint8_t STRIP_PINS[4] = {
    PIN_STRIP_1, PIN_STRIP_2, PIN_STRIP_3, PIN_STRIP_4
};

static Config config;
static LEDController strips[4];
static WebServer server;

// boardPinName returns the WeMos D1 Mini board label for a GPIO number.
static const char* boardPinName(uint8_t gpio) {
    switch (gpio) {
        case 5:  return "D1";
        case 14: return "D5";
        case 12: return "D6";
        case 13: return "D7";
        default: return "?";
    }
}

// updateLEDs writes each strip's staged color to its LEDs.
static void updateLEDs() {
    for (auto& s : strips) {
        s.show();
    }
}

// handleHome serves the configuration HTML page with device name, 4 colour
// pickers, 4 LED-count inputs, and GPIO pin labels, all pre-filled from the
// current config.
static void handleHome() {
    string html =
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<head>"
        "<title>ESP8266 LED Controller</title>"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
        "</head>\n"
        "<body>\n"
        "<h2>LED Controller</h2>\n"
        "<form id=\"configForm\">\n"
        "<label>Device Name:<br><input type=\"text\" id=\"deviceName\" value=\""
        + config.device_name + "\"></label><br><br>\n";

    for (int i = 0; i < 4; i++) {
        string hex = rgbToHex(
            config.strips[i].color[0],
            config.strips[i].color[1],
            config.strips[i].color[2]);
        char leds_buf[8];
        snprintf(leds_buf, sizeof(leds_buf), "%d", config.strips[i].n_leds);
        char idx0_buf[2];
        snprintf(idx0_buf, sizeof(idx0_buf), "%d", i);
        const char* pin_name = boardPinName(STRIP_PINS[i]);
        int f = config.strips[i].frequency;
        html += "<fieldset>\n"
                "<legend>" + string(pin_name) + "</legend>\n"
                "<label>Color: <input type=\"color\" id=\"color" + string(idx0_buf) + "\" value=\"" + hex + "\"></label><br>\n"
                "<label>LEDs: <input type=\"number\" id=\"leds" + string(idx0_buf) + "\" value=\"" + leds_buf + "\" min=\"0\"></label><br>\n"
                "<label>Type: <select id=\"freq" + string(idx0_buf) + "\">\n"
                "<option value=\"800\"" + (f == 800 ? " selected" : "") + ">WS2812B</option>\n"
                "<option value=\"400\"" + (f == 400 ? " selected" : "") + ">WS2811</option>\n"
                "</select></label>\n"
                "</fieldset>\n";
    }

    html +=
        "<button type=\"button\" onclick=\"saveConfig()\">Save</button>\n"
        "</form>\n"
        "<script>\n"
        "async function saveConfig() {\n"
        "    const strips = [];\n"
        "    for (let i = 0; i < 4; i++) {\n"
        "        strips.push({\n"
        "            color: document.getElementById('color' + i).value,\n"
        "            n_leds: parseInt(document.getElementById('leds' + i).value) || 0,\n"
        "            frequency: parseInt(document.getElementById('freq' + i).value) || 800\n"
        "        });\n"
        "    }\n"
        "    const body = JSON.stringify({\n"
        "        device_name: document.getElementById('deviceName').value,\n"
        "        strips: strips\n"
        "    });\n"
        "    await fetch('/configure', {\n"
        "        method: 'POST',\n"
        "        headers: { 'Content-Type': 'application/json' },\n"
        "        body: body\n"
        "    });\n"
        "}\n"
        "</script>\n"
        "</body>\n"
        "</html>\n";
    server.send(200, "text/html", html);
}

// handleConfigure parses the JSON body from the config form and updates the
// current state, the persisted config, and optionally the DHCP hostname.
static void handleConfigure() {
    string body = server.body();
    if (body.empty()) {
        server.send(411, "text/plain", "Length Required");
        return;
    }

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, body);
    if (err) {
        server.send(400, "text/plain", "Bad Request");
        return;
    }

    string new_name = doc["device_name"] | config.device_name;
    if (!new_name.empty()) {
        config.device_name = new_name;
        WiFi.hostname(config.device_name.c_str());
    }

    JsonArray strips_arr = doc["strips"].as<JsonArray>();
    if (strips_arr.size() == 4) {
        for (size_t i = 0; i < 4; i++) {
            JsonObject s = strips_arr[i];
            string hex = s["color"] | "#000000";
            hexToRgb(hex,
                     config.strips[i].color[0],
                     config.strips[i].color[1],
                     config.strips[i].color[2]);
            config.strips[i].n_leds = s["n_leds"] | 0;
            config.strips[i].frequency = s["frequency"] | 800;
        }
    }

    config.save();

    // Re-initialise strips so LED-count changes take effect.
    for (int i = 0; i < 4; i++) {
        Serial.printf("[save] Strip %d: pin=%d n_leds=%d freq=%d color=[%d,%d,%d]\n",
            i, STRIP_PINS[i], config.strips[i].n_leds, config.strips[i].frequency,
            config.strips[i].color[0], config.strips[i].color[1], config.strips[i].color[2]);
        strips[i].begin(STRIP_PINS[i], config.strips[i].n_leds, config.strips[i].frequency);
        strips[i].setColor(
            config.strips[i].color[0],
            config.strips[i].color[1],
            config.strips[i].color[2]);
    }

    server.send(200, "text/plain", "OK");
}

// setup initialises the filesystem, loads persisted config, creates the LED
// strips, initiates an async WiFi connection, and registers HTTP routes.
void setup() {
    Serial.begin(115200);
    Serial.println("\n[boot] ESP-12F LED Controller starting...");

    if (!LittleFS.begin()) {
        Serial.println("[boot] LittleFS mount failed — using defaults");
    }

    pinMode(PIN_LED_BUILTIN, OUTPUT);
    digitalWrite(PIN_LED_BUILTIN, HIGH);

    if (!config.load()) {
        Serial.println("[boot] Config load failed — using defaults");
    }

    Serial.printf("[boot] Device: %s\n", config.device_name.c_str());
    for (int i = 0; i < 4; i++) {
        Serial.printf("[boot] Strip %d: pin=%d n_leds=%d freq=%d color=[%d,%d,%d]\n",
            i, STRIP_PINS[i], config.strips[i].n_leds, config.strips[i].frequency,
            config.strips[i].color[0], config.strips[i].color[1], config.strips[i].color[2]);
        strips[i].begin(STRIP_PINS[i], config.strips[i].n_leds, config.strips[i].frequency);
        strips[i].setColor(
            config.strips[i].color[0],
            config.strips[i].color[1],
            config.strips[i].color[2]);
    }

    WiFi.mode(WIFI_STA);
    WiFi.hostname(config.device_name.c_str());
    WiFi.begin(config.ssid.c_str(), config.password.c_str());

    server.on("GET", "/", handleHome);
    server.on("POST", "/configure", handleConfigure);
    server.begin();

    Serial.printf("[boot] WiFi connecting to %s...\n", config.ssid.c_str());
}

unsigned long last_led_update = 0;
static bool wifi_printed = false;

// loop processes HTTP requests and drives the LED strips at 100 Hz.
void loop() {
    server.handleClient();
    if (!wifi_printed && WiFi.status() == WL_CONNECTED) {
        Serial.printf("[wifi] Connected — IP: %s\n", WiFi.localIP().toString().c_str());
        wifi_printed = true;
    }
    unsigned long now = millis();
    if (now - last_led_update >= 100) {
        last_led_update = now;
        updateLEDs();
    }
}
