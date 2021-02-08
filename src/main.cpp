#include "Parser.h"
#include "Preproc.h"

int main(int argc, char** argv) {
  Preproc prep;
  prep.Start("src/source.txt");
	Parser parse("src/source.txt");
	parse.main_prog();
  prep.Finish("src/source.txt");
	return 0;
}
