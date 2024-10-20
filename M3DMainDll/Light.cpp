#include "pch.h"
#include "Light.h"
#include "Scene.h"
#include "Class.h"
#include "EntityComponent.h"
#include "RenderDeferredPipeline.h"
#include "RenderAtomspherePipeline.h"
#include "RenderDepthMappingPipeline.h"
#include "Camera.h"
#include "TimeOfDay.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

GLuint publicID = 0;

MPointLight::MPointLight(MScene* scene_, const std::string& name_, const std::string& pymodule_path,
	const glm::vec3& position, const glm::vec3& color, 
	float linear_, float quadratic_, bool cast_shadow) {
	this->gPlatform = scene_->GetPlatform();
	this->mParent = scene_;
	this->mName = name_;
	this->linear = linear_;
	this->quadratic = quadratic_;
	this->mPosition = position;
	this->mColor = color;
	this->lightID = this->mParent->GetPointLightID();
	this->mCastShadow = cast_shadow;
	//
	this->vnLightAmbient = "lightAmbient" + std::to_string(this->lightID);
	this->vnLightLinear = "lightLinear" + std::to_string(this->lightID);
	this->vnLightQuadratic = "lightQuadratic" + std::to_string(this->lightID);
	this->vnLightDiffuse = "lightDiffuse" + std::to_string(this->lightID);
	this->vnLightSpecular = "lightSpecular" + std::to_string(this->lightID);
	this->vnLightPos = "lightPos" + std::to_string(this->lightID);
	//
}

MPointLight::~MPointLight() {

}

void MPointLight::Save()
{
}

void MPointLight::InitializeLuaInstance()
{
}

void MPointLight::Update(double delta_time) {
}

MDirectionalLight::MDirectionalLight(MScene* scene_, const std::string& name_,
	const std::string& pymodule_path, const glm::vec3& direction,
	const glm::vec3& color, bool cast_shadow) {
	this->gPlatform = scene_->GetPlatform();
	this->mParent = scene_;
	this->mName = name_;
	this->mDirection = direction;
	this->mColor = color;
	this->mCastShadow = cast_shadow;
}

MDirectionalLight::~MDirectionalLight()
{
}

void MDirectionalLight::InitializeLuaInstance()
{
}

void MDirectionalLight::Update(double dt) {
	mColor = mTimeOfDay->GetSunColor();
	mDirection = mTimeOfDay->GetSunDirection();
	gPlatform->gDeferredPipeline->SetDirectionalLight(mDirection, mColor);
	gPlatform->gForwardPipeline->SetDirectionalLight(mDirection, mColor);
	//gPlatform->gForwardPipeline->SetDirectionalLight(mDirection, mColor);
	glm::mat4 projection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, -2.0f, 100.0f);
	glm::mat4 view(1.0f);
	gPlatform->gDeferredPipeline->SetGeometryPassVec3("lightDir", this->mDirection);
	//float dist = float(abs(sin(mTimeOfDay->GetTime() / mTimeOfDay->mSpeed))) * 100.0f;
	if (!mParent->IsEditorMode() && mParent->gLocalCamera) {
		view = glm::lookAt(mParent->gLocalCamera->mPosition + 18.0f * glm::normalize(mDirection), mParent->gLocalCamera->mPosition, glm::vec3(0.0f, 1.0f, 0.0f));
		//view = glm::lookAt(50.0f * glm::normalize(mDirection), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (mParent->gEditorCamera) {
		view = glm::lookAt(mParent->gEditorCamera->position + 18.0f * glm::normalize(mDirection), mParent->gEditorCamera->position, glm::vec3(0.0f, 1.0f, 0.0f));
		//view = glm::lookAt(500.0f * glm::normalize(mDirection), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glm::mat4 lightSpaceMatrix = projection * view;
	gPlatform->gDeferredPipeline->SetLightSpaceMatrix(lightSpaceMatrix);
	gPlatform->gForwardPipeline->SetLightSpaceMatrix(lightSpaceMatrix);
	gPlatform->gDepthMappingPipeline->SetLightSpaceMatrix(lightSpaceMatrix);
}
