#include "pch.h"
#include "Entity.h"
#include "tinyxml2.h"
#include "SharedFunc.h"
#include "Scene.h"
#include "ActorEditorAgent.h"
#include "GameContext.h"

class MScene;

MEntity::MEntity(MScene* parent_, const std::string& name_, const std::string& def_file_path_, const std::string& script_path_,
	const glm::vec3& position_, const glm::vec3& scale_, const glm::vec4& rotate_) {
	this->mPrototypeName = "Entity";
	this->mParent = parent_;
	this->mPosition = position_;
	this->mScale = scale_;
	this->mRotate = rotate_;
	this->mDefFilePath = def_file_path_;
	this->gPlatform = this->mParent->GetPlatform();
	this->mName = name_;
	this->LoadDefinition();
	this->mEditorAgent = new MActorEditorAgent(gPlatform, this);
	gPlatform->gGameContext->gThisEntity = this;
}

MEntity::~MEntity() {

}

void MEntity::Save()
{
}

void MEntity::LoadDefinition() {
	if (this->entityDefDoc.LoadFile(GetFullAssetPathA(this->mDefFilePath.c_str())) != tinyxml2::XML_SUCCESS) {
		this->gPlatform->ConsoleWriteLine(1.0f, 0.0f, 0.0f, std::string("[ERROR] Engine : Failed to load entity from XML document ") + this->mDefFilePath);
	}
	else {
		tinyxml2::XMLElement* root = this->entityDefDoc.RootElement();
		tinyxml2::XMLElement* componentNode = root->FirstChildElement("EntityComponent");

		this->mLuaScriptPath = root->Attribute("script");
		this->mClassName = root->Attribute("name");

		InitializeLuaInstance();

		std::string componentClass, componentName;

		while (componentNode) {
			componentName = componentNode->Attribute("name");
			componentClass = componentNode->Attribute("class");

			if (componentClass == "StaticMeshComponent") {
				this->AddStaticMesh(componentName, componentNode);
			}
			else if (componentClass == "RigidBodyPhysicsProxySphere") {
				this->AddRigidBodyPhysicsProxySphere(componentName, componentNode);
			}
			else if (componentClass == "RigidBodyPhysicsProxyBox") {
				this->AddRigidBodyPhysicsProxyBox(componentName, componentNode);
			}
			else if (componentClass == "RigidBodyPhysicsProxyCapsule") {
				this->AddRigidBodyPhyscisProxyCapsule(componentName, componentNode);
			}
			else if (componentClass == "CapsuleShapedCharacterController") {
				this->AddCapsuleShapedCharacterController(componentName, componentNode);
			}
			else if (componentClass == "FirstPersonCamera") {
				this->AddFirstPersonCamera(componentName, componentNode);
			}
			else if (componentClass == "CinematicCamera") {
				this->AddCinematicCamera(componentName, componentNode);
			}

			componentNode = componentNode->NextSiblingElement("EntityComponent");
		}
	}
}

void MEntity::BindThisEntityComponent() {
	mCurrentComponent = mThisComponent;
}

void MEntity::BindEntityComponent(const std::string& name) {
	if (componentMap[name] != NULL) {
		mCurrentComponent = componentMap[name];
	}
}

glm::vec3 MEntity::GetEntityComponentPosition() {
	if (mCurrentComponent != NULL && mCurrentComponent->IsRigidBodyPhysicsProxy()) {
		return reinterpret_cast<MRigidBodyPhysicsProxy*>(mCurrentComponent)->GetPosition();
	}
	else {
		return glm::vec3(0.0f);
	}
}

MRigidBodyPhysicsProxySphere* MEntity::GetRigidBodyPhysicsProxySphere(const std::string& name) {
	if (componentMap[name]) {
		return reinterpret_cast<MRigidBodyPhysicsProxySphere*>(componentMap[name]);
	}
	return nullptr;
}

MRigidBodyPhysicsProxyBox* MEntity::GetRigidBodyPhysicsProxyBox(const std::string& name) {
	if (componentMap[name]) {
		return reinterpret_cast<MRigidBodyPhysicsProxyBox*>(componentMap[name]);
	}
	return nullptr;
}

