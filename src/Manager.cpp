#include "Manager.h"

Manager::Manager(std::string filename) {
	error = new Error;
	preproc = new Preproc;
	parser = new Parser(filename);
	solver = new Solver;
	this->filename = filename;
}

Manager::~Manager() {
	delete preproc;
	delete parser;
	delete solver;
}

void Manager::compile() {
	preproc->Start(filename);
	parser->get_start(error);
	int tmp = 0;
	tmp = parser->compile_exp();
	while (tmp == 0) {
		solver->solve(parser->Get_exp(), parser->Get_vars());
		tmp = parser->compile_exp();
	}
	preproc->Finish(filename);
}