#include "pch.h"
#include "MatrixManager.h"

MMatrixManager::MMatrixManager(MBasicPlatform* platform__, unsigned int shaderPipeline__) {
	this->platform = platform__;
	this->shaderPipeline = shaderPipeline__;

}

MMatrixManager::~MMatrixManager() {
	
}

void MMatrixManager::Perspective(float fovy, float aspect, float zNear, float zFar) {
	glm::mat4 matrix = glm::perspective(fovy, aspect, zNear, zFar);

}
