#include "Parser.h"

Parser::Parser(std::string filename) {
	lex = new lexer(filename);
	err = new Error;
	is_error = false;
	max_exp_length = 1000;
	line = 1;
	set_func();
	set_spec_words();
	set_types();
	set_func_map();
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
			get_lex(line);
			continue;
		}
		expression('p');
		if (cur_lex.first != "lexDOTCOMA") {
			is_error = true;
			if (lex->get_is_next_line()) {
				err->Expected(";", line - 1);
			}
			else {
				err->Expected(";", line);
			}
		}
		solve_expression();
		get_lex(line);
	}
}

void Parser::main_prog() {
	get_start();
	compile();
}

void Parser::get_lex(int& line) {
	cur_lex = lex->get_lex(line);
	if (cur_lex.first == "lexQUOTES") {
		cur_lex = lex->get_string(is_error, err, line);
	}
}

void Parser::expression(char sign) {
	if (is_error) {
		return;
	}
	var_dif();
	term(sign);
	while (cur_lex.first == "lexPLUS" || cur_lex.first == "lexMINUS" || cur_lex.first=="lexEQUAL") {
		std::string q = "";
		if (cur_lex.first == "lexEQUAL") {
			get_lex(line);
			expression('p');
			cur_exp.push_back(std::pair<std::string,std::string>("OP","="));
			continue;
		}
		if (cur_lex.first == "lexPLUS" && sign=='p' || cur_lex.first == "lexMINUS" && sign == 'm') {
			q = "+";
		}
		else {
			q = "-";
		}
		get_lex(line);
		term('p');
		cur_exp.push_back(std::pair<std::string, std::string>("OP", q));
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
		get_lex(line);
		mult('p');
		cur_exp.push_back(std::pair<std::string, std::string>("OP", q));
	}
}

