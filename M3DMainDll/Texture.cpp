#include "pch.h"
#include "Texture.h"
#include "AssetManager.h"

MTexture::MTexture(bool immediately_load, MAssetManager* asset_manager, const std::string& guid, const std::string& tex_type, const std::string& tex_path) {
	this->gAssetManager = asset_manager;
	this->superType = M3D_ASSET_TEXTURE;
	this->path = tex_path;
	this->type = tex_type;
	this->GUID = guid;
	//generate texture
	glGenTextures(1, &this->texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	if (immediately_load) {
		this->LoadTexture();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

MTexture::~MTexture()
{
}

unsigned int MTexture::GetTextureID() {
	if (mLoaded) {
		return this->texID;
	}
	else {
		this->LoadTexture();
		return this->texID;
	}
}

unsigned char* MTexture::GetTextureData() {
	if (mLoaded) {
		return this->texData;
	}
	else {
		this->LoadTexture();
		return this->texData;
	}
}

void MTexture::LoadTexture() {
	//加载纹理到RAM
	this->texData = stbi_load(GetFullAssetPathA(this->path), &this->width, &this->height, &this->channelCount, 0);
	if (this->texData) {
		GLenum format = GL_RGB;
		switch (this->channelCount)
		{
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			break;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format, this->width, this->height, 0, format, GL_UNSIGNED_BYTE, this->texData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else if (!this->texData && this->path != "") {
		printf("Failed to load texture %s\n", this->path.c_str());
	}
	//release data from RAM
	stbi_image_free(this->texData);
	this->mLoaded = true;
}

MCubemap::MCubemap(bool immediately_load, MAssetManager* asset_manager, 
	const std::string& name_, const std::string& guid,
	const std::string& right_, const std::string& left_,
	const std::string& top_, const std::string& bottom_,
	const std::string& back_, const std::string& front_) {
	this->name = name_;
	this->gAssetManager = asset_manager;
	this->GUID = guid;
	this->texPaths[0] = right_;
	this->texPaths[1] = left_;
	this->texPaths[2] = top_;
	this->texPaths[3] = bottom_;
	this->texPaths[4] = back_;
	this->texPaths[5] = front_;
	//生成立方体纹理
	glGenTextures(1, &this->texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->texID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	if (immediately_load) {
		this->LoadCubemap();
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void MCubemap::LoadCubemap() {
	for (unsigned int i = 0; i < 6; i++) {
		unsigned char* data = stbi_load(GetFullAssetPathA(this->texPaths[i]), &width[i], &height[i], &channelCount[i], 0);

		GLenum format = 0;

		if (data) {
			switch (channelCount[i])
			{
			case 1:
				format = GL_RED;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			default:
				break;
			}
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width[i], height[i], 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			stbi_image_free(data);
		}
	}
}
