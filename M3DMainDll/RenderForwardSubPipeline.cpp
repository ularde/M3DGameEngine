#include "pch.h"
#include "RenderForwardSubPipeline.h"
#include "RenderDeferredPipeline.h"
#include "RenderDepthMappingPipeline.h"
#include "Model.h"
#include "RigidStatic.h"

class MRenderDeferredPipeline;
class MRenderDepthMappingPipelinel;

static glm::vec4 zeroFillerVec(0.0f);
static glm::vec4 oneFillerVec(1.0f);

MRenderForwardPipeline::MRenderForwardPipeline(MBasicPlatform* platform)
{
	gPlatform = platform;

	mTransparentLightingShader = new MShaderProgram(gPlatform);
	mTransparentLightingShader->AttachShader(MShaderType::VERTEX, "Shaders\\transparent_lighting.vert");
	mTransparentLightingShader->AttachShader(MShaderType::FRAGMENT, "Shaders\\transparent_lighting.frag");
	mTransparentLightingShader->LinkProgram();

	mTransparentCompositionShader = new MShaderProgram(gPlatform);
	mTransparentCompositionShader->AttachShader(MShaderType::VERTEX, "Shaders\\transparent_composition.vert");
	mTransparentCompositionShader->AttachShader(MShaderType::FRAGMENT, "Shaders\\transparent_composition.frag");
	mTransparentCompositionShader->LinkProgram();

	mTransparentCompositionShader->UniformInt("accumChannel", 0);
	mTransparentCompositionShader->UniformInt("revealChannel", 1);

	mTransparentLightingShader->UniformInt("depth_texture", 7);

	glGenFramebuffers(1, &gTransparentLightingBuffer);
	glGenFramebuffers(1, &gTransparentCompositionBuffer);
	glGenTextures(1, &gAccumChannel);
	glGenTextures(1, &gRevealChannel);
	glGenTextures(1, &gTransparentCompositionChannel);

	glBindFramebuffer(GL_FRAMEBUFFER, gTransparentLightingBuffer);


	glBindTexture(GL_TEXTURE_2D, gAccumChannel);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight, 0, GL_RGBA, GL_HALF_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gAccumChannel, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, gRevealChannel);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gRevealChannel, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint depthBuffer = gPlatform->gDeferredPipeline->GetDepthBuffer();
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	const GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer incomplete.\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, gPlatform->gDefaultFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, gTransparentCompositionBuffer);

	glBindTexture(GL_TEXTURE_2D, gTransparentCompositionChannel);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight, 0, GL_RGBA, GL_HALF_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gTransparentCompositionChannel, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, gPlatform->gDefaultFBO);
} 

MRenderForwardPipeline::~MRenderForwardPipeline()
{
}

void MRenderForwardPipeline::UpdateFramebufferSize() {
	glBindFramebuffer(GL_FRAMEBUFFER, gTransparentLightingBuffer);

	glBindTexture(GL_TEXTURE_2D, gAccumChannel);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight, 0, GL_RGBA, GL_HALF_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gAccumChannel, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, gRevealChannel);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gRevealChannel, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint depthBuffer = gPlatform->gDeferredPipeline->GetDepthBuffer();
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	const GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, gTransparentCompositionBuffer);

	glBindTexture(GL_TEXTURE_2D, gTransparentCompositionChannel);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight, 0, GL_RGBA, GL_HALF_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gTransparentCompositionChannel, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, gPlatform->gDefaultFBO);
}

void MRenderForwardPipeline::DisableColorMask()
{
}

void MRenderForwardPipeline::EnableColorMask()
{
}

void MRenderForwardPipeline::SendMatricesToShader() {
	mTransparentLightingShader->UniformMat4("projection", gCurrentProjection);
	mTransparentLightingShader->UniformMat4("view", gCurrentView);
	mTransparentLightingShader->UniformMat4("model", gCurrentModel);
}

void MRenderForwardPipeline::RenderQueueInstances() {
	glBindFramebuffer(GL_FRAMEBUFFER, gTransparentLightingBuffer);
	glDisable(GL_CULL_FACE);
	glViewport(0, 0, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunci(0, GL_ONE, GL_ONE);
	glBlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
	glBlendEquation(GL_FUNC_ADD);
	mTransparentLightingShader->UniformVec3("camPos", gCamPos);
	mTransparentLightingShader->Use();
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
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, gPlatform->gDepthMappingPipeline->GetDepthTextureID());

	glBindFramebuffer(GL_FRAMEBUFFER, gTransparentCompositionBuffer);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gAccumChannel);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gRevealChannel);
	mTransparentCompositionShader->Use();
	RenderNDCQuad();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, gPlatform->gDefaultFBO);
}

void MRenderForwardPipeline::AddRenderInstanceToQueue(MRenderInstance mesh) {
	gRenderInstanceQueue.push_back(mesh);
}

void MRenderForwardPipeline::ClearRenderInstanceQueue() {
	gRenderInstanceQueue.clear();
	gRenderInstanceQueue.shrink_to_fit();
}

void MRenderForwardPipeline::SetLightSpaceMatrix(const glm::mat4& matrix) {
	mTransparentLightingShader->UniformMat4("lightSpaceMatrix", matrix);
}

void MRenderForwardPipeline::SetDirectionalLight(const glm::vec3& dir, const glm::vec3& color) {
	mTransparentLightingShader->UniformVec3(std::string("lightDir"), dir);
	mTransparentLightingShader->UniformVec3(std::string("lightColor"), color);
}

void MRenderForwardPipeline::SetTransparentLightingShaderFloat(const std::string& n, const float v) {
	mTransparentLightingShader->UniformFloat(n, v);
}

void MRenderForwardPipeline::SetTransparentLightingShaderInteger(const std::string& n, const int v) {
	mTransparentLightingShader->UniformInt(n, v);
}

void MRenderForwardPipeline::SetTransparentLightingShaderVec3(const std::string& n, const glm::vec3& v) {
	mTransparentLightingShader->UniformVec3(n, v);
}

void MRenderForwardPipeline::SetTransparentLightingShaderVec2(const std::string& n, const glm::vec2& v) {
	mTransparentLightingShader->UniformVec2(n, v);
}

void MRenderForwardPipeline::SetTransparentLightingShaderMat4(const std::string& n, const glm::mat4& v) {
	mTransparentLightingShader->UniformMat4(n, v);
}

void MRenderForwardPipeline::ClearBuffers() {
	glBindFramebuffer(GL_FRAMEBUFFER, gTransparentLightingBuffer);
	glClearBufferfv(GL_COLOR, 0, &zeroFillerVec[0]);
	glClearBufferfv(GL_COLOR, 1, &oneFillerVec[0]);
	glClearBufferfv(GL_COLOR, 2, &oneFillerVec[0]);
}

void MRenderForwardPipeline::RenderNDCQuad()
{
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
