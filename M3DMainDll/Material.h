#pragma once
#include "Platforms.h"
#include "AssetManager.h"
#include "Texture.h"

#include <string>
#include <vector>
#include "tinyxml2.h"

class MAssetManager;
class MTexture;

class MMaterial :public MAsset {
public:
	MMaterial(bool immediately_load, MAssetManager* asset_manager, const std::string& guid, const std::string& mat_type, const std::string& mat_path);
private:
	unsigned int texCount;
	void LoadMaterial();
};
