#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

#include "config.h"
#include "led_controller.h"
#include "webserver.h"
#include "utils.h"

constexpr uint8_t PIN_LED_BUILTIN = 2;
constexpr uint8_t PIN_NEOPIXEL = 14;
constexpr uint8_t PIN_SWITCH = 4;

static Config config;
static LEDController leds;
static WebServer server;
static int current_color[3] = {255, 0, 0};

// updateLEDs reads the current color, applies the switch override and
// potentiometer brightness, then writes the result to the LED strip.
static void updateLEDs() {
    int r = current_color[0];
    int g = current_color[1];
    int b = current_color[2];

    if (digitalRead(PIN_SWITCH) == HIGH) {
        r = 255; g = 255; b = 255;
    }

    float pct = readPotentiometer(analogRead(A0));
    r = static_cast<int>(r * pct);
    g = static_cast<int>(g * pct);
    b = static_cast<int>(b * pct);

    leds.setColor(r, g, b);
    leds.show();
}

// handleHome serves the color-picker HTML page. The current color is
// embedded into the page so the picker reflects the active value.
static void handleHome() {
    string hex = rgbToHex(current_color[0], current_color[1], current_color[2]);
    string html =
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<head><title>ESP8266 LED Controller</title></head>\n"
        "<body>\n"
        "    <h2>Color Picker</h2>\n"
        "    <input type=\"color\" id=\"colorPicker\" value=\"" + hex + "\">\n"
        "    <button onclick=\"sendJson()\">Send</button>\n"
        "    <script>\n"
        "        async function sendJson() {\n"
        "            const colorVal = document.getElementById('colorPicker').value;\n"
        "            const res = await fetch('/set-color', {\n"
        "                method: 'POST',\n"
        "                headers: { 'Content-Type': 'application/json' },\n"
        "                body: JSON.stringify({ color: colorVal })\n"
        "            });\n"
        "        }\n"
        "    </script>\n"
        "</body>\n"
        "</html>\n";
    server.send(200, "text/html", html);
}

// handleSetColor parses the JSON body from a color-picker POST, updates
// the current color and the persisted config, and sends back an RGB summary.
static void handleSetColor() {
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

    string hex = doc["color"] | "#000000";
    hexToRgb(hex, current_color[0], current_color[1], current_color[2]);
    config.color[0] = current_color[0];
    config.color[1] = current_color[1];
    config.color[2] = current_color[2];
    config.save();

    char resp[64];
    snprintf(resp, sizeof(resp), "RGB: (%d, %d, %d)",
             current_color[0], current_color[1], current_color[2]);
    server.send(200, "text/plain", resp);
}

// setup initialises the filesystem, loads persisted config, starts the LED
// controller, initiates an async WiFi connection, and registers HTTP routes.
void setup() {
    if (!LittleFS.begin()) {
        // LittleFS mount failed — will use defaults
    }

    pinMode(PIN_LED_BUILTIN, OUTPUT);
    digitalWrite(PIN_LED_BUILTIN, HIGH);
    pinMode(PIN_SWITCH, INPUT_PULLUP);

    if (!config.load()) {
        // Config load failed — using defaults
    }
    current_color[0] = config.color[0];
    current_color[1] = config.color[1];
    current_color[2] = config.color[2];

    leds.begin(PIN_NEOPIXEL, config.n_leds);

    WiFi.mode(WIFI_STA);
    WiFi.begin(config.ssid.c_str(), config.password.c_str());

    server.on("GET", "/", handleHome);
    server.on("POST", "/set-color", handleSetColor);
    server.begin();
}

unsigned long last_led_update = 0;

// loop processes HTTP requests and drives the LED strip at 100 Hz.
void loop() {
    server.handleClient();
    unsigned long now = millis();
    if (now - last_led_update >= 100) {
        last_led_update = now;
        updateLEDs();
    }
}
