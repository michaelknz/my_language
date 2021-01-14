#include "Error.h"

Error::Error() {

}

Error::~Error() {

}

void Error::Expected(std::string s, int line) {
	std::cout << "line " << line << ": " << "Expected " << '"' << s << '"' << std::endl;
}

void Error::Unexpected(std::string s, int line) {
	std::cout << "line " << line << ": " << "Unexpected " << '"' << s << '"' << std::endl;
}