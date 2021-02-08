#ifndef PREPROC_H
#define PREPROC_H 

#include<iostream>
#include<fstream>
#include<string>

class Preproc{
public:
  Preproc();
  ~Preproc();
  void Start(std::string filename);
  void Finish(std::string filename);
};

#endif
