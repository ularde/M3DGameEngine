#pragma once
#include "Class.h"
#include "tinyxml2.h"
#include "AssetManager.h"

class MBasicPlatform;
class MMaterial;
class MTexture;
class MAsset;

class MSubMaterial :public MUnscriptableObject {
public:
	MSubMaterial(MBasicPlatform* gPlatform, MMaterial* mParent,
		const unsigned int ID, const bool useForward, int surfaceTypeID,
		const std::string& albedoPath, const std::string& normalPath, const std::string& roughnessPath,
		const std::string& metallicPath, const std::string& AOPath);
	MSubMaterial(MBasicPlatform* platform, MMaterial* mParent,
		const unsigned int ID, const bool useForward, int surfaceTypeID,
		const std::string& albedoPath, const std::string& normalPath, const std::string& roughnessPath,
		const std::string& metallicPath, const std::string& AOPath, const glm::vec3& albedoColor,
		const float metallicColor, const float roughnessColor, const float aoColor);
	~MSubMaterial();
	void LoadTextures(const std::string& albedoPath, 
		const std::string& normalPath, const std::string& roughnessPath,
		const std::string& metallicPath, const std::string& AOPath);
	void Assign();
	void Unassign();
	int GetSurfaceTypeID() { return mSurfaceTypeID; }
protected:
	glm::vec3 mAlbedoColor = glm::vec3(0.0f);
	float mMetallicColor = 0.0f, mRoughnessColor = 0.0f, mAOColor = 0.0f;
	int mSurfaceTypeID = 0;
	MBasicPlatform* gPlatform = NULL;
	MMaterial* mParent = NULL;
	std::vector<MTexture*> mTextures;
	bool mUseDefaultNormal = false;
	bool mUseForwardPipeline = false;
};

class MMaterial :public MAsset {
public:
	MMaterial(MBasicPlatform* gPlatform, const std::string& path);
	~MMaterial();
	MSubMaterial* GetSubMaterial(const unsigned int ID);
	void LoadMaterial(const std::string& path);
	void Assign(const unsigned int ID);
	void Unassign(const unsigned int ID);
protected:
	MBasicPlatform* gPlatform = NULL;
	tinyxml2::XMLDocument mDocument;
	std::unordered_map<unsigned int, MSubMaterial*> mSubMaterialTable;
};