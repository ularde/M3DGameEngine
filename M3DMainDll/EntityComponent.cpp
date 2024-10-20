#include "pch.h"
#include "Entity.h"
#include "EntityComponent.h"
#include "Model.h"
#include "AssetManager.h"
#include "Platforms.h"
#include "Scene.h"
#include "RenderDeferredPipeline.h"
#include "RenderAtomspherePipeline.h"
#include "RenderDepthMappingPipeline.h"

MStaticMeshComponent::MStaticMeshComponent(MEntity* parent_, MEntityComponent* physics_proxy,
	const std::string& name_, const std::string& model_path_, 
	const glm::vec3& position_, const glm::vec3& scale_, const glm::vec4& rotate_) {
	this->mClassName = "StaticMeshComponent";
	this->mPrototypeName = "EntityComponent";
	this->name = name_;
	this->mParent = parent_;
	this->gPlatform = mParent->GetPlatform();
	this->gAssetManager = this->gPlatform->gAssetManager;
	this->mPhysicsProxy = reinterpret_cast<MRigidBodyPhysicsProxy*>(physics_proxy);
	this->mModelPath = model_path_;
	mPosition = position_;
	mRotate = rotate_;
	this->LoadModelAsset();
	this->GeneratePublicFunctionNames();
}

MStaticMeshComponent::~MStaticMeshComponent()
{
}

void MStaticMeshComponent::LoadModelAsset() {
	this->model = this->gAssetManager->AddModel(true, "model_static", this->mModelPath, (char*)GenerateGuidA());

	if (!this->model) {
		this->gPlatform->ConsoleWriteLine(1.0f, 0.0f, 0.0f, "[ERROR] Engine : Failed to load model to initialize an Entity.");
	}
}

void MStaticMeshComponent::Update(double dt) {
	this->OnUpdate(dt);
	model->DeclareOccupation(this);
	if (!this->mPhysicsProxy->mOutputMeshAttachmentAssigned) {
		this->mPhysicsProxy->SetOutputAttachment(this->name);
	}
	glm::mat4 modelMatrix = mPhysicsProxy->GetModelMatrix();
	gPlatform->gDeferredPipeline->LoadMatrix(MMatrixType::MODEL, modelMatrix);
	gPlatform->gForwardPipeline->LoadMatrix(MMatrixType::MODEL, modelMatrix);
	if (this->mPhysicsProxy->mOutputMeshAttachmentAssigned && this->model) {
		this->model->PushUseCustomMaterialFlag();
		this->model->SetUseCustomMaterialFlag(false);
		this->model->CommitGeometryInstances();
		this->model->PopUseCustomMaterialFlag();
	}
}

MRigidBodyPhysicsProxySphere::MRigidBodyPhysicsProxySphere(MEntity* parent_, const std::string& name_, 
	float radius_, float density_, const glm::vec3& mass_center_,
	int surface_type_ID) {
	mClassName = "RigidBodyPhysicsProxySphere";
	mPrototypeName = "EntityComponent";
	name = name_;
	mRadius = radius_;
	mDensity = density_;
	mMassCenter = mass_center_;
	mParent = parent_;
	mSurfaceTypeID = surface_type_ID;
	gPlatform = this->mParent->GetPlatform();
	InitializePhysics();
	this->GeneratePublicFunctionNames();
	this->OnCreate();
}

MRigidBodyPhysicsProxySphere::MRigidBodyPhysicsProxySphere(MEntity* parent_, const std::string& name_,
	float radius_, float density_, const glm::vec3& mass_center_,
	int surface_type_ID, bool is_kinematic) {
	mClassName = "RigidBodyPhysicsProxySphere";
	mPrototypeName = "EntityComponent";
	name = name_;
	mRadius = radius_;
	mDensity = density_;
	mMassCenter = mass_center_;
	mParent = parent_;
	mSurfaceTypeID = surface_type_ID;
	mIsKinematic = is_kinematic;
	gPlatform = this->mParent->GetPlatform();
	InitializePhysics();
	this->GeneratePublicFunctionNames();
	this->OnCreate();
}

