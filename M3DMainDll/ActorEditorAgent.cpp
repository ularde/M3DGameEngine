#include "pch.h"
#include "ActorEditorAgent.h"
#include "Actor.h"
#include "Entity.h"
#include "RigidStatic.h"
#include "Model.h"
#include "Scene.h"
#include "Camera.h"
#include "RenderDeferredPipeline.h"
#include "SceneEditorAgent.h"
#include <GL/glut.h>

class MEntity;
class MRigidStatic;
class MSceneEditorAgent;

MActorEditorAgent::MActorEditorAgent(MBasicPlatform* platform, MActor* parent) {
	gPlatform = platform;
	mParent = parent;
	Initialize();
}

MActorEditorAgent::MActorEditorAgent(MBasicPlatform* platform, MActor* parent, const glm::vec3& pos, float x, float y, float z) {
	gPlatform = platform;
	mParent = parent;
	mPosition = pos;
	mx = x;
	my = y;
	mz = z;
}

MActorEditorAgent::~MActorEditorAgent()
{
}

glm::vec2 MActorEditorAgent::GetAxisTopPosWinCoordSys(MAxis axis) {
	switch (axis)
	{
	case MAxis::X_AXIS:
		return mXAxisTopPosWinCoordSys;
		break;
	case MAxis::Y_AXIS:
		return mYAxisTopPosWinCoordSys;
		break;
	case MAxis::Z_AXIS:
		return mZAxisTopPosWinCoordSys;
		break;
	default:
		break;
	}
	return glm::vec2(0.0f);
}

void MActorEditorAgent::DrawSignalElement(MAxis axis, const glm::mat4& dmat) {
	switch (axis)
	{
	case MAxis::X_AXIS:
		switch (mParent->mParent->mEditorAgent->GetEditMode())
		{
		case MEditMode::TRANSLATE:
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(2.0f, 00.0f, 0.0f);
			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			fghCone(0.1, 0.5, 10, 10);
			break;
		case MEditMode::TRANSLATING:
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(2.0f, 0.0f, 0.0f);
			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			fghCone(0.1, 0.5, 10, 10);
			break;
		case MEditMode::ROTATE:
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(2.0f, 0.0f, 0.0f);
			fghSphere(0.15, 10, 10);
			break;
		case MEditMode::ROTATING:
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(2.0f, 0.0f, 0.0f);
			fghSphere(0.15, 10, 10);
			break;
		case MEditMode::SCALE:
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(2.0f, 0.0f, 0.0f);
			glutSolidCube(0.5);
			break;
		case MEditMode::SCALING:
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(2.0f, 0.0f, 0.0f);
			glutSolidCube(0.5);
			break;
		default:
			break;
		}
		glLoadMatrixf(&dmat[0][0]);
		break;
	case MAxis::Y_AXIS:
		switch (mParent->mParent->mEditorAgent->GetEditMode())
		{
		case MEditMode::TRANSLATE:
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(0.0f, 2.0f, 0.0f);
			glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
			fghCone(0.1, 0.5, 10, 10);
			break;
		case MEditMode::TRANSLATING:
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(0.0f, 2.0f, 0.0f);
			glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
			fghCone(0.1, 0.5, 10, 10);
			break;
		case MEditMode::ROTATE:
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(0.0f, 2.0f, 0.0f);
			fghSphere(0.15, 10, 10);
			break;
		case MEditMode::ROTATING:
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(0.0f, 2.0f, 0.0f);
			fghSphere(0.15, 10, 10);
			break;
		case MEditMode::SCALE:
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(0.0f, 2.0f, 0.0f);
			glutSolidCube(0.5);
			break;
		case MEditMode::SCALING:
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(0.0f, 2.0f, 0.0f);
			glutSolidCube(0.5);
			break;
		default:
			break;
		}
		glLoadMatrixf(&dmat[0][0]);
		break;
	case MAxis::Z_AXIS:
		switch (mParent->mParent->mEditorAgent->GetEditMode())
		{
		case MEditMode::TRANSLATE:
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(0.0f, 0.0f, 2.0f);
			fghCone(0.1, 0.5, 10, 10);
			break;
		case MEditMode::TRANSLATING:
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(0.0f, 0.0f, 2.0f);
			fghCone(0.1, 0.5, 10, 10);
			break;
		case MEditMode::ROTATE:
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(0.0f, 0.0f, 2.0f);
			fghSphere(0.15, 10, 10);
			break;
		case MEditMode::ROTATING:
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(0.0f, 0.0f, 2.0f);
			fghSphere(0.15, 10, 10);
			break;
		case MEditMode::SCALE:
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(0.0f, 0.0f, 2.0f);
			glutSolidCube(0.5);
			break;
		case MEditMode::SCALING:
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(0.0f, 0.0f, 2.0f);
			glutSolidCube(0.5);
			break;
		default:
			break;
		}
		glLoadMatrixf(&dmat[0][0]);
		break;
	default:
		break;
	}
}

