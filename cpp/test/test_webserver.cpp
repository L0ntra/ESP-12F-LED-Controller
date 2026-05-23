#include "framework.h"
#include "Arduino.h"
#include "ESP8266WebServer.h"
#include "../include/webserver.h"

TEST(WebServer, registers_and_routes_GET) {
    ESP8266WebServer mock(80);
    bool called = false;

    mock.on("/test", HTTP_GET, [&]() { called = true; });
    mock.begin();
    mock.setRequest("GET", "/test");
    mock.handleClient();

    ASSERT_TRUE(called);
}

TEST(WebServer, route_not_found_returns_404) {
    ESP8266WebServer mock(80);
    mock.begin();
    mock.setRequest("GET", "/nonexistent");
    mock.handleClient();

    ASSERT_EQ(mock.status_code_, 404);
}

TEST(WebServer, registers_and_routes_POST) {
    ESP8266WebServer mock(80);
    bool called = false;

    mock.on("/set-color", HTTP_POST, [&]() { called = true; });
    mock.begin();
    mock.setRequest("POST", "/set-color", "{\"color\":\"#ff0000\"}");
    mock.handleClient();

    ASSERT_TRUE(called);
}

TEST(WebServer, plain_arg_contains_body) {
    ESP8266WebServer mock(80);
    string body;

    mock.on("/set-color", HTTP_POST, [&]() {
        body = mock.arg("plain").c_str();
    });
    mock.begin();
    mock.setRequest("POST", "/set-color", "{\"color\":\"#ff0000\"}");
    mock.handleClient();

    ASSERT_STREQ(body, "{\"color\":\"#ff0000\"}");
}

TEST(WebServer, send_sets_status_code) {
    ESP8266WebServer mock(80);
    mock.begin();
    mock.setRequest("GET", "/test");
    mock.send(200, "text/html", "<h1>OK</h1>");

    ASSERT_EQ(mock.status_code_, 200);
    ASSERT_STREQ(mock.content_type_, "text/html");
    ASSERT_STREQ(mock.response_body_, "<h1>OK</h1>");
}

TEST(WebServer, on_before_begin_does_not_crash) {
    WebServer ws;
    ws.on("GET", "/route", []() {});
    // The fix: constructor allocates ESP8266WebServer immediately,
    // so on() never dereferences a null server_ptr_
}
