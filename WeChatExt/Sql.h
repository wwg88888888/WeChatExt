#include <Windows.h>
#include <string>
#include "struct.h"
#include <list>
using namespace std;

void HookDB();

list<DbHandle> GetDbHandleList();
void ClearDbHandleList();

int RunSql(string dbName, string sqlStr, sqlite3_callback callBack, char*& sqlErrmsg);
INT RunSqlCallBack(void* para, int nColumn, char** colValue, char** colName);