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
CORE_OBJ_DIR := $(OBJ_DIR)/core

# ---------------------------------------------------------------------------
# Source files — project
# ---------------------------------------------------------------------------
SRC_CXX := \
    src/main.cpp \
    src/utils.cpp \
    src/config.cpp \
    src/led_controller.cpp \
    src/webserver.cpp

OBJ := $(SRC_CXX:src/%.cpp=$(OBJ_DIR)/%.o)

# ---------------------------------------------------------------------------
# Source files — ESP8266 Arduino core
# ---------------------------------------------------------------------------
CORE_SRC_CXX := $(wildcard $(ARDUINO_CORE_DIR)/*.cpp) \
                $(wildcard $(ARDUINO_CORE_DIR)/libb64/*.cpp) \
                $(wildcard $(ARDUINO_CORE_DIR)/umm_malloc/*.cpp)
CORE_SRC_C   := $(wildcard $(ARDUINO_CORE_DIR)/umm_malloc/*.c)
CORE_SRC_S   := $(wildcard $(ARDUINO_CORE_DIR)/*.S)
CORE_OBJ     := $(CORE_SRC_CXX:$(ARDUINO_CORE_DIR)/%.cpp=$(CORE_OBJ_DIR)/%.o) \
                $(CORE_SRC_C:$(ARDUINO_CORE_DIR)/%.c=$(CORE_OBJ_DIR)/%.o) \
                $(CORE_SRC_S:$(ARDUINO_CORE_DIR)/%.S=$(CORE_OBJ_DIR)/%.o)

ARDUINO_CORE_A := $(BUILD_DIR)/core.a

# ---------------------------------------------------------------------------
# Source files — ESP8266 Arduino libraries
# ---------------------------------------------------------------------------
LIB_WIFI_SRC    := $(wildcard $(ARDUINO_DIR)/libraries/ESP8266WiFi/src/*.cpp)
LIB_WEBSERVER_SRC := $(wildcard $(ARDUINO_DIR)/libraries/ESP8266WebServer/src/*.cpp) \
                     $(wildcard $(ARDUINO_DIR)/libraries/ESP8266WebServer/src/detail/*.cpp)
LIB_LITTLEFS_SRC := $(wildcard $(ARDUINO_DIR)/libraries/LittleFS/src/*.cpp) \
                    $(wildcard $(ARDUINO_DIR)/libraries/LittleFS/src/*.c)

LIB_WIFI_A       := $(BUILD_DIR)/ESP8266WiFi.a
LIB_WEBSERVER_A  := $(BUILD_DIR)/ESP8266WebServer.a
LIB_LITTLEFS_A   := $(BUILD_DIR)/LittleFS.a

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
    -D_GNU_SOURCE -D__ets__ -DICACHE_FLASH \
    -DARDUINO=10819 -DARDUINO_ESP8266_ESP12F \
    -DARDUINO_ARCH_ESP8266 -DESP8266 \
    -DF_CPU=80000000L -DFLASHMODE_DOUT \
    -DLWIP_OPEN_SRC -DTCP_MSS=1460 \
    -DTCP_SND_BUF=32768 -DTCP_WND=32768 \
    -DLWIP_FEATURES=1 -DLWIP_IPV6=0 \
    -DDEBUG_ESP_PORT=Serial \
    -DUSE_OPTIMIZE_PRINTF \
    -include $(ARDUINO_CORE_DIR)/Arduino.h \
    -I$(ARDUINO_CORE_DIR) \
    -I$(ARDUINO_VARIANT_DIR) \
    -I$(SDK_DIR)/include \
    -I$(SDK_DIR)/lwip2/include \
    -I$(ARDUINOJSON_DIR)/src \
    -I$(NEOPIXEL_DIR) \
    $(patsubst %,-I%/src, $(ARDUINO_LIB_DIRS)) \
    -Iinclude

LDFLAGS := \
    -Os -g0 -Wl,--gc-sections \
    -Wl,-wrap,system_restart_local -Wl,-wrap,register_chipv6_phy \
    -T$(LD_SCRIPT) \
    -L$(SDK_DIR)/lib \
    -L$(SDK_DIR)/ld

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

# Compile core .cpp -> .o
$(CORE_OBJ_DIR)/%.o: $(ARDUINO_CORE_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(CORE_OBJ_DIR)/%.o: $(ARDUINO_CORE_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CXXFLAGS) -o $@ $<

$(CORE_OBJ_DIR)/%.o: $(ARDUINO_CORE_DIR)/%.S
	@mkdir -p $(dir $@)
	$(CC) $(CXXFLAGS) -o $@ $<

# Build core.a
$(ARDUINO_CORE_A): $(CORE_OBJ)
	$(AR) cru $@ $^

# Compile library source files
$(OBJ_DIR)/ESP8266WiFi/%.o: $(ARDUINO_DIR)/libraries/ESP8266WiFi/src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/ESP8266WebServer/%.o: $(ARDUINO_DIR)/libraries/ESP8266WebServer/src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/ESP8266WebServer/detail/%.o: $(ARDUINO_DIR)/libraries/ESP8266WebServer/src/detail/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/LittleFS/%.o: $(ARDUINO_DIR)/libraries/LittleFS/src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/LittleFS/%.o: $(ARDUINO_DIR)/libraries/LittleFS/src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CXXFLAGS) -o $@ $<

WIFI_OBJ      := $(patsubst $(ARDUINO_DIR)/libraries/ESP8266WiFi/src/%.cpp,$(OBJ_DIR)/ESP8266WiFi/%.o,$(LIB_WIFI_SRC))
WEBSERVER_OBJ := $(patsubst $(ARDUINO_DIR)/libraries/ESP8266WebServer/src/%.cpp,$(OBJ_DIR)/ESP8266WebServer/%.o,$(LIB_WEBSERVER_SRC))
LITTLEFS_OBJ  := $(patsubst $(ARDUINO_DIR)/libraries/LittleFS/src/%.cpp,$(OBJ_DIR)/LittleFS/%.o,$(LIB_LITTLEFS_SRC)) \
                 $(patsubst $(ARDUINO_DIR)/libraries/LittleFS/src/%.c,$(OBJ_DIR)/LittleFS/%.o,$(filter %.c,$(LIB_LITTLEFS_SRC)))

$(LIB_WIFI_A): $(WIFI_OBJ)
	$(AR) cru $@ $^

$(LIB_WEBSERVER_A): $(WEBSERVER_OBJ)
	$(AR) cru $@ $^

$(LIB_LITTLEFS_A): $(LITTLEFS_OBJ)
	$(AR) cru $@ $^

# Link
$(BUILD_DIR)/firmware.elf: $(OBJ) $(ARDUINO_CORE_A) $(LIB_WIFI_A) $(LIB_WEBSERVER_A) $(LIB_LITTLEFS_A)
	$(CXX) $(LDFLAGS) \
	    -Wl,--start-group \
	    $(OBJ) $(ARDUINO_CORE_A) \
	    -lcirom -lethernet -llwip -lpp -lphy -lwpa -lmain -lnet80211 -lcrypto -lhal -lgcc \
	    $(LIB_WIFI_A) $(LIB_WEBSERVER_A) $(LIB_LITTLEFS_A) \
	    -Wl,--end-group \
	    -lm -o $@
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
