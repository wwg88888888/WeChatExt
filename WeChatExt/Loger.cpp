#include "stdafx.h"
#include "Loger.h"
#include <iostream>
#include <fstream>
#include <time.h> 
#include "Util.h"
#include <string>
#include "EVString.h"

using namespace std;

typedef enum enLogLevel {
	LOG_DEBUG = 1,
	LOG_INFO,
	LOG_ERROR,
	LOG_WARN,
	LOG_FATAL
}LogLevel;


char folder[MAX_PATH];


const char LogTip[][8] = { "","Debug","Info","Error","Warn","Fatal" };

HMODULE GetSelfModuleHandle()
{
	MEMORY_BASIC_INFORMATION mbi;
	return ((::VirtualQuery(GetSelfModuleHandle, &mbi, sizeof(mbi)) != 0) ? (HMODULE)mbi.AllocationBase : NULL);
}

void GetLogFolder() {
	char szBuff[MAX_PATH] = { 0 };
	GetModuleFileNameA(GetSelfModuleHandle(), szBuff, MAX_PATH);

	string temp = szBuff;
	temp = temp.substr(0, temp.find_last_of("\\"));
	char path[MAX_PATH] = "\\Logs\\";
	char tmp[MAX_PATH] = "";
	strcat_s(tmp, temp.c_str());
	strcat_s(tmp, path);
	CreateDir(tmp);
	strcat_s(folder, tmp);
}

void GetLogFileName(string& logFileName)
{
	time_t t = time(0);
	struct tm now_time;
	localtime_s(&now_time, &t);

	char fileName[MAX_PATH] = { NULL };
	strftime(fileName, sizeof(fileName), "%Y%m%d", &now_time);
	strcat_s(fileName, ".log");
	if (strlen(folder) <= 0)
		GetLogFolder();
	char tmp[MAX_PATH] = "";
	strcat_s(tmp, folder);
	strcat_s(tmp, fileName);
	logFileName = tmp;
}


void WriteLog(int logLevel, const string& log)
{
	string logFileName;
	GetLogFileName(logFileName);

	time_t t = time(0);
	struct tm now_time;
	localtime_s(&now_time, &t);
	char date[32] = { NULL };
	strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", &now_time);

	char prefix[50];
	sprintf_s(prefix, "%s : [%s][%s] ", date, to_string(GetCurrentProcessId()).c_str(), LogTip[logLevel]);

	std::ofstream  OsWrite(logFileName, std::ofstream::app);

	OsWrite << prefix;
	OsWrite << log.c_str();
	OsWrite << "\n";
	OsWrite << std::endl;
	OsWrite.close();
}

void WriteDebug(const string& log) {
	WriteLog(LOG_DEBUG, log);
}

void WriteInfo(const string& log) {
	WriteLog(LOG_INFO, log);
}

void WriteError(const string& log) {
	WriteLog(LOG_ERROR, log);
}

void WriteWarn(const string& log) {
	WriteLog(LOG_WARN, log);
}

void WriteFatal(const string& log) {
	WriteLog(LOG_FATAL, log);
}

