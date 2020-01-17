#include <Windows.h>
#include <string>
#include "Util.h"
#include "offset.h"
#include "CJsonObject.hpp"
#include "EVString.h"
#include "WSClient.h"
#include "Command.h"
using namespace std;

void SetChatoomName(wchar_t* roomwxid, wchar_t* roomname)
{
	struct SetRoomNameStruct
	{
		wchar_t* pStr;
		int strLen;
		int strMaxLen;
		char full[0x8] = { 0 };
	};
	SetRoomNameStruct pRoomwxid = { 0 };
	pRoomwxid.pStr = roomwxid;
	pRoomwxid.strLen = wcslen(roomwxid) + 1;
	pRoomwxid.strMaxLen = (wcslen(roomwxid) + 1) * 2;
	SetRoomNameStruct pRoomname = { 0 };
	pRoomname.pStr = roomname;
	pRoomname.strLen = wcslen(roomname) + 1;
	pRoomname.strMaxLen = (wcslen(roomname) + 1) * 2;

	char* asmWxid = (char*)&pRoomwxid.pStr;
	char* asmRoomname = (char*)&pRoomname.pStr;

	DWORD dwCall1 = GetWeChatWinBase() + SETCHATROOMNAME;
	__asm
	{
		mov edx, asmRoomname;
		mov ecx, asmWxid;
		call dwCall1;
	}
}

void SetChatroomAnnouncement(wchar_t* chatroomwxid, wchar_t* Announcement)
{
	DWORD callAdrress = GetWeChatWinBase() + SETCHATROOMANNOUNCEMENT;
	CHAR bufferA[0xD0] = { 0 };
	DWORD* bufA = (DWORD*)&bufferA;

	CHAR buffer[0xD0] = { 0 };
	DWORD* buf = (DWORD*)&buffer;

	buf[0] = (DWORD)chatroomwxid;
	buf[1] = wcslen(chatroomwxid);
	buf[2] = wcslen(chatroomwxid) * 2;
	buf[3] = 0;
	buf[4] = 0;

	buf[0 + 5] = (DWORD)Announcement;
	buf[1 + 5] = wcslen(Announcement);
	buf[2 + 5] = wcslen(Announcement) * 2;
	buf[3 + 5] = 0;
	buf[4 + 5] = 0;

	bufA[0] = (DWORD)&buffer;
	bufA[1] = bufA[0] + 0x60;
	bufA[2] = bufA[0] + 0x60;

	DWORD r_esp = 0;
	__asm
	{
		mov r_esp, esp
		lea eax, bufferA
		push eax
		call callAdrress

		mov eax, r_esp
		mov esp, eax
	}
}

void AddChatroomMember(wchar_t* chatroomwxid, wchar_t* wxid)
{
	DWORD dwCallAddr1 = GetWeChatWinBase() + ADDCHATROOMMEMBER1;
	DWORD dwCallAddr2 = GetWeChatWinBase() + ADDCHATROOMMEMBER2;
	DWORD dwCallAddr3 = GetWeChatWinBase() + ADDCHATROOMMEMBER3;
	DWORD dwCallAddr4 = GetWeChatWinBase() + ADDCHATROOMMEMBER4;

	DWORD dwParam = GetWeChatWinBase() + ADDCHATROOMPARAM1;
	DWORD dwParam2 = GetWeChatWinBase() + ADDCHATROOMPARAM2;

	struct Wxid
	{
		wchar_t* str;
		int strLen = 0;
		int maxLen = 0;
		char file[0x8] = { 0 };
	};

	struct tempChatRoom
	{
		char fill2[0x8] = { 0 };
		wchar_t* str;
		int strLen = 0;
		int maxLen = 0;
		char fill[0x8] = { 0 };
	};

	Wxid wxidStruct = { 0 };
	wxidStruct.str = wxid;
	wxidStruct.strLen = wcslen(wxid) * 2;
	wxidStruct.maxLen = wcslen(wxid) * 4;

	tempChatRoom chatroomStruct = { 0 };
	chatroomStruct.str = chatroomwxid;
	chatroomStruct.strLen = wcslen(chatroomwxid) * 2;
	chatroomStruct.maxLen = wcslen(chatroomwxid) * 4;

	char wxidBuff[0xC] = { 0 };
	char tempWxid[0x14] = { 0 };
	char tempBuff[0x14] = { 0 };

	char* pWxid = (char*)&wxidStruct.str;
	char* pChatRoom = (char*)&chatroomStruct.fill2;

	__asm {
		pushad;
		lea esi, wxidBuff;
		mov ecx, esi;
		mov eax, pWxid;
		push eax;
		call dwCallAddr1;
		push 0;
		push dwParam;
		lea ecx, tempWxid;
		call dwCallAddr2;
		sub esp, 0x14;
		mov ecx, pChatRoom;
		mov eax, esp;
		push eax;
		call dwCallAddr3;
		push esi;
		mov ecx, dwParam2;
		call dwCallAddr4;
		popad;
	}
}

