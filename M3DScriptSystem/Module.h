#pragma once
#include <vector>


class __declspec(dllexport)M3DS_Module {
public:
	M3DS_Module() {}
	~M3DS_Module() {}
	int LoadModule(const char* module_path);
	void UnloadModule();
	void RunModule();
protected:
	unsigned int VM_InstanceID = 0;
	bool file_loaded = false;
	std::vector<int> instructions;
	FILE* file = NULL;
};