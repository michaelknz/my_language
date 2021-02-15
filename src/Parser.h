#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include <vector>
#include <set>
#include <functional>
class Parser
{
public:
	Parser(std::string filename);
	~Parser();
	void get_lex(int& line);
	void expression(char sign);
	void term(char sign);
	void mult(char sign);
	void get_start();
	void set_func();
	void set_spec_words();
	void set_types();
	void var_dif();
	std::string val_from_var(std::string var);
	void func_to_exp(char sign);
	int compile_exp();
	std::vector<std::pair<std::string, std::string>> Get_exp();
	std::map<std::string, pss>* Get_vars();
private:
	std::vector<std::pair<std::string,std::string>> cur_exp;
	pss cur_lex;
	lexer* lex;
	Error* err;
	std::map<std::string,std::pair<std::string,int>> funcs;
	std::map<std::string, std::string> spec_words;
	std::map<std::string, pss> vars;
	std::set<std::string> types;
	int line;
	bool is_error;
};
#endif
