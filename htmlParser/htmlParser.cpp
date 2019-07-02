
#define CURL_STATICLIB
#define U_STATIC_IMPLEMENTATION

#include "pch.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <curl/curl.h>
#include <windows.h>
#include "Parser.h"
#include "Encoding.h"
#include <filesystem>
#include <fstream>

#pragma comment(lib,"libcurl.lib")
using namespace std; 
namespace fs = std::experimental::filesystem;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

int main(int argc, char* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	list<string> blacklist;
	Encoding encoding;
	Parser parser;
	CURL *curl;
	CURLcode res;
	string readBuffer;
	curl = curl_easy_init();
	string url = argv[1];
	cout << url << endl;
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (iPhone; CPU iPhone OS 10_3_2 like Mac OS X) AppleWebKit/603.2.4 (KHTML, like Gecko) FxiOS/7.5b3349 Mobile/14F89 Safari/603.2.4");
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		string path = parser.url_to_path(url);
		fs::create_directories(path);

		//если кодировка 1251
		const char * c = readBuffer.c_str();
		if(!encoding.is_valid_utf8(c))
			readBuffer = encoding.cp1251_to_utf8(c);
	  
		readBuffer = parser.whitelist(readBuffer);
		//удаление лишних тегов
		readBuffer = parser.delete_tag(readBuffer, "&mdash");
		readBuffer = parser.delete_tag(readBuffer, "<br>");
	
		readBuffer = parser.href(readBuffer);
		readBuffer = parser.separator(readBuffer);
		path += "text.txt";
		FILE *f = fopen(path.c_str(), "wb");
		fprintf(f, "%s", readBuffer.c_str());
		fclose(f);
	}
	return 0;
}



