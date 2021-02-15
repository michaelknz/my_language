#include "Parser.h"

Parser::Parser(std::string filename) {
	lex = new lexer(filename);
	err = new Error;
	is_error = false;
	line = 1;
	set_func();
	set_spec_words();
	set_types();
}

Parser::~Parser() {
	delete lex;
	delete err;
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


int Parser::compile_exp() {
	cur_exp.clear();
	if (cur_lex.first == "lexCFB") {
		get_lex(line);
		return 2;
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
	get_lex(line);
	return 0;
}

std::vector<std::pair<std::string, std::string>> Parser::Get_exp() {
	return cur_exp;
}

std::map<std::string, pss>* Parser::Get_vars() {
	return &vars;
}
