#include "pch.h"
#include "AssetManager.h"
#include "Material.h"
#include "Texture.h"

#include "RenderDeferredPipeline.h"
#include "RenderAtomspherePipeline.h"

class MRenderDeferredPipeline;
class MRenderAtomspherePipeline;

MMaterial::MMaterial(MBasicPlatform* platform, const std::string& path) {
	gPlatform = platform;
	LoadMaterial(path);
}

MMaterial::~MMaterial()
{
}

MSubMaterial* MMaterial::GetSubMaterial(const unsigned int index) {
	if (mSubMaterialTable[index] == NULL) {
		if (mSubMaterialTable[0]) {
			mSubMaterialTable[index] = mSubMaterialTable[0];
			return mSubMaterialTable[index];
		}
		else if (mSubMaterialTable[1]) {
			mSubMaterialTable[index] = mSubMaterialTable[1];
			return mSubMaterialTable[index];
		}
		else {
			MessageBox(NULL, L"Material failure.", L"ENGINE", MB_ICONHAND);
		}
	}
	else {
		return mSubMaterialTable[index];
	}
	return 0;
}

void MMaterial::LoadMaterial(const std::string& path) {
	if (mDocument.LoadFile(GetFullAssetPathA(path)) != tinyxml2::XML_SUCCESS) {
		printf("Failed to load material %s\n", path.c_str());

		MSubMaterial* subMaterial = new MSubMaterial(gPlatform, this, 0, false, 0,
			"Textures\\default\\default_albedo.png", "Textures\\default\\default_normal.png",
			"Textures\\default\\default_roughness.png", "Textures\\default\\default_metallic.png",
			"Textures\\default\\default_ao.png");
		mSubMaterialTable[0] = subMaterial;
	}
	else {
		tinyxml2::XMLElement* root = mDocument.RootElement();
		tinyxml2::XMLElement* subMaterialNode = root->FirstChildElement("SubMaterial");

		//Find sub materials
		while (subMaterialNode) {
			std::string albedoPath, normalPath,
				roughnessPath, metallicPath, AOPath,
				opacityPath,
				albedoColorStr;
			glm::vec3 albedoColor(0.0f);
			float metallicColor, roughnessColor, aoColor, opacityColor;

			bool useColor = subMaterialNode->BoolAttribute("use_color");
			bool useForward = subMaterialNode->IntAttribute("forward_pipeline");
			unsigned int index = subMaterialNode->UnsignedAttribute("index");
			std::string name = subMaterialNode->Attribute("name");

			std::string surfaceType = subMaterialNode->Attribute("surface_type");
			int surfaceTypeID = 0;

			if (surfaceType == "metal") {
				surfaceTypeID = M3D_SURFACE_TYPE_METAL;
			}
			else if (surfaceType == "stone") {
				surfaceTypeID = M3D_SURFACE_TYPE_STONE;
			}
			else if (surfaceType == "mud") {
				surfaceTypeID = M3D_SURFACE_TYPE_MUD;
			}
			else if (surfaceType == "wood") {
				surfaceTypeID = M3D_SURFACE_TYPE_WOOD;
			}
			else if (surfaceType == "plastic") {
				surfaceTypeID = M3D_SURFACE_TYPE_PLASTIC;
			}

			//Find textures
			tinyxml2::XMLElement* textureNode = subMaterialNode->FirstChildElement("Texture");

			while (textureNode) {
				std::string type, path;

				type = textureNode->Attribute("type");
				path = textureNode->Attribute("path");

				if (type == "texture_albedo") {
					albedoPath = path;
					if (useColor) {
						albedoColorStr = textureNode->Attribute("color");
						int n = GetVec3FromString(albedoColorStr.c_str(), &albedoColor.r, &albedoColor.g, &albedoColor.b);
						assert(n == 3);
					}
				}
				else if (type == "texture_normal") {
					normalPath = path;
				}
				else if (type == "texture_roughness") {
					roughnessPath = path;
					if (useColor) {
						roughnessColor = textureNode->FloatAttribute("color");
					}
				}
				else if (type == "texture_metallic") {
					metallicPath = path;
					if (useColor) {
						metallicColor = textureNode->FloatAttribute("color");
					}
				}
				else if (type == "texture_AO") {
					AOPath = path;
					if (useColor) {
						aoColor = textureNode->FloatAttribute("color");
					}
				}
				else if (type == "texture_opacity") {
					opacityPath = path;
					if (useColor) {
						opacityColor = textureNode->FloatAttribute("color");
					}
				}

				textureNode = textureNode->NextSiblingElement();
			}

			if (!useColor) {
				if (!useForward) {
					MSubMaterial* subMaterial = new MSubMaterial(gPlatform, this, index,
						useForward, surfaceTypeID, albedoPath, normalPath,
						roughnessPath, metallicPath, AOPath);
					assert(subMaterial);
					mSubMaterialTable[index] = subMaterial;
				}
				else {
					MSubMaterial* subMaterial = new MSubMaterial(gPlatform, this, index,
						useForward, surfaceTypeID, albedoPath, normalPath,
						roughnessPath, metallicPath, AOPath, opacityPath);
					assert(subMaterial);
					mSubMaterialTable[index] = subMaterial;
				}
			}
			else {
				if (!useForward) {
					MSubMaterial* subMaterial = new MSubMaterial(gPlatform, this, index,
						useForward, surfaceTypeID, albedoPath, normalPath,
						roughnessPath, metallicPath, AOPath, albedoColor,
						metallicColor, roughnessColor, aoColor);
					assert(subMaterial);
					mSubMaterialTable[index] = subMaterial;
				}
				else {
					MSubMaterial* subMaterial = new MSubMaterial(gPlatform, this, index,
						useForward, surfaceTypeID, albedoPath, normalPath,
						roughnessPath, metallicPath, AOPath, opacityPath, albedoColor,
						metallicColor, roughnessColor, aoColor, opacityColor);
					assert(subMaterial);
					mSubMaterialTable[index] = subMaterial;
				}
			}

			subMaterialNode = subMaterialNode->NextSiblingElement();
		}
	}
}

