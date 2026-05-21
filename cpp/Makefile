CXX := xtensa-lx106-elf-g++
CC  := xtensa-lx106-elf-gcc
AR  := xtensa-lx106-elf-ar
OBJCOPY := xtensa-lx106-elf-objcopy
SIZE := xtensa-lx106-elf-size
ESPTOOL ?= esptool.py
MONITOR ?= picocom

# ---------------------------------------------------------------------------
# Required paths — set these to match your local environment.
# Example:
#   make build \
#     ARDUINO_DIR=/opt/esp8266/arduino \
#     ARDUINOJSON_DIR=/opt/esp8266/ArduinoJson \
#     NEOPIXEL_DIR=/opt/esp8266/Adafruit_NeoPixel
# ---------------------------------------------------------------------------
ifndef ARDUINO_DIR
$(error ARDUINO_DIR is not set. Point it to the ESP8266 Arduino core directory)
endif
ifndef ARDUINOJSON_DIR
$(error ARDUINOJSON_DIR is not set. Point it to the ArduinoJson library root)
endif
ifndef NEOPIXEL_DIR
$(error NEOPIXEL_DIR is not set. Point it to the Adafruit NeoPixel library root)
endif

# ---------------------------------------------------------------------------
# Build directories
# ---------------------------------------------------------------------------
BUILD_DIR := build
OBJ_DIR   := $(BUILD_DIR)/obj

# ---------------------------------------------------------------------------
# Source files
# ---------------------------------------------------------------------------
SRC_CXX := \
    src/main.cpp \
    src/utils.cpp \
    src/config.cpp \
    src/led_controller.cpp \
    src/webserver.cpp

OBJ := $(SRC_CXX:src/%.cpp=$(OBJ_DIR)/%.o)

# ---------------------------------------------------------------------------
# ESP8266 Arduino core object files
# ---------------------------------------------------------------------------
ARDUINO_CORE_DIR  := $(ARDUINO_DIR)/cores/esp8266
ARDUINO_VARIANT_DIR := $(ARDUINO_DIR)/variants/d1_mini
ARDUINO_LIB_DIRS  := \
    $(ARDUINO_DIR)/libraries/ESP8266WiFi \
    $(ARDUINO_DIR)/libraries/ESP8266WebServer \
    $(ARDUINO_DIR)/libraries/LittleFS

SDK_DIR           := $(ARDUINO_DIR)/tools/sdk
LD_SCRIPT         := $(SDK_DIR)/ld/eagle.flash.4m.ld

# ---------------------------------------------------------------------------
# Compiler / linker flags
# ---------------------------------------------------------------------------
CXXFLAGS := \
    -std=c++17 \
    -Os -g0 \
    -Wpointer-arith -Wno-implicit-function-declaration \
    -Wl,-EL -fno-inline-functions -nostdlib -c \
    -mlongcalls -mtext-section-literals -fno-rtti \
    -falign-functions=4 -fdata-sections -ffunction-sections \
    -D__ets__ -DICACHE_FLASH \
    -DARDUINO=10819 -DARDUINO_ESP8266_ESP12F \
    -DARDUINO_ARCH_ESP8266 -DESP8266 \
    -DF_CPU=80000000L -DFLASHMODE_DOUT \
    -DLWIP_OPEN_SRC -DTCP_MSS=1460 \
    -DTCP_SND_BUF=32768 -DTCP_WND=32768 \
    -DLWIP_FEATURES=1 -DLWIP_IPV6=0 \
    -DDEBUG_ESP_PORT=Serial \
    -DCORE_MOCK -DUSE_OPTIMIZE_PRINTF \
    -include $(ARDUINO_CORE_DIR)/Arduino.h \
    -I$(ARDUINO_CORE_DIR) \
    -I$(ARDUINO_VARIANT_DIR) \
    -I$(SDK_DIR)/include \
    -I$(SDK_DIR)/lwip/include \
    -I$(ARDUINOJSON_DIR)/src \
    -I$(NEOPIXEL_DIR) \
    $(patsubst %,-I%/src, $(ARDUINO_LIB_DIRS)) \
    -Iinclude

LDFLAGS := \
    -Os -g0 -Wl,--gc-sections \
    -Wl,-wrap,system_restart_local -Wl,-wrap,register_chipv6_phy \
    -T$(LD_SCRIPT) \
    -L$(SDK_DIR)/lib \
    -L$(SDK_DIR)/ld \
    -Wl,--start-group \
    $(ARDUINO_CORE_DIR)/core.a \
    -lcirom -lethernet -llwip -lpp -lphy -lwpa -lmain -lnet80211 -lcrypto -lhal -lgcc \
    $(ARDUINO_DIR)/libraries/ESP8266WiFi/ESP8266WiFi.a \
    $(ARDUINO_DIR)/libraries/ESP8266WebServer/ESP8266WebServer.a \
    $(ARDUINO_DIR)/libraries/LittleFS/LittleFS.a \
    -Wl,--end-group \
    -lm

# ---------------------------------------------------------------------------
# Targets
# ---------------------------------------------------------------------------
.PHONY: all build flash flashfs clean

all: $(BUILD_DIR)/firmware.bin

build: all

# Compile .cpp -> .o
$(OBJ_DIR)/%.o: src/%.cpp include/*.h
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

# Link
$(BUILD_DIR)/firmware.elf: $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^
	$(SIZE) $@

# Convert ELF -> BIN
$(BUILD_DIR)/firmware.bin: $(BUILD_DIR)/firmware.elf
	$(OBJCOPY) -O binary $< $@
	@echo "=== Firmware written to $@ ==="
	@echo "Flash size: $$(stat -c%s $@ 2>/dev/null || stat -f%z $@ 2>/dev/null) bytes"

# Flash firmware
flash: $(BUILD_DIR)/firmware.bin
	$(ESPTOOL) --port $(PORT) --baud 460800 write_flash \
	    --flash_mode dout --flash_freq 80m \
	    0x0 $<

# Flash LittleFS filesystem (needs mklittlefs tool)
flashfs:
	@echo "Building LittleFS image..."
	mklittlefs -c data/ -s 1048576 $(BUILD_DIR)/littlefs.bin
	$(ESPTOOL) --port $(PORT) --baud 460800 write_flash \
	    0x300000 $(BUILD_DIR)/littlefs.bin

# Serial monitor
monitor:
	$(MONITOR) $(PORT) -b 115200

clean:
	rm -rf $(BUILD_DIR)
