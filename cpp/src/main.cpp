#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#include "config.h"
#include "led_controller.h"
#include "webserver.h"
#include "utils.h"

constexpr uint8_t PIN_LED_BUILTIN = 2;
constexpr uint8_t PIN_NEOPIXEL = 14;
constexpr uint8_t PIN_SWITCH = 4;

Config config;
LEDController leds;
WebServer server;
int current_color[3] = {255, 0, 0};

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

    Serial.printf("Updated color: (%d, %d, %d)\n",
                  current_color[0], current_color[1], current_color[2]);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    pinMode(PIN_LED_BUILTIN, OUTPUT);
    digitalWrite(PIN_LED_BUILTIN, HIGH);
    pinMode(PIN_SWITCH, INPUT_PULLUP);

    if (!config.load()) {
        Serial.println("Config load failed, using defaults");
    }
    current_color[0] = config.color[0];
    current_color[1] = config.color[1];
    current_color[2] = config.color[2];

    Serial.print("Connecting to WiFi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(config.ssid.c_str(), config.password.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        digitalWrite(PIN_LED_BUILTIN, !digitalRead(PIN_LED_BUILTIN));
    }
    Serial.println();
    Serial.print("Connected! IP: ");
    Serial.println(WiFi.localIP());
    digitalWrite(PIN_LED_BUILTIN, LOW);

    server.on("GET", "/", handleHome);
    server.on("POST", "/set-color", handleSetColor);
    server.begin();
    leds.begin(PIN_NEOPIXEL, config.n_leds);
}

unsigned long last_led_update = 0;

void loop() {
    server.handleClient();
    unsigned long now = millis();
    if (now - last_led_update >= 100) {
        last_led_update = now;
        updateLEDs();
    }
}
