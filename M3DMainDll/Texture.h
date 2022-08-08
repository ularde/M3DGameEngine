#pragma once
#include "AssetManager.h"
#include "Platforms.h"
#include <string>

#include <stb_image.h>

class MAssetManager;

class MTexture: public MAsset {
public:
	MTexture(bool immediately_load, MAssetManager* asset_manager, const std::string& guid, const std::string& tex_type, const std::string& tex_path);
	~MTexture();
	unsigned int GetTextureID() { return this->texID; }
	unsigned char* GetTextureData() { return this->texData; }
	int GetTextureWidth() { return this->width; }
	int GetTextureHeight() { return this->height; }
private:
	MAssetManager* assetManager = NULL;
	int width, height, channelCount;
	unsigned int texID;
	unsigned char* texData = NULL;
	void LoadTexture();
};