#pragma once
#include<string>
using namespace std;
class Encoding
{
	
public:
	string cp1251_to_utf8(const char *);//https://ru.stackoverflow.com/questions/783946/��������������-�-���������-utf8
	bool is_valid_utf8(const char *); //https://ru.stackoverflow.com/questions/783946/��������������-�-���������-utf8
	Encoding();
	~Encoding();
};

