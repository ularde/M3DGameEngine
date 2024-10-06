#pragma once
#include <string>
#include <vector>
#include <map>

#include "Platforms.h"
#include "Global.h"
#include "SharedFunc.h"
#include "Class.h"

class MAsset;
class MTexture;
class MModel;
class MCubemap;
class MMaterial;

class MAssetManager {
public:
	MAssetManager(MBasicPlatform* platform_);
	~MAssetManager();
	MTexture* AddTexture(bool immediately_load, const std::string& type, 
		const std::string& path, char* guid);
	MCubemap* AddCubemap(bool immediately_load, const std::string& type,
		const std::string& name, char* guid,
		const std::string& right_, const std::string& left_,
		const std::string& top_, const std::string& bottom_,
		const std::string& back_, const std::string& front_);
	MModel* AddModel(bool immediately_load, const std::string& type, 
		const std::string& path, char* guid);
	MMaterial* AddMaterial(const std::string& type, const std::string& path,
		char* guid);
	void UnloadAsset(const std::string& guid);
	MAsset* GetAsset(const std::string& guid);
	MBasicPlatform* GetPlatform() { return this->gPlatform; }
private:
	MBasicPlatform* gPlatform = NULL;
	std::vector<MAsset*> gAssetsPtrs;
	std::map<std::string, MCubemap*> gCubemapMap;
	std::map<std::string, MTexture*> gTexturesMap;
	std::map<std::string, MModel*> gModelsMap;
	std::map<std::string, MMaterial*> gMaterialsMap;
};

class MAsset :public MUnscriptableObject {
public:
	int superType = 0;
	const char* GetPath() { return this->path.c_str(); }
	const char* GetGUID() { return this->GUID.c_str(); }
	const char* GetType() { return this->type.c_str(); }
protected:
	bool mLoaded = false;
	MAssetManager* gAssetManager = NULL;
	std::string GUID;
	std::string path;
	std::string type;
	std::string directory;
};