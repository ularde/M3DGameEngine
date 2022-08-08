#pragma once
#include <objbase.h>
#include <stdio.h>
#include <string>

const char* GenerateGuidA();
const wchar_t* GenerateGuidW();
const wchar_t* GetFullAssetPathW(const std::wstring& short_path);
const char* GetFullAssetPathA(const std::string& short_path);