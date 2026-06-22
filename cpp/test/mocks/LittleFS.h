#pragma once
#include <cstdint>
#include <string>
#include <map>
#include <cstring>
using std::string;
using std::map;

class File {
    friend class LittleFSClass;
    string* storage_ = nullptr;
    size_t read_pos_ = 0;

public:
    File() = default;

    size_t size() const { return storage_ ? storage_->size() : 0; }

    size_t readBytes(char* buf, size_t len) {
        if (!storage_) return 0;
        size_t avail = storage_->size() - read_pos_;
        size_t to_read = (len < avail) ? len : avail;
        memcpy(buf, storage_->data() + read_pos_, to_read);
        read_pos_ += to_read;
        return to_read;
    }

    size_t write(uint8_t b) {
        if (!storage_) return 0;
        *storage_ += static_cast<char>(b);
        return 1;
    }

    size_t write(const uint8_t* data, size_t len) {
        if (!storage_) return 0;
        storage_->append(reinterpret_cast<const char*>(data), len);
        return len;
    }

    void close() {}
    operator bool() const { return storage_ != nullptr; }
};

class LittleFSClass {
    map<string, string> files_;

public:
    bool begin() { return true; }
    void end() {}

    File open(const char* path, const char* mode) {
        if (!path) return File();
        File f;
        if (mode && mode[0] == 'w') {
            files_[path] = "";
            f.storage_ = &files_[path];
        } else {
            auto it = files_.find(path);
            if (it != files_.end()) {
                f.storage_ = &it->second;
            }
        }
        return f;
    }

    string& fileContent(const char* path) { return files_[path]; }
    void clear() { files_.clear(); }
};

extern LittleFSClass LittleFS;
