#include "ChatRecord.h"
#include <Windows.h>
#include <string>
#include "Util.h"
#include "offset.h"
#include "struct.h"
#include "WSClient.h"
#include "EVString.h"
#include "Command.h"
#include "Loger.h"

DWORD recieveMsgCall, recieveMsgJmpAddr;


string Wstr2Str(wstring wstr)
{
	if (wstr.length() == 0)
		return "";

	std::string str;
	str.assign(wstr.begin(), wstr.end());
	return str;
}

void ReceiveMsgProc(LPVOID Context)
{
	recieveMsgStruct* msg = (recieveMsgStruct*)Context;

	WriteInfo("收到消息1");
	//todo:处理自动功能(自动收款、自动加名片等)
	neb::CJsonObject data;
	//todo:fromWxid、senderWxid某些特殊消息有异常

	WriteInfo("收到消息2");
	data.Add("Type", msg->type);
	data.Add("FromWxid", EVString::w2a(msg->fromWxid));
	//data.Add("SendWxid", EVString::w2a(msg->senderWxid));
	data.Add("Content", EVString::w2a(msg->content));
	data.Add("Other", EVString::w2a(msg->unkonwStr));
	delete msg;
	WriteInfo("收到消息3");
	Send(Cmd_ReceiveMessage, data);
}

/**
 * 处理拦截到的消息内容
 * @param DWORD esp
 * @return VOID
 */
VOID RecieveMessageJump(DWORD esp)
{
	WriteInfo("收到消息");
	DWORD* msgAddress = (DWORD*)(esp + 0x20);
	recieveMsgStruct* msg = new recieveMsgStruct;
	msg->type = (int)(*((DWORD*)(*msgAddress + 0x128)));
	msg->isSelf = (int)(*((DWORD*)(*msgAddress + 0x12C)));
	msg->fromWxid = (wchar_t*)(*((LPVOID*)(*msgAddress + 0x138)));
	msg->content = (wchar_t*)(*((LPVOID*)(*msgAddress + 0x160)));
	msg->senderWxid = (wchar_t*)(*((LPVOID*)(*msgAddress + 0x210)));
	msg->unkonwStr = (wchar_t*)(*((LPVOID*)(*msgAddress + 0x224)));

	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReceiveMsgProc, msg, 0, NULL);
}


/**
 * 被写入到hook点的接收消息裸函数
 */
__declspec(naked) void RecieveMsgDeclspec()
{
	__asm
	{
		pushad
		push esp
		call RecieveMessageJump
		add esp, 4
		popad
		call recieveMsgCall
		jmp recieveMsgJmpAddr
	}
}

void HookChatRecord() {
	//HOOK接收消息
	DWORD recieveMsgHookAddr = GetWeChatWinBase() + RECIEVEHOOKADDR;
	recieveMsgCall = GetWeChatWinBase() + RECIEVEHOOKCALL;
	recieveMsgJmpAddr = recieveMsgHookAddr + 5;
	BYTE msgJmpCode[5] = { 0xE9 };
	*(DWORD*)&msgJmpCode[1] = (DWORD)RecieveMsgDeclspec - recieveMsgHookAddr - 5;
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)recieveMsgHookAddr, msgJmpCode, 5, NULL);
}