void MRigidBodyPhysicsProxySphere::Update(double dt) {
	if (this->pActor) {
		this->OnUpdate(dt);
		this->transform = this->pActor->getGlobalPose();
	}
	else {
		this->InitializePhysics();
	}
}

void MRigidBodyPhysicsProxySphere::SetOutputAttachment(const std::string& name) {
	this->mOutputMeshAttachment = reinterpret_cast<MMeshComponent*>(this->mParent->componentMap[name]);
	if (mOutputMeshAttachment) {
		this->mOutputMeshAttachmentAssigned = true;
	}
}

glm::mat4 MRigidBodyPhysicsProxySphere::GetModelMatrix() {
	if (this->pActor) {
		physx::PxTransform worldTransform = this->pActor->getGlobalPose();

		physx::PxVec3 localPos(this->mOutputMeshAttachment->mPosition.x, this->mOutputMeshAttachment->mPosition.y, this->mOutputMeshAttachment->mPosition.z);
		physx::PxQuat localRot(this->mOutputMeshAttachment->mRotate.x, this->mOutputMeshAttachment->mRotate.y, this->mOutputMeshAttachment->mRotate.z, this->mOutputMeshAttachment->mRotate.w);

		physx::PxTransform localTransform(localPos, localRot);//, localRot);

		worldTransform *= localTransform;

		return ConvertPxMat44ToGLMMat4(physx::PxMat44(worldTransform));
	}
	else {
		this->InitializePhysics();
		return glm::mat4(1.0f);
	}
}

void MRigidBodyPhysicsProxySphere::InitializePhysics() {
	switch (mSurfaceTypeID)
	{
	case M3D_SURFACE_TYPE_METAL:
		pMaterial = gPlatform->gMetalMaterial;
		break;
	case M3D_SURFACE_TYPE_STONE:
		pMaterial = gPlatform->gStoneMaterial;
		break;
	case M3D_SURFACE_TYPE_MUD:
		pMaterial = gPlatform->gMudMaterial;
		break;
	case M3D_SURFACE_TYPE_WOOD:
		pMaterial = gPlatform->gWoodMaterial;
		break;
	case M3D_SURFACE_TYPE_PLASTIC:
		pMaterial = gPlatform->gPlasticMaterial;
		break;
	default:
		pMaterial = gPlatform->gPlasticMaterial;
		break;
	}

	if (this->mOutputMeshAttachmentAssigned) {
		if (this->mOutputMeshAttachment->mClassName == "StaticMeshComponent") {
			MStaticMeshComponent* aStaticMesh = reinterpret_cast<MStaticMeshComponent*>(this->mOutputMeshAttachment);

			physx::PxVec3 worldPos(this->mParent->mPosition.x, this->mParent->mPosition.y, this->mParent->mPosition.z);
			physx::PxQuat worldRot(this->mParent->mRotate.x, this->mParent->mRotate.y, this->mParent->mRotate.z, this->mParent->mRotate.w);
			physx::PxTransform worldTransform(worldPos, worldRot);// , worldRot);

			physx::PxVec3 localPos(physx::PxVec3(aStaticMesh->mPosition.x, aStaticMesh->mPosition.y, aStaticMesh->mPosition.z));
			physx::PxQuat localRot(aStaticMesh->mRotate.x, aStaticMesh->mRotate.y, aStaticMesh->mRotate.z, aStaticMesh->mRotate.w);
			physx::PxTransform localTransform(localPos, localRot);// , localRot);

			physx::PxVec3 pMassCenter(this->mMassCenter.x, this->mMassCenter.y, this->mMassCenter.z);

			physx::PxTransform pose = worldTransform * localTransform;

			this->pActor = this->gPlatform->pPhysics->createRigidDynamic(pose);

			if (this->pActor) {
				physx::PxSphereGeometry geom(this->mRadius);
				physx::PxRigidActorExt::createExclusiveShape(*this->pActor, geom, *this->pMaterial);
				physx::PxRigidBodyExt::updateMassAndInertia(*this->pActor, this->mDensity, &pMassCenter);
				this->pActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, mIsKinematic);
				this->mParent->mParent->pScene->addActor(*this->pActor);
			}

			this->mPhysicsReady = true;
		}
	}
}

