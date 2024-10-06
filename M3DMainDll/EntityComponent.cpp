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

MStaticMesh::MStaticMesh(MEntity* parent_, MEntityComponent* physics_proxy,
	const std::string& name_, const std::string& model_path_, 
	const glm::vec3& position_, const glm::vec3& scale_, const glm::vec3& rotate_) {
	this->mClassName = "StaticMesh";
	this->mFatherClassName = "EntityComponent";
	this->name = name_;
	this->mParent = parent_;
	this->gPlatform = mParent->GetPlatform();
	this->gAssetManager = this->gPlatform->gAssetManager;
	this->mPhysicsProxy = reinterpret_cast<MRigidBodyPhysicsProxy*>(physics_proxy);
	this->mModelPath = model_path_;
	position = position_;
	this->LoadModelAsset();
	this->GeneratePublicFunctionNames();
}

MStaticMesh::~MStaticMesh()
{
}

void MStaticMesh::LoadModelAsset() {
	this->model = this->gAssetManager->AddModel(true, "model_static", this->mModelPath, (char*)GenerateGuidA());

	if (!this->model) {
		this->gPlatform->ConsoleWriteLine(1.0f, 0.0f, 0.0f, "[ERROR] Engine : Failed to load model to initialize an Entity.");
	}
}

void MStaticMesh::Update(double delta_time) {
	if (!this->mPhysicsProxy->mClientAssigned) {
		this->mPhysicsProxy->SetClient(this->name);
	}
}

void MStaticMesh::Render() {
	glm::mat4 modelMatrix = mPhysicsProxy->GetModelMatrix();
	gPlatform->gDeferredPipeline->LoadMatrix(MMatrixType::MODEL, modelMatrix);
	if (this->mPhysicsProxy->mClientAssigned && this->model) {
		this->model->PushUseCustomMaterialFlag();
		this->model->SetUseCustomMaterialFlag(false);
		this->model->Render();
		this->model->PopUseCustomMaterialFlag();
	}
}

void MStaticMesh::RenderForDepthMapping() {
	glm::mat4 modelMatrix = mPhysicsProxy->GetModelMatrix();
	gPlatform->gDepthMappingPipeline->SetModelMatrix(modelMatrix);
	if (this->mPhysicsProxy->mClientAssigned && this->model) {
		this->model->RenderForDepthMapping();
	}
}

