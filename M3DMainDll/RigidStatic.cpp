#include "pch.h"
#include "RigidStatic.h"
#include "Actor.h"
#include "Model.h"
#include "Scene.h"
#include "RenderDeferredPipeline.h"
#include "RenderAtomspherePipeline.h"
#include "RenderDepthMappingPipeline.h"
#include "SharedFunc.h"
#include "ActorEditorAgent.h"

class MActor;
class MScene;

MRigidStatic::MRigidStatic(MScene* scene_, const std::string& name_, const std::string& model_path, const std::string& material_path,
	const glm::vec3& position_, const glm::vec3& scale_, const glm::vec4& rotate_) {
	this->mName = name_;
	this->mParent = scene_;
	this->gPlatform = scene_->GetPlatform();
	this->mPosition = position_;
	this->mScale = scale_;
	this->mRotate = rotate_;
	this->mModelPath = model_path;
	this->mMaterialPath = material_path;
	this->mClassName = "RigidStatic";
	this->LoadModelAsset();
	this->InitializePhysics();
	this->InitializeLuaInstance();
	if (mMaterialPath != "") {
		mCustomMaterialGUID = GenerateGuidA();
		mCustomMaterial = gPlatform->gAssetManager->AddMaterial("model_material", mMaterialPath, (char*)mCustomMaterialGUID.c_str());
	}
	this->mEditorAgent = new MActorEditorAgent(gPlatform, this);
}

MRigidStatic::~MRigidStatic()
{
}

void MRigidStatic::Update(double dt) {
	mModel->DeclareOccupation(this);
	mModelMatrix = ConvertPxMat44ToGLMMat4(physx::PxMat44(pActor->getGlobalPose()));
	gPlatform->gDeferredPipeline->LoadMatrix(MMatrixType::MODEL, mModelMatrix);
	this->mModel->PushUseCustomMaterialFlag();
	if (this->mCustomMaterial) {
		this->mModel->SetUseCustomMaterialFlag(true);
		this->mModel->SetMaterial(this->mCustomMaterial);
	}
	else {
		this->mModel->SetUseCustomMaterialFlag(false);
	}
	mModel->CommitRenderInstances();
	this->mModel->PopUseCustomMaterialFlag();
}

void MRigidStatic::InitializePhysics() {
	physx::PxQuat localRot(mRotate.x, mRotate.y, mRotate.z, mRotate.w);

	this->pActor = this->gPlatform->pPhysics->createRigidStatic(physx::PxTransform(physx::PxVec3(mPosition.x, mPosition.y, mPosition.z),
		localRot));
	assert(this->pActor);

	for (unsigned int i = 0; i < this->mModel->GetMeshCount(); i++) {
		if (this->mModel->pTriangleMeshes[i]) {
			physx::PxMaterial* pMeshMaterial = NULL;
			int surfaceTypeID = mModel->GetMeshSurfaceTypeID(i);

			switch (surfaceTypeID)
			{
			case M3D_SURFACE_TYPE_METAL:
				pMeshMaterial = gPlatform->gMetalMaterial;
				break;
			case M3D_SURFACE_TYPE_STONE:
				pMeshMaterial = gPlatform->gStoneMaterial;
				break;
			case M3D_SURFACE_TYPE_MUD:
				pMeshMaterial = gPlatform->gMudMaterial;
				break;
			case M3D_SURFACE_TYPE_WOOD:
				pMeshMaterial = gPlatform->gWoodMaterial;
				break;
			case M3D_SURFACE_TYPE_PLASTIC:
				pMeshMaterial = gPlatform->gPlasticMaterial;
				break;
			default:
				pMeshMaterial = gPlatform->gPlasticMaterial;
				break;
			}

			physx::PxTriangleMeshGeometry geom(this->mModel->pTriangleMeshes[i]);
			physx::PxShape* shape = this->gPlatform->pPhysics->createShape(geom, *pMeshMaterial);
			this->pActor->attachShape(*shape);
			shape->release();
		}
	}
	this->mParent->pScene->addActor(*this->pActor);
}

void MRigidStatic::Save() {

}

void MRigidStatic::UpdatePositionWhileCreating(const physx::PxVec3& pos) {
	this->mPosition = glm::vec3(pos.x, pos.y, pos.z);
	this->mEditorAgent->SetPosition(mPosition);
	physx::PxTransform t(pos);
	this->pActor->setGlobalPose(t);
}

void MRigidStatic::LoadModelAsset() {
	this->mModel = this->gPlatform->gAssetManager->AddModel(true, "model_static", this->mModelPath, (char*)GenerateGuidA());
	if (!this->mModel) {
		this->gPlatform->ConsoleWriteLine(1.0f, 0.0f, 0.0f, "[ERROR] Engine : Failed to create StaticObject instance.");
	}
}

void MRigidStatic::InitializeLuaInstance() {
	//std::string luaCommand = this->name +
	//	" = RigidStatic.new()\n";
	//this->platform->luaState->script(luaCommand);
}