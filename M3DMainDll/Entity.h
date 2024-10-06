#pragma once
#include "Actor.h"
#include "EntityComponent.h"

#include <string>
#include <vector>
#include <map>

class MScene;
class MEntityComponent;

class MEntity :public MActor {
public:
	MEntity(MScene* parent_, const std::string& name_, const std::string& def_file_path_, const std::string& script_path_,
		const glm::vec3& position_, const glm::vec3& scale_, const glm::vec3& rotate_);
	~MEntity();
	virtual void Save()override;
	void LoadDefinition();
	void BindThisEntityComponent();
	void BindEntityComponent(const std::string& name);
	glm::vec3 GetEntityComponentPosition();
	MRigidBodyPhysicsProxySphere* GetRigidBodyPhysicsProxySphere(const std::string& name);
	MRigidBodyPhysicsProxyBox* GetRigidBodyPhysicsProxyBox(const std::string& name);
	MCinematicCamera* GetCinematicCamera(const std::string& name);
	MFirstPersonCamera* GetFirstPersonCamera(const std::string& name);
	void RigidBodyPhysicsProxy_Invalidate();
	void RigidBodyPhysicsProxy_Validate();
	void RigidBodyPhysicsProxy_SetPosition(double x, double y, double z);
	void RigidBodyPhysicsProxy_AddForce(double x, double y, double z, int mode);
	void RigidBodyPhysicsProxy_AddTorque(double x, double y, double z, int mode);
	void CinematicCamera_SetPosition(double x, double y, double z);
	void CinematicCamera_SetFront(double x, double y, double z);
	void CinematicCamera_SetUp(double x, double y, double z);
	void CinematicCamera_SetFovy(double fovy);
	glm::vec3 CinematicCamera_GetUp();
	glm::vec3 CinematicCamera_GetPosition();
	glm::vec3 CinematicCamera_GetFront();
	double CinematicCamera_GetFovy();
	virtual void Update(double delta_time)override;
	virtual void Render()override;
	virtual void RenderForDepthMapping()override;
	virtual void InitializeLuaInstance()override;
	MEntityComponent* mCurrentComponent = NULL;
	MEntityComponent* mThisComponent = NULL;
	std::string mDefFilePath, scriptPath, secondaryClassName;
	glm::vec3 position, scale, rotate;
	tinyxml2::XMLDocument entityDefDoc;
	std::vector<MEntityComponent*> components;
	std::map<std::string, MEntityComponent*> componentMap;
	bool mInvalidated = false;
protected:
	void AddStaticMesh(const std::string& name, tinyxml2::XMLElement* node);
	void AddRigidBodyPhysicsProxySphere(const std::string& name, tinyxml2::XMLElement* node);
	void AddRigidBodyPhysicsProxyBox(const std::string& name, tinyxml2::XMLElement* node);
	void AddRigidBodyPhyscisProxyCapsule(const std::string& name, tinyxml2::XMLElement* node);
	void AddCapsuleShapedCharacterController(const std::string& name, tinyxml2::XMLElement* node);
	void AddFirstPersonCamera(const std::string& name, tinyxml2::XMLElement* node);
	void AddCinematicCamera(const std::string& name, tinyxml2::XMLElement* node);
};