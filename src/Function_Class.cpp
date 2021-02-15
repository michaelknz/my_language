#include "Function_Class.h"

Function_Class::Function_Class() {
	converter = new Converter;
	set_func_map();
}

Function_Class::~Function_Class() {
	delete converter;
}

std::string Function_Class::val_from_var(std::string var) const{
	std::string out = "";
	if (var[0] == '-') {
		var = var.substr(1, var.size() - 1);
		out = "-" + (*vars)[var].second;
	}
	else {
		out = (*vars)[var].second;
	}
	return out;
}

void Function_Class::set_func_map() {
	func_map["*"] = std::bind(&Function_Class::multiply, this, std::placeholders::_1, std::placeholders::_2);
	func_map["+"] = std::bind(&Function_Class::add, this, std::placeholders::_1, std::placeholders::_2);
	func_map["-"] = std::bind(&Function_Class::subtruct, this, std::placeholders::_1, std::placeholders::_2);
	func_map["/"] = std::bind(&Function_Class::divide, this, std::placeholders::_1, std::placeholders::_2);
	func_map["="] = std::bind(&Function_Class::equate, this, std::placeholders::_1, std::placeholders::_2);
	func_map["lexPRINT"] = std::bind(&Function_Class::print, this, std::placeholders::_1, std::placeholders::_2);
	func_map["lexABS"] = std::bind(&Function_Class::abs, this, std::placeholders::_1, std::placeholders::_2);
}

void Function_Class::Call_Func(std::string ind, pss* stack, int& ptr) {
	func_map[ind](stack, ptr);
}

void Function_Class::SetVars(std::map<std::string, pss>* vars) {
	this->vars = vars;
}

void Function_Class::add(pss* stack, int& ptr) {
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
	out.second = converter->from_int_to_str(converter->from_str_to_int(a) + converter->from_str_to_int(b));
	stack[ptr - 1] = out;
	ptr--;
}

void Function_Class::subtruct(pss* stack, int& ptr) {
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
	out.second = converter->from_int_to_str(converter->from_str_to_int(a) - converter->from_str_to_int(b));
	stack[ptr - 1] = out;
	ptr--;
}

void Function_Class::multiply(pss* stack, int& ptr) {
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
	out.second = converter->from_int_to_str(converter->from_str_to_int(a) * converter->from_str_to_int(b));
	stack[ptr - 1] = out;
	ptr--;
}

void Function_Class::divide(pss* stack, int& ptr) {
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
	out.second = converter->from_int_to_str(converter->from_str_to_int(a) / converter->from_str_to_int(b));
	stack[ptr - 1] = out;
	ptr--;
}

void Function_Class::print(pss* stack, int& ptr) {
	std::string a = "";
	if (stack[ptr].first == "VAR") {
		a = val_from_var(stack[ptr].second);
	}
	else {
		a = stack[ptr].second;
	}
	std::cout << a << std::endl;
	ptr--;
}

void Function_Class::equate(pss* stack, int& ptr) {
	std::string a = "";
	if (stack[ptr].first == "VAR") {
		a = val_from_var(stack[ptr].second);
	}
	else {
		a = stack[ptr].second;
	}
	(*vars)[stack[ptr - 1].second].second = a;
	ptr -= 2;
}

void Function_Class::abs(pss* stack, int& ptr) {
	std::string a = "";
	if (stack[ptr].first == "VAR") {
		a = val_from_var(stack[ptr].second);
		stack[ptr].first = "INT";
		stack[ptr].second = converter->from_int_to_str(std::abs(converter->from_str_to_int(a)));
	}
	else {
		a = stack[ptr].second;
		stack[ptr].second = converter->from_int_to_str(std::abs(converter->from_str_to_int(a)));
	}
}