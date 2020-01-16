#include "stdafx.h"
#include "EVString.h"
#include <regex>
#include <string>


typedef union _uacode
{
	struct {
		BYTE LowByte;
		BYTE HighByte;
	}DUMMYSTRUCTNAME;
	struct {
		BYTE LowByte;
		BYTE HighByte;
	} u;
	wchar_t ch;
}UACODE;


EVString::EVString()
{
}


EVString::~EVString()
{
}

char* EVString::w2a(const wchar_t* lpszSrc, UINT CodePage)
{
	if (lpszSrc != NULL)
	{
		int  nANSILen = WideCharToMultiByte(CodePage, 0, lpszSrc, -1, NULL, 0, NULL, NULL);
		char* pANSI = new char[nANSILen + 1];
		if (pANSI != NULL)
		{
			ZeroMemory(pANSI, nANSILen + 1);
			WideCharToMultiByte(CodePage, 0, lpszSrc, -1, pANSI, nANSILen, NULL, NULL);
			return pANSI;
		}
	}
	return NULL;
}

wchar_t* EVString::a2w(const char* lpszSrc, UINT CodePage)
{
	if (lpszSrc != NULL)
	{
		int nUnicodeLen = MultiByteToWideChar(CodePage, 0, lpszSrc, -1, NULL, 0);
		LPWSTR pUnicode = new WCHAR[nUnicodeLen + 1];
		if (pUnicode != NULL)
		{
			ZeroMemory((void*)pUnicode, (nUnicodeLen + 1) * sizeof(WCHAR));
			MultiByteToWideChar(CodePage, 0, lpszSrc, -1, pUnicode, nUnicodeLen);
			return pUnicode;
		}
	}
	return NULL;
}


string EVString::w2a_string(const wstring& wStr, UINT CodePage)
{
	string strRet = "";

	if (wStr.size() > 0)
	{
		int  nANSILen = WideCharToMultiByte(CodePage, 0, wStr.c_str(), -1, NULL, 0, NULL, NULL);
		char* pANSI = new char[nANSILen + 1];
		if (pANSI != NULL)
		{
			ZeroMemory(pANSI, nANSILen + 1);
			WideCharToMultiByte(CodePage, 0, wStr.c_str(), -1, pANSI, nANSILen, NULL, NULL);
			strRet = pANSI;
			delete[] pANSI;
		}
	}
	return strRet;
}

wstring EVString::a2w_string(const string& aStr, UINT CodePage)
{
	wstring wstrRet = L"";

	if (aStr.size() > 0)
	{
		int nUnicodeLen = MultiByteToWideChar(CodePage, 0, aStr.c_str(), -1, NULL, 0);
		LPWSTR pUnicode = new WCHAR[nUnicodeLen + 1];
		if (pUnicode != NULL)
		{
			ZeroMemory((void*)pUnicode, (nUnicodeLen + 1) * sizeof(WCHAR));
			MultiByteToWideChar(CodePage, 0, aStr.c_str(), -1, pUnicode, nUnicodeLen);

			wstrRet = pUnicode;
			delete[] pUnicode;
		}
	}
	return wstrRet;
}


wstring EVString::formatW(LPCWSTR strFormat, ...)
{
	WCHAR szBuffer[4096];
	va_list pArgList;

	va_start(pArgList, strFormat);

	_vsnwprintf_s(szBuffer, sizeof(szBuffer) / sizeof(WCHAR), strFormat, pArgList);

	va_end(pArgList);

	return szBuffer;
}

DWORD EVString::wstring_count(const wstring& wStr, const wstring& wStrFind)
{
	int position = 0;
	int i = 0;
	while ((position = wStr.find_first_of(wStrFind, position)) != wstring::npos)
	{
		position += wStrFind.size();
		i++;
	}
	return i;
}

wstring EVString::wstring_replace_once(const wstring& orignStr, const wstring& oldStr, const wstring& newStr)
{
	size_t pos = 0;
	wstring tempStr = orignStr;
	wstring::size_type oldStrLen = oldStr.length();
	pos = tempStr.find(oldStr, pos);
	if (pos != wstring::npos)
	{
		tempStr.replace(pos, oldStrLen, newStr);
	}
	return tempStr;
}

