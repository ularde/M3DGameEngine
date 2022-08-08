#pragma once
#include <vector>
#include <map>
#include "glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platforms.h"

class MBasicPlatform;

class MMatrixManager {
public:
	MMatrixManager(MBasicPlatform* platform__, unsigned int shaderPipeline__);
	~MMatrixManager();
	void PushMatrix();
	void PopMatrix();

	void Translate(float x, float y, float z);
	void Rotate(float angle, float x, float y, float z);
	void Perspective(float fovy, float aspect, float zNear, float zFar);
	//void LookAt

	void LoadProjectionMatrix(glm::mat4 matrix);
	void LoadModelMatrix(glm::mat4 matrix);
	void LoadViewMatrix(glm::mat4 matrix);
private:
	MBasicPlatform* platform = NULL;
	unsigned int basicVertShader = 0;
	unsigned int shaderPipeline = 0;
};