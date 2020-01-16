#pragma once

//用户个人信息
struct Information
{
	wchar_t wxid[0x200]			= L"NULL";	//微信ID
	wchar_t account[0x200]		= L"NULL";	//自定义账号
	wchar_t nickname[0x500]		= L"NULL";	//昵称
	wchar_t device[0x100]		= L"NULL";	//登陆设备
	wchar_t phone[0x100]		= L"NULL";	//绑定手机号
	wchar_t email[0x200]		= L"NULL";	//绑定邮箱
	int		sex					= 0;		//性别
	wchar_t nation[0x100]		= L"NULL";	//国籍
	wchar_t province[0x100]		= L"NULL";	//省份
	wchar_t city[0x100]			= L"NULL";	//城市
	wchar_t signName[0x500]		= L"NULL";	//签名
	wchar_t bigHeader[0x1000]	= L"NULL";	//大头像
};

//用户消息
struct recieveMsgStruct
{
	int type;
	int isSelf;
	wchar_t* fromWxid;
	wchar_t* senderWxid;
	wchar_t* unkonwStr;
	wchar_t* content;
};

//好友列表
struct friendStruct
{
	wchar_t wxid[0x100] = L"NULL";
	wchar_t account[0x200] = L"NULL";
	wchar_t nickname[0x200] = L"NULL";
	wchar_t head[0x1000] = L"NULL";
	wchar_t v1[0x200] = L"NULL";
	wchar_t reMark[0x200] = L"NULL";
	wchar_t bigHeadImgUrl[0x1000] = L"NULL";
};

//数据库句柄
struct DbHandle
{
	int handler;
	char path[0x1000];
};

typedef int (*sqlite3_callback)(void*, int, char**, char**);

typedef int(WINAPI* Sqlite3_exec)(
	DWORD,
	const char*,
	sqlite3_callback,
	void*,
	char**
);