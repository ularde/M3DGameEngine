#include "pch.h"
#include "Entity.h"
#include "tinyxml2.h"
#include "SharedFunc.h"
#include "Scene.h"
#include "ActorEditorAgent.h"

class MScene;

MEntity::MEntity(MScene* parent_, const std::string& name_, const std::string& def_file_path_, const std::string& script_path_,
	const glm::vec3& position_, const glm::vec3& scale_, const glm::vec3& rotate_) {
	this->mClassName = "Entity";
	this->mParent = parent_;
	this->position = position_;
	this->scale = scale_;
	this->roate = rotate_;
	this->defFilePath = def_file_path_;
	this->gPlatform = this->mParent->GetPlatform();
	this->name = name_;
	this->LoadDefinition();
	this->mEditorAgent = new MActorEditorAgent(gPlatform, this);
}

MEntity::~MEntity() {

}

void MEntity::LoadDefinition() {
	if (this->entityDefDoc.LoadFile(GetFullAssetPathA(this->defFilePath.c_str())) != tinyxml2::XML_SUCCESS) {
		this->gPlatform->ConsoleWriteLine(1.0f, 0.0f, 0.0f, std::string("[ERROR] Engine : Failed to load entity from XML document ") + this->defFilePath);
	}
	else {
		tinyxml2::XMLElement* root = this->entityDefDoc.RootElement();
		tinyxml2::XMLElement* componentNode = root->FirstChildElement("EntityComponent");

		this->mLuaScriptPath = root->Attribute("script");
		this->secondaryClassName = root->Attribute("name");

		std::string componentClass, componentName;

		while (componentNode) {
			componentName = componentNode->Attribute("name");
			componentClass = componentNode->Attribute("class");

			if (componentClass == "StaticMesh") {
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

			componentNode = componentNode->NextSiblingElement("EntityComponent");
		}
	}
}

void MEntity::BindEntityComponent(const std::string& name) {
	if (componentMap[name] != NULL) {
		mCurrentComponent = componentMap[name];
	}
}

glm::vec3 MEntity::GetEntityComponentPosition() {
	if (mCurrentComponent != NULL && mCurrentComponent->mClassName == "RigidBodyPhysicsProxySphere" || mCurrentComponent->mClassName == "RigidBodyPhysicsProxyBox" || mCurrentComponent->mClassName == "CharacterController") {
		return reinterpret_cast<MRigidBodyPhysicsProxy*>(mCurrentComponent)->GetPosition();
	}
	else {
		return glm::vec3(0.0f);
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

void MEntity::Update(double delta_time) {
	for (unsigned int i = 0; i < this->components.size(); i++) {
		this->components[i]->Update(delta_time);
	}
}

void MEntity::Render() {
	for (unsigned int i = 0; i < this->components.size(); i++) {
		this->components[i]->Render();
	}
}

void MEntity::RenderForDepthMapping() {
	for (unsigned int i = 0; i < this->components.size(); i++) {
		this->components[i]->RenderForDepthMapping();
	}
}

void MEntity::InitializeLuaInstance()
{
}

void MEntity::AddStaticMesh(const std::string& name, tinyxml2::XMLElement* node) {
	const char* modelPath = node->Attribute("model");
	const char* positionStr = node->Attribute("position");
	const char* scaleStr = node->Attribute("scale");
	const char* rotateStr = node->Attribute("rotate");
	std::string physicsProxyName = node->Attribute("physics_proxy");

	float pos_x = 0.0f, pos_y = 0.0f, pos_z = 0.0f;
	assert(GetVec3FromString(positionStr, &pos_x, &pos_y, &pos_z) == 3);
	float scale_x = 0.0f, scale_y = 0.0f, scale_z = 0.0f;
	assert(GetVec3FromString(scaleStr, &scale_x, &scale_y, &scale_z) == 3);
	float rotate_x = 0.0f, rotate_y = 0.0f, rotate_z = 0.0f;
	assert(GetVec3FromString(rotateStr, &rotate_x, &rotate_y, &rotate_z) == 3);

	MStaticMesh* object = new MStaticMesh(this, this->componentMap[physicsProxyName],
		name, modelPath,
		glm::vec3(pos_x, pos_y, pos_z),
		glm::vec3(scale_x, scale_y, scale_z),
		glm::vec3(rotate_x, rotate_y, rotate_z));
	this->components.push_back(object);
	this->componentMap[name] = object;
}

void MEntity::AddRigidBodyPhysicsProxySphere(const std::string& name, tinyxml2::XMLElement* node) {
	float radius = node->FloatAttribute("radius");
	float density = node->FloatAttribute("density");
	bool kinematic = node->IntAttribute("kinematic");
	std::string surfaceType = node->Attribute("surface_type");
	const char* massCenterStr = node->Attribute("mass_center");
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
	assert(GetVec3FromString(massCenterStr, &mc_x, &mc_y, &mc_z) == 3);

	MRigidBodyPhysicsProxySphere* object = new MRigidBodyPhysicsProxySphere(this, name, 
		radius, density, 
		glm::vec3(mc_x, mc_y, mc_z), 
		surfaceTypeID);
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
	const char* iPosStr = node->Attribute("position");
	const char* iFrontStr = node->Attribute("front");

	float i_front_x = 0.0f, i_front_y = 0.0f, i_front_z = 0.0f;
	assert(GetVec3FromString(iFrontStr, &i_front_x, &i_front_y, &i_front_z) == 3);

	MFirstPersonCamera* object = new MFirstPersonCamera(this, name,
		glm::vec3(i_front_x, i_front_y, i_front_z));
	this->components.push_back(object);
	this->componentMap[name] = object;
}