MCinematicCamera* MEntity::GetCinematicCamera(const std::string& name) {
	if (componentMap[name]) {
		return reinterpret_cast<MCinematicCamera*>(componentMap[name]);
	}
	return nullptr;
}

MFirstPersonCamera* MEntity::GetFirstPersonCamera(const std::string& name) {
	if (componentMap[name]) {
		return reinterpret_cast<MFirstPersonCamera*>(componentMap[name]);
	}
	return nullptr;
}

void MEntity::RigidBodyPhysicsProxy_Invalidate() {
	if (mCurrentComponent != NULL && mCurrentComponent->mClassName == "RigidBodyPhysicsProxySphere" || mCurrentComponent->mClassName == "RigidBodyPhysicsProxyBox") {
		physx::PxRigidDynamic* actor = reinterpret_cast<MRigidBodyPhysicsProxy*>(mCurrentComponent)->GetPxActor();
		physx::PxScene* scene = mParent->pScene;
		if (actor && scene) {
			if (!mInvalidated) {
				mParent->pScene->removeActor(*actor);
				mInvalidated = true;
			}
		}
	}
}

void MEntity::RigidBodyPhysicsProxy_Validate() {
	if (mCurrentComponent != NULL && mCurrentComponent->mClassName == "RigidBodyPhysicsProxySphere" || mCurrentComponent->mClassName == "RigidBodyPhysicsProxyBox") {
		physx::PxRigidDynamic* actor = reinterpret_cast<MRigidBodyPhysicsProxy*>(mCurrentComponent)->GetPxActor();
		physx::PxScene* scene = mParent->pScene;
		if (actor && scene) {
			if (mInvalidated) {
				mParent->pScene->addActor(*actor);
				mInvalidated = false;
			}
		}
	}
}

void MEntity::RigidBodyPhysicsProxy_SetPosition(double x, double y, double z) {
	if (mCurrentComponent != NULL && mCurrentComponent->mClassName == "RigidBodyPhysicsProxySphere" || mCurrentComponent->mClassName == "RigidBodyPhysicsProxyBox") {
		physx::PxTransform t(physx::PxVec3(x, y, z));
		physx::PxRigidDynamic* actor = reinterpret_cast<MRigidBodyPhysicsProxy*>(mCurrentComponent)->GetPxActor();
		if (actor) {
			actor->setGlobalPose(t);
		}
	}
}

void MEntity::RigidBodyPhysicsProxy_AddForce(double x, double y, double z, int mode) {
	if (mCurrentComponent != NULL && mCurrentComponent->mClassName == "RigidBodyPhysicsProxySphere" || mCurrentComponent->mClassName == "RigidBodyPhysicsProxyBox") {
		reinterpret_cast<MRigidBodyPhysicsProxy*>(mCurrentComponent)->AddForce(x, y, z, mode);
	}
}

void MEntity::RigidBodyPhysicsProxy_AddTorque(double x, double y, double z, int mode) {
	if (mCurrentComponent != NULL && mCurrentComponent->mClassName == "RigidBodyPhysicsProxySphere" || mCurrentComponent->mClassName == "RigidBodyPhysicsProxyBox") {
		reinterpret_cast<MRigidBodyPhysicsProxy*>(mCurrentComponent)->AddTorque(x, y, z, mode);
	}
}

void MEntity::CinematicCamera_SetPosition(double x, double y, double z) {
	if (mCurrentComponent != NULL && mCurrentComponent->mClassName == "CinematicCamera") {
		reinterpret_cast<MCinematicCamera*>(mCurrentComponent)->SetPosition(glm::vec3(x, y, z));
	}
}

void MEntity::CinematicCamera_SetFront(double x, double y, double z) {
	if (mCurrentComponent != NULL && mCurrentComponent->mClassName == "CinematicCamera") {
		reinterpret_cast<MCinematicCamera*>(mCurrentComponent)->SetFront(glm::vec3(x, y, z));
	}
}