MCharacterController::MCharacterController(MEntity* parent,
	const std::string& name, MCameraComponent* camera,
	const float radius, const float height, const float contact_offset,
	const float step_offset, const float max_jump_height, int surface_type_ID) {
	mParent = parent;
	gPlatform = mParent->GetPlatform();
	mHeight = height;
	mRadius = radius;
	mSurfaceTypeID = surface_type_ID;
	mClassName = "CharacterController";
	mPrototypeName = "EntityComponent";
	mMaxJumpHeight = max_jump_height;
	this->name = name;

	switch (mSurfaceTypeID)
	{
	case M3D_SURFACE_TYPE_METAL:
		pMaterial = gPlatform->gMetalMaterial;
		break;
	case M3D_SURFACE_TYPE_STONE:
		pMaterial = gPlatform->gStoneMaterial;
		break;
	case M3D_SURFACE_TYPE_MUD:
		pMaterial = gPlatform->gMudMaterial;
		break;
	case M3D_SURFACE_TYPE_WOOD:
		pMaterial = gPlatform->gWoodMaterial;
		break;
	case M3D_SURFACE_TYPE_PLASTIC:
		pMaterial = gPlatform->gPlasticMaterial;
		break;
	default:
		pMaterial = gPlatform->gPlasticMaterial;
		break;
	}

	mInputCameraAttachment = camera;
	mInputCameraAttached = true;
	mInputCameraAttachment->SetOwner(this);

	if (this->mInputCameraAttached) {
		physx::PxCapsuleControllerDesc desc;
		desc.contactOffset = contact_offset;
		desc.height = height;
		desc.radius = radius;
		desc.maxJumpHeight = max_jump_height;
		desc.userData = (void*)this;
		desc.material = pMaterial;
		desc.upDirection = physx::PxVec3(0.0f, 1.0f, 0.0f);
		desc.position = physx::PxExtendedVec3(mParent->mPosition.x, mParent->mPosition.y, mParent->mPosition.z);
		desc.stepOffset = step_offset;
		desc.invisibleWallHeight = 0.0f;
		desc.behaviorCallback = NULL;
		desc.density = 10.0f;
		desc.scaleCoeff = 0.8f;
		desc.volumeGrowth = 1.5f;
		desc.registerDeletionListener = false;
		desc.reportCallback = NULL;
		desc.slopeLimit = 45.0f;

		bool iv = desc.isValid();
		assert(iv);
		pController = mParent->mParent->pControllerManager->createController(desc);
		assert(pController);
		pActor = pController->getActor();
		assert(pActor);

		this->mPhysicsReady = true;
	}
	this->GeneratePublicFunctionNames();
	this->OnCreate();
}

MCharacterController::~MCharacterController()
{
}

void MCharacterController::Update(double dt) {
	this->OnUpdate(dt);

	pActor = pController->getActor();

	physx::PxExtendedVec3 footPosition = pController->getFootPosition();
	physx::PxExtendedVec3 controllerPosition = pController->getPosition();

	mControllerPosition = glm::vec3(controllerPosition.x, controllerPosition.y, controllerPosition.z);
	mFootPosition = glm::vec3(footPosition.x, footPosition.y, footPosition.z);
	mEyePosition = glm::vec3(footPosition.x, footPosition.y + mHeight + 2.0 * mRadius - 0.1, footPosition.z);
}

void MCharacterController::SetOutputAttachment(const std::string& name) {
	this->mOutputMeshAttachment = reinterpret_cast<MMeshComponent*>(this->mParent->componentMap[name]);
	if (mOutputMeshAttachment) {
		this->mOutputMeshAttachmentAssigned = true;
	}
}

glm::mat4 MCharacterController::GetModelMatrix() {
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(pController->getFootPosition().x,
		pController->getFootPosition().y,
		pController->getFootPosition().z));
	if (this->mOutputMeshAttachment) {
		model = glm::translate(model, glm::vec3(mOutputMeshAttachment->mPosition.x, mOutputMeshAttachment->mPosition.y, mOutputMeshAttachment->mPosition.z));
	}
	return model;
}