void DeleteChatroomMember(wchar_t* roomid, wchar_t* memberwxid)
{
	struct RoomIdStruct
	{
		wchar_t* roomid;
		int roomidLen;
		int roomidMaxLen;
		int full = 0;
		int full2 = 0;
	};

	DWORD dwCall1 = GetWeChatWinBase() + DELETECHATROOMMEMBER1;
	DWORD dwCall2 = GetWeChatWinBase() + DELETECHATROOMMEMBER2;
	DWORD dwCall3 = GetWeChatWinBase() + DELETECHATROOMMEMBER3;

	RoomIdStruct roomiddata;
	roomiddata.roomid = roomid;
	roomiddata.roomidLen = wcslen(roomid) + 1;
	roomiddata.roomidMaxLen = (wcslen(roomid) + 1) * 2;

	wchar_t wxidbuff[0xD0] = { 0 };
	DWORD* dwBuff = (DWORD*)&wxidbuff;
	dwBuff[0] = (DWORD)memberwxid;
	dwBuff[1] = wcslen(memberwxid);
	dwBuff[2] = wcslen(memberwxid) * 2;
	dwBuff[3] = 0;
	dwBuff[4] = 0;

	wchar_t datatbuffer[0xD0] = { 0 };
	DWORD* dwDatabuf = (DWORD*)&datatbuffer;
	dwDatabuf[0] = (DWORD)&wxidbuff;
	dwDatabuf[1] = dwDatabuf[0] + 0x14;
	dwDatabuf[2] = dwDatabuf[0] + 0x14;

	__asm
	{
		pushad;
		sub esp, 0x14;
		lea eax, roomiddata.roomid;
		mov ecx, esp;
		push eax;
		call dwCall1;
		lea edi, datatbuffer;
		push edi;
		call dwCall2;
		mov ecx, eax;
		call dwCall3;
		popad;
	}
}

void SendChatroomAtMsg(wchar_t* chatroomid, wchar_t* memberwxid, wchar_t* membernickname, wchar_t* msg)
{
	struct TEXT_WX
	{
		wchar_t* pWxid = nullptr;
		DWORD length = 0;
		DWORD maxLength = 0;
		DWORD fill1 = 0;
		DWORD fill2 = 0;
		wchar_t wxid[1024] = { 0 };

		TEXT_WX(wstring wsWxid)
		{
			const wchar_t* temp = wsWxid.c_str();
			wmemcpy(wxid, temp, wsWxid.length());
			length = wsWxid.length();
			maxLength = wsWxid.capacity();
			fill1 = 0;
			fill2 = 0;
			pWxid = wxid;
		}
	};

	struct ROOM_AT
	{
		DWORD at_WxidList = 0;
		DWORD at_end1 = 0;
		DWORD at_end2 = 0;
	};
	struct TEXT_WXID
	{
		wchar_t* pWxid = nullptr;
		DWORD length = 0;
		DWORD maxLength = 0;
		DWORD fill1 = 0;
		DWORD fill2 = 0;
	};


	DWORD callAddress_SendText = GetWeChatWinBase() + SENDTEXTADDR;
	TEXT_WX wxId(chatroomid);
	wchar_t tempmsg[100] = { 0 };
	swprintf_s(tempmsg, L"@%s %s", membernickname, msg);
	TEXT_WX wxMsg(tempmsg);

	TEXT_WXID wxAtId;
	wxAtId.pWxid = memberwxid;
	wxAtId.length = wcslen(memberwxid);
	wxAtId.maxLength = wcslen(memberwxid) * 2;
	wxAtId.fill1 = 0;
	wxAtId.fill2 = 0;
	//DWORD* asmWxid = (DWORD*)& wxAtId.pWxid;
	ROOM_AT roomAt;
	roomAt.at_WxidList = (DWORD)&wxAtId.pWxid;
	roomAt.at_end1 = roomAt.at_WxidList + 5 * 4;
	roomAt.at_end2 = roomAt.at_end1;
	BYTE buff[0x81C] = { 0 };
	__asm
	{
		lea edx, wxId
		push 0x1
		lea eax, roomAt
		push eax
		lea ebx, wxMsg
		push ebx
		lea ecx, buff
		call callAddress_SendText
		add esp, 0xC
	}
}

