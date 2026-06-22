# ESP-12F LED Controller — C++ Implementation

This is a C++ rewrite of the Python LED controller, targeting the **ESP-12F (ESP8266)**
microcontroller via the D1 Mini board. All testing is done through unit and
integration tests on the host machine.

---

## Prerequisites

### For the ESP-12F release build

| Dependency | Notes |
|---|---|
| **xtensa-lx106-elf toolchain** | Download from the [Arduino ESP8266 release](https://github.com/esp8266/Arduino) |
| **ESP8266 Arduino core** | Clone [`esp8266/Arduino`](https://github.com/esp8266/Arduino) |
| **ArduinoJson** v7+ | Download from [bblanchon/ArduinoJson](https://github.com/bblanchon/ArduinoJson) |
| **Adafruit NeoPixel** | Download from [adafruit/Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) |
| **esptool.py** | `pip install esptool` |
| **mklittlefs** | Tool for building the SPIFFS/LittleFS filesystem image |

The `Makefile` requires these paths — set them as environment variables or pass them to `make`:

| Variable | Example value |
|---|---|
| `ARDUINO_DIR` | `/opt/esp8266/arduino` |
| `ARDUINOJSON_DIR` | `/opt/esp8266/ArduinoJson` |
| `NEOPIXEL_DIR` | `/opt/esp8266/Adafruit_NeoPixel` |
| `PORT` | `/dev/ttyUSB0` |
| `ESPTOOL` | `esptool.py` (default) |
| `MONITOR` | `picocom` (default) |

### For running tests

- **g++** (C++17 support, e.g. GCC 8+ or Clang 7+)
- **curl** or **wget** (to auto-download ArduinoJson; skipped if already present)

The test Makefile automatically downloads ArduinoJson v7.4.3 into `test/build/`.

---

## Building the ESP-12F firmware

```bash
cd cpp

# Build the firmware binary (paths must be set)
make build \
    ARDUINO_DIR=/opt/esp8266/arduino \
    ARDUINOJSON_DIR=/opt/esp8266/ArduinoJson \
    NEOPIXEL_DIR=/opt/esp8266/Adafruit_NeoPixel \
    PORT=/dev/ttyUSB0
```

Output: `build/firmware.bin`

---

## Deploying to the ESP-12F

### Flash firmware

```bash
make flash \
    ARDUINO_DIR=/opt/esp8266/arduino \
    ARDUINOJSON_DIR=/opt/esp8266/ArduinoJson \
    NEOPIXEL_DIR=/opt/esp8266/Adafruit_NeoPixel \
    PORT=/dev/ttyUSB0
```

### Flash filesystem (config.json)

First edit `data/config.json` with your WiFi credentials and LED count, then:

```bash
make flashfs PORT=/dev/ttyUSB0
```

### Serial monitor

```bash
make monitor PORT=/dev/ttyUSB0
```

> **Tip:** Export the paths in your shell profile to avoid typing them repeatedly:
> ```bash
> export ARDUINO_DIR=/opt/esp8266/arduino
> export ARDUINOJSON_DIR=/opt/esp8266/ArduinoJson
> export NEOPIXEL_DIR=/opt/esp8266/Adafruit_NeoPixel
> ```

---

## Running tests

```bash
cd cpp/test
make test
```

This downloads ArduinoJson on first run, then compiles and executes all tests.

To rebuild from scratch:

```bash
make clean && make test
```

---

## Project structure

```
cpp/
├── Makefile               # Build system for ESP-12F firmware
├── include/
│   ├── config.h           # Settings persistence (load/save JSON via LittleFS)
│   ├── led_controller.h   # NeoPixel LED strip driver
│   ├── utils.h            # Pure functions (hex/rgb conversion, math)
│   └── webserver.h        # HTTP server (wraps ESP8266WebServer)
├── src/
│   ├── main.cpp           # Entry point, WiFi, HTTP handlers, LED update loop
│   ├── config.cpp         # JSON load/save via LittleFS + ArduinoJson
│   ├── led_controller.cpp # Adafruit NeoPixel wrapper
│   ├── utils.cpp          # rgbToHex, hexToRgb, readPotentiometer
│   └── webserver.cpp      # ESP8266WebServer wrapper
├── data/
│   └── config.json        # Runtime config template (edit before flashfs)
├── test/
│   ├── Makefile            # Test build system
│   ├── framework.h         # Minimal test framework header
│   ├── main.cpp            # Test runner entry point
│   ├── test_utils.cpp      # Unit tests for pure functions
│   ├── test_config.cpp     # Integration tests for config persistence
│   ├── test_led_controller.cpp
│   ├── test_webserver.cpp
│   └── mocks/
│       ├── Arduino.h / .cpp
│       ├── ESP8266WiFi.h / .cpp
│       ├── ESP8266WebServer.h / .cpp
│       ├── LittleFS.h / .cpp
│       └── Adafruit_NeoPixel.h
└── README.md
```

## Pin mapping

| Component     | GPIO | Board Pin | Function                   |
|---------------|------|-----------|----------------------------|
| Built-in LED  | 2    | D4        | Status indicator           |
| NeoPixel data | 14   | D5        | LED strip data             |
| Toggle switch | 4    | D2        | White override (HIGH=white)|
| Potentiometer | A0   | A0        | Brightness control         |

## License

GNU General Public License v3 — see `LICENSE` in the repository root.
