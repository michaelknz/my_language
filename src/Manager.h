#ifndef MANAGER_H
#define MANAGER_H
#include <fstream>
#include "Preproc.h"
#include "Parser.h"
#include "Solver.h"
class Manager
{
public:
	Manager(std::string filename);
	~Manager();
	void compile();
private:
	Preproc* preproc;
	Parser* parser;
	Solver* solver;
	std::ifstream file;
	std::string filename;
};
#endif

