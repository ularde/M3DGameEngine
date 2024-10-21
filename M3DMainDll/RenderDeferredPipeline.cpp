#include "pch.h"
#include "RenderDeferredPipeline.h"
#include "RenderAtomspherePipeline.h"
#include "RenderDepthMappingPipeline.h"
#include "Model.h"

inline float ourLerp(float a, float b, float f) {
	return a + f * (b - a);
}


MRenderDeferredPipeline::MRenderDeferredPipeline(MBasicPlatform* platform) {
	gPlatform = platform;

	mGeometryPassShader = new MShaderProgram(gPlatform);
	mGeometryPassShader->AttachShader(MShaderType::VERTEX, "Shaders\\deferred_geometry.vert");
	mGeometryPassShader->AttachShader(MShaderType::FRAGMENT, "Shaders\\deferred_geometry.frag");
	mGeometryPassShader->LinkProgram();

	mLightingPassShader = new MShaderProgram(gPlatform);
	mLightingPassShader->AttachShader(MShaderType::VERTEX, "Shaders\\deferred_lighting.vert");
	mLightingPassShader->AttachShader(MShaderType::FRAGMENT, "Shaders\\deferred_lighting.frag");
	mLightingPassShader->LinkProgram();

	mIndirectLightingPassShader = new MShaderProgram(gPlatform);
	mIndirectLightingPassShader->AttachShader(MShaderType::VERTEX, "Shaders\\indirect_lighting.vert");
	if (gPlatform->gSSDOFlag) {
		mIndirectLightingPassShader->AttachShader(MShaderType::FRAGMENT, "Shaders\\indirect_lighting.frag");
	}
	else {
		mIndirectLightingPassShader->AttachShader(MShaderType::FRAGMENT, "Shaders\\indirect_lighting_ssao_only.frag");
	}
	mIndirectLightingPassShader->LinkProgram();

	mCompositionPassShader = new MShaderProgram(gPlatform);
	mCompositionPassShader->AttachShader(MShaderType::VERTEX, "Shaders\\composition.vert");
	mCompositionPassShader->AttachShader(MShaderType::FRAGMENT, "Shaders\\composition.frag");
	mCompositionPassShader->LinkProgram();

	mGeometryPassShader->UniformInt("depth_texture", 6);

	mLightingPassShader->UniformInt("positionChannel", 0);
	mLightingPassShader->UniformInt("normalChannel", 1);
	mLightingPassShader->UniformInt("albedoChannel", 2);
	mLightingPassShader->UniformInt("mraChannel", 3);

	mIndirectLightingPassShader->UniformInt("lightingResult", 0);
	mIndirectLightingPassShader->UniformInt("positionChannel", 1);
	mIndirectLightingPassShader->UniformInt("normalChannel", 2);
	mIndirectLightingPassShader->UniformInt("mraChannel", 3);
	mIndirectLightingPassShader->UniformInt("noiseTex", 4);

	mCompositionPassShader->UniformInt("atomsphereResult", 0);
	mCompositionPassShader->UniformInt("lightingResult", 1);
	mCompositionPassShader->UniformInt("indirectLightingResult", 2);
	mCompositionPassShader->UniformInt("albedoChannel", 3);
	mCompositionPassShader->UniformInt("mraChannel", 4);
	mCompositionPassShader->UniformInt("forwardResult", 5);

	glGenFramebuffers(1, &gGeomBuffer);
	glGenTextures(1, &gAlbedoChannel);
	glGenTextures(1, &gPositionChannel);
	glGenTextures(1, &gNormalChannel);
	glGenTextures(1, &gMRAChannel);
	glGenTextures(1, &gIndirectLightingResultChannel);
	glGenTextures(1, &gLightingResultChannel);
	glBindFramebuffer(GL_FRAMEBUFFER, gGeomBuffer);

	glBindTexture(GL_TEXTURE_2D, gPositionChannel);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPositionChannel, 0);

	glBindTexture(GL_TEXTURE_2D, gNormalChannel);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormalChannel, 0);

	glBindTexture(GL_TEXTURE_2D, gAlbedoChannel);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoChannel, 0);

	glBindTexture(GL_TEXTURE_2D, gMRAChannel);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gMRAChannel, 0);

	GLenum attachments[] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);

	glGenRenderbuffers(1, &gDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, gDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gDepthBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer incomplete.\n");
	}

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &gLightingBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gLightingBuffer);
	glBindTexture(GL_TEXTURE_2D, gLightingResultChannel);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gLightingResultChannel, 0);

	glGenFramebuffers(1, &gIndirectLightingBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gIndirectLightingBuffer);
	glBindTexture(GL_TEXTURE_2D, gIndirectLightingResultChannel);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gPlatform->gFramebufferWidth / 2, gPlatform->gFramebufferHeight / 2, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gIndirectLightingResultChannel, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, gPlatform->gDefaultFBO);
	glBindTexture(GL_TEXTURE_2D, 0);

	//SSAO
	this->InitializeIndirectLighting();
}

