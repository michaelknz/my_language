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
	void Set_err(Error* err);
	std::string number();
	std::string name();
	pss get_string();
	pss get_lex();
	bool is_func(std::string s);
	const std::map<std::string, std::string>* get_spec_words() const;
	const std::map<std::string, std::pair<std::string, int>>* get_built_in_funcs() const;
	const std::map<std::string, std::string>* get_types() const;
	void Skip_empty_sym();
private:
	void name_diff(pss& x, std::string s);
	void sym_check(pss& out);
	Error* err;
	std::ifstream file;
	char cur_let;
	std::map<std::string, std::string> spec_words;
	std::map<std::string, std::pair<std::string, int>> built_in_funcs;
	std::map<std::string, std::string> types;
};
#endif
