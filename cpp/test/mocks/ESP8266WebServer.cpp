#include "ESP8266WebServer.h"

void ESP8266WebServer::handleClient() {
    if (!started_) return;
    for (auto& route : routes_) {
        if (route.uri == current_uri_.c_str() &&
            (route.method == HTTP_ANY || route.method == current_method_)) {
            route.handler();
            return;
        }
    }
    send(404, "text/plain", "Not Found");
}

void ESP8266WebServer::send(int code, const char* ct, const String& content) {
    status_code_ = code;
    content_type_ = ct ? ct : "";
    response_body_ = content.c_str();
}
