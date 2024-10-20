#pragma once
#include <glm/glm.hpp>
#include "RenderPipeline.h"

class MBasicPlatform;

class MRenderGuiPipeline :public MRenderPipeline {
public:
	MRenderGuiPipeline(MBasicPlatform* platform);
	~MRenderGuiPipeline();
	MShaderProgram* GetGuiShader() { return mGuiShader; }
	virtual void UpdateFramebufferSize()override;
	virtual void RenderQueueGeometryInstances()override;
	virtual void SendMatricesToShader()override;
	virtual void DisableColorMask()override;
	virtual void EnableColorMask()override;
	void BeginRendering();
	void EndRendering();
	void SetColor(const glm::vec3& color);
protected:
	MShaderProgram* mGuiShader = NULL;
	std::vector<MShaderProgram*>mShaders;
	std::unordered_map<std::string, MShaderProgram*>mShaderTable;
};