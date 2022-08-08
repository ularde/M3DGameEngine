#include "pch.h"
#include "Camera.h"

MFreeCamera::MFreeCamera(MBasicPlatform* platform__, MVertexShaderProgram* vertexShader__, MFragmentShaderProgram* fragmentShader__,float speed__, glm::vec3 position__, glm::vec3 up__) {
	this->platform = platform__;
	this->vertexShader = vertexShader__;
	this->fragmentShader = fragmentShader__;
	this->platform->SetCursorInvisible();
	this->speed = speed__;
	this->position = position__;
	this->worldUp = up__;
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);
	this->yaw = -90.0f;
	this->pitch = 0.0f;
	this->right = glm::vec3(1.0f, 0.0f, 0.0f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->lastX = this->platform->GetWindowWidth() / 2.0f;
	this->lastY = this->platform->GetWindowHeight() / 2.0f;
}

MFreeCamera::~MFreeCamera() {

}

void MFreeCamera::DoMovement(MCameraMovementType movement_type) {
	static float velocity = this->speed * this->platform->GetDeltaTime();
	switch (movement_type)
	{
	case CAMERA_FORWARD:
		this->position += this->front * velocity;
		break;
	case CAMERA_BACKWARD:
		this->position -= this->front * velocity;
		break;
	case CAMERA_LEFT:
		this->position -= this->right * velocity;
		break;
	case CAMERA_RIGHT:
		this->position += this->right * velocity;
		break;
	case CAMERA_UPWARD:
		this->position += this->worldUp * velocity;
		break;
	default:
		break;
	}
}

void MFreeCamera::Update(double xpos, double ypos) {
	glBindProgramPipeline(this->platform->pipelineID);
	this->vertexShader->UseProgramStage();
	this->fragmentShader->UseProgramStage();

	this->xOffset = -(xpos - this->lastX);
	this->yOffset = -(ypos - this->lastY);

	this->lastX = xpos;
	this->lastY = ypos;


	this->xOffset *= this->platform->mouseSensitivity;
	this->yOffset *= this->platform->mouseSensitivity;
	
	this->yaw += this->xOffset;
	this->pitch += this->yOffset;

	if (this->pitch > 89.0f) {
		this->pitch = 89.0f;
	}
	if (this->pitch < -89.0f) {
		this->pitch = -89.0f;
	}

	glm::vec3 nFront_yz, nFront_xz;
	nFront_xz.x = 0.0f;
	nFront_xz.y = glm::sin(glm::radians(this->pitch));
	nFront_xz.z = -glm::cos(glm::radians(this->pitch));

	nFront_yz.y = 0.0f;
	nFront_yz.z = -glm::cos(glm::radians(this->yaw));
	nFront_yz.x = -glm::sin(glm::radians(this->yaw));

	this->front = glm::normalize(nFront_xz + nFront_yz);
	this->right = glm::normalize(glm::cross(this->front, this->worldUp));
	this->up = glm::normalize(glm::cross(this->right, this->front));
	glBindProgramPipeline(0);
}

glm::mat4 MFreeCamera::GetViewMatrix() {
	return glm::lookAt(this->position, this->position + this->front, this->up);
}

void MFreeCamera::ProcessMovement() {
	if (this->platform->GetKey(GLFW_KEY_W) == GLFW_PRESS) {
		this->DoMovement(CAMERA_FORWARD);
	}
	else if (this->platform->GetKey(GLFW_KEY_A) == GLFW_PRESS) {
		this->DoMovement(CAMERA_LEFT);
	}
	else if (this->platform->GetKey(GLFW_KEY_S) == GLFW_PRESS) {
		this->DoMovement(CAMERA_BACKWARD);
	}
	else if (this->platform->GetKey(GLFW_KEY_D) == GLFW_PRESS) {
		this->DoMovement(CAMERA_RIGHT);
	}
	else if (this->platform->GetKey(GLFW_KEY_E) == GLFW_PRESS) {
		this->DoMovement(CAMERA_UPWARD);
	}
}

void MFreeCamera::UpdateVectors() {

}