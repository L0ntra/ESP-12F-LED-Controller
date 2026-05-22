CXX := g++
CXXFLAGS := -std=c++17 -g -Wall -Wextra
LDFLAGS  :=

# ---------------------------------------------------------------------------
# Paths
# ---------------------------------------------------------------------------
SRC_DIR      := ..
MOCK_DIR     := mocks
BUILD_DIR    := build

# Download ArduinoJson if not present
ARDUINOJSON_DIR := $(BUILD_DIR)/ArduinoJson
ARDUINOJSON_VERSION := 7.4.3

# ---------------------------------------------------------------------------
# Include paths (mocks come first to shadow ESP8266 headers)
# ---------------------------------------------------------------------------
INCLUDES := \
    -I$(MOCK_DIR) \
    -I$(SRC_DIR)/include \
    -I$(ARDUINOJSON_DIR)/src

# ---------------------------------------------------------------------------
# Source files
# ---------------------------------------------------------------------------
SRC := \
    $(SRC_DIR)/src/utils.cpp \
    $(SRC_DIR)/src/config.cpp \
    $(SRC_DIR)/src/led_controller.cpp \
    $(SRC_DIR)/src/webserver.cpp \
    $(MOCK_DIR)/Arduino.cpp \
    $(MOCK_DIR)/ESP8266WiFi.cpp \
    $(MOCK_DIR)/ESP8266WebServer.cpp \
    $(MOCK_DIR)/LittleFS.cpp

TEST_SRC := \
    test_utils.cpp \
    test_config.cpp \
    test_led_controller.cpp \
    test_webserver.cpp \
    main.cpp

OBJ := $(SRC:%.cpp=$(BUILD_DIR)/%.o) $(TEST_SRC:%.cpp=$(BUILD_DIR)/%.o)

# ---------------------------------------------------------------------------
# Targets
# ---------------------------------------------------------------------------
.PHONY: all test clean

all: fetch_deps test

test: $(BUILD_DIR)/run_tests
	./$(BUILD_DIR)/run_tests

$(BUILD_DIR)/run_tests: $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

$(BUILD_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

fetch_deps:
	@if [ ! -d "$(ARDUINOJSON_DIR)" ]; then \
	    echo "==> Downloading ArduinoJson v$(ARDUINOJSON_VERSION)..."; \
	    mkdir -p $(BUILD_DIR); \
	    curl -sL "https://github.com/bblanchon/ArduinoJson/archive/refs/tags/v$(ARDUINOJSON_VERSION).tar.gz" \
	        | tar xz -C $(BUILD_DIR); \
	    mv $(BUILD_DIR)/ArduinoJson-$(ARDUINOJSON_VERSION) $(ARDUINOJSON_DIR); \
	    echo "==> Done"; \
	fi

clean:
	rm -rf $(BUILD_DIR)