void MCharacterController::AttachCamera(const std::string& name) {
	this->mInputCameraAttachment = reinterpret_cast<MCameraComponent*>(this->mParent->componentMap[name]);
	this->mInputCameraAttached = true;
}

void MCharacterController::Move(MCCTMovementType type, double dt) {
	static int n = 0;
	if (mInputCameraAttached) {
		switch (type)
		{
		case MCCTMovementType::FORWARD:
			pController->move(
				physx::PxVec3(mInputCameraAttachment->mMovementFront.x,
					0.0f,
					mInputCameraAttachment->mMovementFront.z) * dt * 4.0f, dt, dt,
				0);
			break;
		case MCCTMovementType::BACKWARD:
			pController->move(
				-physx::PxVec3(mInputCameraAttachment->mMovementFront.x,
					0.0f,
					mInputCameraAttachment->mMovementFront.z) * dt * 4.0f, dt, dt,
				0);
			break;
		case MCCTMovementType::LEFT:
			pController->move(
				-physx::PxVec3(mInputCameraAttachment->mMovementRight.x,
					0.0f,
					mInputCameraAttachment->mMovementRight.z) * dt * 4.0f, dt, dt,
				0);
			break;
		case MCCTMovementType::RIGHT:
			pController->move(
				physx::PxVec3(mInputCameraAttachment->mMovementRight.x,
					0.0f,
					mInputCameraAttachment->mMovementRight.z) * dt * 4.0f, dt, dt,
				0);
			break;
		case MCCTMovementType::UPWARD:
			if (this->IsGrounded()) {
				yVelocity = sqrt(-2.0f * 0.5f * cGravity);
			}
			break;
		default:
			break;
		}
	}
}

bool MCharacterController::IsGrounded() {
	return mParent->mParent->pScene->raycast(physx::PxVec3(pController->getFootPosition().x, pController->getFootPosition().y + this->pController->getContactOffset(), pController->getFootPosition().z),
		physx::PxVec3(0.0f, -1.0f, 0.0f),
		this->pController->getContactOffset() * 2.0f, mGroundHit);
}

MFirstPersonCamera::MFirstPersonCamera(MEntity* parent, const std::string& name,
	const glm::vec3& i_front) {
	mParent = parent;
	this->name = name;
	mPosition = mParent->mPosition;
	mFront = glm::vec3(0.0f, 0.0f, -1.0f);
	mUpDirection = glm::vec3(0.0f, 1.0f, 0.0f);
	mClassName = "FirstPersonCamera";
	mPrototypeName = "EntityComponent";
	gPlatform = mParent->GetPlatform();
	mWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->GeneratePublicFunctionNames();
	this->GeneratePrivateFunctionNames();
	this->OnCreate();
}

MFirstPersonCamera::~MFirstPersonCamera()
{
}

void MFirstPersonCamera::Update(double dt) {
	if (!mParent->mParent->IsEditorMode()) {
		this->OnUpdate(dt);
		ProcessMovement(dt);
		if (mOwned) {
			mPosition = mOwner->GetEyePosition();
		}
	}
}

void MFirstPersonCamera::SubmitMousePosition(double xpos, double ypos) {
	static float lastX, lastY;
	static float xOffset, yOffset;
	static float yaw, pitch;
	xOffset = xpos - lastX;
	yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	xOffset *= this->gPlatform->mouseSensitivity;
	yOffset *= this->gPlatform->mouseSensitivity;
	yaw += xOffset;
	pitch += yOffset;
	if (abs(pitch) > 89.0f && pitch > 0) {
		pitch = 89.0f;
	}
	if (abs(pitch) > 89.0f && pitch < 0) {
		pitch = -89.0f;
	}
	glm::vec3 nFront;
	nFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	nFront.y = sin(glm::radians(pitch));
	nFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	mFront = glm::normalize(nFront);
	mRight = glm::normalize(glm::cross(mFront, mWorldUp));
	mUpDirection = glm::normalize(glm::cross(mRight, mFront));
	mMovementFront = glm::normalize(glm::vec3(mFront.x, 0.0f, mFront.z));
	mMovementRight = glm::normalize(glm::cross(mMovementFront, mWorldUp));
}

