#pragma once
#include <cstdint>
#include <string>
using std::string;

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
