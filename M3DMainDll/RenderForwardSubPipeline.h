#pragma once
#include "RenderPipeline.h"
#include "Platforms.h"
#include "GeometryInstance.h"

class MRenderPipeline;
class MBasicPlatform;
class MShaderProgram;
class MTriangleMesh;

class MRenderForwardPipeline :public MRenderPipeline {
public:
	MRenderForwardPipeline(MBasicPlatform* platform);
	~MRenderForwardPipeline();
	virtual void UpdateFramebufferSize()override;
	virtual void DisableColorMask()override;
	virtual void EnableColorMask()override;
	virtual void SendMatricesToShader()override;
	virtual void RenderQueueGeometryInstances()override;
	unsigned int GetTransparentCompositionResultChannel() { return gTransparentCompositionChannel; }
	void AddGeometryInstanceToQueue(MGeometryInstance mesh);
	void ClearGeometryInstanceQueue();
	void SetLightSpaceMatrix(const glm::mat4& matrix);
	void SetDirectionalLight(const glm::vec3& dir, const glm::vec3& color);
	void SetTransparentLightingShaderFloat(const std::string& n, const float v);
	void SetTransparentLightingShaderInteger(const std::string& n, const int v);
	void SetTransparentLightingShaderVec3(const std::string& n, const glm::vec3& v);
	void SetTransparentLightingShaderVec2(const std::string& n, const glm::vec2& v);
	void SetTransparentLightingShaderMat4(const std::string& n, const glm::mat4& v);
	void ClearBuffers();
	void RenderNDCQuad();
protected:
	std::vector<MGeometryInstance> gGeometryInstanceQueue;
	MShaderProgram* mTransparentLightingShader = NULL;
	MShaderProgram* mTransparentCompositionShader = NULL;
	GLuint gTransparentCompositionBuffer = 0;
	GLuint gTransparentCompositionChannel = 0;
	GLuint gTransparentLightingBuffer = 0;
	GLuint gAccumChannel = 0;
	GLuint gRevealChannel = 0;
};