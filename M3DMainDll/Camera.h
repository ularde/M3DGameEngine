#pragma once
#include "ShaderManager.h"
#include "Platforms.h"
#include "Class.h"

class MVertexShaderProgram;
class MFragmentShaderProgram;
class MBasicPlatform;
class MScriptableObject;

enum class MCameraMovementType {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UPWARD
};

class MCamera: public MScriptableObject {
public:
	virtual void ProcessMovement() = 0;
	virtual void SubmitMousePosition(double xpos, double ypos) = 0;
	virtual void DoMovement(MCameraMovementType movement_type) = 0;
	virtual glm::mat4 GetViewMatrix() = 0;
	virtual glm::mat4 GetProjectionMatrix() = 0;

	glm::vec3 position = glm::vec3(1.0f);
	glm::vec3 up = glm::vec3(1.0f);
	glm::vec3 worldUp = glm::vec3(1.0f);
	glm::vec3 front = glm::vec3(1.0f);
	glm::vec3 right = glm::vec3(1.0f);
protected:
	virtual void UpdateVectors() = 0;
	MBasicPlatform* gPlatform = NULL;
	float lastX, lastY;
	float xOffset, yOffset;
};

class MFreeCamera :public MCamera {
public:
	float speed;
	float yaw, pitch;

	MFreeCamera(MBasicPlatform* platform__, float speed__, glm::vec3 position__ = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up__ = glm::vec3(0.0f, 1.0f, 0.0f));
	~MFreeCamera();
	virtual void InitializeLuaInstance()override;
	void ProcessMovement();
	void DoMovement(MCameraMovementType movement_type);
	void SubmitMousePosition(double xpos, double ypos);
	virtual glm::mat4 GetViewMatrix()override;
	virtual glm::mat4 GetProjectionMatrix()override;
private:
	void UpdateVectors();
};