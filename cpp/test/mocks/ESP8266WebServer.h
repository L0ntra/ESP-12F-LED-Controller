#pragma once
#include <functional>
#include <map>
#include <string>
#include <vector>
#include "Arduino.h"
using std::string;
using std::function;
using std::map;
using std::vector;

enum HTTPMethod : uint8_t { HTTP_ANY = 0, HTTP_GET = 1, HTTP_POST = 2 };

class ESP8266WebServer {
public:
    using Handler = function<void()>;

    struct Route {
        string uri;
        HTTPMethod method;
        Handler handler;
    };

    ESP8266WebServer(int port = 80) { (void)port; }

    void on(const String& uri, HTTPMethod method, Handler handler) {
        routes_.push_back({uri.c_str(), method, handler});
    }

    void begin() { started_ = true; }
    void handleClient();

    String uri() const { return current_uri_; }
    HTTPMethod method() const { return current_method_; }
    String arg(const String& name) const {
        if (name == "plain") return request_body_;
        auto it = args_.find(name.c_str());
        return (it != args_.end()) ? it->second.c_str() : "";
    }
    void send(int code, const char* content_type, const String& content);

    // Test helpers
    void setRequest(const String& method, const String& uri, const String& body = "") {
        current_method_ = (method == "GET") ? HTTP_GET : HTTP_POST;
        current_uri_ = uri;
        request_body_ = body;
    }

    int status_code_ = 0;
    string content_type_;
    string response_body_;

private:
    bool started_ = false;
    String current_uri_;
    HTTPMethod current_method_ = HTTP_ANY;
    String request_body_;
    map<string, String> args_;
    vector<Route> routes_;
};
