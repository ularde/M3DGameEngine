#include "pch.h"
#include "GameContext.h"
#include "Platforms.h"
#include "Scene.h"
#include "Entity.h"
#include "Actor.h"
#include "SharedFunc.h"
#include "Camera.h"
#include <glm/glm.hpp>

MGameContext* gGameContext = NULL;

MGameContext::MGameContext(MBasicPlatform* platform) {
	gPlatform = platform;
	gGameContext = this;
}

MGameContext::~MGameContext()
{
}

void MGameContext::UpdateEntityTable() {
	if (gPlatform->gMode == MPlatformMode::SCENE && gPlatform->gCurrentContainer != NULL) {
		gCurrentScene = reinterpret_cast<MScene*>(gPlatform->gCurrentContainer);
		for (unsigned int i = 0; i < gCurrentScene->mActorMap.size(); i++) {
			if (gCurrentScene->mActors[i]->mClassName == "Entity") {
				gEntityTable[gCurrentScene->mActors[i]->name] = reinterpret_cast<MEntity*>(gCurrentScene->mActors[i]);
			}
		}
	}
}

glm::vec3 MGameContext::CastRayByCursorPosition(double x, double y) {
	if (gPlatform->gMode == MPlatformMode::SCENE && gCurrentScene != NULL) {
		if (gCurrentScene->gLocalCamera != NULL) {
			glm::vec2 cpos = glm::vec2(x, y);
			glm::vec3 ray_start = glm::unProject(glm::vec3(cpos.x, gPlatform->gFramebufferHeight - cpos.y, 0.0f), gCurrentScene->gLocalCamera->GetViewMatrix(), gCurrentScene->gLocalCamera->GetProjectionMatrix(), glm::vec4(0.0f, 0.0f, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight));
			glm::vec3 ray_end = glm::unProject(glm::vec3(cpos.x, gPlatform->gFramebufferHeight - cpos.y, 1.0f), gCurrentScene->gLocalCamera->GetViewMatrix(), gCurrentScene->gLocalCamera->GetProjectionMatrix(), glm::vec4(0.0f, 0.0f, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight));
			glm::vec3 ray = glm::normalize(ray_end - ray_start);
			return ray;
		}
	}
	return glm::vec3(0.0);
}

glm::vec3 MGameContext::GetRaycastingBlockPosition(const glm::vec3& o, const glm::vec3& r, double d) {
	if (gPlatform->gMode == MPlatformMode::SCENE && gCurrentScene != NULL) {
		physx::PxVec3 origin(o.x, o.y, o.z);
		physx::PxVec3 unitDir(r.x, r.y, r.z);
		physx::PxReal maxDist = d;
		physx::PxRaycastBuffer hit;
		bool statue = gCurrentScene->pScene->raycast(origin, unitDir, maxDist, hit);
		if (statue) {
			return glm::vec3(hit.block.position.x, hit.block.position.y, hit.block.position.z);
		}
	}
	return glm::vec3(0.0);
}

void MGameContext::SetLocalCamera() {
	if (gPlatform->gMode == MPlatformMode::SCENE && gCurrentScene != NULL) {
		//gCurrentScene->SetLocalCamera(*reinterpret_cast<MCameraComponent*>(gCurrentEntity->mCurrentComponent));
	}
}

void MGameContext::BindThisEntity() {
	if (gThisEntity) {
		gCurrentEntity = gThisEntity;
	}
}

void MGameContext::BindThisEntityComponent() {
	if (gCurrentEntity != NULL) {
		gCurrentEntity->BindThisEntityComponent();
	}
}

void MGameContext::BindEntity(const std::string& name) {
	if (gEntityTable[name]) {
		gCurrentEntity = gEntityTable[name];
	}
}

void MGameContext::BindEntityComponent(const std::string& name) {
	if (gCurrentEntity != NULL && gCurrentEntity->componentMap[name] != NULL) {
		gCurrentEntity->BindEntityComponent(name);
	}
}

glm::vec3 MGameContext::RigidDynamic_GetPosition() {
	if (gCurrentEntity != NULL) {
		return gCurrentEntity->GetEntityComponentPosition();
	}
	else {
		return glm::vec3(0.0f);
	}
}

void MGameContext::RigidDynamic_Invalidate() {
	if (gCurrentEntity != NULL) {
		gCurrentEntity->RigidBodyPhysicsProxy_Invalidate();
	}
}

