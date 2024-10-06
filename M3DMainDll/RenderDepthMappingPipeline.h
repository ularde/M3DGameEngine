#pragma once
#include "RenderPipeline.h"

class MRenderPipeline;
class MShaderProgram;
class MBasicPlatform;

class MRenderDepthMappingPipeline :public MRenderPipeline {
public:
	MRenderDepthMappingPipeline(MBasicPlatform* platform);
	~MRenderDepthMappingPipeline();
	virtual void UpdateFramebufferSize() { }
	virtual void DisableColorMask()override;
	virtual void EnableColorMask()override;
	virtual void SendMatricesToShader()override;
	virtual void BeginRendering()override;
	virtual void EndRendering()override;
	void SetLightSpaceMatrix(const glm::mat4& matrix);
	void ClearBuffers();
	void SetModelMatrix(const glm::mat4& matrix);
	GLuint GetFramebuffer() { return mDepthFBO; }
	GLuint GetDepthTextureID() { return mDepthTex; }
protected:
	int gDepthMapWidth = 1024, gDepthMapHeight = 1024;
	MShaderProgram* mShader = NULL;
	GLuint mDepthFBO, mDepthTex;
};