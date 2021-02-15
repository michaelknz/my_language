#ifndef LEXER_H
#define LEXER_H
#include "Error.h"
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include "Defines.h"
class lexer
{
public:
	lexer(std::string file_name);
	~lexer();
	std::string number();
	std::string name();
	pss get_string(bool& is_error, Error* err, int& line);
	pss get_lex(int& line);
	bool is_func(std::string s);
	const std::map<std::string, std::string>* get_spec_words() const;
	const std::map<std::string, std::pair<std::string, int>>* get_built_in_funcs() const;
	const std::map<std::string, std::string>* get_types() const;
	const int get_pos() const;
	const bool get_is_next_line() const;
private:
	std::ifstream file;
	char cur_let;
	std::map<std::string, std::string> spec_words;
	std::map<std::string, std::pair<std::string, int>> built_in_funcs;
	std::map<std::string, std::string> types;
	int pos;
	bool is_next_line;
};
#endif
