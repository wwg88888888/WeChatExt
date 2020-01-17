#pragma once  
#include "stdafx.h" 
#include <Windows.h>
#include <string>

using namespace std;

/**
 * 获取WeChatWin.dll地址
 * @param
 * @return DWORD
 */
DWORD GetWeChatWinBase();

/**
 * 检查微信版本是否匹配
 * @param
 * @return BOOL
*/
BOOL IsWxVersionValid();
 
/**
 * 编码转换
 * @param const wchar_t* unicode
 * @return char*
 */
char* UnicodeToUtf8(const wchar_t* unicode);


/**
 * 编码转换
 * @param const char* str
 * @return wchar_t*
*/
wchar_t* UTF8ToUnicode(const char* str);


/**
 * 编码转换
 * @param std::string sInfo, INT sourceEncode, INT targetEncode
 * @return std::string
 */
string EncodeConvert(std::string sInfo, INT sourceEncode, INT targetEncode);


/**
 * 编码转换
 * @param std::string str
 * @return wchar_t*
 */
wchar_t* StrToWchar(std::string str);


/**
 * 创建目录
 * @param const char* dir
 * @return void
*/
void CreateDir(const char* dir);