void MGameContext::RigidDynamic_Validate() {
	if (gCurrentEntity != NULL) {
		gCurrentEntity->RigidBodyPhysicsProxy_Validate();
	}
}

void MGameContext::RigidDynamic_SetPosition(double x, double y, double z) {
	if (gCurrentEntity != NULL) {
		return gCurrentEntity->RigidBodyPhysicsProxy_SetPosition(x, y, z);
	}
}

void MGameContext::RigidDynamic_AddForce(double x, double y, double z, int mode) {
	if (gCurrentEntity != NULL) {
		return gCurrentEntity->RigidBodyPhysicsProxy_AddForce(x, y, z, mode);
	}
}

void MGameContext::RigidDynamic_AddTorque(double x, double y, double z, int mode) {
	if (gCurrentEntity != NULL) {
		return gCurrentEntity->RigidBodyPhysicsProxy_AddTorque(x, y, z, mode);
	}
}

void MGameContext::CinematicCamera_SetPosition(double x, double y, double z) {
	if (gCurrentEntity != NULL) {
		return gCurrentEntity->CinematicCamera_SetPosition(x, y, z);
	}
}

void MGameContext::CinematicCamera_SetFront(double x, double y, double z) {
	if (gCurrentEntity != NULL) {
		return gCurrentEntity->CinematicCamera_SetFront(x, y, z);
	}
}

void MGameContext::CinematicCamera_SetUp(double x, double y, double z) {
	if (gCurrentEntity != NULL) {
		return gCurrentEntity->CinematicCamera_SetUp(x, y, z);
	}
}

void MGameContext::CinematicCamera_SetFovy(double fovy) {
	if (gCurrentEntity != NULL) {
		return gCurrentEntity->CinematicCamera_SetFovy(fovy);
	}
}

glm::vec3 MGameContext::CinematicCamera_GetUp() {
	if (gCurrentEntity != NULL) {
		return gCurrentEntity->CinematicCamera_GetUp();
	}
	else {
		return glm::vec3(0.0);
	}
}

glm::vec3 MGameContext::CinematicCamera_GetPosition() {
	if (gCurrentEntity != NULL) {
		return gCurrentEntity->CinematicCamera_GetPosition();
	}
	else {
		return glm::vec3(0.0);
	}
}

glm::vec3 MGameContext::CinematicCamera_GetFront() {
	if (gCurrentEntity != NULL) {
		return gCurrentEntity->CinematicCamera_GetFront();
	}
	else {
		return glm::vec3(0.0);
	}
}

double MGameContext::CinematicCamera_GetFovy() {
	if (gCurrentEntity != NULL) {
		return gCurrentEntity->CinematicCamera_GetFovy();
	}
	else {
		return 0.0;
	}
}

void MGameContext::ConsoleWriteLine(const std::string& text) {
	gPlatform->ConsoleWriteLine(1.0f, 1.0f, 1.0f, text);
}

void MGameContext::LoadScene(const std::string& name) {
	gPlatform->LoadScene(name);
}

void MGameContext::SpawnEntity(const std::string& name, const std::string& def) {
	if (gPlatform->gMode == MPlatformMode::SCENE && gCurrentScene != NULL) {
		gCurrentScene->SpawnEntity(name, def);
	}
}

void MGameContext::SpawnEntityAutoName(const std::string& def) {
	if (gPlatform->gMode == MPlatformMode::SCENE && gCurrentScene != NULL) {
		gCurrentScene->SpawnEntity(GenerateGuidA(), def);
	}
}

void MGameContext::SpawnEntityEx(const std::string& name, const std::string& def, double xpos, double ypos, double zpos) {
	if (gPlatform->gMode == MPlatformMode::SCENE && gCurrentScene != NULL) {
		gCurrentScene->SpawnEntityEx(name, def, xpos, ypos, zpos);
	}
}

void MGameContext::SetFullscreen() {

}

void MGameContext::SetCursorInvisible() {
	gPlatform->SetCursorInvisible();
}

void MGameContext::SetCursorVisible() {
	gPlatform->SetCursorVisible();
}

int MGameContext::GetKey(int key) {
	if (gPlatform) {
		return gPlatform->GetKey(key);
	}
	else {
		return 0;
	}
}

int MGameContext::GetMouseLButtonDown() {
	if (gPlatform) {
		return gPlatform->GetMouseLeftButtonDown();
	}
	else {
		return 0;
	}
}

int MGameContext::GetMouseLButtonUp() {
	if (gPlatform) {
		return gPlatform->GetMouseLeftButtonUp();
	}
	else {
		return 0;
	}
}

