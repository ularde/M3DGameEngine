#pragma once
#include "Class.h"
#include <PxPhysicsAPI.h>

class MModel;
class MUnscriptableObject;
class MEntity;
class MAssetManager;
class MBasicPlatform;
class MStaticMeshComponent;
class MSkeletonMesh;
class MRigidBodyPhysicsProxy;
class MMeshComponent;
class MCharacterController;
class MCameraComponent;

enum class MCCTMovementType {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UPWARD,
	FORWARD_UPWARD
};

class MEntityComponent :public MScriptableObject {
public:
	bool mPhysicsReady = false;
	std::string name;
	MEntity* GetParent() { return mParent; }
	virtual void InitializeLuaInstance()override {};
	virtual void Update(double delta_time) = 0;
protected:
	void OnUpdate(double dt);
	void OnCreate();
	void OnDestroy();
	void GeneratePublicFunctionNames();
	void SetThisComponent();
	std::string nOnUpdate, nOnRender,
		nOnCreate, nOnDestroy;
	MEntity* mParent = NULL;
};

class MMeshComponent :public MEntityComponent {
public:
	MModel* model = NULL;
	MAssetManager* gAssetManager = NULL;
	MRigidBodyPhysicsProxy* mPhysicsProxy = NULL;
	std::string mModelPath;
	glm::vec3 mPosition, mScale;
	glm::vec4 mRotate;
};

class MRigidBodyPhysicsProxy :public MEntityComponent {
public:
	bool mIsKinematic = false;
	bool mOutputMeshAttachmentAssigned = false;
	virtual glm::mat4 GetModelMatrix() = 0;
	virtual void SetOutputAttachment(const std::string& name) = 0;
	glm::vec3 GetPosition();
	void SetPosition(double x, double y, double z);
	void AddForce(double x, double y, double z, int mode);
	void AddTorque(double x, double y, double z, int mode);
	void Invalidate();
	void Validate();
	physx::PxRigidDynamic* GetPxActor() { return pActor; }
protected:
	int mSurfaceTypeID = 0;
	MMeshComponent* mOutputMeshAttachment = NULL;
	physx::PxMaterial* pMaterial = NULL;
	physx::PxRigidDynamic* pActor = NULL;
	physx::PxTransform transform;
};

class MRigidBodyPhysicsProxySphere :public MRigidBodyPhysicsProxy {
public:
	MRigidBodyPhysicsProxySphere(MEntity* parent_, const std::string& name_,
		float radius_, float density_, const glm::vec3& mass_center_,
		int surface_type_ID);
	MRigidBodyPhysicsProxySphere(MEntity* parent_, const std::string& name_,
		float radius_, float density_, const glm::vec3& mass_center_,
		int surface_type_ID, bool is_kinematic);
	~MRigidBodyPhysicsProxySphere() { }
	virtual void Update(double delta_time)override;
	virtual void SetOutputAttachment(const std::string& name)override;
	virtual glm::mat4 GetModelMatrix()override;
protected:
	float mRadius, mDensity;
	glm::vec3 mMassCenter;
	void InitializePhysics();
};

class MRigidBodyPhysicsProxyBox :public MRigidBodyPhysicsProxy {
public:

};

class MCharacterController :public MRigidBodyPhysicsProxy {
public:
	MCharacterController(MEntity* parent,
		const std::string& name, MCameraComponent* camera,
		const float radius, const float height, const float contact_offset,
		const float step_offset, const float max_jump_height, int surface_type_ID);
	~MCharacterController();
	virtual void Update(double dt)override;
	virtual void SetOutputAttachment(const std::string& name)override;
	virtual glm::mat4 GetModelMatrix()override;
	void AttachCamera(const std::string& name);
	void Move(MCCTMovementType type, double dt);
	bool IsGrounded();
	physx::PxController* GetPxController() { return pController; }
	glm::vec3 GetControllerPosition() { return mControllerPosition; }
	glm::vec3 GetEyePosition() { return mEyePosition; }
	float yVelocity = 0.0f;
	float cGravity = -9.81f;
protected:
	std::string mCameraName;
	MCameraComponent* mInputCameraAttachment = NULL;
	MMeshComponent* mOutputMeshAttachment = NULL;
	bool mInputCameraAttached = false;
	glm::vec3 mFootPosition,
		mControllerPosition,
		mEyePosition,
		mUpDirection;
	physx::PxController* pController = NULL;
	physx::PxRaycastBuffer mGroundHit;
	double mMaxMidAirDist = 0.5;
	float mHeight, mRadius;
	float mMaxJumpHeight = 4.0f;
};

class MCameraComponent :public MEntityComponent {
public:
	bool mOwned = false;
	glm::vec3 mPosition, mFront,
		mMovementFront,
		mMovementRight, mWorldUp,
		mUpDirection, mRight;
	void SetOwner(MCharacterController* obj);
	virtual void SubmitMousePosition(double xpos, double ypos) = 0;
	virtual void ProcessMovement(double dt) = 0;
	virtual glm::mat4 GetViewMatrix() = 0;
	virtual glm::mat4 GetProjectionMatrix() = 0;
protected:
	void GeneratePrivateFunctionNames();
	std::string nOnCursorInput;
	MCharacterController* mOwner = NULL;
};

class MFirstPersonCamera :public MCameraComponent {
public:
	MFirstPersonCamera(MEntity* parent, const std::string& name,
		const glm::vec3& i_front);
	~MFirstPersonCamera();
	virtual void Update(double dt)override;
	virtual void SubmitMousePosition(double xpos, double ypos)override;
	virtual void ProcessMovement(double dt)override;
	virtual glm::mat4 GetViewMatrix()override;
	virtual glm::mat4 GetProjectionMatrix()override;
protected:
};

class MCinematicCamera :public MCameraComponent {
public:
	MCinematicCamera(MEntity* parent, const std::string& name,
		const glm::vec3& i_front);
	~MCinematicCamera();
	void SetPosition(const glm::vec3& pos);
	void SetFront(const glm::vec3& front);
	void SetUp(const glm::vec3& up);
	void SetFovy(const double fovy);
	virtual void Update(double dt)override;
	virtual void SubmitMousePosition(double xpos, double ypos)override;
	virtual void ProcessMovement(double dt)override;
	virtual glm::mat4 GetViewMatrix()override;
	virtual glm::mat4 GetProjectionMatrix()override;
	float mFovy = 60.0f;
};

class MStaticMeshComponent :public MMeshComponent {
public:
	MStaticMeshComponent(MEntity* parent_, MEntityComponent* physics_proxy, 
		const std::string& name_, const std::string& model_path_,
		const glm::vec3& position_, const glm::vec3& scale_, const glm::vec4& rotate_);
	~MStaticMeshComponent();
	void LoadModelAsset();
	virtual void Update(double delta_time)override;
};

class MSkeletonMesh :public MMeshComponent {
public:

};