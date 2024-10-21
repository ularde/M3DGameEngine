#include "pch.h"
#include "RenderGuiPipeline.h"

class MShaderProgram;

MRenderGuiPipeline::MRenderGuiPipeline(MBasicPlatform* platform) {
	gPlatform = platform;
	mGuiShader = new MShaderProgram(gPlatform);
	mGuiShader->AttachShader(MShaderType::VERTEX, "Shaders\\gui_2d.vert");
	mGuiShader->AttachShader(MShaderType::FRAGMENT, "Shaders\\gui_2d.frag");
	mGuiShader->LinkProgram();
}

MRenderGuiPipeline::~MRenderGuiPipeline()
{
}

void MRenderGuiPipeline::UpdateFramebufferSize()
{
}

void MRenderGuiPipeline::RenderQueueInstances()
{
}

void MRenderGuiPipeline::BeginRendering() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	mGuiShader->Use();
	mGuiShader->UniformInt("glyph", 0);
	this->LoadMatrix(MMatrixType::PROJECTION, glm::ortho(0.0f, static_cast<float>(gPlatform->gFramebufferWidth), 0.0f, static_cast<float>(gPlatform->gFramebufferHeight)));
	this->LoadIdentity(MMatrixType::VIEW);
	this->LoadIdentity(MMatrixType::MODEL);
}

void MRenderGuiPipeline::EndRendering() {
	glDisable(GL_BLEND);
	glUseProgram(0);
}

void MRenderGuiPipeline::SendMatricesToShader() {
	mGuiShader->UniformMat4("projection", gCurrentProjection);
	mGuiShader->UniformMat4("view", gCurrentView);
	mGuiShader->UniformMat4("model", gCurrentModel);
}

void MRenderGuiPipeline::DisableColorMask()
{
}

void MRenderGuiPipeline::EnableColorMask()
{
}

void MRenderGuiPipeline::SetColor(const glm::vec3& color) {
	mGuiShader->UniformVec3("color", color);
}