int MGameContext::GetMouseRButtonDown() {
	if (gPlatform) {
		return gPlatform->GetMouseRightButtonDown();
	}
	else {
		return 0;
	}
}

int MGameContext::GetMouseRButtonUp() {
	if (gPlatform) {
		return gPlatform->GetMouseRightButtonDown();
	}
	else {
		return 0;
	}
}

glm::vec2 MGameContext::GetCursorPos() {
	if (gPlatform) {
		return glm::vec2(gPlatform->GetCursorPosX(), gPlatform->GetCursorPosY());
	}
	else {
		return glm::vec2(0.0);
	}
}

glm::vec2 MGameContext::GetScrollOffset() {
	if (gPlatform) {
		return glm::vec2(gPlatform->GetScrollOffsetX(), gPlatform->GetScrollOffsetY());
	}
	else {
		return glm::vec2(0.0);
	}
}

double MGameContext::GetWorldTime() {
	if (gPlatform->gMode == MPlatformMode::SCENE && gCurrentScene != NULL) {
		return gCurrentScene->GetWorldTime();
	}
	else {
		return 0.0;
	}
}

void MGameContext::SetWorldTime(double t) {
	if (gPlatform->gMode == MPlatformMode::SCENE && gCurrentScene != NULL) {
		gCurrentScene->SetWorldTime(t);
	}
}

void MGameContext::SetDynamicTimeOfDay(int flag) {
	if (gPlatform->gMode == MPlatformMode::SCENE && gCurrentScene != NULL) {
		gCurrentScene->SetDynamicTimeOfDay(flag);
	}
}

void Game_SetCursorInvisible() {
	if (gGameContext) {
		gGameContext->SetCursorInvisible();
	}
}

void Game_SetCursorVisible() {
	if (gGameContext) {
		gGameContext->SetCursorVisible();
	}
}

void Game_SetLocalCamera() {
	if (gGameContext) {
		gGameContext->SetLocalCamera();
	}
}

void Game_BindThisEntity() {
	if (gGameContext) {
		gGameContext->BindThisEntity();
	}
}

void Game_BindEntity(const std::string& v) {
	if (gGameContext) {
		gGameContext->BindEntity(v);
	}
}

void Game_ConsoleWriteLine(const std::string& v) {
	if (gGameContext) {
		gGameContext->ConsoleWriteLine(v);
	}
}

void Game_LoadScene(const std::string& v) {
	if (gGameContext) {
		gGameContext->LoadScene(v);
	}
}

void Game_SpawnEntity(const std::string& v1, const std::string& v2) {
	if (gGameContext) {
		gGameContext->SpawnEntity(v1, v2);
	}
}

void Game_SpawnEntityAutoName(const std::string& v1) {
	if (gGameContext) {
		gGameContext->SpawnEntityAutoName(v1);
	}
}

void Game_SpawnEntityEx(const std::string& v1, const std::string& v2, double x, double y, double z) {
	if (gGameContext) {
		gGameContext->SpawnEntityEx(v1, v2, x, y, z);
	}
}

std::string Game_SpawnEntityAutoNameEx(const std::string& v2, double x, double y, double z) {
	if (gGameContext) {
		std::string name = GenerateGuidA();
		gGameContext->SpawnEntityEx(name, v2, x, y, z);
		return name;
	}
	return std::string();
}

int Game_GetKey(int n) {
	if (gGameContext) {
		return gGameContext->GetKey(n);
	}
	else {
		return 0;
	}
}

int Game_GetMouseLButtonDown() {
	if (gGameContext) {
		return gGameContext->GetMouseLButtonDown();
	}
	else {
		return 0;
	}
}

int Game_GetMouseLButtonUp() {
	if (gGameContext) {
		return gGameContext->GetMouseLButtonUp();
	}
	else {
		return 0;
	}
}

int Game_GetMouseRButtonDown() {
	if (gGameContext) {
		return gGameContext->GetMouseRButtonDown();
	}
	else {
		return 0;
	}
}

int Game_GetMouseRButtonUp() {
	if (gGameContext) {
		return gGameContext->GetMouseRButtonUp();
	}
	else {
		return 0;
	}
}

glm::vec3 Game_CastRayByCursorPosition(double x, double y) {
	if (gGameContext) {
		return gGameContext->CastRayByCursorPosition(x, y);
	}
	else {
		return glm::vec3(0.0);
	}
}

