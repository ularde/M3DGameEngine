#pragma once
#include "Actor.h"

#include <string>

class MCamera;
class MActor;
class MScene;
class MBasicPlatform;
class MAssetManager;
class MCubemap;

class MSkybox :public MActor {
public:
	MSkybox(MScene* parent_, const std::string& name_,
		const std::string& top_, const std::string& bottom_, const std::string& front_,
		const std::string& back_, const std::string& left_, const std::string& right_);
	~MSkybox() { }
	virtual void Save()override {}
	virtual void InitializeLuaInstance() { }
	virtual void Update(double dt)override;
	virtual void Render()override { }
	virtual void RenderForDepthMapping()override { }

protected:
	MAssetManager* gAssetManager = NULL;
	MBasicPlatform* gPlatform = NULL;
	MCamera* camera = NULL;
	MCubemap* cubemap = NULL;

	std::string top, bottom, front,
		back, left, right;
	GLuint texID;
	GLuint VAO, VBO;

	void InitializeSkybox();
};