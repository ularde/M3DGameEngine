#pragma once
#include <iostream>
#include <string>

void M3DConsole_PrintInfo(const char* moduleName, int code, const char* infoText);
void M3DConsole_PrintInfo(const char* moduleName, int code);
void M3DConsole_PrintError(const char* moduleName, int code, const char* infoText);
void M3DConsole_PrintError(const char* moduleName, int code);
void M3DConsole_PrintWarning(const char* moduleName, int code, const char* infoText);
void M3DConsole_PrintWarning(const char* moduleName, int code);