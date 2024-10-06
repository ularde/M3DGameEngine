#include "pch.h"
#include "SceneEditorAgent.h"
#include "Scene.h"
#include "ActorEditorAgent.h"
#include "Actor.h"
#include "EntityComponent.h"
#include "RigidStatic.h"
#include "Camera.h"
#include "Entity.h"
#include <glm/glm.hpp>
#include <algorithm>

static bool gActorRemoved = false;

MSceneEditorAgent::MSceneEditorAgent(MBasicPlatform* platform, MScene* parent) {
	gPlatform = platform;
	mParent = parent;
}

MSceneEditorAgent::~MSceneEditorAgent()
{
}

void MSceneEditorAgent::CreateRigidStatic(const std::string& model) {
	//query name
	std::vector<int> objIDs;
	for (unsigned int i = 0u; i < mParent->mActors.size(); i++) {
		if (mParent->mActors[i]->mClassName == "RigidStatic") {
			MRigidStatic* csm = reinterpret_cast<MRigidStatic*>(mParent->mActors[i]);
			const char* objName = csm->name.c_str();
			int n = 0;
			if (sscanf(objName, "RigidStatic%d", &n) == 1) {
				objIDs.push_back(n);
			}
		}
	}
	int maxID = 0;
	if (objIDs.size() > 0) {
		maxID = *std::max_element(objIDs.begin(), objIDs.end());
	}
	//create object
	std::string name = std::string("RigidStatic") + std::to_string(maxID + 1);
	glm::vec3 position(0.0f, 0.0f, 0.0f);
	MRigidStatic* object = new MRigidStatic(mParent, name, model, "", position, glm::vec3(1.0f), glm::vec3(0.0f));
	mCreatingActor = object;
	mParent->mActors.push_back(object);
	mParent->mActorMap[name] = object;
	SetEditMode(MEditMode::CREATING);
}

void MSceneEditorAgent::CreateEntity(const std::string& def) {

}

void MSceneEditorAgent::SetEditMode(MEditMode mode) {
	gCurrentEditMode = mode;
}

bool MSceneEditorAgent::IsAnyAxisFocused() {
	bool v = false;
	for (unsigned int i = 0; i < mParent->mActors.size(); i++) {
		v |= mParent->mActors[i]->mEditorAgent->IsAnyAxisFocused();
	}
	return v;
}

bool MSceneEditorAgent::GetAnyAxisSelected() {
	return mXAxisSelected || mYAxisSelected || mZAxisSelected;
}

bool MSceneEditorAgent::GetAxisSelectionFlag(MAxis axis) {
	switch (axis)
	{
	case MAxis::X_AXIS:
		return mXAxisSelected;
		break;
	case MAxis::Y_AXIS:
		return mYAxisSelected;
		break;
	case MAxis::Z_AXIS:
		return mZAxisSelected;
		break;
	default:
		break;
	}
	return false;
}

void MSceneEditorAgent::SelectAxis(MAxis axis, bool flag) {
	switch (axis)
	{
	case MAxis::X_AXIS:
		mXAxisSelected = flag;
		break;
	case MAxis::Y_AXIS:
		mYAxisSelected = flag;
		break;
	case MAxis::Z_AXIS:
		mZAxisSelected = flag;
		break;
	default:
		break;
	}
}