void MEntity::CinematicCamera_SetUp(double x, double y, double z) {
	if (mCurrentComponent != NULL && mCurrentComponent->mClassName == "CinematicCamera") {
		reinterpret_cast<MCinematicCamera*>(mCurrentComponent)->SetUp(glm::vec3(x, y, z));
	}
}

void MEntity::CinematicCamera_SetFovy(double fovy) {
	if (mCurrentComponent != NULL && mCurrentComponent->mClassName == "CinematicCamera") {
		reinterpret_cast<MCinematicCamera*>(mCurrentComponent)->SetFovy(fovy);
	}
}

glm::vec3 MEntity::CinematicCamera_GetUp() {
	if (mCurrentComponent != NULL && mCurrentComponent->mClassName == "CinematicCamera") {
		return reinterpret_cast<MCinematicCamera*>(mCurrentComponent)->mUpDirection;
	}
	else {
		return glm::vec3(0.0);
	}
}

glm::vec3 MEntity::CinematicCamera_GetPosition() {
	if (mCurrentComponent != NULL && mCurrentComponent->mClassName == "CinematicCamera") {
		return reinterpret_cast<MCinematicCamera*>(mCurrentComponent)->mPosition;
	}
	else {
		return glm::vec3(0.0);
	}
}

glm::vec3 MEntity::CinematicCamera_GetFront() {
	if (mCurrentComponent != NULL && mCurrentComponent->mClassName == "CinematicCamera") {
		return reinterpret_cast<MCinematicCamera*>(mCurrentComponent)->mFront;
	}
	else {
		return glm::vec3(0.0);
	}
}

double MEntity::CinematicCamera_GetFovy() {
	if (mCurrentComponent != NULL && mCurrentComponent->mClassName == "CinematicCamera") {
		return reinterpret_cast<MCinematicCamera*>(mCurrentComponent)->mFovy;
	}
	else {
		return 0.0;
	}
}

void MEntity::Update(double dt) {
	gPlatform->gGameContext->gThisEntity = this;
	for (unsigned int i = 0; i < this->components.size(); i++) {
		this->components[i]->Update(dt);
	}
}
void MEntity::InitializeLuaInstance() {
	//import the script
	gPlatform->gLuaState->safe_script_file(GetFullAssetPathA(mLuaScriptPath));
}

void MEntity::AddStaticMesh(const std::string& name, tinyxml2::XMLElement* node) {
	std::string modelPath = node->Attribute("model");
	std::string positionStr = node->Attribute("position");
	std::string scaleStr = node->Attribute("scale");
	std::string rotateStr = node->Attribute("rotate");
	std::string physicsProxyName = node->Attribute("physics_proxy");

	float pos_x = 0.0f, pos_y = 0.0f, pos_z = 0.0f;
	int num = GetVec3FromString(positionStr, &pos_x, &pos_y, &pos_z);
	assert(num == 3);
	float scale_x = 0.0f, scale_y = 0.0f, scale_z = 0.0f;
	num = GetVec3FromString(scaleStr, &scale_x, &scale_y, &scale_z);
	assert(num == 3);
	float rotate_w = 0.0f, rotate_x = 0.0f, rotate_y = 0.0f, rotate_z = 0.0f;
	num = GetVec4FromString(rotateStr, &rotate_x, &rotate_y, &rotate_z, &rotate_w);
	assert(num == 4);

	MStaticMeshComponent* object = new MStaticMeshComponent(this, this->componentMap[physicsProxyName],
		name, modelPath,
		glm::vec3(pos_x, pos_y, pos_z),
		glm::vec3(scale_x, scale_y, scale_z),
		glm::vec4(rotate_x, rotate_y, rotate_z, rotate_w));
	this->components.push_back(object);
	this->componentMap[name] = object;
}

