#include "doctest.h"
#include "Arduino.h"
#include "ESP8266WebServer.h"
#include "../include/webserver.h"

TEST_CASE("WebServer.registers_and_routes_GET") {
    ESP8266WebServer mock(80);
    bool called = false;

    mock.on("/test", HTTP_GET, [&]() { called = true; });
    mock.begin();
    mock.setRequest("GET", "/test");
    mock.handleClient();

    CHECK(called);
}

TEST_CASE("WebServer.route_not_found_returns_404") {
    ESP8266WebServer mock(80);
    mock.begin();
    mock.setRequest("GET", "/nonexistent");
    mock.handleClient();

    CHECK_EQ(mock.status_code_, 404);
}

TEST_CASE("WebServer.registers_and_routes_POST") {
    ESP8266WebServer mock(80);
    bool called = false;

    mock.on("/set-color", HTTP_POST, [&]() { called = true; });
    mock.begin();
    mock.setRequest("POST", "/set-color", "{\"color\":\"#ff0000\"}");
    mock.handleClient();

    CHECK(called);
}

TEST_CASE("WebServer.plain_arg_contains_body") {
    ESP8266WebServer mock(80);
    string body;

    mock.on("/set-color", HTTP_POST, [&]() {
        body = mock.arg("plain").c_str();
    });
    mock.begin();
    mock.setRequest("POST", "/set-color", "{\"color\":\"#ff0000\"}");
    mock.handleClient();

    CHECK_EQ(body, "{\"color\":\"#ff0000\"}");
}

TEST_CASE("WebServer.send_sets_status_code") {
    ESP8266WebServer mock(80);
    mock.begin();
    mock.setRequest("GET", "/test");
    mock.send(200, "text/html", "<h1>OK</h1>");

    CHECK_EQ(mock.status_code_, 200);
    CHECK_EQ(mock.content_type_, "text/html");
    CHECK_EQ(mock.response_body_, "<h1>OK</h1>");
}

TEST_CASE("WebServer.on_before_begin_does_not_crash") {
    WebServer ws;
    ws.on("GET", "/route", []() {});
}
