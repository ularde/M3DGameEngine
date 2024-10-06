#include "pch.h"
#include "Camera.h"

MFreeCamera::MFreeCamera(MBasicPlatform* platform__, float speed__, glm::vec3 position__, glm::vec3 up__) {
	this->gPlatform = platform__;
	this->gPlatform->SetCursorInvisible();
	this->speed = speed__;
	this->position = position__;
	this->worldUp = up__;
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);
	this->yaw = -90.0f;
	this->pitch = 0.0f;
	this->right = glm::vec3(1.0f, 0.0f, 0.0f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->lastX = this->gPlatform->GetWindowWidth() / 2.0f;
	this->lastY = this->gPlatform->GetWindowHeight() / 2.0f;
}

MFreeCamera::~MFreeCamera() {

}

void MFreeCamera::InitializeLuaInstance() {

}

void MFreeCamera::DoMovement(MCameraMovementType movement_type) {
	static float velocity = this->speed * this->gPlatform->GetDeltaTime();
	switch (movement_type)
	{
	case MCameraMovementType::FORWARD:
		this->position += this->front * velocity;
		break;
	case MCameraMovementType::BACKWARD:
		this->position -= this->front * velocity;
		break;
	case MCameraMovementType::LEFT:
		this->position -= this->right * velocity;
		break;
	case MCameraMovementType::RIGHT:
		this->position += this->right * velocity;
		break;
	case MCameraMovementType::UPWARD:
		this->position += this->worldUp * velocity;
		break;
	default:
		break;
	}
}

void MFreeCamera::SubmitMousePosition(double xpos, double ypos) {
	this->xOffset = xpos - this->lastX;
	this->yOffset = this->lastY - ypos;

	this->lastX = xpos;
	this->lastY = ypos;

	if (gPlatform->GetMouseRightButtonDown()) {


		this->xOffset *= this->gPlatform->mouseSensitivity;
		this->yOffset *= this->gPlatform->mouseSensitivity;

		this->yaw += this->xOffset;
		this->pitch += this->yOffset;

		if (abs(pitch) > 89.0f && pitch > 0) {
			pitch = 89.0f;
		}
		if (abs(pitch) > 89.0f && pitch < 0) {
			pitch = -89.0f;
		}

		glm::vec3 nFront;
		nFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		nFront.y = sin(glm::radians(pitch));
		nFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		this->front = glm::normalize(nFront);
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}
}

glm::mat4 MFreeCamera::GetViewMatrix() {
	return glm::lookAt(this->position, this->position + this->front, this->up);
}

glm::mat4 MFreeCamera::GetProjectionMatrix() {
	return glm::perspective(glm::radians(60.0f), (float)this->gPlatform->GetWindowWidth() / (float)this->gPlatform->GetWindowHeight(), 0.3f, 1000.0f);
}

void MFreeCamera::ProcessMovement() {
	if (this->gPlatform->GetKey(GLFW_KEY_W) == GLFW_PRESS) {
		this->DoMovement(MCameraMovementType::FORWARD);
	}
	else if (this->gPlatform->GetKey(GLFW_KEY_A) == GLFW_PRESS) {
		this->DoMovement(MCameraMovementType::LEFT);
	}
	else if (this->gPlatform->GetKey(GLFW_KEY_S) == GLFW_PRESS) {
		this->DoMovement(MCameraMovementType::BACKWARD);
	}
	else if (this->gPlatform->GetKey(GLFW_KEY_D) == GLFW_PRESS) {
		this->DoMovement(MCameraMovementType::RIGHT);
	}
	else if (this->gPlatform->GetKey(GLFW_KEY_E) == GLFW_PRESS) {
		this->DoMovement(MCameraMovementType::UPWARD);
	}
}

void MFreeCamera::UpdateVectors() {

}