#include "webserver.h"
#include <ESP8266WebServer.h>

WebServer::WebServer(int port) {
    server_ptr_ = new ESP8266WebServer(port);
}

WebServer::~WebServer() {
    delete server_ptr_;
}

void WebServer::begin() {
    server_ptr_->begin();
}

void WebServer::on(const string& method, const string& path, Handler handler) {
    routes_[{method, path}] = handler;
    HTTPMethod m = (method == "GET") ? HTTP_GET : HTTP_POST;
    server_ptr_->on(
        path.c_str(), m, [handler]() { handler(); });
}

void WebServer::handleClient() {
    if (server_ptr_) {
        server_ptr_->handleClient();
    }
}

string WebServer::path() const {
    if (!server_ptr_) return {};
    return string(server_ptr_->uri().c_str());
}

string WebServer::method() const {
    if (!server_ptr_) return {};
    auto m = server_ptr_->method();
    return (m == HTTP_GET) ? "GET" : "POST";
}

string WebServer::body() const {
    if (!server_ptr_) return {};
    return string(server_ptr_->arg("plain").c_str());
}

void WebServer::send(int code, const string& content_type, const string& content) {
    if (!server_ptr_) return;
    server_ptr_->send(
        code, content_type.c_str(), content.c_str());
}
