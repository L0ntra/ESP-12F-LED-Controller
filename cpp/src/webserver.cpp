#include "webserver.h"
#include <ESP8266WebServer.h>

// WebServer allocates the underlying ESP8266WebServer instance on the
// heap so that the header only needs a forward declaration.
WebServer::WebServer(int port) {
    server_ptr_ = new ESP8266WebServer(port);
}

WebServer::~WebServer() {
    delete server_ptr_;
}

void WebServer::begin() {
    server_ptr_->begin();
}

// on registers a handler for the given HTTP method and path pair. The
// route is stored in both the WebServer route map and the underlying
// ESP8266WebServer so that handleClient dispatches correctly.
void WebServer::on(const string& method, const string& path, Handler handler) {
    routes_[{method, path}] = handler;
    HTTPMethod m = (method == "GET") ? HTTP_GET : HTTP_POST;
    server_ptr_->on(
        path.c_str(), m, [handler]() { handler(); });
}

// handleClient delegates to the underlying server. It is safe to call
// before the server is started (it becomes a no-op).
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