static int n = 0;
static int m = 0;

void MFirstPersonCamera::ProcessMovement(double dt) {
	if (!gPlatform->gGameConsole->GetConsoleVisibility()) {
		gPlatform->SetCursorInvisible();
	}
	else {
		gPlatform->SetCursorVisible();
	}

	if (mOwned) {
		if (this->gPlatform->GetKey(GLFW_KEY_W) == GLFW_PRESS) {
			mOwner->Move(MCCTMovementType::FORWARD, dt);
			if (this->gPlatform->GetKey(GLFW_KEY_SPACE) == GLFW_PRESS && !n) {
				n += 1;
				mOwner->Move(MCCTMovementType::UPWARD, dt);
			}
		}
		else if (this->gPlatform->GetKey(GLFW_KEY_A) == GLFW_PRESS) {
			mOwner->Move(MCCTMovementType::LEFT, dt);
			if (this->gPlatform->GetKey(GLFW_KEY_SPACE) == GLFW_PRESS && !n) {
				n += 1;
				mOwner->Move(MCCTMovementType::UPWARD, dt);
			}
		}
		else if (this->gPlatform->GetKey(GLFW_KEY_S) == GLFW_PRESS) {
			mOwner->Move(MCCTMovementType::BACKWARD, dt);
			if (this->gPlatform->GetKey(GLFW_KEY_SPACE) == GLFW_PRESS && !n) {
				n += 1;
				mOwner->Move(MCCTMovementType::UPWARD, dt);
			}
		}
		else if (this->gPlatform->GetKey(GLFW_KEY_D) == GLFW_PRESS) {
			mOwner->Move(MCCTMovementType::RIGHT, dt);
			if (this->gPlatform->GetKey(GLFW_KEY_SPACE) == GLFW_PRESS && !n) {
				n += 1;
				mOwner->Move(MCCTMovementType::UPWARD, dt);
			}
		}
		else if (this->gPlatform->GetKey(GLFW_KEY_SPACE) == GLFW_PRESS && !n) {
			n += 1;
			mOwner->Move(MCCTMovementType::UPWARD, dt);
		}
		if (this->gPlatform->GetKey(GLFW_KEY_SPACE) == GLFW_RELEASE && n) {
			n = 0;
		}
		//gravity
		mOwner->yVelocity += mOwner->cGravity * dt;
		mOwner->GetPxController()->move(physx::PxVec3(0.0f, mOwner->yVelocity, 0.0f) * dt, dt, dt, 0);
	}
}

glm::mat4 MFirstPersonCamera::GetViewMatrix() {
	return glm::lookAt(mPosition, mPosition + mFront, mUpDirection);
}

glm::mat4 MFirstPersonCamera::GetProjectionMatrix() {
	return glm::perspective(glm::radians(60.0f), (float)this->gPlatform->GetWindowWidth() / (float)this->gPlatform->GetWindowHeight(), 0.1f, 1000.0f);
}

void MCameraComponent::SetOwner(MCharacterController* obj) {
	this->mOwner = obj;
	this->mOwned = true;
}

void MCameraComponent::GeneratePrivateFunctionNames() {
	nOnCursorInput = name + std::string("_OnCursorInput");
}

glm::vec3 MRigidBodyPhysicsProxy::GetPosition() {
	if (pActor) {
		return glm::vec3(pActor->getGlobalPose().p.x, pActor->getGlobalPose().p.y, pActor->getGlobalPose().p.z);
	}
	else {
		return glm::vec3(0.0f);
	}
}

void MRigidBodyPhysicsProxy::SetPosition(double x, double y, double z) {
	if (this->pActor) {
		physx::PxTransform transform(physx::PxVec3(x, y, z));
		this->pActor->setGlobalPose(transform);
	}
}

void MRigidBodyPhysicsProxy::AddForce(double x, double y, double z, int mode) {
	if (pActor) {
		pActor->addForce(physx::PxVec3(x, y, z), (physx::PxForceMode::Enum)mode);
	}
}

