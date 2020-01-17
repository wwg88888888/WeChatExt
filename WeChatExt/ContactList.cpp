#include "ContactList.h"
#include <Windows.h>
#include <string>
#include <list>
#include "Sql.h"
#include "Util.h"
#include "WSClient.h"
#include "CJsonObject.hpp"
#include "Command.h"

INT GetContactCallBack(void* para, int nColumn, char** colValue, char** colName)
{
	neb::CJsonObject respons;

	for (int i = 0; i < nColumn; i++)
	{
		char data[1000] = { 0 };
		sprintf_s(data, "%s", colValue[i]);

		respons.Add(*(colName + i), data);
	}
	Send(Cmd_ContactList, respons);
	return 0;
}

void GetContactList()
{
	char* sqlErrmsg = NULL;
	RunSql("MicroMsg.db", "select Contact.UserName,Contact.Alias,Contact.NickName,Contact.Remark,ContactHeadImgUrl.BigHeadImgUrl from Contact LEFT OUTER JOIN ContactHeadImgUrl ON Contact.UserName = ContactHeadImgUrl.usrName", GetContactCallBack, sqlErrmsg);
}