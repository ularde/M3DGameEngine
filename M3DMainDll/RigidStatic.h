#pragma once
#include <string>
#include <glm/glm.hpp>
#include <PxPhysicsAPI.h>

#include "Actor.h"
#include "Material.h"

class MActor;
class MScene;
class MModel;
class MMaterial;

class MRigidStatic :public MActor {
public:
	MRigidStatic(MScene* scene_, const std::string& name_, const std::string& model_path, const std::string& material_path,
		const glm::vec3& position_, const glm::vec3& scale_, const glm::vec4& rotate_);
	~MRigidStatic();
	glm::vec3 mPosition;
	glm::vec4 mRotate;
	glm::vec3 mScale;
	glm::mat4 mModelMatrix = glm::mat4(1.0f);
	physx::PxScene* pParentScene = NULL;
	physx::PxRigidStatic* pActor = NULL;
	std::string mModelPath, mMaterialPath, mCustomMaterialGUID;
	MModel* mModel = NULL;
	MMaterial* mCustomMaterial = NULL;
	void LoadModelAsset();
	void InitializePhysics();
	virtual void Save()override;
	void UpdatePositionWhileCreating(const physx::PxVec3& pos);
	virtual void InitializeLuaInstance()override;
	virtual void Update(double dt)override;
private:
	bool mShouldRefreshTransparentMeshes = true;
};