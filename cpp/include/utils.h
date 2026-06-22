#pragma once
#include <string>
using std::string;

string rgbToHex(int r, int g, int b);
void hexToRgb(const string& hex, int& r, int& g, int& b);
float readPotentiometer(int raw_adc);
