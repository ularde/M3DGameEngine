#include "pch.h"
#include "RenderAtomspherePipeline.h"
#include "Platforms.h"
#include "ShaderManager.h"
#include "RenderDeferredPipeline.h"
#include "Scene.h"
#include "Camera.h"
#include "EntityComponent.h"
#include "TimeOfDay.h"

class MScene;

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

void MRenderAtomspherePipeline::RenderQueueGeometryInstances() {
	glBindFramebuffer(GL_FRAMEBUFFER, mAtomsphereFrambuffer);
	glViewport(0, 0, gPlatform->gAtmosphereFramebufferWidth, gPlatform->gAtmosphereFramebufferHeight);
	if (gCurrentAtomsphereShader) {
		gCurrentAtomsphereShader->Use();
	}
	MScene* scene = gPlatform->GetCurrentScene();
	if (scene) {
		glm::mat4 modelMatrix(1.0f);
		if (scene->IsEditorMode() && scene->gEditorCamera) {
			modelMatrix = glm::translate(glm::mat4(1.0f), scene->gEditorCamera->position);
		}
		else if (scene->gLocalCamera) {
			modelMatrix = glm::translate(glm::mat4(1.0f), scene->gLocalCamera->mPosition);
		}
		LoadMatrix(MMatrixType::MODEL, modelMatrix);
		SelectShader("atmosphere");
		gCurrentAtomsphereShader->UniformVec3("sunDir", scene->mTimeOfDay->GetSunDirection());
		RenderSphere();
	}
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

void MRenderAtomspherePipeline::RenderSphere() {
	const unsigned int X_SEGMENTS = 8;
	const unsigned int Y_SEGMENTS = 8;

	static bool initFlag = false;
	static GLuint sVAO, sVBO, sEBO;
	static unsigned int indexCount;

	if (!initFlag) {
		glGenVertexArrays(1, &sVAO);
		glGenBuffers(1, &sVBO);
		glGenBuffers(1, &sEBO);
		std::vector<glm::vec3> positions;
		std::vector<unsigned int> indices;

		for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
			for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				positions.push_back(glm::vec3(xPos, yPos, zPos));
			}

			bool oddRow = false;
			for (unsigned int y = 0; y < Y_SEGMENTS; ++y) {
				if (!oddRow) {
					for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
						indices.push_back(y * (X_SEGMENTS + 1) + x);
						indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					}
				}
				else {
					for (int x = X_SEGMENTS; x >= 0; --x) {
						indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
						indices.push_back(y * (X_SEGMENTS + 1) + x);
					}
				}
				oddRow = !oddRow;
			}
		}
		indexCount = static_cast<unsigned int>(indices.size());

		std::vector<float> data;
		for (unsigned int i = 0; i < positions.size(); ++i) {
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);
		}
		glBindVertexArray(sVAO);
		glBindBuffer(GL_ARRAY_BUFFER, sVBO);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		initFlag = true;
	}
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glBindVertexArray(sVAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}
