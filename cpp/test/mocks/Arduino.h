#pragma once
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <string>
using std::string;

typedef uint8_t byte;

#define INPUT          0x0
#define OUTPUT         0x1
#define INPUT_PULLUP   0x2
#define LOW            0
#define HIGH           1

struct _mock_pin_state {
    uint8_t mode = INPUT;
    uint8_t value = LOW;
};

extern _mock_pin_state mock_pins[17];
extern int mock_analog_value;
extern unsigned long mock_millis;

inline void pinMode(uint8_t pin, uint8_t mode) {
    if (pin < 17) mock_pins[pin].mode = mode;
}
inline void digitalWrite(uint8_t pin, uint8_t val) {
    if (pin < 17) mock_pins[pin].value = val;
}
inline int digitalRead(uint8_t pin) {
    return (pin < 17) ? mock_pins[pin].value : LOW;
}
inline int analogRead(uint8_t) { return mock_analog_value; }
inline void delay(unsigned long ms) { mock_millis += ms; }
inline unsigned long millis() { return mock_millis; }

class String {
    string s_;
public:
    String() = default;
    String(const char* s) : s_(s ? s : "") {}
    String(const String& o) : s_(o.s_) {}
    String& operator=(const String& o) { s_ = o.s_; return *this; }
    String& operator=(const char* s) { s_ = s ? s : ""; return *this; }
    bool operator==(const String& o) const { return s_ == o.s_; }
    bool operator!=(const String& o) const { return s_ != o.s_; }
    const char* c_str() const { return s_.c_str(); }
    size_t length() const { return s_.size(); }
};

class Serial_ {
public:
    void begin(int) {}
    void print(const char* s) { if (s) printf("%s", s); }
    void print(int v) { printf("%d", v); }
    void println(const char* s) { if (s) printf("%s\n", s); else printf("\n"); }
    void println(int v) { printf("%d\n", v); }
    void println() { printf("\n"); }
    void printf(const char* fmt, ...) __attribute__((format(printf,2,3))) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
};

extern Serial_ Serial;

#define F(string_literal) (string_literal)
#define PROGMEM
#define pgm_read_byte(addr) (*(const unsigned char*)(addr))
#define pgm_read_word(addr) (*(const unsigned short*)(addr))
#define pgm_read_dword(addr) (*(const unsigned long*)(addr))
