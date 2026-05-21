#pragma once
#include <string>
#include <functional>
#include <map>

class WebServer {
public:
    using Handler = std::function<void()>;

    void begin(int port = 80);
    void on(const std::string& method, const std::string& path, Handler handler);
    void handleClient();
    void close();

    std::string path() const;
    std::string method() const;
    std::string body() const;
    void send(int code, const std::string& content_type, const std::string& content);

private:
    std::map<std::pair<std::string, std::string>, Handler> routes_;
    void* server_ptr_ = nullptr;
};
