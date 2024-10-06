#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Actor.h"
#include "Scene.h"
#include "Camera.h"
#include "tinyxml2.h"

class MActor;
class MScene;
class MFreeCamera;

class MPlayer :public MActor {
public:
	MPlayer(MScene* scene_, const std::string& name_, const std::string& player_def_path, const glm::vec3& initial_pos);
	~MPlayer();
	virtual void InitializeLuaInstance()override;
	virtual void Update(double delata_time) override;
	virtual void Render()override;
	virtual void RenderForDepthMapping()override {}
	void UpdateCamera(double xpos, double ypos);
	virtual void Save()override {}
	std::string mClassName = "Player";
	std::string playerTypeName;
	std::string cameraPrototypeName;
	std::string actorName;
	std::string mDefFilePath;

	float speed = 3.0f;
	float position[3];
	float up[3];
	float right[3];

	tinyxml2::XMLDocument playerConfigDoc;
	MCamera* mCamera = NULL;
	void CreateCameraInstance();
};