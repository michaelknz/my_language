#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "Error.h"
#include <vector>
#include <set>
class Parser
{
public:
	Parser(std::string filename);
	~Parser();
	void compile();
	void main_prog();
	void expression(char sign);
	void term(char sign);
	void mult(char sign);
	void solve_expression();
	int from_str_to_int(std::string s);
	std::string from_int_to_string(int x);
	void get_start();
	void set_func();
	void set_spec_words();
	void set_types();
	void var_dif();
private:
	int max_exp_length;
	std::vector<std::string> cur_exp;
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