MRenderDeferredPipeline::~MRenderDeferredPipeline()
{
}

void MRenderDeferredPipeline::UpdateFramebufferSize() {
	glBindFramebuffer(GL_FRAMEBUFFER, gGeomBuffer);

	glBindTexture(GL_TEXTURE_2D, gPositionChannel);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPositionChannel, 0);

	glBindTexture(GL_TEXTURE_2D, gNormalChannel);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormalChannel, 0);

	glBindTexture(GL_TEXTURE_2D, gAlbedoChannel);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoChannel, 0);

	glBindTexture(GL_TEXTURE_2D, gMRAChannel);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gMRAChannel, 0);

	GLenum attachments[] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);

	glBindRenderbuffer(GL_RENDERBUFFER, gDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gDepthBuffer);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, gLightingBuffer);
	glBindTexture(GL_TEXTURE_2D, gLightingResultChannel);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gLightingResultChannel, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, gIndirectLightingBuffer);
	glBindTexture(GL_TEXTURE_2D, gIndirectLightingResultChannel);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gPlatform->gFramebufferWidth / 2, gPlatform->gFramebufferHeight / 2, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gIndirectLightingResultChannel, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, gPlatform->gDefaultFBO);
	glBindTexture(GL_TEXTURE_2D, 0);

	//update framebuffer size for indirect lighting 
	this->mIndirectLightingPassShader->UniformIVec2("framebufferSize",
		glm::vec2(gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight));
}

void MRenderDeferredPipeline::RenderQueueInstances() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, gGeomBuffer);
	mGeometryPassShader->UniformVec3("camPos", gCamPos);
	mGeometryPassShader->Use();
	for (auto i = 0u; i < gRenderInstanceQueue.size(); i++) {
		if (gRenderInstanceQueue[i].mOwner->IsRigidStatic()) {
			MTriangleMesh* mesh = reinterpret_cast<MTriangleMesh*>(gRenderInstanceQueue[i].mShape);
			MRigidStatic* rs = reinterpret_cast<MRigidStatic*>(gRenderInstanceQueue[i].mOwner);

			LoadMatrix(MMatrixType::MODEL, rs->mModelMatrix);
			rs->mModel->PushUseCustomMaterialFlag();

			if (rs->mCustomMaterial) {
				rs->mModel->SetUseCustomMaterialFlag(true);
				rs->mModel->SetMaterial(rs->mCustomMaterial);
			}
			else {
				rs->mModel->SetUseCustomMaterialFlag(false);
			}
			MSubMaterial* sm = rs->mModel->GetMaterial()->GetSubMaterial(mesh->GetMaterialIndex());
			sm->Assign();
			mesh->Render();
			sm->Unassign();

			rs->mModel->PopUseCustomMaterialFlag();
		}
		else if (gRenderInstanceQueue[i].mOwner->mClassName == "StaticMeshComponent") {
			MTriangleMesh* mesh = reinterpret_cast<MTriangleMesh*>(gRenderInstanceQueue[i].mShape);
			MStaticMeshComponent* mc = reinterpret_cast<MStaticMeshComponent*>(gRenderInstanceQueue[i].mOwner);

			if (mc->mPhysicsProxy) {
				LoadMatrix(MMatrixType::MODEL, mc->mPhysicsProxy->GetModelMatrix());
			}
			mc->model->PushUseCustomMaterialFlag();
			mc->model->SetUseCustomMaterialFlag(false);
			MSubMaterial* sm = mc->model->GetMaterial()->GetSubMaterial(mesh->GetMaterialIndex());
			sm->Assign();
			mesh->Render();
			sm->Unassign();
			mc->model->PopUseCustomMaterialFlag();
		}
	}
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, gPlatform->gDepthMappingPipeline->GetDepthTextureID());
	glBindFramebuffer(GL_FRAMEBUFFER, gPlatform->gDefaultFBO);
}

