#include "pch.h"
#include "AssetManager.h"
#include "Texture.h"
#include "Material.h"

class MTexture;
class MMaterial;


MAssetManager::MAssetManager(MBasicPlatform* platform_) {
	this->platform = platform_;
}

MAssetManager::~MAssetManager()
{
}

MTexture* MAssetManager::AddTexture(bool immediately_load, const std::string& type, const std::string& path, char* guid) {
	static MTexture* object = NULL;
	for (unsigned int i = 0; i < this->assetsPtrs.size(); ++i) {
		if (path == this->assetsPtrs[i]->GetPath() && type == this->assetsPtrs[i]->GetType() && this->assetsPtrs[i]->superType == M3D_ASSET_TEXTURE) {
			object = reinterpret_cast<MTexture*>(this->assetsPtrs[i]);
			this->assetsPtrs.push_back(object);
			return object;
		}
		else {
			break;
		}
	}
	object = new MTexture(immediately_load, this, guid, type, path);
	return object;
}

MMaterial* MAssetManager::AddMaterial(bool immediately_load, const std::string& type, const std::string& path, char* guid) {
	static MMaterial* object = NULL;
	for (unsigned int i = 0; i < this->assetsPtrs.size(); ++i) {
		if (path == this->assetsPtrs[i]->GetPath() && type == this->assetsPtrs[i]->GetType() && this->assetsPtrs[i]->superType == M3D_ASSET_MATERIAL) {
			object = reinterpret_cast<MMaterial*>(this->assetsPtrs[i]);
			this->assetsPtrs.push_back(object);
			return object;
		}
		else {
			break;
		}
	}
	object = new MMaterial(immediately_load, this, guid, type, path);
	return object;
}

void MAssetManager::UnloadAsset(const std::string& guid) {

}

MAsset* MAssetManager::GetAsset(const std::string& guid) {
	for (unsigned int i = 0; i < this->assetsPtrs.size(); ++i) {
		if (this->assetsPtrs[i]->GetGUID() == guid) {
			return this->assetsPtrs[i];
		}
		else {
			break;
		}
	}
	return NULL;
}
