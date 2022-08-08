#include "pch.h"
#include "Texture.h"
#include "AssetManager.h"

MTexture::MTexture(bool immediately_load, MAssetManager* asset_manager, const std::string& guid, const std::string& tex_type, const std::string& tex_path) {
	this->superType = M3D_ASSET_TEXTURE;
	//生成纹理
	glGenTextures(1, &this->texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (immediately_load) {
		this->LoadTexture();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

MTexture::~MTexture()
{
}

void MTexture::LoadTexture() {
	//加载纹理到RAM
	this->texData = stbi_load(GetFullAssetPathA(this->path), &this->width, &this->height, &this->channelCount, 0);
	if (this->texData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->texData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	//从RAM释放纹理数据
	stbi_image_free(this->texData);
	this->loaded = true;
}