bool EVString::split(const std::wstring& content, const std::wstring& delim, std::vector<std::wstring>& elems)
{
	elems.clear();

	std::wstring::size_type pos1, pos2;
	size_t len = content.length();
	pos2 = content.find(delim);
	pos1 = 0;
	while (std::wstring::npos != pos2)
	{
		elems.emplace_back(content.substr(pos1, pos2 - pos1));

		pos1 = pos2 + delim.size();
		pos2 = content.find(delim, pos1);
	}
	if (pos1 != len)
		elems.emplace_back(content.substr(pos1));

	return !elems.empty();
}

bool EVString::split(const std::wstring& content, const std::wstring& delim, std::list<std::wstring>& elems)
{
	elems.clear();

	std::wstring::size_type pos1, pos2;
	size_t len = content.length();
	pos2 = content.find(delim);
	pos1 = 0;
	while (std::wstring::npos != pos2)
	{
		elems.emplace_back(content.substr(pos1, pos2 - pos1));

		pos1 = pos2 + delim.size();
		pos2 = content.find(delim, pos1);
	}
	if (pos1 != len)
		elems.emplace_back(content.substr(pos1));

	return !elems.empty();
}

bool EVString::starts_with(const wstring& content, const wstring& delim)
{
	return content.find_first_of(delim) == 0;
}
bool EVString::ends_with(const wstring& content, const wstring& delim)
{
	return content.find_last_of(delim) == (content.length() - delim.length());
}


bool EVString::xor_string(string& strContent, const string& strKey)
{
	if (strContent.empty() || strKey.empty()) return false;

	for (unsigned int i = 0, j = 0; i < strContent.size(); i++)
		strContent[i] = strContent[i] ^ strKey[j == strKey.size() ? 0 : j++];

	return true;
}

std::string EVString::Unicode2AnsiCode(const std::wstring& str)
{
	const wchar_t* p = str.c_str();
	UACODE ua;
	string sResult(str.length() * 6 + 1, '\0');
	char* pa = &sResult[0];
	for (const wchar_t it : str)
	{
		ua.ch = it;
		if (ua.HighByte)
		{
			sprintf_s(pa, 7, R"(\u%02x%02x)", ua.HighByte, ua.LowByte);
			pa += 6;
		}
		else {
			*pa = ua.LowByte;
			pa++;
		}

	}
	return sResult.c_str();
}

unsigned char EVString::ToHex(unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}

unsigned char EVString::FromHex(unsigned char x)
{
	unsigned char y;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else if (x >= '0' && x <= '9') y = x - '0';
	else y = '\0';
	return y;
}

std::string EVString::UrlEncode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (isalnum((unsigned char)str[i]) ||
			(str[i] == '-') ||
			(str[i] == '_') ||
			(str[i] == '.') ||
			(str[i] == '~'))
			strTemp += str[i];
		else if (str[i] == ' ')
			strTemp += "+";
		else
		{
			strTemp += '%';
			strTemp += ToHex((unsigned char)str[i] >> 4);
			strTemp += ToHex((unsigned char)str[i] % 16);
		}
	}
	return strTemp;
}

std::string EVString::UrlDecode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (str[i] == '+') strTemp += ' ';
		else if (str[i] == '%')
		{
			//assert(i + 2 < length);
			unsigned char high = FromHex((unsigned char)str[++i]);
			unsigned char low = FromHex((unsigned char)str[++i]);
			strTemp += high * 16 + low;
		}
		else strTemp += str[i];
	}
	return strTemp;
}

std::string EVString::GetUrlParam(const std::string& sUrl, const std::string& sKey)
{
	smatch result;
	if (regex_search(sUrl.cbegin(), sUrl.cend(), result, regex(sKey + "=(.*?)&"))) {
		// 匹配具有多个参数的url

		// *? 重复任意次，但尽可能少重复  
		return result[1];
	}
	else if (regex_search(sUrl.cbegin(), sUrl.cend(), result, regex(sKey + "=(.*)"))) {
		// 匹配只有一个参数的url

		return result[1];
	}
	else {
		// 不含参数或制定参数不存在
		return string();
	}
}