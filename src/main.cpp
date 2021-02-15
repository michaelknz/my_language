#include "Parser.h"
#include "Preproc.h"
#include "Manager.h"

int main(int argc, char** argv) {
	Manager man("src/source.txt");
	man.compile();
	return 0;
}