void MSceneEditorAgent::Tick() {
	static MActorEditorAgent* target;
	glm::vec2 cpos = glm::vec2(gPlatform->GetCursorPosX(), gPlatform->GetCursorPosY());
	glm::vec3 ray_start = glm::unProject(glm::vec3(cpos.x, gPlatform->gFramebufferHeight - cpos.y, 0.0f), mParent->gEditorCamera->GetViewMatrix(), mParent->gEditorCamera->GetProjectionMatrix(), glm::vec4(0.0f, 0.0f, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight));
	glm::vec3 ray_end = glm::unProject(glm::vec3(cpos.x, gPlatform->gFramebufferHeight - cpos.y, 1.0f), mParent->gEditorCamera->GetViewMatrix(), mParent->gEditorCamera->GetProjectionMatrix(), glm::vec4(0.0f, 0.0f, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight));
	glm::vec3 ray = glm::normalize(ray_end - ray_start);
	glm::vec3 ppos(0.0f);
	glm::vec3 pos(0.0f);
	if (target) {
		ppos = target->GetPositionWorldCoordSys();
		float d = glm::dot(target->GetPositionWorldCoordSys() - mParent->gEditorCamera->position, mParent->gEditorCamera->front) / glm::dot(ray, mParent->gEditorCamera->front);
		pos = mParent->gEditorCamera->position + d * ray;
	}
	glUseProgram(0);
	glMatrixMode(GL_PROJECTION);
	glm::mat4 projection = mParent->gEditorCamera->GetProjectionMatrix();
	glLoadMatrixf(&projection[0][0]);
	glm::mat4 view = mParent->gEditorCamera->GetViewMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&view[0][0]);
	switch (gCurrentEditMode)
	{
	case MEditMode::TRANSLATING:
		for (unsigned int i = 0; i < mParent->mActors.size(); i++) {
			if (mParent->mActors[i]->mEditorAgent) {
				mParent->mActors[i]->mEditorAgent->Render();
				if (mParent->mActors[i]->mEditorAgent->IsSelected()) {
					if (mXAxisSelected) {
						mParent->mActors[i]->mEditorAgent->Translate(MAxis::X_AXIS, pos.x);
					}
					else if (mYAxisSelected) {
						mParent->mActors[i]->mEditorAgent->Translate(MAxis::Y_AXIS, pos.y);
					}
					else if (mZAxisSelected) {
						mParent->mActors[i]->mEditorAgent->Translate(MAxis::Z_AXIS, pos.z);
					}
				}
			}
		}
		if (!gPlatform->GetMouseLeftButtonDown()) {
			CancelTransforming();
		}
		break;
	case MEditMode::ROTATING:
		for (unsigned int i = 0; i < mParent->mActors.size(); i++) {
			if (mParent->mActors[i]->mEditorAgent) {
				mParent->mActors[i]->mEditorAgent->Render();
			}
		}
		if (!gPlatform->GetMouseLeftButtonDown()) {
			CancelTransforming();
		}
		break;
	case MEditMode::SCALING:
		for (unsigned int i = 0; i < mParent->mActors.size(); i++) {
			if (mParent->mActors[i]->mEditorAgent) {
				mParent->mActors[i]->mEditorAgent->Render();
			}
		}
		if (!gPlatform->GetMouseLeftButtonDown()) {
			CancelTransforming();
		}
		break;
	case MEditMode::CREATING:
		if (mCreatingActor) {
			MRigidStatic* rs = reinterpret_cast<MRigidStatic*>(mCreatingActor);
			MEntity* ent = reinterpret_cast<MEntity*>(mCreatingActor);
			if (!gPlatform->GetMouseLeftButtonDown()) {
				if (mCreatingActor->mClassName == "RigidStatic") {
					//ray casting (for creating RigidStatic)
					glm::vec2 cpos = glm::vec2(gPlatform->GetCursorPosX(), gPlatform->GetCursorPosY());
					glm::vec3 ray_start = glm::unProject(glm::vec3(cpos.x, gPlatform->gFramebufferHeight - cpos.y, 0.0f), mParent->gEditorCamera->GetViewMatrix(), mParent->gEditorCamera->GetProjectionMatrix(), glm::vec4(0.0f, 0.0f, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight));
					glm::vec3 ray_end = glm::unProject(glm::vec3(cpos.x, gPlatform->gFramebufferHeight - cpos.y, 1.0f), mParent->gEditorCamera->GetViewMatrix(), mParent->gEditorCamera->GetProjectionMatrix(), glm::vec4(0.0f, 0.0f, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight));
					glm::vec3 ray = glm::normalize(ray_end - ray_start);
					physx::PxVec3 origin(mParent->gEditorCamera->position.x, mParent->gEditorCamera->position.y, mParent->gEditorCamera->position.z);
					physx::PxVec3 unitDir(ray.x, ray.y, ray.z);
					physx::PxReal maxDist = 10000.0f;
					physx::PxRaycastBuffer hit;
					bool statue = mParent->pScene->raycast(origin, unitDir, maxDist, hit);
					if (statue) {
						//temporarily remove the actor from the PxScene
						if (!gActorRemoved) {
							mParent->pScene->removeActor(*rs->pActor);
							gActorRemoved = true;
						}
						//updating position by position of cursor
						rs->UpdatePositionWhileCreating(hit.block.position);
					}
					mParent->pScene->flushQueryUpdates();
				}
				else if (mCreatingActor->mClassName == "Entity") {
					//ray casting (for creating Entity)
					//...
					//temporarily remove the actor from the PxScene
					if (!gActorRemoved) {
						for (unsigned int i = 0u; i < ent->components.size(); i++) {
							if (ent->components[i]->IsRigidBodyPhysicsProxy()) {
								mParent->pScene->removeActor(*reinterpret_cast<MRigidBodyPhysicsProxy*>(ent->components[i])->GetPxActor());
							}
						}
						gActorRemoved = true;
					}
				}
				else {
					SetEditMode(MEditMode::NONE);
				}
			}
			else {
				gActorRemoved = false;
				//add actor to PxScene again
				if (mCreatingActor->mClassName == "RigidStatic") {
					mParent->pScene->addActor(*rs->pActor);
				}
				else if (mCreatingActor->mClassName == "Entity") {
					for (unsigned int i = 0u; i < ent->components.size(); i++) {
						if (ent->components[i]->IsRigidBodyPhysicsProxy()) {
							mParent->pScene->addActor(*reinterpret_cast<MRigidBodyPhysicsProxy*>(ent->components[i])->GetPxActor());
						}
					}
				}
				SetEditMode(MEditMode::NONE);
			}
		}
		else {
			SetEditMode(MEditMode::NONE);
		}
		break;
	default:
		//esc cancel all
		if (gPlatform->GetKey(VK_ESCAPE) == 1) {
			for (unsigned int i = 0; i < mParent->mActors.size(); i++) {
				if (mParent->mActors[i]->mEditorAgent->IsSelected()) {
					mParent->mActors[i]->mEditorAgent->Select(false);
				}
			}
		}
		//delete
		if (gPlatform->GetKey(VK_DELETE) == 1) {
			int ok = MessageBox(NULL, L"Are you sure you want to delete selected objects?", L"M3DEditor", MB_YESNO | MB_ICONQUESTION);
			if (ok == IDYES) {
				for (unsigned int i = 0; i < mParent->mActors.size(); i++) {
					if (mParent->mActors[i]->mEditorAgent->IsSelected()) {
						if (mParent->mActors[i]->mClassName == "RigidStatic") {
							mParent->pScene->removeActor(*reinterpret_cast<MRigidStatic*>(mParent->mActors[i])->pActor);
						}
						mParent->mActors.erase(std::begin(mParent->mActors) + i);
						mParent->mActors.shrink_to_fit();
						i--;
					}
				}
			}
			reinterpret_cast<MEditor*>(gPlatform)->SubmitKeyboardInput(VK_DELETE, 0);
		}
		//normal loop
		for (unsigned int i = 0; i < mParent->mActors.size(); i++) {
			if (mParent->mActors[i]->mEditorAgent) {
				mParent->mActors[i]->mEditorAgent->Render();
				glm::vec2 xp2c = glm::normalize(mParent->mActors[i]->mEditorAgent->GetPositionWinCoordSys() - cpos);
				glm::vec2 xc2t = glm::normalize(cpos - mParent->mActors[i]->mEditorAgent->GetAxisTopPosWinCoordSys(MAxis::X_AXIS));
				float xangle = abs(acos(xp2c.x * xc2t.x + xp2c.y * xc2t.y));
				glm::vec2 yp2c = glm::normalize(mParent->mActors[i]->mEditorAgent->GetPositionWinCoordSys() - cpos);
				glm::vec2 yc2t = glm::normalize(cpos - mParent->mActors[i]->mEditorAgent->GetAxisTopPosWinCoordSys(MAxis::Y_AXIS));
				float yangle = abs(acos(yp2c.x * yc2t.x + yp2c.y * yc2t.y));
				glm::vec2 zp2c = glm::normalize(mParent->mActors[i]->mEditorAgent->GetPositionWinCoordSys() - cpos);
				glm::vec2 zc2t = glm::normalize(cpos - mParent->mActors[i]->mEditorAgent->GetAxisTopPosWinCoordSys(MAxis::Z_AXIS));
				float zangle = abs(acos(zp2c.x * zc2t.x + zp2c.y * zc2t.y));
				float rad = abs(glm::radians(1.0f + glm::distance(mParent->gEditorCamera->position, mParent->mActors[i]->mEditorAgent->GetPositionWorldCoordSys()) / 20.0f));
				bool xfocused = (xangle <= rad),
					yfocused = (yangle <= rad),
					zfocused = (zangle <= rad);
				//focuse of axes of the object
				if (xfocused) {
					mParent->mActors[i]->mEditorAgent->FocusAxis(MAxis::X_AXIS, true);
				}
				else if (yfocused) {
					mParent->mActors[i]->mEditorAgent->FocusAxis(MAxis::Y_AXIS, true);
				}
				else if (zfocused) {
					mParent->mActors[i]->mEditorAgent->FocusAxis(MAxis::Z_AXIS, true);
				}
				else {
					mParent->mActors[i]->mEditorAgent->FocusAxis(MAxis::X_AXIS, false);
					mParent->mActors[i]->mEditorAgent->FocusAxis(MAxis::Y_AXIS, false);
					mParent->mActors[i]->mEditorAgent->FocusAxis(MAxis::Z_AXIS, false);
				}
				//selection of axes
				if (xfocused && gPlatform->GetMouseLeftButtonDown()) {
					target = mParent->mActors[i]->mEditorAgent;
					SelectAxis(MAxis::X_AXIS, true);
					SelectAxis(MAxis::Y_AXIS, false);
					SelectAxis(MAxis::Z_AXIS, false);
					SetTransforming();
				}
				else if (yfocused && gPlatform->GetMouseLeftButtonDown()) {
					target = mParent->mActors[i]->mEditorAgent;
					SelectAxis(MAxis::X_AXIS, false);
					SelectAxis(MAxis::Y_AXIS, true);
					SelectAxis(MAxis::Z_AXIS, false);
					SetTransforming();
				}
				else if (zfocused && gPlatform->GetMouseLeftButtonDown()) {
					target = mParent->mActors[i]->mEditorAgent;
					SelectAxis(MAxis::X_AXIS, false);
					SelectAxis(MAxis::Y_AXIS, false);
					SelectAxis(MAxis::Z_AXIS, true);
					SetTransforming();
				}
				//focuse on an object
				if (glm::distance(mParent->mActors[i]->mEditorAgent->GetPositionWinCoordSys(), cpos) <= 20.0f) {
					mParent->mActors[i]->mEditorAgent->Focuse(true);
				}
				for (unsigned int j = 0u; j < mParent->mActors.size(); j++) {
					if (glm::distance(mParent->mActors[j]->mEditorAgent->GetPositionWinCoordSys(), cpos) > 20.0f) {
						mParent->mActors[j]->mEditorAgent->Focuse(false);
						if (gPlatform->GetMouseLeftButtonDown() && gPlatform->GetKey(VK_CONTROL) != 1 && !IsAnyAxisFocused()) {
							mParent->mActors[j]->mEditorAgent->Select(false);
						}
					}
				}
				//selection of an obejct
				if (mParent->mActors[i]->mEditorAgent->IsFocused() && gPlatform->GetMouseLeftButtonDown()) {
					if (gPlatform->GetKey(VK_CONTROL) != 1) {
						for (auto j = 0u; j < mParent->mActors.size(); j++) {
							mParent->mActors[j]->mEditorAgent->Select(false);
						}
					}
					mParent->mActors[i]->mEditorAgent->Select(true);
				}
			}
		}
		break;
	}
}

void MSceneEditorAgent::SetTransforming() {
	switch (gCurrentEditMode)
	{
	case MEditMode::NONE:
		break;
	case MEditMode::TRANSLATE:
		gCurrentEditMode = MEditMode::TRANSLATING;
		break;
	case MEditMode::ROTATE:
		gCurrentEditMode = MEditMode::ROTATING;
		break;
	case MEditMode::SCALE:
		gCurrentEditMode = MEditMode::SCALING;
		break;
	default:
		break;
	}
}

void MSceneEditorAgent::CancelTransforming() {
	switch (gCurrentEditMode)
	{
	case MEditMode::NONE:
		break;
	case MEditMode::TRANSLATING:
		gCurrentEditMode = MEditMode::TRANSLATE;
		break;
	case MEditMode::ROTATING:
		gCurrentEditMode = MEditMode::ROTATE;
		break;
	case MEditMode::SCALING:
		gCurrentEditMode = MEditMode::SCALE;
		break;
	default:
		break;
	}
}
