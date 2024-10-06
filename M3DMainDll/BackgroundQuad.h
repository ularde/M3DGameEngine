#pragma once
#include "Platforms.h"

class MBasicPlatform;

class MBackgroundQuad {
public:
	MBackgroundQuad(MBasicPlatform* platform__);
	~MBackgroundQuad();
	void UpdateWindowSize(int width, int height);
	void RenderQuad();
	void BeginRender();
	void EndRender();
	GLuint GetFramebufferID() { return this->FBO; }
private:
	bool enableDebugQuad = false;

	GLuint FBO = 0, RBO = 0, backgroundTex = 0;
	MBasicPlatform* gPlatform = NULL;
protected:
};