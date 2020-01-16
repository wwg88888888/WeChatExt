#pragma once 

#include <stdio.h>
#include <Windows.h>
#include <string>

using namespace std;
 
void WriteDebug(const string& log);

void WriteInfo(const string& log);

void WriteError(const string& log);

void WriteWarn(const string& log);

void WriteFatal(const string& log);

