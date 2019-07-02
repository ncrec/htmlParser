#pragma once
#include <string>
using namespace std;
class Parser
{
public:
	string separator(string);
	string href(string);
	string whitelist(string);
	string url_to_path(string);
	string delete_tag(string, string);
	Parser();
	~Parser();
};

