#pragma once
#include <vector>
#include <string>

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "AssetManager.h"
#include "Mesh.h"
#include "Global.h"

class MAsset;
class MAssetManager;
class MMesh;
class MMaterial;

class MModel :public MAsset {
public:
	MModel(bool immediately_load, MAssetManager* asset_manager, const std::string& guid, const std::string& model_type, const std::string& model_path);
	~MModel();
	glm::vec3 GetMaxXYZ();
	glm::vec3 GetOffsetXYZ();
	std::vector<physx::PxTriangleMesh*> pTriangleMeshes;
	unsigned int GetMeshCount() { return this->meshCount; }
	void SetUseCustomMaterialFlag(bool flag) { mUseCustomMaterial = flag; }
	bool GetUseCustomMaterialFlag() { return mUseCustomMaterial; }
	void PushUseCustomMaterialFlag() { mUseCustomMaterialStack = mUseCustomMaterial; }
	void PopUseCustomMaterialFlag() { mUseCustomMaterial = mUseCustomMaterialStack; }
	void Render();
	void RenderForDepthMapping();
	void ProcessNode(aiNode* node);
	void LoadModel();
	void LoadMaterial();
	void SetMaterial(MMaterial* mat);
	void InitializePhysics();
	int GetMeshSurfaceTypeID(unsigned int ID);
	MMesh GenerateMMesh(aiMesh* mesh);
private:
	bool mUseCustomMaterial = false;
	bool mUseCustomMaterialStack = false;
	unsigned int meshCount = 0;
	const aiScene* mAssimpScene;
	std::vector<MMesh> mMeshes;
	std::string mMaterialPath;
	MMaterial* mMaterial = NULL, * mInitialMaterial = NULL;
};