void MActorEditorAgent::Render() {
	glm::mat4 projection = mParent->mParent->gEditorCamera->GetProjectionMatrix();
	glm::mat4 view = mParent->mParent->gEditorCamera->GetViewMatrix();
	glm::vec3 wpos = glm::project(mPosition, view, projection, glm::vec4(0, 0, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight));
	glm::vec3 xwpos = glm::project(mPosition + glm::vec3(2.0f, 0.0f, 0.0f), view, projection, glm::vec4(0, 0, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight));
	glm::vec3 ywpos = glm::project(mPosition + glm::vec3(0.0f, 2.0f, 0.0f), view, projection, glm::vec4(0, 0, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight));
	glm::vec3 zwpos = glm::project(mPosition + glm::vec3(0.0f, 0.0f, 2.0f), view, projection, glm::vec4(0, 0, gPlatform->gFramebufferWidth, gPlatform->gFramebufferHeight));
	mPositionWinCoordSys.x = wpos.x;
	mPositionWinCoordSys.y = gPlatform->gFramebufferHeight - wpos.y;
	mXAxisTopPosWinCoordSys.x = xwpos.x;
	mXAxisTopPosWinCoordSys.y = gPlatform->gFramebufferHeight - xwpos.y;
	mYAxisTopPosWinCoordSys.x = ywpos.x;
	mYAxisTopPosWinCoordSys.y = gPlatform->gFramebufferHeight - ywpos.y;
	mZAxisTopPosWinCoordSys.x = zwpos.x;
	mZAxisTopPosWinCoordSys.y = gPlatform->gFramebufferHeight - zwpos.y;
	if (mParent->mParent->IsEditorMode()) {
		if (mFocused) {
			RenderAABB();
			if (mSelected) {
				RenderAxes();
			}
		}
		else if (mSelected) {
			RenderAABB();
			RenderAxes();
		}
	}
}

bool MActorEditorAgent::IsSpecificAxisFocused(MAxis axis) {
	switch (axis)
	{
	case MAxis::X_AXIS:
		return mXAxisFocused;
		break;
	case MAxis::Y_AXIS:
		return mYAxisFocused;
		break;
	case MAxis::Z_AXIS:
		return mZAxisFocused;
		break;
	default:
		break;
	}
	return false;
}

void MActorEditorAgent::SetPosition(const glm::vec3& pos) {
	mPosition = pos;
}

void MActorEditorAgent::FocusAxis(MAxis axis, bool flag) {
	switch (axis)
	{
	case MAxis::X_AXIS:
		mXAxisFocused = flag;
		break;
	case MAxis::Y_AXIS:
		mYAxisFocused = flag;
		break;
	case MAxis::Z_AXIS:
		mZAxisFocused = flag;
		break;
	default:
		break;
	}
}

void MActorEditorAgent::Select(bool flag) {
	mSelected = flag;
}

void MActorEditorAgent::Focuse(bool flag) {
	mFocused = flag;
}

void MActorEditorAgent::Translate(MAxis axis, double len) {
	if (mParent->mClassName == "RigidStatic") {
		MRigidStatic* rs = reinterpret_cast<MRigidStatic*>(mParent);
		physx::PxTransform pose;
		physx::PxTransform ipose = rs->pActor->getGlobalPose();
		physx::PxVec3 ipos = ipose.p;
		switch (axis)
		{
		case MAxis::X_AXIS:
			mPosition = glm::vec3(len, ipos.y, ipos.z);
			pose = physx::PxTransform(physx::PxVec3(len, ipos.y, ipos.z));
			rs->pActor->setGlobalPose(pose);
			break;
		case MAxis::Y_AXIS:
			mPosition = glm::vec3(ipos.x, len, ipos.z);
			pose = physx::PxTransform(physx::PxVec3(ipos.x, len, ipos.z));
			rs->pActor->setGlobalPose(pose);
			break;
		case MAxis::Z_AXIS:
			mPosition = glm::vec3(ipos.x, ipos.y, len);
			pose = physx::PxTransform(physx::PxVec3(ipos.x, ipos.y, len));
			rs->pActor->setGlobalPose(pose);
			break;
		default:
			break;
		}
	}
}

void MActorEditorAgent::Rotate(MAxis axis, double angle)
{
}

void MActorEditorAgent::Scale(MAxis axis, double level)
{
}