void MRenderDeferredPipeline::SendMatricesToShader() {
	mGeometryPassShader->UniformMat4("projection", gCurrentProjection);
	mGeometryPassShader->UniformMat4("view", gCurrentView);
	mGeometryPassShader->UniformMat4("model", gCurrentModel);
	mLightingPassShader->UniformMat4("view", gCurrentView);
	mLightingPassShader->UniformMat4("model", gCurrentModel);
	//mSSDOPassShader->UniformMat4("projection", gCurrentProjection);
	//mSSDOPassShader->UniformMat4("view", gCurrentView);
	//mSSDOPassShader->UniformMat4("model", gCurrentModel);
}

void MRenderDeferredPipeline::DisableColorMask() {
	glBindFramebuffer(GL_FRAMEBUFFER, gGeomBuffer);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
}

void MRenderDeferredPipeline::EnableColorMask() {
	glBindFramebuffer(GL_FRAMEBUFFER, gGeomBuffer);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

void MRenderDeferredPipeline::AddRenderInstanceToQueue(MRenderInstance mesh) {
	gRenderInstanceQueue.push_back(mesh);
}

void MRenderDeferredPipeline::ClearRenderInstanceQueue() {
	gRenderInstanceQueue.clear();
	gRenderInstanceQueue.shrink_to_fit();
}

void MRenderDeferredPipeline::CopyForwardDepthBufferToDeferredDepthBuffer() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gGeomBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gPlatform->gAtomspherePipeline->GetForwardRenderingFramebufferID());
	glBlitFramebuffer(0, 0, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight,
		0, 0, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight,
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

void MRenderDeferredPipeline::ClearBuffers() {
	glBindFramebuffer(GL_FRAMEBUFFER, gGeomBuffer);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MRenderDeferredPipeline::SetLightingPassSamplerUnit(const std::string& name, const int unit) {
	mLightingPassShader->UniformInt(name, unit);
}

void MRenderDeferredPipeline::SetLightSpaceMatrix(const glm::mat4& matrix) {
	mGeometryPassShader->UniformMat4("lightSpaceMatrix", matrix);
}

void MRenderDeferredPipeline::SetGeometryPassInteger(const std::string& name, const int unit) {
	mGeometryPassShader->UniformInt(name, unit);
}

void MRenderDeferredPipeline::SetGeometryPassVec2(const std::string& name, const glm::vec2& vec) {
	mGeometryPassShader->UniformVec2(name, vec);
}

void MRenderDeferredPipeline::SetGeometryPassVec3(const std::string& name, const glm::vec3& vec) {
	mGeometryPassShader->UniformVec3(name, vec);
}

void MRenderDeferredPipeline::SetGeometryPassFloat(const std::string& name, const float v) {
	mGeometryPassShader->UniformFloat(name, v);
}

void MRenderDeferredPipeline::SetLightingPassVec2(const std::string& name, const glm::vec2& vec) {
	mLightingPassShader->UniformVec2(name, vec);
}

void MRenderDeferredPipeline::SetLightingPassVec3(const std::string& name, const glm::vec3& vec) {
	mLightingPassShader->UniformVec3(name, vec);
}

void MRenderDeferredPipeline::SetDirectionalLight(const glm::vec3& dir, const glm::vec3& color) {
	mLightingPassShader->UniformVec3(std::string("lightDir"), dir);
	mLightingPassShader->UniformVec3(std::string("lightColor"), color);
}
void MRenderDeferredPipeline::InitializeIndirectLighting() {
	//generate sample kernel
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
	std::default_random_engine generator;
	for (unsigned int i = 0; i < 64; ++i) {
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / 64.0f;

		scale = ourLerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		gSSAOKernel.push_back(sample);
	}

	//generate noise texture
	std::vector<glm::vec3> ssaoNoise;
	for (unsigned int i = 0; i < 16; i++) {
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
		ssaoNoise.push_back(noise);
	}
	glGenTextures(1, &gNoiseTex);
	glBindTexture(GL_TEXTURE_2D, gNoiseTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//update framebuffer size for indirect lighting  
	this->mIndirectLightingPassShader->UniformVec2("framebufferSize",
		glm::vec2((float)gPlatform->gFramebufferWidth, (float)gPlatform->gFramebufferHeight));
}


void MRenderDeferredPipeline::DoLightingPass() {
	glBindFramebuffer(GL_FRAMEBUFFER, gLightingBuffer);

	mLightingPassShader->UniformVec3("camPos", gCamPos);
	mLightingPassShader->UniformMat4("view", this->gCurrentView);
	mLightingPassShader->UniformMat4("projection", this->gCurrentProjection);

	//for (unsigned int i = 0; i < 64; i++) {
	//	this->mLightingPassShader->UniformVec3("samples[" + std::to_string(i) + "]", gSSAOKernel[i]);
	//}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPositionChannel);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormalChannel);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedoChannel);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gMRAChannel);

	mLightingPassShader->Use();
	RenderNDCQuad();

	glBindFramebuffer(GL_FRAMEBUFFER, gPlatform->gDefaultFBO);
}

void MRenderDeferredPipeline::DoIndirectLightingPass() {
	glBindFramebuffer(GL_FRAMEBUFFER, gIndirectLightingBuffer);
	glViewport(0, 0, gPlatform->GetWindowWidth() / 2, gPlatform->GetWindowHeight() / 2);
	glDisable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gLightingResultChannel);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gPositionChannel);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gNormalChannel);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gMRAChannel);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gNoiseTex);
	//mSSDOPassShader->UniformMat4("view", this->gCurrentView);
	mIndirectLightingPassShader->UniformMat4("projection", this->gCurrentProjection);
	mIndirectLightingPassShader->Use();
	RenderNDCQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, gPlatform->gDefaultFBO);
	glViewport(0, 0, gPlatform->GetWindowWidth(), gPlatform->GetWindowHeight());
}

void MRenderDeferredPipeline::DoCompositionPass() {
	glBindFramebuffer(GL_FRAMEBUFFER, gPlatform->gDefaultFBO);
	glDisable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPlatform->gAtomspherePipeline->GetAtomsphereRenderingResultTextureID());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gLightingResultChannel);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gIndirectLightingResultChannel);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gAlbedoChannel);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gMRAChannel);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, gPlatform->gForwardPipeline->GetTransparentCompositionResultChannel());
	mCompositionPassShader->Use();
	RenderNDCQuad();
}

void MRenderDeferredPipeline::RenderNDCQuad() {
	static bool initFlag = false;
	static GLuint qVAO, qVBO;
	static float vertices[] = {
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f
	};

	if (!initFlag) {
		glGenVertexArrays(1, &qVAO);
		glGenBuffers(1, &qVBO);
		glBindVertexArray(qVAO);
		glBindBuffer(GL_ARRAY_BUFFER, qVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		initFlag = true;
	}

	glBindVertexArray(qVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
