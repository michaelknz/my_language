#include"Preproc.h"
#include <fstream>
#include <ostream>
#include <string>

Preproc::Preproc(){}

Preproc::~Preproc(){}

void Preproc::Start(std::string filename){
  std::ofstream file;
  file.open(filename,std::ios_base::app);
  file << "EOF";
  file.close();
}

void Preproc::Finish(std::string filename){
  std::ifstream file(filename.c_str());
  std::string tmp;
  std::string line;
  while(file.good()){
   getline(file,line);
   tmp.append(line+'\n');
  }
  while(tmp[tmp.size() - 1]!='F'){
    tmp.pop_back();
  }
  tmp.pop_back();
  tmp.pop_back();
  tmp.pop_back();
  file.close();
  std::ofstream file1;
  file1.open(filename.c_str());
  file1.clear();
  file1 << tmp;
  file1.close();
}