MRigidBodyPhysicsProxySphere::MRigidBodyPhysicsProxySphere(MEntity* parent_, const std::string& name_, 
	float radius_, float density_, const glm::vec3& mass_center_,
	int surface_type_ID) {
	mClassName = "RigidBodyPhysicsProxySphere";
	mFatherClassName = "EntityComponent";
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
	mFatherClassName = "EntityComponent";
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

void MRigidBodyPhysicsProxySphere::Render() {
	this->OnRender();
}

void MRigidBodyPhysicsProxySphere::SetClient(const std::string& name) {
	this->mClient = reinterpret_cast<MMeshComponent*>(this->mParent->componentMap[name]);
	if (mClient) {
		this->mClientAssigned = true;
	}
}

glm::mat4 MRigidBodyPhysicsProxySphere::GetModelMatrix() {
	if (this->pActor) {
		physx::PxTransform worldTransform = this->pActor->getGlobalPose();

		physx::PxVec3 localPos(this->mClient->position.x, this->mClient->position.y, this->mClient->position.z);
		physx::PxQuat localRot(glm::radians(this->mClient->rotate.x),
			glm::radians(this->mClient->rotate.x),
			glm::radians(this->mClient->rotate.x), 1.0f);

		physx::PxTransform localTransform(localPos);//, localRot);

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

	if (this->mClientAssigned) {
		if (this->mClient->mClassName == "StaticMesh") {
			MStaticMesh* aStaticMesh = reinterpret_cast<MStaticMesh*>(this->mClient);

			physx::PxVec3 worldPos(this->mParent->position.x, this->mParent->position.y, this->mParent->position.z);
			physx::PxQuat worldRot(glm::radians(this->mParent->rotate.x),
				glm::radians(this->mParent->rotate.y),
				glm::radians(this->mParent->rotate.z), 1.0f);
			physx::PxTransform worldTransform(worldPos, worldRot);

			physx::PxVec3 localPos(physx::PxVec3(aStaticMesh->position.x, aStaticMesh->position.y, aStaticMesh->position.z));
			physx::PxQuat localRot(glm::radians(aStaticMesh->rotate.x),
				glm::radians(aStaticMesh->rotate.y),
				glm::radians(aStaticMesh->rotate.z), 1.0f);
			physx::PxTransform localTransform(localPos);// , localRot);

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
	mFatherClassName = "EntityComponent";
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

	mCameraAttachment = camera;
	mCameraAttached = true;
	mCameraAttachment->SetHost(this);

	if (this->mCameraAttached) {
		physx::PxCapsuleControllerDesc desc;
		desc.contactOffset = contact_offset;
		desc.height = height;
		desc.radius = radius;
		desc.maxJumpHeight = max_jump_height;
		desc.userData = (void*)this;
		desc.material = pMaterial;
		desc.upDirection = physx::PxVec3(0.0f, 1.0f, 0.0f);
		desc.position = physx::PxExtendedVec3(mParent->position.x, mParent->position.y, mParent->position.z);
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

void MCharacterController::Render() {
	this->OnRender();
}

void MCharacterController::SetClient(const std::string& name) {
	this->mClient = reinterpret_cast<MMeshComponent*>(this->mParent->componentMap[name]);
	if (mClient) {
		this->mClientAssigned = true;
	}
}

glm::mat4 MCharacterController::GetModelMatrix() {
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(pController->getFootPosition().x,
		pController->getFootPosition().y,
		pController->getFootPosition().z));
	if (this->mClient) {
		model = glm::translate(model, glm::vec3(mClient->position.x, mClient->position.y, mClient->position.z));
	}
	return model;
}

void MCharacterController::AttachCamera(const std::string& name) {
	this->mCameraAttachment = reinterpret_cast<MCameraComponent*>(this->mParent->componentMap[name]);
	this->mCameraAttached = true;
}

void MCharacterController::Move(MCCTMovementType type, double dt) {
	static int n = 0;
	if (mCameraAttached) {
		switch (type)
		{
		case MCCTMovementType::FORWARD:
			pController->move(
				physx::PxVec3(mCameraAttachment->mMovementFront.x,
					0.0f,
					mCameraAttachment->mMovementFront.z) * dt * 4.0f, dt, dt,
				0);
			break;
		case MCCTMovementType::BACKWARD:
			pController->move(
				-physx::PxVec3(mCameraAttachment->mMovementFront.x,
					0.0f,
					mCameraAttachment->mMovementFront.z) * dt * 4.0f, dt, dt,
				0);
			break;
		case MCCTMovementType::LEFT:
			pController->move(
				-physx::PxVec3(mCameraAttachment->mMovementRight.x,
					0.0f,
					mCameraAttachment->mMovementRight.z) * dt * 4.0f, dt, dt,
				0);
			break;
		case MCCTMovementType::RIGHT:
			pController->move(
				physx::PxVec3(mCameraAttachment->mMovementRight.x,
					0.0f,
					mCameraAttachment->mMovementRight.z) * dt * 4.0f, dt, dt,
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
		0.5f - this->pController->getContactOffset(), mGroundHit);
}

MFirstPersonCamera::MFirstPersonCamera(MEntity* parent, const std::string& name,
	const glm::vec3& i_front) {
	mParent = parent;
	this->name = name;
	mPosition = mParent->position;
	mFront = glm::vec3(0.0f, 0.0f, -1.0f);
	mUpDirection = glm::vec3(0.0f, 1.0f, 0.0f);
	mClassName = "FirstPersonCamera";
	mFatherClassName = "EntityComponent";
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
		if (mHostAssigned) {
			mPosition = mHost->GetEyePosition();
		}
	}
}

void MFirstPersonCamera::Render() {
	this->OnRender();
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

	if (mHostAssigned) {
		if (this->gPlatform->GetKey(GLFW_KEY_W) == GLFW_PRESS) {
			mHost->Move(MCCTMovementType::FORWARD, dt);
			if (this->gPlatform->GetKey(GLFW_KEY_SPACE) == GLFW_PRESS && !n) {
				n += 1;
				mHost->Move(MCCTMovementType::UPWARD, dt);
			}
		}
		else if (this->gPlatform->GetKey(GLFW_KEY_A) == GLFW_PRESS) {
			mHost->Move(MCCTMovementType::LEFT, dt);
			if (this->gPlatform->GetKey(GLFW_KEY_SPACE) == GLFW_PRESS && !n) {
				n += 1;
				mHost->Move(MCCTMovementType::UPWARD, dt);
			}
		}
		else if (this->gPlatform->GetKey(GLFW_KEY_S) == GLFW_PRESS) {
			mHost->Move(MCCTMovementType::BACKWARD, dt);
			if (this->gPlatform->GetKey(GLFW_KEY_SPACE) == GLFW_PRESS && !n) {
				n += 1;
				mHost->Move(MCCTMovementType::UPWARD, dt);
			}
		}
		else if (this->gPlatform->GetKey(GLFW_KEY_D) == GLFW_PRESS) {
			mHost->Move(MCCTMovementType::RIGHT, dt);
			if (this->gPlatform->GetKey(GLFW_KEY_SPACE) == GLFW_PRESS && !n) {
				n += 1;
				mHost->Move(MCCTMovementType::UPWARD, dt);
			}
		}
		else if (this->gPlatform->GetKey(GLFW_KEY_SPACE) == GLFW_PRESS && !n) {
			n += 1;
			mHost->Move(MCCTMovementType::UPWARD, dt);
		}
		if (this->gPlatform->GetKey(GLFW_KEY_SPACE) == GLFW_RELEASE && n) {
			n = 0;
		}
		//gravity
		mHost->yVelocity += mHost->cGravity * dt;
		mHost->GetPxController()->move(physx::PxVec3(0.0f, mHost->yVelocity, 0.0f) * dt, dt, dt, 0);
	}
}

glm::mat4 MFirstPersonCamera::GetViewMatrix() {
	return glm::lookAt(mPosition, mPosition + mFront, mUpDirection);
}

glm::mat4 MFirstPersonCamera::GetProjectionMatrix() {
	return glm::perspective(glm::radians(60.0f), (float)this->gPlatform->GetWindowWidth() / (float)this->gPlatform->GetWindowHeight(), 0.1f, 1000.0f);
}

void MCameraComponent::SetHost(MCharacterController* obj) {
	this->mHost = obj;
	this->mHostAssigned = true;
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
	mPosition = mParent->position;
	mFront = glm::vec3(0.0f, 0.0f, -1.0f);
	mUpDirection = glm::vec3(0.0f, 1.0f, 0.0f);
	mClassName = "CinematicCamera";
	mFatherClassName = "EntityComponent";
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

void MCinematicCamera::Render() {
	this->OnRender();
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

void MEntityComponent::OnRender() {
	if (!mParent->mParent->IsEditorMode()) {
		mParent->mThisComponent = this;
		SetThisComponent();
		(*gPlatform->gLuaState)[nOnRender]();
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