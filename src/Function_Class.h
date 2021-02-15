#ifndef FUNCTION_CLASS_H
#define FUNCTION_CLASS_H
#include <functional>
#include "Converter.h"
#include "Defines.h"
class Function_Class
{
public:
	Function_Class();
	~Function_Class();
	void add(pss* stack, int& ptr);
	void subtruct(pss* stack, int& ptr);
	void multiply(pss* stack, int& ptr);
	void divide(pss* stack, int& ptr);
	void equate(pss* stack, int& ptr);
	void abs(pss* stack, int& ptr);
	void print(pss* stack, int& ptr);
	void SetVars(std::map<std::string, pss>* vars);
	void Call_Func(std::string ind, pss* stack, int& ptr);
private:
	std::string val_from_var(std::string var) const;
	void set_func_map();
	std::map<std::string, std::function<void(pss*, int&)>> func_map;
	Converter* converter;
	std::map<std::string, pss>* vars;
};
#endif

