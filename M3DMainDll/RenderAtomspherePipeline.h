#pragma once
#include <glm/glm.hpp>
#include "RenderPipeline.h"

class MBasicPlatform;

class MRenderAtomspherePipeline :public MRenderPipeline {
public:
	MRenderAtomspherePipeline(MBasicPlatform* platform);
	~MRenderAtomspherePipeline();
	void AddNewShader(const std::string& name, const std::string& path);
	GLuint GetAtomsphereRenderingResultTextureID() { return mAtomsphereResult; }
	GLuint GetForwardRenderingFramebufferID() { return mAtomsphereFrambuffer; }
	void SelectShader(const std::string& shader);
	void ClearBuffers();
	MShaderProgram* GetCurrentAtomsphereShader() { return gCurrentAtomsphereShader; }
	virtual void UpdateFramebufferSize()override;
	virtual void RenderQueueGeometryInstances()override;
	virtual void SendMatricesToShader()override;
	virtual void DisableColorMask()override;
	virtual void EnableColorMask()override;
protected:
	GLuint mAtomsphereFrambuffer = 0;
	GLuint mAtomsphereResult = 0;
	GLuint mDepthBuffer = 0;
	MShaderProgram* gCurrentAtomsphereShader = NULL;
	std::vector<MShaderProgram*>mShaders;
	std::unordered_map<std::string, MShaderProgram*>mShaderTable;
	void RenderSphere();
};