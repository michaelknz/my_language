#ifndef CONVERTER_H
#define CONVERTER_H
#include <string>
class Converter
{
public:
	Converter();
	~Converter();
	int from_str_to_int(std::string s);
	std::string from_int_to_str(int x);
};
#endif

