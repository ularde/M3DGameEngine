#include "pch.h"
#include "ConsoleOperations.h"
#include "ProblemCodes.h"

void M3DConsole_PrintInfo(const char* moduleName, int code, const char* infoText) {
	printf("[%s]Code %d : %s\n", moduleName, code, infoText);
}

void M3DConsole_PrintInfo(const char* moduleName, int code) {

}

void M3DConsole_PrintError(const char* moduleName, int code, const char* infoText) {
	printf("\033[0m\033[1;31m\033[0m[%s]Code %d : %s\033[0m\n", moduleName, code, infoText);
}

void M3DConsole_PrintError(const char* moduleName, int code) {

}

void M3DConsole_PrintWarning(const char* moduleName, int code, const char* infoText) {
	printf("\033[0m\033[1;33m\033[0m[%s]Code %d : %s\033[0m\n", moduleName, code, infoText);
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
