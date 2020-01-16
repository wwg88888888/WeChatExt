#pragma once
#include "stdafx.h" 
#include <string>
#include <vector>
#include <list>

using namespace std;

class EVString
{
public:
	EVString();
	~EVString();


private:
	static unsigned char ToHex(unsigned char x);
	static unsigned char FromHex(unsigned char x);

public:
	static char* w2a(const wchar_t* lpszSrc, UINT CodePage = CP_UTF8);
	static wchar_t* a2w(const char* lpszSrc, UINT   CodePage = CP_UTF8);

	static string w2a_string(const wstring& wStr, UINT CodePage = CP_UTF8);
	static wstring a2w_string(const string& aStr, UINT   CodePage = CP_UTF8);

	static wstring formatW(LPCWSTR strFormat, ...);
	static DWORD wstring_count(const wstring& wStr, const wstring& wStrFind);
	static wstring wstring_replace_once(const wstring& orignStr, const wstring& oldStr, const wstring& newStr);


	static bool split(const std::wstring& content, const std::wstring& delim, std::list<std::wstring>& elems);
	static bool split(const std::wstring& content, const std::wstring& delim, std::vector<std::wstring>& elems);


	static bool starts_with(const wstring& content, const wstring& delim);
	static bool ends_with(const wstring& content, const wstring& delim);

	static bool xor_string(string& strContent, const string& strKey);

	static std::string Unicode2AnsiCode(const std::wstring& str);

	static std::string UrlEncode(const std::string& str);

	static std::string UrlDecode(const std::string& str);

	static std::string GetUrlParam(const std::string& sUrl, const std::string& sKey);
};
