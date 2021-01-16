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
	std::string val_from_var(std::string var);
	void set_func_map();
	void add(pss* stack, int& ptr);
	void subtract(pss* stack, int& ptr);
	void multiply(pss* stack, int& ptr);
	void divide(pss* stack, int& ptr);
	void print(pss* stack, int& ptr);
	void equate(pss* stack, int& ptr);
	void abs(pss* stack, int& ptr);
	void func_to_exp(char sign);
private:
	int max_exp_length;
	std::vector<std::pair<std::string,std::string>> cur_exp;
	pss cur_lex;
	lexer* lex;
	Error* err;
	std::map<std::string,std::pair<std::string,int>> funcs;
	std::map<std::string, std::string> spec_words;
	std::map<std::string, pss> vars;
	std::map<std::string, std::function<void(pss*, int&)>> func_map;
	std::set<std::string> types;
	int line;
	bool is_error;
};
#endif
