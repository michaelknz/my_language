#ifndef ERROR_H
#define ERROR_H
#include <iostream>
#include <string>
class Error
{
public:
	Error();
	~Error();
	void Expected(std::string s);
	void Unexpected(std::string s);
	void ExistName(std::string s);
	void NextCh();
	void NextLine();
	void SetPrev();
	void Set_Is_Error(bool val);
	bool Get_Is_Error();
private:
	int pos;
	int line;
	int prev_pos;
	int prev_line;
	bool is_error;
};
#endif

