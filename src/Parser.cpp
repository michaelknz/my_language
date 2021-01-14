#include "Parser.h"

Parser::Parser(std::string filename) {
	lex = new lexer(filename);
	err = new Error;
	is_error = false;
	max_exp_length = 1000;
	line = 1;
	set_func();
	set_spec_words();
}

Parser::~Parser() {
	delete lex;
	delete err;
}

void Parser::compile() {
	while (cur_lex.first != "lexEOF") {
		if (is_error) {
			return;
		}
		if (cur_lex.first == "lexCFB") {
			cur_lex = lex->get_lex(line);
			continue;
		}
		expression('p');
		if (cur_lex.first != "lexDOTCOMA") {
			is_error = true;
			err->Expected(";", line);
		}
		solve_expression();
		cur_lex = lex->get_lex(line);
	}
}

void Parser::main_prog() {
	get_start();
	compile();
}

void Parser::expression(char sign) {
	if (is_error) {
		return;
	}
	term(sign);
	while (cur_lex.first == "lexPLUS" || cur_lex.first == "lexMINUS") {
		std::string q = "";
		if (cur_lex.first == "lexPLUS" && sign=='p' || cur_lex.first == "lexMINUS" && sign == 'm') {
			q = "+";
		}
		else {
			q = "-";
		}
		cur_lex = lex->get_lex(line);
		term('p');
		cur_exp.push_back(q);
	}
}

void Parser::term(char sign) {
	if (is_error) {
		return;
	}
	mult(sign);
	while (cur_lex.first == "lexMULT" || cur_lex.first == "lexDIV") {
		std::string q = "";
		if (cur_lex.first == "lexMULT") {
			q = "*";
		}
		else {
			q = "/";
		}
		cur_lex = lex->get_lex(line);
		mult('p');
		cur_exp.push_back(q);
	}
}

void Parser::mult(char sign) {
	if (is_error) {
		return;
	}
	if (cur_lex.first == "lexMINUS") {
		sign = 'm';
		cur_lex = lex->get_lex(line);
	}
	if (cur_lex.first == "lexINT") {
		if (sign == 'm') {
			cur_exp.push_back(std::string(1, '-') + cur_lex.second);
		}
		else {
			cur_exp.push_back(cur_lex.second);
		}
		cur_lex = lex->get_lex(line);
	}
	else if (cur_lex.first == "lexORB") {
		cur_lex = lex->get_lex(line);
		expression(sign);
		cur_lex = lex->get_lex(line);
	}
	else if (funcs.find(cur_lex.first) != funcs.end()) {
		std::string q = cur_lex.first;
		cur_lex = lex->get_lex(line);
		cur_lex = lex->get_lex(line);
		expression(sign);
		cur_exp.push_back(q);
		cur_lex = lex->get_lex(line);
	}
	else {
		is_error = true;
		if (cur_lex.first == "lexNONE") {
			err->Unexpected(cur_lex.second, line);
		}
		else if(spec_words.find(cur_lex.first)!=spec_words.end()) {
			err->Unexpected(spec_words[cur_lex.first], line);
		}
	}
}

int Parser::from_str_to_int(std::string s) {
	int out = 0;
	int mul = 1;
	for (int i = 0; i < s.size(); ++i) {
		if (s[i] == '-') {
			mul = -1;
			continue;
		}
		out *= 10;
		out += (int)s[i] - (int)'0';
	}
	return mul * out;
}

std::string Parser::from_int_to_string(int x) {
	std::string out = "";
	std::string q = "";
	if (x < 0) {
		q += "-";
		x = abs(x);
	}
	while (x > 0) {
		out = std::string(1, (char)((int)'0' + x % 10)) + out;
		x /= 10;
	}
	return q+out;
}

void Parser::get_start() {
	cur_lex = lex->get_lex(line);
	if (cur_lex.first != "lexSTART") {
		err->Expected("START", line);
		is_error = true;
	}
	cur_lex = lex->get_lex(line);
	if (cur_lex.first != "lexOFB") {
		err->Expected("{", line);
		is_error = true;
	}
	cur_lex = lex->get_lex(line);
}

void Parser::solve_expression() {
	if (is_error) {
		return;
	}
	std::string* stack = new std::string[max_exp_length];
	int ptr = -1;
	for (int i = 0; i < cur_exp.size(); ++i) {
		if ((int)cur_exp[i][cur_exp[i].size()-1] >= (int)'0' && (int)cur_exp[i][cur_exp[i].size()-1] <= '9') {
			ptr++;
			stack[ptr] = cur_exp[i];
		}
		else if (cur_exp[i] == "+") {
			stack[ptr - 1] = from_int_to_string(from_str_to_int(stack[ptr]) + from_str_to_int(stack[ptr - 1]));
			ptr--;
		}
		else if (cur_exp[i] == "-") {
			stack[ptr - 1] = from_int_to_string(from_str_to_int(stack[ptr - 1]) - from_str_to_int(stack[ptr]));
			ptr--;
		}
		else if (cur_exp[i] == "*") {
			stack[ptr - 1] = from_int_to_string(from_str_to_int(stack[ptr - 1]) * from_str_to_int(stack[ptr]));
			ptr--;
		}
		else if (cur_exp[i] == "/") {
			stack[ptr - 1] = from_int_to_string(from_str_to_int(stack[ptr - 1]) / from_str_to_int(stack[ptr]));
			ptr--;
		}
		else if (cur_exp[i] == "lexPRINT") {
			std::cout << stack[ptr] << std::endl;
			ptr--;
		}
	}
	cur_exp.clear();
	delete[] stack;
}

void Parser::set_func() {
	const std::map<std::string, std::pair<std::string, int>>* tmp = lex->get_built_in_funcs();
	for (auto i = (*tmp).begin(); i != (*tmp).end(); ++i) {
		funcs.insert((*i).second.first);
	}
}

void Parser::set_spec_words() {
	const std::map<std::string, std::string>* tmp = lex->get_spec_words();
	for (auto i = (*tmp).begin(); i != (*tmp).end(); ++i) {
		spec_words.insert(std::pair< std::string, std::string>((*i).second, (*i).first));
	}
}