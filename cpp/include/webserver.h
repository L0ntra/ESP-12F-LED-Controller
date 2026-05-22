#pragma once
#include <string>
#include <functional>
#include <map>
using std::string;
using std::function;
using std::map;
using std::pair;

class WebServer {
public:
    using Handler = function<void()>;

    void begin(int port = 80);
    void on(const string& method, const string& path, Handler handler);
    void handleClient();
    void close();

    string path() const;
    string method() const;
    string body() const;
    void send(int code, const string& content_type, const string& content);

private:
    map<pair<string, string>, Handler> routes_;
    void* server_ptr_ = nullptr;
};
