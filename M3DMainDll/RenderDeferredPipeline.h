#pragma once
#include <glm/glm.hpp>
#include "RenderPipeline.h"
#include "RenderForwardSubPipeline.h"
#include "GeometryInstance.h"

class MRenderPipeline;
class MShaderProgram;
class MBasicPlatform;
class MRenderForwardPipeline;
struct MGeometryInstance;

class MRenderDeferredPipeline :public MRenderPipeline {
public:
	MRenderDeferredPipeline(MBasicPlatform* gPlatform);
	~MRenderDeferredPipeline();
	virtual void UpdateFramebufferSize()override;
	virtual void RenderQueueGeometryInstances()override;
	virtual void SendMatricesToShader()override;
	virtual void DisableColorMask()override;
	virtual void EnableColorMask()override;
	void AddGeometryInstanceToQueue(MGeometryInstance mesh);
	void ClearGeometryInstanceQueue();
	void CopyForwardDepthBufferToDeferredDepthBuffer();
	void ClearBuffers();
	void SetLightingPassSamplerUnit(const std::string& name, const int unit);
	void SetLightSpaceMatrix(const glm::mat4& matrix);
	void SetGeometryPassInteger(const std::string& name, const int unit);
	void SetGeometryPassVec2(const std::string& name, const glm::vec2& vec);
	void SetGeometryPassVec3(const std::string& name, const glm::vec3& vec);
	void SetGeometryPassFloat(const std::string& name, const float v);
	void SetLightingPassVec2(const std::string& name, const glm::vec2& vec);
	void SetLightingPassVec3(const std::string& name, const glm::vec3& vec);
	void SetDirectionalLight(const glm::vec3& dir, const glm::vec3& color);
	unsigned int GetDepthBuffer() { return gDepthBuffer; }
	//void SetSpotLight();
	//void SetPointLight();
	void DoLightingPass();
	void DoIndirectLightingPass();
	void DoCompositionPass();
	void RenderNDCQuad();
protected:
	MShaderProgram* mGeometryPassShader = NULL;
	MShaderProgram* mLightingPassShader = NULL;
	MShaderProgram* mIndirectLightingPassShader = NULL;
	MShaderProgram* mCompositionPassShader = NULL;
	GLuint gIndirectLightingResultChannel = 0;
	GLuint gLightingResultChannel = 0;
	GLuint gIndirectLightingBuffer = 0;
	GLuint gLightingBuffer = 0;
	GLuint gGeomBuffer = 0;
	GLuint gAlbedoChannel = 0;
	GLuint gPositionChannel = 0;
	GLuint gNormalChannel = 0;
	GLuint gMRAChannel = 0;
	GLuint gDepthBuffer = 0;
	std::vector<MGeometryInstance> gGeometryInstanceQueue;
	//SSAO
	void InitializeIndirectLighting();
	GLuint gNoiseTex = 0;
	std::vector<glm::vec3> gSSAOKernel;
};