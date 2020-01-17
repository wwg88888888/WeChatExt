// dllmain.cpp : 定义 DLL 应用程序的入口点。 
#include "stdafx.h"
#include "WSClient.h"

DWORD WINAPI ThreadProc(
	_In_ HMODULE hModule
);

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, hModule, 0, NULL);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		StopWebSocket();
		break;
	}
	return TRUE;
}


DWORD WINAPI ThreadProc(_In_ HMODULE hModule)
{
	InitWebSocket();
	return true;
}


/*
 * 说明：此项目中关于微信hook功能的实现代码来自codeByDog开源的pcWechat项目，非常感谢codeByDog。
 * 作者QQ：50728123
 * 交流群：810420984
 * 微信：codeByDog
 * Git：https://github.com/codeByDog/pcWechat.git
*/