void MActorEditorAgent::Initialize() {
	if (mParent->mClassName == "Entity") {
		MEntity* ent = reinterpret_cast<MEntity*>(mParent);
		mPosition = ent->position;
		mPositionWinCoordSys = glm::vec2(0.0f);
		std::vector<float> xarray, oxarray;
		std::vector<float> yarray, oyarray;
		std::vector<float> zarray, ozarray;
		for (unsigned int i = 0; i < ent->components.size(); i++) {
			if (ent->components[i]->mClassName == "StaticMesh") {
				MStaticMesh* sm = reinterpret_cast<MStaticMesh*>(ent->components[i]);
				glm::vec3 mxyz = sm->model->GetMaxXYZ();
				glm::vec3 oxyz = sm->model->GetOffsetXYZ();
				oxarray.push_back(oxyz.x);
				oyarray.push_back(oxyz.y);
				ozarray.push_back(oxyz.z);
				xarray.push_back(mxyz.x);
				yarray.push_back(mxyz.y);
				zarray.push_back(mxyz.z);
			}
			else if (ent->components[i]->mClassName == "RigidBodyPhysicsProxySphere") {

			}
			else if (ent->components[i]->mClassName == "CharacterController") {

			}
			else if (ent->components[i]->mClassName == "FirstPersonCamera") {

			}
			else if (ent->components[i]->mClassName == "RigidBodyPhysicsProxyBox") {

			}
		}
		if (((xarray.size() & yarray.size() & zarray.size()) > 0) && ((oxarray.size() & oyarray.size() & ozarray.size()) > 0)) {
			mx = *std::max_element(xarray.begin(), xarray.end());
			my = *std::max_element(yarray.begin(), yarray.end());
			mz = *std::max_element(zarray.begin(), zarray.end());
			ox = *std::max_element(oxarray.begin(), oxarray.end());
			oy = *std::max_element(oyarray.begin(), oyarray.end());
			oz = *std::max_element(ozarray.begin(), ozarray.end());
		}
	}
	else if (mParent->mClassName == "RigidStatic") {
		MRigidStatic* rs = reinterpret_cast<MRigidStatic*>(mParent);
		mPosition = rs->position;
		mPositionWinCoordSys = glm::vec2(0.0f);
		glm::vec3 mxyz = rs->mModel->GetMaxXYZ();
		glm::vec3 oxyz = rs->mModel->GetOffsetXYZ();
		ox = oxyz.x;
		oy = oxyz.y;
		oz = oxyz.z;
		mx = mxyz.x;
		my = mxyz.y;
		mz = mxyz.z;
	}
	else {

	}
}

void MActorEditorAgent::RenderAABB() {
	glUseProgram(0);
	glMatrixMode(GL_PROJECTION);
	glm::mat4 projection = mParent->mParent->gEditorCamera->GetProjectionMatrix();
	glLoadMatrixf(&projection[0][0]);
	glm::mat4 view = mParent->mParent->gEditorCamera->GetViewMatrix();
	glm::vec3 aabbpos(mPosition.x - ox, mPosition.y - oy, mPosition.z - oz);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), aabbpos);
	glm::mat4 modelview = view * model;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&modelview[0][0]);
	glBegin(GL_LINES);
	glVertex3f(0.0f, my, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f, my, 0.0f);
	glVertex3f(mx, my, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(mx, 0.0f, 0.0f);
	glVertex3f(mx, my, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(mx, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(mx, 0.0f, mz);
	glVertex3f(mx, 0.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, mz);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, mz);
	glVertex3f(0.0f, my, mz);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f, my, mz);
	glVertex3f(0.0f, 0.0f, mz);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f, my, mz);
	glVertex3f(0.0f, my, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, mz);
	glVertex3f(mx, 0.0f, mz);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(mx, 0.0f, 0.0f);
	glVertex3f(mx, 0.0f, mz);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(mx, my, mz);
	glVertex3f(mx, 0.0f, mz);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f, my, mz);
	glVertex3f(mx, my, mz);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(mx, my, 0.0f);
	glVertex3f(mx, my, mz);
	glEnd();
}

void MActorEditorAgent::RenderAxes() {
	glm::mat4 view = mParent->mParent->gEditorCamera->GetViewMatrix();
	glm::mat4 model = glm::translate(glm::mat4(1.0f), mPosition);
	glm::mat4 modelview = view * model;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&modelview[0][0]);
	if (mXAxisFocused || mParent->mParent->mEditorAgent->GetAxisSelectionFlag(MAxis::X_AXIS)) {
		glColor3f(1.0f, 1.0f, 0.0f);
	}
	else {
		glColor3f(1.0f, 0.0f, 0.0f);
	}
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 0.0f);
	glEnd();
	DrawSignalElement(MAxis::X_AXIS, modelview);
	if (mYAxisFocused || mParent->mParent->mEditorAgent->GetAxisSelectionFlag(MAxis::Y_AXIS)) {
		glColor3f(1.0f, 1.0f, 0.0f);
	}
	else {
		glColor3f(0.0f, 1.0f, 0.0f);
	}
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 2.0f, 0.0f);
	glEnd();
	DrawSignalElement(MAxis::Y_AXIS, modelview);
	if (mZAxisFocused || mParent->mParent->mEditorAgent->GetAxisSelectionFlag(MAxis::Z_AXIS)) {
		glColor3f(1.0f, 1.0f, 0.0f);
	}
	else {
		glColor3f(0.0f, 0.0f, 1.0f);
	}
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 2.0f);
	glEnd();
	DrawSignalElement(MAxis::Z_AXIS, modelview);
	glColor3f(1.0f, 1.0f, 1.0f);
}
