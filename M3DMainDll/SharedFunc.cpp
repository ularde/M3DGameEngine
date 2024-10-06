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

int GetVec3FromString(const char* str, float* x, float* y, float* z) {
	return sscanf(str, "%f,%f,%f", x, y, z);
}

int GetVec3FromString(const std::string& str, float* x, float* y, float* z) {
	return sscanf(str.c_str(), "%f,%f,%f", x, y, z);
}

int GetVec4FromString(const char* str, float* x, float* y, float* z, float* w) {
	return sscanf(str, "%f,%f,%f,%f", x, y, z, w);
}

int GetVec4FromString(const std::string& str, float* x, float* y, float* z, float* w) {
	return sscanf(str.c_str(), "%f,%f,%f,%f", x, y, z, w);
}

const char* GenStringByVec2(const glm::vec2& vec) {
	char str[1024] = { 0 };
	sprintf(str, "%lf,%lf", vec.x, vec.y);
	temp_string = str;
	return temp_string.c_str();
}

const char* GenStringByVec3(const glm::vec3& vec) {
	char str[1024] = { 0 };
	sprintf(str, "%lf,%lf,%lf", vec.x, vec.y, vec.z);
	temp_string = str;
	return temp_string.c_str();
}

const char* GenStringByVec4(const glm::vec4& vec) {
	char str[1024] = { 0 };
	sprintf(str, "%lf,%lf,%lf,%lf", vec.x, vec.y, vec.z, vec.a);
	temp_string = str;
	return temp_string.c_str();
}

glm::mat4 ConvertPxMat44ToGLMMat4(const physx::PxMat44& matrix) {
	return glm::mat4(matrix.column0.x, matrix.column0.y, matrix.column0.z, matrix.column0.w,
		matrix.column1.x, matrix.column1.y, matrix.column1.z, matrix.column1.w,
		matrix.column2.x, matrix.column2.y, matrix.column2.z, matrix.column2.w,
		matrix.column3.x, matrix.column3.y, matrix.column3.z, matrix.column3.w);
}

const wchar_t* GetFullAssetPathW(const std::wstring& short_path) {
	temp_wstring = L".\\..\\..\\Game\\" + short_path;
	return temp_wstring.c_str();
}
