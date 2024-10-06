#include "pch.h"
#include "AssetManager.h"
#include "Texture.h"
#include "Model.h"
#include "Material.h"

class MModel;
class MTexture;
class MMaterial;
class MAssetManager;

MAssetManager::MAssetManager(MBasicPlatform* platform_) {
	gPlatform = platform_;
}

MAssetManager::~MAssetManager()
{
}

MTexture* MAssetManager::AddTexture(bool immediately_load, const std::string& type, const std::string& path, char* guid) {
	MTexture* object = NULL;
	if (gTexturesMap[path]) {
		object = gTexturesMap[path];
		return object;
	}
	else {
		object = new MTexture(immediately_load, this, guid, type, path);
		gAssetsPtrs.push_back(object);
		gTexturesMap[path] = object;
		return object;
	}
}

MCubemap* MAssetManager::AddCubemap(bool immediately_load, const std::string& type, 
	const std::string& name, char* guid,
	const std::string& right, const std::string& left,
	const std::string& top, const std::string& bottom,
	const std::string& back, const std::string& front) {
	MCubemap* object = NULL;
	if (gCubemapMap[name]) {
		object = gCubemapMap[name];
		return object;
	}
	else {
		object = new MCubemap(immediately_load, this, 
			name, guid, right, left, 
			top, bottom, back, front);
		gAssetsPtrs.push_back(object);
		gCubemapMap[name] = object;
		return object;
	}
}

MModel* MAssetManager::AddModel(bool immediately_load, const std::string& type, const std::string& path, char* guid) {
	MModel* object = NULL;
	if (gModelsMap[path]) {
		object = gModelsMap[path];
		return object;
	}
	else {
		object = new MModel(immediately_load, this, guid, type, path);
		gAssetsPtrs.push_back(object);
		gModelsMap[path] = object;
		return object;
	}
}

MMaterial* MAssetManager::AddMaterial(const std::string& type, const std::string& path, char* guid) {
	MMaterial* object = NULL;
	if (gMaterialsMap[path]) {
		object = gMaterialsMap[path];
		return object;
	}
	else {
		object = new MMaterial(gPlatform, path);
		gAssetsPtrs.push_back(object);
		gMaterialsMap[path] = object;
		return object;
	}
}

void MAssetManager::UnloadAsset(const std::string& guid) {
	
}

MAsset* MAssetManager::GetAsset(const std::string& guid) {
	for (unsigned int i = 0; i < gAssetsPtrs.size(); ++i) {
		if (gAssetsPtrs[i]->GetGUID() == guid) {
			return gAssetsPtrs[i];
		}
		else {
			break;
		}
	}
	return NULL;
}
