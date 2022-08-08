#pragma once
#include <string>
#include <vector>

#include "Platforms.h"
#include "Global.h"
#include "Class.h"
#include "SharedFunc.h"

class MNonPythonBoundObject;
class MAsset;
class MTexture;
class MMaterial;

class MAssetManager {
public:
	MAssetManager(MBasicPlatform* platform_);
	~MAssetManager();
	MTexture* AddTexture(bool immediately_load, const std::string& type, const std::string& path, char* guid);
	MMaterial* AddMaterial(bool immediately_load, const std::string& type, const std::string& path, char* guid);
	void AddModel(const std::string& type, const std::string& path, char* guid);
	void AddScene(const std::string& type, const std::string& path, char* guid);
	void AddUIForm(const std::string& type, const std::string& path, char* guid);
	void UnloadAsset(const std::string& guid);
	MAsset* GetAsset(const std::string& guid);
private:
	MBasicPlatform* platform = NULL;
	std::vector<MAsset*> assetsPtrs;
};

class MAsset :public MNonPythonBoundObject {
public:
	int superType = 0;
	const char* GetPath() { return this->path.c_str(); }
	const char* GetGUID() { return this->GUID.c_str(); }
	const char* GetType() { return this->type.c_str(); }
protected:
	bool loaded = false;
	MAssetManager* assetManager = NULL;
	std::string GUID;
	std::string path;
	std::string type;
};