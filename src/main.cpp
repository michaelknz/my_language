#include "Parser.h"

int main(int argc, char** argv) {
	Parser parse("src/source.txt");
	parse.main_prog();
	return 0;
}