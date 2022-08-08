#include "pch.h"
#include "ConsoleOperations.h"
#include "Global.h"

void M3DConsole_SetTextColor(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void M3DConsole_PrintInfo(const char* moduleName, int code, const char* infoText) {
	printf("[%s]Code %d : %s\n", moduleName, code, infoText);
}

void M3DConsole_PrintInfo(const char* moduleName, int code) {

}

void M3DConsole_PrintError(const char* moduleName, int code, const char* infoText) {
	M3DConsole_SetTextColor(12);
	printf("[%s]Code %d : %s\n", moduleName, code, infoText);
	M3DConsole_SetTextColor(7);
}

void M3DConsole_PrintError(const char* moduleName, int code) {
	M3DConsole_SetTextColor(12);
	switch (code)
	{
	case M3D_M3DS_BADMAINMODULE:
		printf("[%s]Code %d : %s\n", moduleName, code, "Could not load main module file. Check the ambient setting XML file and the module file.");
		break;
	default:
		break;
	}
	M3DConsole_SetTextColor(7);
}

void M3DConsole_PrintWarning(const char* moduleName, int code, const char* infoText) {
	printf("[%s]Code %d : %s\n", moduleName, code, infoText);
}

void M3DConsole_PrintWarning(const char* moduleName, int code) {
	switch (code)
	{
	case M3D_CFG_BADATTRIBUTION:
		printf("[%s]Code %d : %s\n", moduleName, code, "Bad attribution(s) of a certain element.");
		break;
	default:
		break;
	}
}