void Parser::mult(char sign) {
	if (is_error) {
		return;
	}
	if (cur_lex.first == "lexMINUS") {
		sign = 'm';
		get_lex(line);
	}
	if (types.find(cur_lex.first+"T")!=types.end()) {
		if (sign == 'm' && cur_lex.first == "lexSTRING") {
			is_error = true;
			err->Unexpected("-", line);
		}
		else if (sign == 'm') {
			cur_exp.push_back(std::pair<std::string,std::string>("INT",std::string(1, '-') + cur_lex.second));
		}
		else {
			cur_exp.push_back(std::pair<std::string, std::string>("INT",cur_lex.second));
		}
		get_lex(line);
	}
	else if (cur_lex.first == "lexORB") {
		cur_lex = lex->get_lex(line);
		expression(sign);
		if (cur_lex.first != "lexCRB") {
			is_error = true;
			err->Expected(")", line);
		}
		get_lex(line);
	}
	else if (funcs.find(cur_lex.first) != funcs.end()) {
		func_to_exp(sign);
	}
	else if (vars.find(cur_lex.second) != vars.end()) {
		std::string a = "";
		if (sign == 'm' && vars[cur_lex.second].first == "lexSTRING") {
			is_error = true;
			err->Unexpected("-", line);
		}
		else if (sign == 'm') {
			a = "-" + cur_lex.second;
		}
		else {
			a = cur_lex.second;
		}
		cur_exp.push_back(std::pair<std::string, std::string>("VAR",a));
		get_lex(line);
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
	if (x == 0) {
		out = "0";
	}
	if (x < 0) {
		q += "-";
		x = std::abs(x);
	}
	while (x > 0) {
		out = std::string(1, (char)((int)'0' + x % 10)) + out;
		x /= 10;
	}
	return q+out;
}

void Parser::get_start() {
	get_lex(line);
	if (cur_lex.first != "lexSTART") {
		err->Expected("START", line);
		is_error = true;
	}
	get_lex(line);
	if (cur_lex.first != "lexOFB") {
		err->Expected("{", line);
		is_error = true;
	}
	get_lex(line);
}

void Parser::solve_expression() {
	if (is_error) {
		return;
	}
	std::pair<std::string,std::string>* stack = new std::pair<std::string, std::string>[max_exp_length];
	int ptr = -1;
	for (int i = 0; i < cur_exp.size(); ++i) {
		if (cur_exp[i].first == "OP" || cur_exp[i].first == "FUNC") {
			func_map[cur_exp[i].second](stack, ptr);
		}
		else {
			ptr++;
			stack[ptr] = cur_exp[i];
		}
	}
	cur_exp.clear();
	delete[] stack;
}

void Parser::set_func() {
	const std::map<std::string, std::pair<std::string, int>>* tmp = lex->get_built_in_funcs();
	for (auto i = (*tmp).begin(); i != (*tmp).end(); ++i) {
		funcs.insert(std::pair<std::string, std::pair<std::string, int>>((*i).second.first, std::pair<std::string, int>((*i).first,(*i).second.second)));
	}
}

void Parser::set_spec_words() {
	const std::map<std::string, std::string>* tmp = lex->get_spec_words();
	for (auto i = (*tmp).begin(); i != (*tmp).end(); ++i) {
		spec_words.insert(std::pair< std::string, std::string>((*i).second, (*i).first));
	}
}

void Parser::var_dif() {
	if (types.find(cur_lex.first) == types.end()) {
		return;
	}
	std::string s = cur_lex.first.substr(0, cur_lex.first.size() - 1);
	get_lex(line);
	if (cur_lex.first != "lexNONE") {
		is_error = true;
		if (funcs.find(cur_lex.first) != funcs.end()) {
			err->Unexpected(funcs[cur_lex.first].first, line);
		}
		else if (spec_words.find(cur_lex.first) != spec_words.end()) {
			err->Unexpected(spec_words[cur_lex.first], line);
		}
	}
	vars.insert(std::pair<std::string, pss>(cur_lex.second, pss(s, "")));
}

void Parser::set_types() {
	const std::map<std::string, std::string>* tmp = lex->get_types();
	for (auto i = (*tmp).begin(); i != (*tmp).end(); ++i) {
		types.insert((*i).second);
	}
}

void Parser::set_func_map() {
	func_map["*"] = std::bind(&Parser::multiply, this, std::placeholders::_1, std::placeholders::_2);
	func_map["+"] = std::bind(&Parser::add, this, std::placeholders::_1, std::placeholders::_2);
	func_map["-"] = std::bind(&Parser::subtract, this, std::placeholders::_1, std::placeholders::_2);
	func_map["/"] = std::bind(&Parser::divide, this, std::placeholders::_1, std::placeholders::_2);
	func_map["="] = std::bind(&Parser::equate, this, std::placeholders::_1, std::placeholders::_2);
	func_map["lexPRINT"] = std::bind(&Parser::print, this, std::placeholders::_1, std::placeholders::_2);
	func_map["lexABS"] = std::bind(&Parser::abs, this, std::placeholders::_1, std::placeholders::_2);
}

std::string Parser::val_from_var(std::string var) {
	std::string out = "";
	if (var[0] == '-') {
		var = var.substr(1, var.size() - 1);
		out = "-" + vars[var].second;
	}
	else {
		out = vars[var].second;
	}
	return out;
}

void Parser::func_to_exp(char sign) {
	std::string q = cur_lex.first;
	get_lex(line);
	if (cur_lex.first != "lexORB") {
		is_error = true;
		err->Expected("(", line);
	}
	get_lex(line);
	for (int i = 0; i < funcs[q].second; ++i) {
		expression('p');
		if (cur_lex.first != "lexCOMMA" && i != funcs[q].second-1) {
			is_error = true;
			err->Expected(",", line);
		}
		else if (cur_lex.first == "lexCOMMA" && i == funcs[q].second - 1) {
			is_error = true;
			err->Unexpected(",", line);
		}
		if (i == funcs[q].second - 1 && cur_lex.first!="lexCRB") {
			is_error = true;
			err->Expected(")", line);
		}
		get_lex(line);
	}
	cur_exp.push_back(std::pair<std::string, std::string>("FUNC", q));
	if (sign == 'm') {
		cur_exp.push_back(pss("INT", "-1"));
		cur_exp.push_back(pss("OP", "*"));
	}
}

void Parser::add(pss* stack, int& ptr) {
	std::string a = "";
	std::string b = "";
	if (stack[ptr - 1].first == "VAR") {
		a = val_from_var(stack[ptr - 1].second);
	}
	else {
		a = stack[ptr - 1].second;
	}
	if (stack[ptr].first == "VAR") {
		b = val_from_var(stack[ptr].second);
	}
	else {
		b = stack[ptr].second;
	}
	pss out;
	out.first = "INT";
	out.second = from_int_to_string(from_str_to_int(a) + from_str_to_int(b));
	stack[ptr - 1] = out;
	ptr--;
}

void Parser::subtract(pss* stack, int& ptr) {
	std::string a = "";
	std::string b = "";
	if (stack[ptr - 1].first == "VAR") {
		a = val_from_var(stack[ptr - 1].second);
	}
	else {
		a = stack[ptr - 1].second;
	}
	if (stack[ptr].first == "VAR") {
		b = val_from_var(stack[ptr].second);
	}
	else {
		b = stack[ptr].second;
	}
	pss out;
	out.first = "INT";
	out.second = from_int_to_string(from_str_to_int(a) - from_str_to_int(b));
	stack[ptr - 1] = out;
	ptr--;
}

void Parser::multiply(pss* stack, int& ptr) {
	std::string a = "";
	std::string b = "";
	if (stack[ptr - 1].first == "VAR") {
		a = val_from_var(stack[ptr - 1].second);
	}
	else {
		a = stack[ptr - 1].second;
	}
	if (stack[ptr].first == "VAR") {
		b = val_from_var(stack[ptr].second);
	}
	else {
		b = stack[ptr].second;
	}
	pss out;
	out.first = "INT";
	out.second = from_int_to_string(from_str_to_int(a) * from_str_to_int(b));
	stack[ptr - 1] = out;
	ptr--;
}

void Parser::divide(pss* stack, int& ptr) {
	std::string a = "";
	std::string b = "";
	if (stack[ptr - 1].first == "VAR") {
		a = val_from_var(stack[ptr - 1].second);
	}
	else {
		a = stack[ptr - 1].second;
	}
	if (stack[ptr].first == "VAR") {
		b = val_from_var(stack[ptr].second);
	}
	else {
		b = stack[ptr].second;
	}
	pss out;
	out.first = "INT";
	out.second = from_int_to_string(from_str_to_int(a) / from_str_to_int(b));
	stack[ptr - 1] = out;
	ptr--;
}

void Parser::print(pss* stack, int& ptr) {
	std::string a = "";
	if (stack[ptr].first=="VAR") {
		a = val_from_var(stack[ptr].second);
	}
	else {
		a = stack[ptr].second;
	}
	std::cout << a << std::endl;
	ptr--;
}

void Parser::equate(pss* stack, int& ptr) {
	std::string a = "";
	if (stack[ptr].first == "VAR") {
		a = val_from_var(stack[ptr].second);
	}
	else {
		a = stack[ptr].second;
	}
	vars[stack[ptr - 1].second].second = a;
	ptr -= 2;
}

void Parser::abs(pss* stack, int& ptr) {
	std::string a = "";
	if (stack[ptr].first == "VAR") {
		a = val_from_var(stack[ptr].second);
		stack[ptr].first = "INT";
		stack[ptr].second = from_int_to_string(std::abs(from_str_to_int(a)));
	}
	else {
		a = stack[ptr].second;
		stack[ptr].second = from_int_to_string(std::abs(from_str_to_int(a)));
	}

}