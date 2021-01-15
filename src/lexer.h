#ifndef LEXER_H
#define LEXER_H
#include <iostream>
#include <string>
#include <fstream>
#include <map>
typedef std::pair<std::string, std::string> pss;
class lexer
{
public:
	lexer(std::string file_name);
	~lexer();
	std::string number();
	std::string name();
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