void GetChatroomUser(wchar_t* chatroomid, string callback)
{
	struct wxStr
	{
		wchar_t* pStr;
		int strLen;
		int strMaxLen;
		int fill = 0;
		int fill2 = 0;
	};
	DWORD dwWxidArr = 0;
	char buff[0x164] = { 0 };
	char userListBuff[0x174] = { 0 };
	wxStr pWxid = { 0 };
	pWxid.pStr = chatroomid;
	pWxid.strLen = wcslen(chatroomid);
	pWxid.strMaxLen = wcslen(chatroomid) * 2;
	char* asmWxid = (char*)&pWxid.pStr;

	DWORD dwCall1 = (DWORD)GetModuleHandle(L"WeChatWin.dll") + GETCHATROOMUSERCALL1;
	DWORD dwCall2 = (DWORD)GetModuleHandle(L"WeChatWin.dll") + GETCHATROOMUSERCALL2;
	DWORD dwCall3 = (DWORD)GetModuleHandle(L"WeChatWin.dll") + GETCHATROOMUSERCALL3;
	DWORD dwCall4 = (DWORD)GetModuleHandle(L"WeChatWin.dll") + GETCHATROOMUSERCALL4;

	__asm {
		lea ecx, buff[16]
		call dwCall1
		lea eax, buff[16]
		push eax
		mov ebx, asmWxid
		push ebx
		call dwCall2
		mov ecx, eax
		call dwCall3
		lea eax, buff
		push eax
		lea ecx, buff[16]
		call dwCall4
		mov dwWxidArr, eax
	}

	//ÄÃµ½Î¢ÐÅID
	wchar_t test[0x100] = { 0 };
	wchar_t tempWxid[0x100] = { 0 };
	DWORD userList = *((DWORD*)dwWxidArr);
	DWORD testTmp = dwWxidArr + 0xB0;
	int Len = *((int*)testTmp);

	neb::CJsonObject respons;
	respons.AddEmptySubArray("Members");
	for (int i = 0; i < Len; i++)
	{
		DWORD temWxidAdd = userList + (i * 20);
		swprintf_s(tempWxid, L"%s", (wchar_t*)*((LPVOID*)temWxidAdd));
		respons["Members"].Add(EVString::w2a((wchar_t*)temWxidAdd));
	}
	Send(Cmd_GetChatRoomMember, respons, callback);
}

void QuitChatRoom(wchar_t* chatroomwxid)
{
	struct StructWxid
	{
		wchar_t* pWxid;
		DWORD length;
		DWORD maxLength;
		DWORD fill1;
		DWORD fill2;
	};

	DWORD dwCallAddr = GetWeChatWinBase() + QUITCHATROOM;

	StructWxid structWxid = { 0 };
	structWxid.pWxid = chatroomwxid;
	structWxid.length = wcslen(chatroomwxid);
	structWxid.maxLength = wcslen(chatroomwxid) * 2;

	DWORD* asmMsg = (DWORD*)&structWxid.pWxid;

	__asm {
		mov ebx, asmMsg;
		push ebx;
		call dwCallAddr;
	}
}