#include "Solver.h"


Solver::Solver() {
	funcs = new Function_Class;
	max_exp_length = 1000;
}

Solver::~Solver() {
	delete funcs;
}

void Solver::solve(std::vector<std::pair<std::string, std::string>> exp, std::map<std::string, pss>* vars) {
	funcs->SetVars(vars);
	std::pair<std::string, std::string>* stack = new std::pair<std::string, std::string>[max_exp_length];
	
	int ptr = -1;
	for (int i = 0; i < exp.size(); ++i) {
		if (exp[i].first == "OP" || exp[i].first == "FUNC") {
			funcs->Call_Func(exp[i].second,stack, ptr);
		}
		else {
			ptr++;
			stack[ptr] = exp[i];
		}
	}
	exp.clear();
	delete[] stack;
}