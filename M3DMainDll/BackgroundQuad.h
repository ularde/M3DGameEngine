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
private:
	GLuint FBO = 0, RBO = 0, backgroundTex = 0;
	MBasicPlatform* platform = NULL;
protected:
};