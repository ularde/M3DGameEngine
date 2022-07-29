#include "pch.h"
#include "Module.h"
#include "VirtualMachine.h"



int M3DS_Module::LoadModule(const char* module_path) {
	//
	this->VM_InstanceID = currentVMInstanceID;
	currentVMInstanceID++;
	M3DS_ModuleVM* object = (M3DS_ModuleVM*)malloc(sizeof(M3DS_ModuleVM));
	vmInstances.push_back(object);
	//
	int buffer = 0;
	fopen_s(&(this->file), module_path, "r");
	if (this->file) {
		while (!feof(this->file)) {
			fread(&buffer, 4, 1, this->file);
			this->instructions.push_back(buffer);
		}
		fclose(this->file);
		this->file_loaded = true;
		return 1;
	}
	else {
		return 0;
	}
}

void M3DS_Module::UnloadModule() {
	free(vmInstances[this->VM_InstanceID]);
}

void M3DS_Module::RunModule() {
	if (this->file_loaded) {
		if (this->instructions.size() > 0) {
			M3DS_ExecuteModuleInstructions(this->VM_InstanceID, this->instructions);
		}
	}
}
