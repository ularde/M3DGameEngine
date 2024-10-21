#pragma once
#include "Class.h"
#include "ShaderManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <PxPhysics.h>

class MShaderProgram;

enum class MMatrixType {
	PROJECTION,
	VIEW,
	MODEL,
	TEXTURE
};

class MRenderPipeline :public MUnscriptableObject {
public:
	void SetCameraPosition(const glm::vec3& pos) { gCamPos = pos; }
	void LoadIdentity(MMatrixType type);
	void LoadMatrix(MMatrixType type, const glm::mat4& matrix);
	void MultMatrix(MMatrixType type, const glm::mat4& matrix);
	void PushMatrix(MMatrixType type);
	void PopMatrix(MMatrixType type);
	virtual void UpdateFramebufferSize() = 0;
	virtual void DisableColorMask() = 0;
	virtual void EnableColorMask() = 0;
	virtual void SendMatricesToShader() = 0;
	virtual void RenderQueueInstances() = 0;
protected:
	glm::vec3 gCamPos;
	MBasicPlatform* gPlatform = NULL;
	glm::mat4 gProjectionStack = glm::mat4(1.0f),
		gCurrentProjection = glm::mat4(1.0f);
	glm::mat4 gViewStack = glm::mat4(1.0f),
		gCurrentView = glm::mat4(1.0f);
	glm::mat4 gModelStack = glm::mat4(1.0f),
		gCurrentModel = glm::mat4(1.0f);
};