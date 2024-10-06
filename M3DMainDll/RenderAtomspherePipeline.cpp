#include "pch.h"
#include "RenderAtomspherePipeline.h"
#include "Platforms.h"
#include "ShaderManager.h"
#include "RenderDeferredPipeline.h"

MRenderAtomspherePipeline::MRenderAtomspherePipeline(MBasicPlatform* platform) {
	gPlatform = platform;

	MShaderProgram* atmosphereShader = new MShaderProgram(gPlatform);
	atmosphereShader->AttachShader(MShaderType::VERTEX, "Shaders\\atmosphere.vert");
	atmosphereShader->AttachShader(MShaderType::FRAGMENT, "Shaders\\atmosphere.frag");
	atmosphereShader->LinkProgram();
	mShaders.push_back(atmosphereShader);
	mShaderTable["atmosphere"] = atmosphereShader;

	glGenFramebuffers(1, &mAtomsphereFrambuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mAtomsphereFrambuffer);

	glGenTextures(1, &mAtomsphereResult);
	glBindTexture(GL_TEXTURE_2D, mAtomsphereResult);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gPlatform->gAtmosphereFramebufferWidth, gPlatform->gAtmosphereFramebufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mAtomsphereResult, 0);
	
	glGenRenderbuffers(1, &mDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, mDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, gPlatform->gAtmosphereFramebufferWidth, gPlatform->gAtmosphereFramebufferHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer incomplete.\n");
	}

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, gPlatform->gDefaultFBO);
	glBindTexture(GL_TEXTURE_2D, 0);
}

MRenderAtomspherePipeline::~MRenderAtomspherePipeline()
{
}

void MRenderAtomspherePipeline::AddNewShader(const std::string& name, const std::string& path) {
	if (!mShaderTable[name]) {
		MShaderProgram* shader = new MShaderProgram(gPlatform);
		shader->AttachShader(MShaderType::VERTEX, "Shaders\\forward_default.vert");
		shader->AttachShader(MShaderType::FRAGMENT, path);
		shader->LinkProgram();
		//add to the shader table and array
		mShaderTable[name] = shader;
		mShaders.push_back(shader);
	}
	else {
		printf("Warning! Shader %s has been occupied. Failed to add new shader from %s.\n", name.c_str(), path.c_str());
	}
}

void MRenderAtomspherePipeline::SelectShader(const std::string& name) {
	if (mShaderTable[name]) {
		gCurrentAtomsphereShader = mShaderTable[name];
	}
}

void MRenderAtomspherePipeline::ClearBuffers() {
	glBindFramebuffer(GL_FRAMEBUFFER, mAtomsphereFrambuffer);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MRenderAtomspherePipeline::UpdateFramebufferSize() {
	glBindFramebuffer(GL_FRAMEBUFFER, mAtomsphereFrambuffer);

	glBindTexture(GL_TEXTURE_2D, mAtomsphereResult);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gPlatform->gAtmosphereFramebufferWidth, gPlatform->gAtmosphereFramebufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mAtomsphereResult, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, mDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, gPlatform->gAtmosphereFramebufferWidth, gPlatform->gAtmosphereFramebufferHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, gPlatform->gDefaultFBO);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void MRenderAtomspherePipeline::BeginRendering() {
	glBindFramebuffer(GL_FRAMEBUFFER, mAtomsphereFrambuffer);
	glViewport(0, 0, gPlatform->gAtmosphereFramebufferWidth, gPlatform->gAtmosphereFramebufferHeight);
	if (gCurrentAtomsphereShader != NULL) {
		gCurrentAtomsphereShader->Use();
	}
}

void MRenderAtomspherePipeline::EndRendering() {
	glBindFramebuffer(GL_FRAMEBUFFER, gPlatform->gDefaultFBO);
	glViewport(0, 0, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight);
}

void MRenderAtomspherePipeline::SendMatricesToShader() {
	for (unsigned int i = 0; i < mShaders.size(); i++) {
		mShaders[i]->UniformMat4("projection", gCurrentProjection);
		mShaders[i]->UniformMat4("view", gCurrentView);
		mShaders[i]->UniformMat4("model", gCurrentModel);
	}
}

void MRenderAtomspherePipeline::DisableColorMask() {
	glBindFramebuffer(GL_FRAMEBUFFER, mAtomsphereFrambuffer);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
}

void MRenderAtomspherePipeline::EnableColorMask() {
	glBindFramebuffer(GL_FRAMEBUFFER, mAtomsphereFrambuffer);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}