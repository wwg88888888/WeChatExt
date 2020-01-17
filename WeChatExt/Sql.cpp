#include <Windows.h>
#include <string> 
#include <Shlwapi.h>
#include "offset.h" 
#include <CommCtrl.h>
#include "struct.h"
#include "Util.h"
#include <list>
#pragma comment(lib,"Shlwapi.lib")
using namespace std;
string sqlResult = "";
DWORD  databaseJumpAddr;

list<DbHandle> DbHandleList;


/**
 * 将获取到的数据库句柄压入list内
 * @param int dbAddress, int dbHandle
 * @return VOID
 */
void DatabaseHwnd(int dbAddress, int dbHandle)
{
	DbHandle Db = { 0 };
	Db.handler = dbHandle;
	sprintf_s(Db.path, "%s", (char*)dbAddress);
	DbHandleList.push_back(Db);
}

/**
 * 被写入到hook点的数据库句柄获取裸函数
 */
__declspec(naked) void DatabaseDeclspec()
{
	__asm
	{
		mov esi, dword ptr ss : [ebp - 0x14]
		add esp, 0x8
		pushad
		push[ebp - 0x14]
		push[ebp - 0x24]
		call DatabaseHwnd
		add esp, 8
		popad
		jmp databaseJumpAddr
	}
}

void HookDB() {
	//HOOK数据库句柄
	DWORD hookAddress = GetWeChatWinBase() + DATABASEHOOKADDR;
	databaseJumpAddr = hookAddress + 6;
	BYTE dbJmpCode[6] = { 0xE9 };
	dbJmpCode[6 - 1] = 0x90;
	*(DWORD*)&dbJmpCode[1] = (DWORD)DatabaseDeclspec - hookAddress - 5;
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)hookAddress, dbJmpCode, 6, NULL);
}

list<DbHandle> GetDbHandleList()
{
	return DbHandleList;
}

void ClearDbHandleList()
{
	DbHandleList.clear();
	return;
}



int RunSql(string dbName, string sqlStr, sqlite3_callback callBack, char*& sqlErrmsg)
{
	Sqlite3_exec sqlite3_exec = (Sqlite3_exec)(GetWeChatWinBase() + DATABASERUNCALL);
	for (auto& db : GetDbHandleList())
	{
		if (StrStrA(db.path, dbName.c_str()))
		{
			sqlResult = "";
			sqlite3_exec(db.handler, (char*)sqlStr.c_str(), callBack, NULL, &sqlErrmsg);
			return 0;
		}
	}
	return 1;
}

INT RunSqlCallBack(void* para, int nColumn, char** colValue, char** colName)
{
	char columns[0x100] = { 0 };
	sprintf_s(columns, "nColumns : %d\r\n", nColumn);
	sqlResult = sqlResult + string(columns);
	for (int i = 0; i < nColumn; i++)
	{
		char data[0x1000] = { 0 };
		sprintf_s(data, "%s :%s\r\n", *(colName + i), colValue[i]);
		sqlResult = sqlResult + string(data);
	}
	return 0;
}