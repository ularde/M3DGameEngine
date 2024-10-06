#pragma once
#include "AssetManager.h"
#include "Platforms.h"
#include <string>
#include <vector>

#include <stb_image.h>

class MAssetManager;

enum class MCubemapFace {
	RIGHT,
	LEFT,
	TOP,
	BOTTOM,
	BACK,
	FRONT
};

class MTexture: public MAsset {
public:
	MTexture(bool immediately_load, MAssetManager* asset_manager, const std::string& guid, const std::string& tex_type, const std::string& tex_path);
	~MTexture();
	unsigned int GetTextureID();
	unsigned char* GetTextureData();
	int GetTextureWidth() { return this->width; }
	int GetTextureHeight() { return this->height; }
private:
	MAssetManager* gAssetManager = NULL;
	int width, height, channelCount;
	unsigned int texID;
	unsigned char* texData = NULL;
	void LoadTexture();
};

class MCubemap :public MAsset {
public:
	MCubemap(bool immediately_load, MAssetManager* asset_manager,
		const std::string& name_, const std::string& guid,
		const std::string& right_, const std::string& left_,
		const std::string& top_, const std::string& bottom_,
		const std::string& back_, const std::string& front_);
	~MCubemap() { }
	unsigned int GetTextureID() { return texID; }
	int GetTextureWidth(MCubemapFace face) { return this->width[(int)face]; }
	int GetTextureHeight(MCubemapFace face) { return this->height[(int)face]; }
private:
	MAssetManager* gAssetManager = NULL;

	int width[6] = { 0 }, 
		height[6] = { 0 }, 
		channelCount[6] = { 0 };
	unsigned int texID;

	std::string name;
	std::string texPaths[6];

	void LoadCubemap();
};