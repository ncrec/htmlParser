#include "pch.h"
#include <string>
#include "Parser.h"
#include <iostream>
#include <unicode/unistr.h>
#pragma comment(lib,"icuuc.lib")
using namespace icu;

Parser::Parser()
{
}


Parser::~Parser()
{
}
pair<int, bool> compare(int min, int tagIndex)
{
	bool changed = false;
	if (min == -1)
	{
		if (tagIndex != -1)
		{
			min = tagIndex;
			changed = true;
		}
	}
	else
	{
		if (tagIndex != -1 && min > tagIndex)
		{
			min = tagIndex;
			changed = true;
		}
	}
	return make_pair(min, changed);
}
string Parser::url_to_path(string Original)
{
	
	string httpString = "http://";
	string httpsString = "https://";
	size_t  http = Original.find(httpString);
	size_t  https = Original.find(httpsString);
	if (https != string::npos)
	{
		
		Original = Original.replace(https, httpsString.length(), "");
	}
	
	if (http != string::npos)
	{
		Original=Original.replace(http, httpString.length(), "");
	}

	return Original + '/';
	
}
string Parser::delete_tag(string Original,string tag)
{
	string output = "";
	int index = -1;
	icu::UnicodeString ramp(icu::UnicodeString::fromUTF8(Original));
	while (ramp.indexOf(tag.c_str()) != -1)
	{
		ramp.replace(ramp.indexOf(tag.c_str()), tag.length(), "");
	}
	return ramp.toUTF8String(output);
}
string Parser::separator(string Original)
{
	string output = "";
	icu::UnicodeString ramp(icu::UnicodeString::fromUTF8(Original));

	int index = 0;
	UnicodeString space = "";
	UnicodeString substr = "";
	int newline = 0;
	int newindex = 0;

	while (index + 80 < ramp.length())
	{
		while (newline != -1)
		{
			substr = ramp.tempSubString(index, 80);
			newline = substr.indexOf('\n');

			if (newline != -1)
			{
				index += newline + 1;
			}

		}
		newline = 0;
		index += 80;
		space = ramp[index];
		if (space == " ")
		{
			ramp.replace(index, 1, '\n');

		}
		else {
			newindex = index;
			while (space != " "&& newindex > index - 80)
			{
				space = ramp[newindex--];
			}

			if (newindex > index - 80)
			{
				ramp.replace(newindex + 1, 1, '\n');
				index = newindex + 1;
			}
			else
			{
				while (space != " " &&index < ramp.length())
				{
					space = ramp[index++];//если в предыдущих 80(например, длинный url) символах нет пробела, то искать среди следующих
				}
				ramp.replace(index - 1, 1, '\n');
			}
		}
	}
	return ramp.toUTF8String(output);
}
string Parser::href(string Original)
{
	int startErase = 0;
	int stopErase = 0;
	string output = "";
	icu::UnicodeString ramp(icu::UnicodeString::fromUTF8(Original));
	while (ramp.indexOf("<a href=") != -1)
	{
		int i = ramp.indexOf("<a href=");
		ramp.replace(i, 9, "[");
		i += 9;
		while (ramp[i] != '"')
		{
			i++;
		}
		startErase = i;
		stopErase = ramp.indexOf("</a>", i);
		ramp.replace(startErase, stopErase - startErase + 4, "]");
	}
	return ramp.toUTF8String(output);
}
string Parser::whitelist(string Original)
{
	string output = "";
	icu::UnicodeString ramp(icu::UnicodeString::fromUTF8(Original));
	icu::UnicodeString rampout(icu::UnicodeString::fromUTF8(output));
	int start = 0;
	int startOfP = 0;
	int startOfPLong = 0;
	int startOfh1 = 0;
	int startOfh1Long = 0;
	int startOfh2 = 0;
	int startOfh2Long = 0;
	int stop = 0;
	int min = 0;
	int minIndex = 0;
	int nextIndex = 0;
	pair<int, bool> myPair = make_pair(0, false);
	while (min != -1)

	{
		min = -1;
		minIndex = -1;
		startOfP = ramp.indexOf("<p>", nextIndex);
		myPair = compare(min, startOfP);
		if (myPair.second == true)
		{
			minIndex = 0;
			min = myPair.first;
			min += 3;
		}

		startOfPLong = ramp.indexOf("<p ", nextIndex);
		myPair = compare(min, startOfPLong);


		if (myPair.second == true)
		{
			min = myPair.first;
			minIndex = 1;
			min += 3;
		}

		startOfh1 = ramp.indexOf("<h1>", nextIndex);
		myPair = compare(min, startOfh1);


		if (myPair.second == true)
		{
			min = myPair.first;
			minIndex = 2;
			min += 4;
		}
		startOfh1Long = ramp.indexOf("<h1 ", nextIndex);
		myPair = compare(min, startOfh1Long);
		min = myPair.first;

		if (myPair.second == true)
		{
			minIndex = 3;
			min += 4;
		}
		startOfh2 = ramp.indexOf("<h2>", nextIndex);

		myPair = compare(min, startOfh2);
		min = myPair.first;

		if (myPair.second == true)
		{

			minIndex = 4;
			min += 4;

		}
		nextIndex = min;
		start = min;
		if (minIndex == 0)
		{
			stop = ramp.indexOf("</p>", nextIndex);
			nextIndex = stop;
			rampout += ramp.tempSubString(start, stop - start) + '\n';
		}
		else if (minIndex == 1)
		{
			stop = ramp.indexOf("</p>", nextIndex);
			nextIndex = stop;
			startOfPLong = ramp.indexOf(">", startOfPLong) + 1;
			rampout += ramp.tempSubString(startOfPLong, stop - startOfPLong) + '\n';
		}
		else if (minIndex == 2)
		{
			stop = ramp.indexOf("</h1>", nextIndex);
			nextIndex = stop;
			rampout += ramp.tempSubString(start, stop - start) + '\n';
		}
		else if (minIndex == 3)
		{
			stop = ramp.indexOf("</h1>", nextIndex);
			nextIndex = stop;
			startOfh1Long = ramp.indexOf(">", startOfh1Long) + 1;
			rampout += ramp.tempSubString(startOfh1Long, stop - startOfh1Long) + '\n';
		}
		else if (minIndex == 4)
		{
			nextIndex = stop;
			stop = ramp.indexOf("</h2>", nextIndex);
			rampout += ramp.tempSubString(start, stop - start) + '\n';
		}
	}
	return rampout.toUTF8String(output);
}
