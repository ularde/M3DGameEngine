#include "pch.h"
#include "SharedFunc.h"

static std::string temp_string;
static std::wstring temp_wstring;

const char* GenerateGuidA() {
	static char stringBuf[62] = { 0 };
	GUID Guid;
	if (CoCreateGuid(&Guid) == S_OK) {
		sprintf(stringBuf, "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
			Guid.Data1, Guid.Data2, Guid.Data3,
			Guid.Data4[0], Guid.Data4[1], Guid.Data4[2],
			Guid.Data4[3], Guid.Data4[4], Guid.Data4[5],
			Guid.Data4[6], Guid.Data4[7]);
	}
	return stringBuf;
}

const wchar_t* GenerateGuidW() {
	static wchar_t stringBuf[62] = { 0 };
	GUID Guid;
	if (CoCreateGuid(&Guid) == S_OK) {
		wsprintf(stringBuf, L"%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
			Guid.Data1, Guid.Data2, Guid.Data3,
			Guid.Data4[0], Guid.Data4[1], Guid.Data4[2],
			Guid.Data4[3], Guid.Data4[4], Guid.Data4[5],
			Guid.Data4[6], Guid.Data4[7]);
	}
	return stringBuf;
}

const char* GetFullAssetPathA(const std::string& short_path) {
	temp_string = ".\\..\\..\\Game\\" + short_path;
	return temp_string.c_str();
}

const wchar_t* GetFullAssetPathW(const std::wstring& short_path) {
	temp_wstring = L".\\..\\..\\Game\\" + short_path;
	return temp_wstring.c_str();
}
