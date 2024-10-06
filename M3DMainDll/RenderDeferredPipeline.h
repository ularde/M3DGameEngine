#pragma once
#include <glm/glm.hpp>
#include "RenderPipeline.h"

class MRenderPipeline;
class MShaderProgram;
class MBasicPlatform;

class MRenderDeferredPipeline :public MRenderPipeline {
public:
	MRenderDeferredPipeline(MBasicPlatform* gPlatform);
	~MRenderDeferredPipeline();
	virtual void UpdateFramebufferSize()override;
	virtual void BeginRendering()override;
	virtual void EndRendering()override;
	virtual void SendMatricesToShader()override;
	virtual void DisableColorMask()override;
	virtual void EnableColorMask()override;
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
	//void SetSpotLight();
	//void SetPointLight();
	void DoLightingPass();
	void RenderNDCQuad();
protected:
	MShaderProgram* mGeometryPassShader = NULL;
	MShaderProgram* mLightingPassShader = NULL;
	GLuint gGeomBuffer = 0;
	GLuint gAlbedoChannel = 0;
	GLuint gPositionChannel = 0;
	GLuint gNormalChannel = 0;
	GLuint gMRAChannel = 0;
	GLuint gDepthBuffer = 0;
	//SSAO
	void InitializeSSAO();
	GLuint gNoiseTex = 0;
	std::vector<glm::vec3> gSSAOKernel;
};