#include "pch.h"
#include "Model.h"
#include "Material.h"
#include "RenderDepthMappingPipeline.h"

MModel::MModel(bool immediately_load, MAssetManager* asset_manager, const std::string& guid, const std::string& model_type, const std::string& model_path) {
	superType = M3D_ASSET_MODEL;
	gAssetManager = asset_manager;
	GUID = guid;
	type = model_type;
	path = model_path;
	directory = path.substr(0, path.find_last_of('\\'));
	std::string name_only = path.substr(0, path.find_last_of('.'));
	mMaterialPath = name_only + ".mmtl";
	gPlatform = gAssetManager->GetPlatform();
	LoadMaterial();
	LoadModel();
}

MModel::~MModel() {
	if (mLoaded) {
		for (unsigned int i = 0; i < mMeshes.size(); i++) {
			mMeshes[i].RenderForDepthMapping();
		}
	}
	else {
		LoadModel();
	}
}

glm::vec3 MModel::GetMaxXYZ() {
	if (mLoaded) {
		std::vector<float> xarray, yarray, zarray;
		for (unsigned int i = 0; i < mMeshes.size(); i++) {
			std::vector<glm::vec3> positions = mMeshes[i].GetVertexPositions();
			for (unsigned int i = 0; i < positions.size(); i++) {
				xarray.push_back(positions[i].x);
				yarray.push_back(positions[i].y);
				zarray.push_back(positions[i].z);
			}
		}
		float x = *std::max_element(xarray.begin(), xarray.end()) + abs(*std::min_element(xarray.begin(), xarray.end()));
		float y = *std::max_element(yarray.begin(), yarray.end()) + abs(*std::min_element(yarray.begin(), yarray.end()));
		float z = *std::max_element(zarray.begin(), zarray.end()) + abs(*std::min_element(zarray.begin(), zarray.end()));
		return glm::vec3(x, y, z);
	}
	else {
		LoadModel();
		return GetMaxXYZ();
	}
}

glm::vec3 MModel::GetOffsetXYZ() {
	if (mLoaded) {
		std::vector<float> xarray, yarray, zarray;
		for (unsigned int i = 0; i < mMeshes.size(); i++) {
			std::vector<glm::vec3> positions = mMeshes[i].GetVertexPositions();
			for (unsigned int i = 0; i < positions.size(); i++) {
				xarray.push_back(positions[i].x);
				yarray.push_back(positions[i].y);
				zarray.push_back(positions[i].z);
			}
		}
		float x = abs(*std::min_element(xarray.begin(), xarray.end()));
		float y = abs(*std::min_element(yarray.begin(), yarray.end()));
		float z = abs(*std::min_element(zarray.begin(), zarray.end()));
		return glm::vec3(x, y, z);
	}
	else {
		LoadModel();
		return GetOffsetXYZ();
	}
}

void MModel::Render() {
	if (mLoaded) {
		for (unsigned int i = 0; i < mMeshes.size(); i++) {
			if (mUseCustomMaterial) {
				mMaterial->Assign(mMeshes[i].GetMaterialIndex());
			}
			else {
				mInitialMaterial->Assign(mMeshes[i].GetMaterialIndex());
			}
			mMeshes[i].Render();
			if (mUseCustomMaterial) {
				mMaterial->Unassign(mMeshes[i].GetMaterialIndex());
			}
			else {
				mInitialMaterial->Unassign(mMeshes[i].GetMaterialIndex());
			}
		}
		//if (mMaterial != mInitialMaterial) {
		//	mMaterial = mInitialMaterial;
		//}
	}
	else {
		LoadModel();
	}
}

void MModel::RenderForDepthMapping() {
	if (mLoaded) {
		for (unsigned int i = 0; i < mMeshes.size(); i++) {
			mMeshes[i].RenderForDepthMapping();
		}
	}
	else {
		LoadModel();
	}
}

void MModel::ProcessNode(aiNode* node) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = mAssimpScene->mMeshes[node->mMeshes[i]];
		mMeshes.push_back(GenerateMMesh(mesh));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i]);
	}
}

MMesh MModel::GenerateMMesh(aiMesh* mesh) {
	std::vector<MMeshVertex> vertices;
	std::vector<unsigned int> indices;
	//position, normal, tex coords, tangent, bitangent of vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		MMeshVertex* vertex = (MMeshVertex*)malloc(sizeof(MMeshVertex));

		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex->position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex->normal = vector;

		if (mesh->mTextureCoords[0]) {
			glm::vec2 vector2;
			vector2.x = mesh->mTextureCoords[0][i].x;
			vector2.y = mesh->mTextureCoords[0][i].y;
			vertex->texCoord = vector2;

			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex->tangent = vector;

			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex->bitangent = vector;
		}
		else {
			vertex->texCoord = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(*vertex);
	}
	//indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < 3; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	MSubMaterial* s_mat = mMaterial->GetSubMaterial(mesh->mMaterialIndex);
	int surface_type_ID = s_mat->GetSurfaceTypeID();

	return MMesh(gAssetManager->GetPlatform(), vertices, indices, mesh->mMaterialIndex, surface_type_ID);
}

void MModel::LoadModel() {
	Assimp::Importer* importer = &(gAssetManager->GetPlatform()->assimpImporter);
	mAssimpScene = importer->ReadFile(GetFullAssetPathA(path), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

	if (!mAssimpScene || mAssimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !mAssimpScene->mRootNode) {
		std::string error = importer->GetErrorString();

		gAssetManager->GetPlatform()->ConsoleWriteLine(1.0f, 0.0f, 0.0f, std::string("[ERROR] Failed to load model ") + path);
		gAssetManager->GetPlatform()->ConsoleWriteLine(1.0f, 0.0f, 0.0f, error);
	}

	ProcessNode(mAssimpScene->mRootNode);

	meshCount = mMeshes.size();

	InitializePhysics();

	mLoaded = true;
}

void MModel::LoadMaterial() {
	mMaterial = gAssetManager->AddMaterial("model_material", mMaterialPath, (char*)GenerateGuidA());
	mInitialMaterial = mMaterial;
}

void MModel::SetMaterial(MMaterial* mat) {
	mMaterial = mat;
}

void MModel::InitializePhysics() {
	for (unsigned int i = 0; i < meshCount; i++) {
		if (mMeshes[i].GetTriangleMesh()) {
			pTriangleMeshes.push_back(mMeshes[i].GetTriangleMesh());
		}
		else {
			pTriangleMeshes.push_back(NULL);
		}
	}
}

int MModel::GetMeshSurfaceTypeID(unsigned int ID) {
	return mMaterial->GetSubMaterial(mMeshes[ID].GetMaterialIndex())->GetSurfaceTypeID();
}