void MMaterial::Assign(const unsigned int index) {
	if (mSubMaterialTable[index]) {
		mSubMaterialTable[index]->Assign();
	}
	else if (mSubMaterialTable[0]) {
		mSubMaterialTable[0]->Assign();
	}
}

void MMaterial::Unassign(const unsigned int index) {
	if (mSubMaterialTable[index]) {
		mSubMaterialTable[index]->Unassign();
	}
	else if (mSubMaterialTable[0]) {
		mSubMaterialTable[0]->Unassign();
	}
}

MSubMaterial::MSubMaterial(MBasicPlatform* platform, MMaterial* parent, const unsigned int ID, const bool useForward, int surfaceTypeID, const std::string& albedoPath, const std::string& normalPath, const std::string& roughnessPath, const std::string& metallicPath, const std::string& AOPath) {
	gPlatform = platform;
	mParent = parent;
	mUseForwardPipeline = useForward;
	mSurfaceTypeID = surfaceTypeID;
	LoadTextures(albedoPath, normalPath, roughnessPath, metallicPath, AOPath);
}

MSubMaterial::MSubMaterial(MBasicPlatform* platform, MMaterial* parent, const unsigned int ID, const bool useForward, int surfaceTypeID, const std::string& albedoPath, const std::string& normalPath, const std::string& roughnessPath, const std::string& metallicPath, const std::string& AOPath, const glm::vec3& albedoColor, const float metallicColor, const float roughnessColor, const float aoColor) {
	gPlatform = platform;
	mParent = parent;
	mUseForwardPipeline = useForward;
	mSurfaceTypeID = surfaceTypeID;
	mAlbedoColor = albedoColor;
	mMetallicColor = metallicColor;
	mRoughnessColor = roughnessColor;
	mAOColor = aoColor;
	mUseDefaultNormal = true;
	LoadTextures(albedoPath, normalPath, roughnessPath, metallicPath, AOPath);
}

MSubMaterial::MSubMaterial(MBasicPlatform* platform, MMaterial* parent, const unsigned int ID, const bool useForward, int surfaceTypeID, const std::string& albedoPath, const std::string& normalPath, const std::string& roughnessPath, const std::string& metallicPath, const std::string& AOPath, const std::string& opacityPath) {
	gPlatform = platform;
	mParent = parent;
	mUseForwardPipeline = useForward;
	mSurfaceTypeID = surfaceTypeID;
	LoadTextures(albedoPath, normalPath, roughnessPath, metallicPath, AOPath, opacityPath);
}

MSubMaterial::MSubMaterial(MBasicPlatform* platform, MMaterial* parent, const unsigned int ID, const bool useForward, int surfaceTypeID, const std::string& albedoPath, const std::string& normalPath, const std::string& roughnessPath, const std::string& metallicPath, const std::string& AOPath, const std::string& opaacityPath, const glm::vec3& albedoColor, const float metallicColor, const float roughnessColor, const float aoColor, const float opacityColor) {
	gPlatform = platform;
	mParent = parent;
	mUseForwardPipeline = useForward;
	mSurfaceTypeID = surfaceTypeID;
	mAlbedoColor = albedoColor;
	mMetallicColor = metallicColor;
	mRoughnessColor = roughnessColor;
	mAOColor = aoColor;
	mOpacityColor = opacityColor;
	mUseDefaultNormal = true;
	LoadTextures(albedoPath, normalPath, roughnessPath, metallicPath, AOPath);
}

