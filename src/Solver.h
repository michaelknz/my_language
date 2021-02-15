#ifndef SOLVER_H
#define SOLVER_H
#include "Function_Class.h"
#include <functional>
#include <vector>
class Solver
{
public:
	Solver();
	~Solver();
	void solve(std::vector<std::pair<std::string, std::string>> exp, std::map<std::string, pss>* vars);
private:
	int max_exp_length;
	Function_Class* funcs;
	std::map<std::string, std::function<void(pss*, int&)>> func_map;
};
#endif