void MEntity::AddRigidBodyPhysicsProxySphere(const std::string& name, tinyxml2::XMLElement* node) {
	float radius = node->FloatAttribute("radius");
	float density = node->FloatAttribute("density");
	bool kinematic = node->BoolAttribute("kinematic");
	std::string surfaceType = node->Attribute("surface_type");
	std::string massCenterStr = node->Attribute("mass_center");
	int surfaceTypeID = 0;

	if (surfaceType == "Metal") {
		surfaceTypeID = M3D_SURFACE_TYPE_METAL;
	}
	else if (surfaceType == "Stone") {
		surfaceTypeID = M3D_SURFACE_TYPE_STONE;
	}
	else if (surfaceType == "Mud") {
		surfaceTypeID = M3D_SURFACE_TYPE_MUD;
	}
	else if (surfaceType == "Wood") {
		surfaceTypeID = M3D_SURFACE_TYPE_WOOD;
	}
	else if (surfaceType == "Plastic") {
		surfaceTypeID = M3D_SURFACE_TYPE_PLASTIC;
	}

	float mc_x = 0.0f, mc_y = 0.0f, mc_z = 0.0f;
	int num = GetVec3FromString(massCenterStr, &mc_x, &mc_y, &mc_z);
	assert(num == 3);

	MRigidBodyPhysicsProxySphere* object = new MRigidBodyPhysicsProxySphere(this, name, 
		radius, density, 
		glm::vec3(mc_x, mc_y, mc_z), 
		surfaceTypeID, kinematic);
	this->components.push_back(object);
	this->componentMap[name] = object;
}

void MEntity::AddRigidBodyPhysicsProxyBox(const std::string& name, tinyxml2::XMLElement* node) {

}

void MEntity::AddRigidBodyPhyscisProxyCapsule(const std::string& name, tinyxml2::XMLElement* node) {

}

void MEntity::AddCapsuleShapedCharacterController(const std::string& name, tinyxml2::XMLElement* node) {
	float radius = node->FloatAttribute("radius");
	float height = node->FloatAttribute("height");
	float maxJumpHeight = node->FloatAttribute("max_jump_height");
	float step_offset = node->FloatAttribute("step_offset");
	float contact_offset = node->FloatAttribute("contact_offset");
	std::string camera = node->Attribute("camera");
	std::string surfaceType = node->Attribute("surface_type");
	int surfaceTypeID = 0;

	if (surfaceType == "Metal") {
		surfaceTypeID = M3D_SURFACE_TYPE_METAL;
	}
	else if (surfaceType == "Stone") {
		surfaceTypeID = M3D_SURFACE_TYPE_STONE;
	}
	else if (surfaceType == "Mud") {
		surfaceTypeID = M3D_SURFACE_TYPE_MUD;
	}
	else if (surfaceType == "Wood") {
		surfaceTypeID = M3D_SURFACE_TYPE_WOOD;
	}
	else if (surfaceType == "Plastic") {
		surfaceTypeID = M3D_SURFACE_TYPE_PLASTIC;
	}

	MCharacterController* object = new
		MCharacterController(this, name,
			reinterpret_cast<MCameraComponent*>(componentMap[camera]),
			radius, height, contact_offset,
			step_offset, maxJumpHeight, surfaceTypeID);
	this->components.push_back(object);
	this->componentMap[name] = object;
}

void MEntity::AddFirstPersonCamera(const std::string& name, tinyxml2::XMLElement* node) {
	std::string iFrontStr = node->Attribute("front");

	float i_front_x = 0.0f, i_front_y = 0.0f, i_front_z = 0.0f;
	int num = GetVec3FromString(iFrontStr, &i_front_x, &i_front_y, &i_front_z);
	assert(num == 3);

	MFirstPersonCamera* object = new MFirstPersonCamera(this, name,
		glm::vec3(i_front_x, i_front_y, i_front_z));
	this->components.push_back((MFirstPersonCamera*)object);
	this->componentMap[name] = (MFirstPersonCamera*)object;
}

void MEntity::AddCinematicCamera(const std::string& name, tinyxml2::XMLElement* node) {
	std::string iFrontStr = node->Attribute("front");

	float i_front_x = 0.0f, i_front_y = 0.0f, i_front_z = 0.0f;
	int num = GetVec3FromString(iFrontStr, &i_front_x, &i_front_y, &i_front_z);
	assert(num == 3);

	MCinematicCamera* object = new MCinematicCamera(this, name,
		glm::vec3(i_front_x, i_front_y, i_front_z));
	this->components.push_back(object);
	this->componentMap[name] = object;
}