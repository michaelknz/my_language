#include "Error.h"

Error::Error() {
	pos = 0;
	line = 1;
	prev_line = 0;
	prev_pos = 0;
	is_error = false;
}

Error::~Error() {

}

void Error::Expected(std::string s) {
	std::cout << "line " << prev_line << ": " << "Expected " << '"' << s << '"' << std::endl;
	is_error = true;
}

void Error::Unexpected(std::string s) {
	std::cout << "line " << line << ": " << "Unexpected " << '"' << s << '"' << std::endl;
	is_error = true;
}

void Error::ExistName(std::string s) {
	std::cout << "line " << line << ": " << "Existing name " << '"' << s << '"' << std::endl;
	is_error = true;
}

void Error::NextCh() {
	pos++;
}

void Error::NextLine() {
	line++;
	pos = 0;
}

void Error::SetPrev() {
	prev_line = line;
	prev_pos = pos;
}

void Error::Set_Is_Error(bool val) {
	is_error = val;
}

bool Error::Get_Is_Error() {
	return is_error;
}