MSubMaterial::~MSubMaterial()
{
}

void MSubMaterial::LoadTextures(const std::string& albedoPath, const std::string& normalPath, const std::string& roughnessPath, const std::string& metallicPath, const std::string& AOPath) {
	MTexture* textures[5] = { NULL };
	textures[0] = gPlatform->gAssetManager->AddTexture(true, "texture_albedo", albedoPath, (char*)GenerateGuidA());
	textures[1] = gPlatform->gAssetManager->AddTexture(true, "texture_normal", normalPath, (char*)GenerateGuidA());
	textures[2] = gPlatform->gAssetManager->AddTexture(true, "texture_metallic", metallicPath, (char*)GenerateGuidA());
	textures[3] = gPlatform->gAssetManager->AddTexture(true, "texture_roughness", roughnessPath, (char*)GenerateGuidA());
	textures[4] = gPlatform->gAssetManager->AddTexture(true, "texture_AO", AOPath, (char*)GenerateGuidA());
	for (int i = 0; i < 5; i++) {
		mTextures.push_back(textures[i]);
	}
}

void MSubMaterial::LoadTextures(const std::string& albedoPath, const std::string& normalPath, const std::string& roughnessPath, const std::string& metallicPath, const std::string& AOPath, const std::string& opacityPath) {
	MTexture* textures[6] = { NULL };
	textures[0] = gPlatform->gAssetManager->AddTexture(true, "texture_albedo", albedoPath, (char*)GenerateGuidA());
	textures[1] = gPlatform->gAssetManager->AddTexture(true, "texture_normal", normalPath, (char*)GenerateGuidA());
	textures[2] = gPlatform->gAssetManager->AddTexture(true, "texture_metallic", metallicPath, (char*)GenerateGuidA());
	textures[3] = gPlatform->gAssetManager->AddTexture(true, "texture_roughness", roughnessPath, (char*)GenerateGuidA());
	textures[4] = gPlatform->gAssetManager->AddTexture(true, "texture_AO", AOPath, (char*)GenerateGuidA());
	textures[5] = gPlatform->gAssetManager->AddTexture(true, "texture_opacity", opacityPath, (char*)GenerateGuidA());
	for (int i = 0; i < 6; i++) {
		mTextures.push_back(textures[i]);
	}
}

void MSubMaterial::Assign() {
	if (mUseForwardPipeline) {
		gPlatform->gForwardPipeline->SetTransparentLightingShaderVec3("albedo_color", mAlbedoColor);
		gPlatform->gForwardPipeline->SetTransparentLightingShaderFloat("metallic_color", mMetallicColor);
		gPlatform->gForwardPipeline->SetTransparentLightingShaderFloat("roughness_color", mRoughnessColor);
		gPlatform->gForwardPipeline->SetTransparentLightingShaderFloat("ao_color", mAOColor);
		gPlatform->gForwardPipeline->SetTransparentLightingShaderFloat("opacity_color", mOpacityColor);
		if (mUseDefaultNormal) {
			gPlatform->gForwardPipeline->SetTransparentLightingShaderInteger("use_default_norm", 1);
		}
		else {
			gPlatform->gForwardPipeline->SetTransparentLightingShaderInteger("use_default_norm", 0);
		}
	}
	else {
		gPlatform->gDeferredPipeline->SetGeometryPassVec3("albedo_color", mAlbedoColor);
		gPlatform->gDeferredPipeline->SetGeometryPassFloat("metallic_color", mMetallicColor);
		gPlatform->gDeferredPipeline->SetGeometryPassFloat("roughness_color", mRoughnessColor);
		gPlatform->gDeferredPipeline->SetGeometryPassFloat("ao_color", mAOColor);
		if (mUseDefaultNormal) {
			gPlatform->gDeferredPipeline->SetGeometryPassInteger("use_default_norm", 1);
		}
		else {
			gPlatform->gDeferredPipeline->SetGeometryPassInteger("use_default_norm", 0);
		}
	}
	for (unsigned int i = 0; i < mTextures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		gPlatform->gDeferredPipeline->SetGeometryPassInteger(mTextures[i]->GetType(), i);
		glBindTexture(GL_TEXTURE_2D, mTextures[i]->GetTextureID());
	}
}

void MSubMaterial::Unassign() {
	for (unsigned int i = 0; i < mTextures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
