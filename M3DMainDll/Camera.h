#pragma once
#include "ShaderManager.h"
#include "Class.h"
#include "Platforms.h"

class MVertexShaderProgram;
class MFragmentShaderProgram;
class MPythonBoundObject;
class MBasicPlatform;

enum MCameraMovementType {
	CAMERA_FORWARD,
	CAMERA_BACKWARD,
	CAMERA_LEFT,
	CAMERA_RIGHT,
	CAMERA_UPWARD
};

class MCamera: public MPythonBoundObject {
public:
	virtual void ProcessMovement() = 0;
	virtual void Update(double xpos, double ypos) = 0;
	virtual void DoMovement(MCameraMovementType movement_type) = 0;
	virtual glm::mat4 GetViewMatrix() = 0;
protected:
	virtual void UpdateVectors() = 0;
	MBasicPlatform* platform = NULL;
	MVertexShaderProgram* vertexShader = NULL;
	MFragmentShaderProgram* fragmentShader = NULL;
	float lastX, lastY;
	float xOffset, yOffset;
};

class MFreeCamera :public MCamera {
public:
	float speed;
	float yaw, pitch;

	glm::vec3 position = glm::vec3(1.0f);
	glm::vec3 up = glm::vec3(1.0f);
	glm::vec3 worldUp = glm::vec3(1.0f);
	glm::vec3 front = glm::vec3(1.0f);
	glm::vec3 right = glm::vec3(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);

	MFreeCamera(MBasicPlatform* platform__, MVertexShaderProgram* vertexShader__, MFragmentShaderProgram* fragmentShader__, float speed__, glm::vec3 position__ = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up__ = glm::vec3(1.0f, 0.0f, 0.0f));
	~MFreeCamera();
	void ProcessMovement();
	void DoMovement(MCameraMovementType movement_type);
	void Update(double xpos, double ypos);
	glm::mat4 GetViewMatrix();
private:
	void UpdateVectors();
};