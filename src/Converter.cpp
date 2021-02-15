#include "Converter.h"

Converter::Converter() {

}

Converter::~Converter() {

}

std::string Converter::from_int_to_str(int x) {
	std::string out = "";
	std::string q = "";
	if (x == 0) {
		out = "0";
	}
	if (x < 0) {
		q += "-";
		x = std::abs(x);
	}
	while (x > 0) {
		out = std::string(1, (char)((int)'0' + x % 10)) + out;
		x /= 10;
	}
	return q + out;
}

int Converter::from_str_to_int(std::string s) {
	int out = 0;
	int mul = 1;
	for (int i = 0; i < s.size(); ++i) {
		if (s[i] == '-') {
			mul = -1;
			continue;
		}
		out *= 10;
		out += (int)s[i] - (int)'0';
	}
	return mul * out;
}