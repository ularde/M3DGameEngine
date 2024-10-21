#include "pch.h"
#include "RenderDepthMappingPipeline.h"
#include "Platforms.h"
#include "Model.h"
#include "ShaderManager.h"

MRenderDepthMappingPipeline::MRenderDepthMappingPipeline(MBasicPlatform* platform) {
	gPlatform = platform;
	mShader = new MShaderProgram(gPlatform);
	mShader->AttachShader(MShaderType::VERTEX, "Shaders\\depth_mapping.vert");
	mShader->AttachShader(MShaderType::FRAGMENT, "Shaders\\depth_mapping.frag");
	mShader->LinkProgram();

	gDepthMapWidth = 1024 * gPlatform->gShadowQuality;
	gDepthMapHeight = 1024 * gPlatform->gShadowQuality;

	glBindFramebuffer(GL_FRAMEBUFFER, gPlatform->gDefaultFBO);
	glGenFramebuffers(1, &this->mDepthFBO);
	glGenTextures(1, &this->mDepthTex);

	glBindTexture(GL_TEXTURE_2D, this->mDepthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, gDepthMapWidth, gDepthMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glBindFramebuffer(GL_FRAMEBUFFER, this->mDepthFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->mDepthTex, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		MessageBox(NULL, L"Failed to create framebuffer for shadow mapping.", L"ERROR", MB_ICONERROR);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, gPlatform->gDefaultFBO);
}

MRenderDepthMappingPipeline::~MRenderDepthMappingPipeline() {

}

void MRenderDepthMappingPipeline::DisableColorMask()
{
}

void MRenderDepthMappingPipeline::EnableColorMask()
{
}

void MRenderDepthMappingPipeline::SendMatricesToShader() {
	mShader->UniformMat4("projection", gCurrentProjection);
	mShader->UniformMat4("view", gCurrentView);
	mShader->UniformMat4("model", gCurrentModel);
}

void MRenderDepthMappingPipeline::RenderQueueInstances() {
	glBindFramebuffer(GL_FRAMEBUFFER, mDepthFBO);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	mShader->Use();
	glViewport(0, 0, gDepthMapWidth, gDepthMapHeight);

	for (auto i = 0u; i < gRenderInstanceQueue.size(); i++) {
		if (gRenderInstanceQueue[i].mOwner->IsRigidStatic()) {
			MTriangleMesh* mesh = reinterpret_cast<MTriangleMesh*>(gRenderInstanceQueue[i].mShape);
			MRigidStatic* rs = reinterpret_cast<MRigidStatic*>(gRenderInstanceQueue[i].mOwner);
			LoadMatrix(MMatrixType::MODEL, rs->mModelMatrix);
			mesh->Render();
		}
		else if (gRenderInstanceQueue[i].mOwner->IsStaticMeshComponent()) {
			MTriangleMesh* mesh = reinterpret_cast<MTriangleMesh*>(gRenderInstanceQueue[i].mShape);
			MStaticMeshComponent* mc = reinterpret_cast<MStaticMeshComponent*>(gRenderInstanceQueue[i].mOwner);
			if (mc->mPhysicsProxy) {
				LoadMatrix(MMatrixType::MODEL, mc->mPhysicsProxy->GetModelMatrix());
			}
			mesh->Render();
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, gPlatform->gDefaultFBO);
	glCullFace(GL_BACK);
	glViewport(0, 0, gPlatform->GetWindowWidth(), gPlatform->GetWindowHeight());
}

void MRenderDepthMappingPipeline::AddRenderInstanceToQueue(MRenderInstance mesh) {
	gRenderInstanceQueue.push_back(mesh);
}

void MRenderDepthMappingPipeline::ClearRenderInstanceQueue() {
	gRenderInstanceQueue.clear();
	gRenderInstanceQueue.shrink_to_fit();
}

void MRenderDepthMappingPipeline::UpdateDepthMappingSize() {
	gDepthMapWidth = 1024 * gPlatform->gShadowQuality;
	gDepthMapHeight = 1024 * gPlatform->gShadowQuality;

	glBindFramebuffer(GL_FRAMEBUFFER, gPlatform->gDefaultFBO);

	glBindTexture(GL_TEXTURE_2D, this->mDepthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, gDepthMapWidth, gDepthMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glBindFramebuffer(GL_FRAMEBUFFER, this->mDepthFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->mDepthTex, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		MessageBox(NULL, L"Failed to create framebuffer for shadow mapping.", L"ERROR", MB_ICONERROR);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, gPlatform->gDefaultFBO);
}

void MRenderDepthMappingPipeline::SetLightSpaceMatrix(const glm::mat4& matrix) {
	mShader->UniformMat4("lightSpaceMatrix", matrix);
}

void MRenderDepthMappingPipeline::ClearBuffers() {
	glBindFramebuffer(GL_FRAMEBUFFER, mDepthFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, gPlatform->gDefaultFBO);
}

void MRenderDepthMappingPipeline::SetModelMatrix(const glm::mat4& matrix) {
	mShader->UniformMat4("model", matrix);
}
