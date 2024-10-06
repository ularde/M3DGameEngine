#include "pch.h"
#include "BackgroundQuad.h"

MBackgroundQuad::MBackgroundQuad(MBasicPlatform* platform__) {
	this->gPlatform = platform__;

	glGenTextures(1, &this->backgroundTex);
	glBindTexture(GL_TEXTURE_2D, this->backgroundTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->gPlatform->GetWindowWidth(), this->gPlatform->GetWindowHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &this->RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->gPlatform->GetWindowWidth(), this->gPlatform->GetWindowHeight());
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &this->FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->backgroundTex, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->RBO);

	glBindFramebuffer(GL_FRAMEBUFFER, gPlatform->gDefaultFBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		MessageBox(NULL, L"Failed to create framebuffer", L"M3D Engine", MB_ICONERROR);
	}
	else {
		this->gPlatform->ConsoleWriteLine(255, 255, 255, std::string("[INFO] OpenGL : Farmebuffer has been created successfully."));
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
}

MBackgroundQuad::~MBackgroundQuad()
{
}

void MBackgroundQuad::UpdateWindowSize(int width, int height) {
	glBindTexture(GL_TEXTURE_2D, this->backgroundTex);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glTexSubImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->backgroundTex, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->RBO);
}

void MBackgroundQuad::RenderQuad() {
	glUseProgram(0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->backgroundTex);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(1.0f, 1.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	//
	if (this->enableDebugQuad) {
		glBindTexture(GL_TEXTURE_2D, publicID);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(-0.2f, 0.2f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-0.2f, -0.2f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(0.2f, -0.2f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(0.2f, 0.2f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void MBackgroundQuad::BeginRender() {
	glViewport(0, 0, this->gPlatform->GetWindowWidth(), this->gPlatform->GetWindowHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
}

void MBackgroundQuad::EndRender() {
	glBindFramebuffer(GL_FRAMEBUFFER, gPlatform->gDefaultFBO);
}