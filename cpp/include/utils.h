#pragma once
#include <string>

std::string rgbToHex(int r, int g, int b);
void hexToRgb(const std::string& hex, int& r, int& g, int& b);
float readPotentiometer(int raw_adc);
