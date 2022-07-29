#pragma once

class __declspec(dllimport)M3DS_Module {
public:
	M3DS_Module() {}
	~M3DS_Module() {}
	int LoadModule(const char* module_path);
	void UnloadModule();
	void RunModule();
};