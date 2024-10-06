#include "pch.h"
#include "Player.h"
#include "SharedFunc.h"
#include "RenderDeferredPipeline.h"
#include "RenderAtomspherePipeline.h"

MPlayer::MPlayer(MScene* scene_, const std::string& name_, const std::string& player_def_path, const glm::vec3& initial_pos) {
	this->name = name_;
	this->mParent = scene_;
	this->actorName = name_;
	this->gPlatform = scene_->GetPlatform();
	this->mDefFilePath = player_def_path;
	this->position[0] = initial_pos.x;
	this->position[1] = initial_pos.y;
	this->position[2] = initial_pos.z;
	//load player definition file
	if (this->playerConfigDoc.LoadFile(GetFullAssetPathA(mDefFilePath)) != tinyxml2::XML_SUCCESS) {
		this->gPlatform->ConsoleWriteLine(1.0f, 0.0f, 0.0f, std::string("[ERROR] Engine : Failed to create actor from XML document ") + this->mDefFilePath);
	}
	else {
		this->playerTypeName = this->playerConfigDoc.RootElement()->Attribute("name");
		this->cameraPrototypeName = this->playerConfigDoc.RootElement()->Attribute("camera_prototype");
		this->speed = this->playerConfigDoc.RootElement()->FloatAttribute("speed");
		//create an instance of the camera
		this->CreateCameraInstance();
	}
}

MPlayer::~MPlayer() {
	delete[]mCamera;
}

void MPlayer::InitializeLuaInstance()
{
}

void MPlayer::Update(double delata_time) {
	this->mCamera->ProcessMovement();

	glm::mat4 projection = mCamera->GetProjectionMatrix();
	glm::mat4 view = mCamera->GetViewMatrix();

	gPlatform->gDeferredPipeline->SetCameraPosition(mCamera->position);
	gPlatform->gDeferredPipeline->LoadMatrix(MMatrixType::PROJECTION, projection);
	gPlatform->gAtomspherePipeline->LoadMatrix(MMatrixType::PROJECTION, projection);
	gPlatform->gDeferredPipeline->LoadMatrix(MMatrixType::VIEW, view);
	gPlatform->gAtomspherePipeline->LoadMatrix(MMatrixType::VIEW, view);
}

void MPlayer::Render() {

}

void MPlayer::UpdateCamera(double xpos, double ypos) {
	this->mCamera->SubmitMousePosition(xpos, ypos);
}

void MPlayer::CreateCameraInstance() {
	if (this->cameraPrototypeName == "FreeCamera") {
		this->up[0] = 0.0f;
		this->up[1] = 1.0f;
		this->up[2] = 0.0f;
		//
		this->mCamera = new MFreeCamera(this->gPlatform, this->speed, 
			glm::vec3(this->position[0], this->position[1], this->position[2]), 
			glm::vec3(this->up[0], this->up[1], this->up[2]));
	}
	else if (this->cameraPrototypeName == "FirstPersonCamera") {

	}
}
