#ifndef ERROR_H
#define ERROR_H
#include <iostream>
#include <string>
class Error
{
public:
	Error();
	~Error();
	void Expected(std::string s, int line);
	void Unexpected(std::string s, int line);
	void ExistName(std::string s, int line);
};
#endif

