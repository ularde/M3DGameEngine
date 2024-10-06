#pragma once
#include <objbase.h>
#include <stdio.h>
#include <string>
#include <PxPhysicsAPI.h>

const char* GenerateGuidA();
const wchar_t* GenerateGuidW();
const wchar_t* GetFullAssetPathW(const std::wstring& short_path);
const char* GetFullAssetPathA(const std::string& short_path);
int GetVec3FromString(const char* str, float* x, float* y, float* z);
int GetVec4FromString(const char* str, float* x, float* y, float* z, float* w);
int GetVec3FromString(const std::string& str, float* x, float* y, float* z);
int GetVec4FromString(const std::string& str, float* x, float* y, float* z, float* w);
const char* GenStringByVec2(const glm::vec2& vec);
const char* GenStringByVec3(const glm::vec3& vec);
const char* GenStringByVec4(const glm::vec4& vec);
glm::mat4 ConvertPxMat44ToGLMMat4(const physx::PxMat44& matrix);