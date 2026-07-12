#pragma once
#include <string>
#include <functional>
#include <ESP8266WebServer.h>
using std::string;
using std::function;

class WebServer {
public:
    using Handler = function<void()>;

    WebServer(int port = 80);
    ~WebServer();

    void begin();
    void on(const string& method, const string& path, Handler handler);
    void handleClient();

    string body() const;
    void send(int code, const string& content_type, const string& content);

private:
    ESP8266WebServer* server_ptr_ = nullptr;
};
