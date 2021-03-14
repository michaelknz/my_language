#include "lexer.h"
#include <ostream>

lexer::lexer(std::string file_name) {
	file.open(file_name.c_str());
	file.get(cur_let);
	spec_words = { {"START","lexSTART"} };
	built_in_funcs = { {"print",std::pair<std::string,int>("lexPRINT",1)}, {"abs",std::pair<std::string,int>("lexABS",1)} };
	types = { {"int","lexINTT"}, {"string", "lexSTRINGT"} };
}

lexer::~lexer() {
	file.close();
}

std::string lexer::number() {
	std::string out(1,cur_let);
	file.get(cur_let);
	err->NextCh();
	while ((int)cur_let >= '0' && (int)cur_let <= '9' && !file.eof()) {
		out += std::string(1, cur_let);
		file.get(cur_let);
		err->NextCh();
	}
	return out;
}

std::string lexer::name() {
	std::string out(1, cur_let);
	file.get(cur_let);
	err->NextCh();
	while (((int)cur_let >= (int)'a' && (int)cur_let <= (int)'z' || (int)cur_let >= (int)'A' && (int)cur_let <= (int)'Z' 
		|| (int)cur_let == (int)'_') && !file.eof()) {
		out += std::string(1, cur_let);
		file.get(cur_let);
		err->NextCh();
	}
	return out;
}

pss lexer::get_lex() {
	err->SetPrev();
	Skip_empty_sym();
	pss out;
	out.first = "";
	out.second = "";
	sym_check(out);
	if (out.first != "") {
		return out;
	}
	if ((int)cur_let >= (int)'a' && (int)cur_let <= (int)'z' || (int)cur_let >= (int)'A' && (int)cur_let <= (int)'Z'
		|| (int)cur_let == (int)'_') {
		std::string s = name();
		name_diff(out, s);
	}
	else if ((int)cur_let >= (int)'0' && (int)cur_let <= (int)'9') {
		std::string s = number();
		out.first = "lexINT";
		out.second = s;
	}
	return out;
}

pss lexer::get_string() {
	pss out;
	file.get(cur_let);
	out.first = "lexSTRING";
	while (cur_let!='"') {
		if (cur_let == '\n') {
			err->Expected("\"");
			err->NextLine();
			file.get(cur_let);
			return out;
		}
		out.second += std::string(1,cur_let);
		file.get(cur_let);
	}
	if (cur_let == '"') {
		file.get(cur_let);
	}
	return out;
}

bool lexer::is_func(std::string s) {
	if (spec_words.find(s) == spec_words.end())
		return false;
	return true;
}

const std::map<std::string, std::string>* lexer::get_spec_words() const {
	return &spec_words;
}

const std::map<std::string, std::pair<std::string, int>>* lexer::get_built_in_funcs() const {
	return &built_in_funcs;
}

const std::map<std::string, std::string>* lexer::get_types() const {
	return &types;
}

void lexer::Set_err(Error* err) {
	this->err = err;
}

void lexer::Skip_empty_sym() {
	while ((int)cur_let == (int)' ' || (int)cur_let == (int)'\n' || (int)cur_let == (int)'\t') {
		if (cur_let == '\n') {
			err->NextLine();
		}
		file.get(cur_let);
		err->NextCh();
	}
}

void lexer::name_diff(pss& x, std::string s) {
	if (spec_words.find(s) != spec_words.end()) {
		x.first = spec_words[s];
		x.second = "";
	}
	else if (built_in_funcs.find(s) != built_in_funcs.end()) {
		x.first = built_in_funcs[s].first;
		x.second = std::string(1, char((int)'0' + built_in_funcs[s].second));
	}
	else if (types.find(s) != types.end()) {
		x.first = types[s];
		x.second = "";
	}
	else if (s == "EOF") {
		x.first = "lexEOF";
		x.second = "";
	}
	else {
		x.first = "lexNONE";
		x.second = s;
	}
}

void lexer::sym_check(pss& out) {
	switch (cur_let) {
	case ';':
		out.first = "lexDOTCOMA";
		out.second = "";
		file.get(cur_let);
		err->NextCh();
		break;
	case '=':
		file.get(cur_let);
		err->NextCh();
		if (cur_let == '=') {
			out.first = "lexIS_EQUAL";
			out.second = "";
			file.get(cur_let);
			err->NextCh();
		}
		else {
			out.first = "lexEQUAL";
			out.second = "";
		}
		break;
	case '<':
		file.get(cur_let);
		err->NextCh();
		if (cur_let == '=') {
			out.first = "lexIS_EOL";
			out.second = "";
			file.get(cur_let);
			err->NextCh();
		}
		else {
			out.first = "lexLESS";
			out.second = "";
		}
		break;
	case '>':
		file.get(cur_let);
		err->NextCh();
		if (cur_let == '=') {
			out.first = "lexIS_EOM";
			out.second = "";
			file.get(cur_let);
			err->NextCh();
		}
		else {
			out.first = "lexMORE";
			out.second = "";
		}
		break;
	case '(':
		out.first = "lexORB";
		out.second = "";
		file.get(cur_let);
		err->NextCh();
		break;
	case ')':
		out.first = "lexCRB";
		out.second = "";
		file.get(cur_let);
		err->NextCh();
		break;
	case '{':
		out.first = "lexOFB";
		out.second = "";
		file.get(cur_let);
		err->NextCh();
		break;
	case '}':
		out.first = "lexCFB";
		out.second = "";
		file.get(cur_let);
		err->NextCh();
		break;
	case '+':
		out.first = "lexPLUS";
		out.second = "";
		file.get(cur_let);
		err->NextCh();
		break;
	case '-':
		out.first = "lexMINUS";
		out.second = "";
		file.get(cur_let);
		err->NextCh();
		break;
	case '*':
		out.first = "lexMULT";
		out.second = "";
		file.get(cur_let);
		err->NextCh();
		break;
	case '/':
		out.first = "lexDIV";
		out.second = "";
		file.get(cur_let);
		err->NextCh();
		break;
	case ',':
		out.first = "lexCOMMA";
		out.second = "";
		file.get(cur_let);
		err->NextCh();
		break;
	case '"':
		out.first = "lexQUOTES";
		out.second = "";
		err->NextCh();
		break;
	default:
		break;
	}
}