glm::vec3 Game_GetRaycastingBlockPosition(const glm::vec3& o, const glm::vec3& r, double d) {
	if (gGameContext) {
		return gGameContext->GetRaycastingBlockPosition(o, r, d);
	}
	return glm::vec3(0.0);
}

double Game_GetCursorPosX() {
	if (gGameContext) {
		return gGameContext->GetCursorPos().x;
	}
	else {
		return 0.0;
	}
}

double Game_GetCursorPosY() {
	if (gGameContext) {
		return gGameContext->GetCursorPos().y;
	}
	else {
		return 0.0;
	}
}

double Game_GetScrollOffsetX() {
	if (gGameContext) {
		return gGameContext->GetScrollOffset().x;
	}
	else {
		return 0.0;
	}
}

double Game_GetScrollOffsetY() {
	if (gGameContext) {
		return gGameContext->GetScrollOffset().y;
	}
	else {
		return 0.0;
	}
}

void Entity_BindThisEntityComponent() {
	if (gGameContext) {
		gGameContext->BindThisEntityComponent();
	}
}

void Entity_BindEntityComponent(const std::string& name) {
	if (gGameContext) {
		gGameContext->BindEntityComponent(name);
	}
}

glm::vec3 RigidDynamic_GetPosition() {
	if (gGameContext) {
		return gGameContext->RigidDynamic_GetPosition();
	}
	else {
		return glm::vec3(0.0f);
	}
}

void RigidDynamic_Invalidate() {
	if (gGameContext) {
		gGameContext->RigidDynamic_Invalidate();
	}
}

void RigidDynamic_Validate() {
	if (gGameContext) {
		gGameContext->RigidDynamic_Validate();
	}
}

void RigidDynamic_SetPosition(double x, double y, double z) {
	if (gGameContext) {
		gGameContext->RigidDynamic_SetPosition(x, y, z);
	}
}

void RigidDynamic_AddForce(double x, double y, double z, int mode) {
	if (gGameContext) {
		gGameContext->RigidDynamic_AddForce(x, y, z, mode);
	}
}

void RigidDynamic_AddTorque(double x, double y, double z, int mode) {
	if (gGameContext) {
		gGameContext->RigidDynamic_AddTorque(x, y, z, mode);
	}
}

void CinematicCamera_SetPosition(double x, double y, double z) {
	if (gGameContext) {
		gGameContext->CinematicCamera_SetPosition(x, y, z);
	}
}

glm::vec3 CinematicCamera_GetPosition() {
	if (gGameContext) {
		return gGameContext->CinematicCamera_GetPosition();
	}
	else {
		return glm::vec3(0.0);
	}
}

glm::vec3 CinematicCamera_GetFront() {
	if (gGameContext) {
		return gGameContext->CinematicCamera_GetFront();
	}
	else {
		return glm::vec3(0.0);
	}
}

glm::vec3 CinematicCamera_GetUp() {
	if (gGameContext) {
		return gGameContext->CinematicCamera_GetUp();
	}
	else {
		return glm::vec3(0.0);
	}
}

double CinematicCamera_GetFovy() {
	if (gGameContext) {
		return gGameContext->CinematicCamera_GetFovy();
	}
	else {
		return 0.0;
	}
}

void CinematicCamera_SetFront(double x, double y, double z) {
	if (gGameContext) {
		gGameContext->CinematicCamera_SetFront(x, y, z);
	}
}

void CinematicCamera_SetUp(double x, double y, double z) {
	if (gGameContext) {
		gGameContext->CinematicCamera_SetUp(x, y, z);
	}
}

void CinematicCamera_SetFovy(double fovy) {
	if (gGameContext) {
		gGameContext->CinematicCamera_SetFovy(fovy);
	}
}

double Game_GetTime() {
	return glfwGetTime();
}

double Game_GetWorldTime() {
	if (gGameContext) {
		return gGameContext->GetWorldTime();
	}
	else {
		return 0.0;
	}
}

void Game_SetWorldTime(double t) {
	if (gGameContext) {
		gGameContext->SetWorldTime(t);
	}
}

void Game_SetDynamicTimeOfDay(int flag) {
	if (gGameContext) {
		gGameContext->SetDynamicTimeOfDay(flag);
	}
}

glm::vec3 Game_CreateVec3(float x, float y, float z) {
	return glm::vec3(x, y, z);
}

glm::vec3 Game_CreateVec4(float x, float y, float z, float w) {
	return glm::vec4(x, y, z, w);
}