void MRigidBodyPhysicsProxy::AddTorque(double x, double y, double z, int mode) {
	if (pActor) {
		pActor->addTorque(physx::PxVec3(x, y, z), (physx::PxForceMode::Enum)mode);
	}
}

void MRigidBodyPhysicsProxy::Invalidate() {
	physx::PxRigidDynamic* actor = this->GetPxActor();
	physx::PxScene* scene = mParent->mParent->pScene;
	if (actor && scene) {
		if (!mParent->mInvalidated) {
			scene->removeActor(*actor);
			mParent->mInvalidated = true;
		}
	}
}

void MRigidBodyPhysicsProxy::Validate() {
	physx::PxRigidDynamic* actor = this->GetPxActor();
	physx::PxScene* scene = mParent->mParent->pScene;
	if (actor && scene) {
		if (mParent->mInvalidated) {
			scene->addActor(*actor);
			mParent->mInvalidated = false;
		}
	}
}

MCinematicCamera::MCinematicCamera(MEntity* parent, const std::string& name, const glm::vec3& i_front) {
	mParent = parent;
	this->name = name;
	mPosition = mParent->mPosition;
	mFront = glm::vec3(0.0f, 0.0f, -1.0f);
	mUpDirection = glm::vec3(0.0f, 1.0f, 0.0f);
	mClassName = "CinematicCamera";
	mPrototypeName = "EntityComponent";
	gPlatform = mParent->GetPlatform();
	mWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->GeneratePublicFunctionNames();
	this->GeneratePrivateFunctionNames();
	this->OnCreate();
}

MCinematicCamera::~MCinematicCamera()
{
}

void MCinematicCamera::SetPosition(const glm::vec3& pos) {
	mPosition = pos;
}

void MCinematicCamera::SetFront(const glm::vec3& front) {
	mFront = front;
}

void MCinematicCamera::SetUp(const glm::vec3& up) {
	mUpDirection = up;
}

void MCinematicCamera::SetFovy(const double fovy) {
	mFovy = (float)fovy;
}

void MCinematicCamera::Update(double dt) {
	this->OnUpdate(dt);
}

void MCinematicCamera::SubmitMousePosition(double xpos, double ypos) {

}

void MCinematicCamera::ProcessMovement(double dt) {

}

glm::mat4 MCinematicCamera::GetViewMatrix() {
	return glm::lookAt(mPosition, mPosition + mFront, mUpDirection);
}

glm::mat4 MCinematicCamera::GetProjectionMatrix() {
	return glm::perspective(glm::radians(mFovy), (float)this->gPlatform->GetWindowWidth() / (float)this->gPlatform->GetWindowHeight(), 0.3f, 1000.0f);
}

void MEntityComponent::GeneratePublicFunctionNames() {
	nOnCreate = name + std::string("_OnCreate");
	nOnDestroy = name + std::string("_OnDestroy");
	nOnUpdate = name + std::string("_OnUpdate");
	nOnRender = name + std::string("_OnRender");
}

void MEntityComponent::SetThisComponent() {
	if (this->IsRigidBodyPhysicsProxySphere()) {
		(*gPlatform->gLuaState)["this"] = reinterpret_cast<MRigidBodyPhysicsProxySphere*>(this);
	}
	else if (this->IsFirstPersonCamera()) {
		(*gPlatform->gLuaState)["this"] = reinterpret_cast<MFirstPersonCamera*>(this);
	}
	else if (this->IsCinematicCamera()) {
		(*gPlatform->gLuaState)["this"] = reinterpret_cast<MCinematicCamera*>(this);
	}
}

void MEntityComponent::OnUpdate(double dt) {
	if (!mParent->mParent->IsEditorMode()) {
		mParent->mThisComponent = this;
		SetThisComponent();
		(*gPlatform->gLuaState)[nOnUpdate](dt);
	}
}

void MEntityComponent::OnCreate() {
	mParent->mThisComponent = this;
	SetThisComponent();
	(*gPlatform->gLuaState)[nOnCreate]();
}

void MEntityComponent::OnDestroy() {
	mParent->mThisComponent = this;
	SetThisComponent();
	(*gPlatform->gLuaState)[nOnDestroy]();
}