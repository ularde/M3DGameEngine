#pragma once
#include "RenderPipeline.h"
#include "GeometryInstance.h"
#include <vector>

class MRenderPipeline;
class MShaderProgram;
class MBasicPlatform;
struct MGeometryInstance;

class MRenderDepthMappingPipeline :public MRenderPipeline {
public:
	MRenderDepthMappingPipeline(MBasicPlatform* platform);
	~MRenderDepthMappingPipeline();
	virtual void UpdateFramebufferSize() { }
	virtual void DisableColorMask()override;
	virtual void EnableColorMask()override;
	virtual void SendMatricesToShader()override;
	virtual void RenderQueueGeometryInstances()override;
	void AddGeometryInstanceToQueue(MGeometryInstance mesh);
	void ClearGeometryInstanceQueue();
	void UpdateDepthMappingSize();
	void SetLightSpaceMatrix(const glm::mat4& matrix);
	void ClearBuffers();
	void SetModelMatrix(const glm::mat4& matrix);
	GLuint GetFramebuffer() { return mDepthFBO; }
	GLuint GetDepthTextureID() { return mDepthTex; }
protected:
	int gDepthMapWidth = 1024, gDepthMapHeight = 1024;
	MShaderProgram* mShader = NULL;
	std::vector<MGeometryInstance> gGeometryInstanceQueue;
	GLuint mDepthFBO, mDepthTex;
};