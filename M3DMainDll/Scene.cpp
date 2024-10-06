#include "pch.h"
#include "Scene.h"
#include "SharedFunc.h"
#include "Actor.h"
#include "Player.h"
#include "RigidStatic.h"
#include "Light.h"
#include "Entity.h"
#include "Skybox.h"
#include "Atmosphere.h"
#include "RenderDeferredPipeline.h"
#include "RenderAtomspherePipeline.h"
#include "RenderDepthMappingPipeline.h"
#include "EntityComponent.h"
#include "Entity.h"
#include "TimeOfDay.h"
#include "ActorEditorAgent.h"
#include "SceneEditorAgent.h"

class MActor;
class MPlayer;
class MRigidStatic;
class MRigidDynamic;
class MPointLight;
class MPhysicsEntity;
class MEntity;
class MSkybox;
class MCameraComponent;
class MTimeOfDay;
class MAtmosphere;

MScene::MScene(bool immediately_load, MBasicPlatform* platform, const std::string& path_) {
	this->gPlatform = platform;
	this->path = path_;
	this->mClassName = "Scene";
	this->mFatherClassName = "Container";
	if (this->mSceneDoc.LoadFile(this->path.c_str()) != tinyxml2::XML_SUCCESS) {
		MessageBox(NULL, L"Failed to load scene from an XML document.", L"M3D GameEngine", MB_ICONERROR);
	}
	else {
		this->InitializePhysics();
		this->LoadScene();
	}
}

MScene::~MScene() {

}

void MScene::InitializeLuaInstance() {
	gPlatform->gLuaState->safe_script_file(GetFullAssetPathA(mLuaScriptPath));
	this->GenerateFunctionNames();
}

void MScene::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (this->gLocalPlayer) {
		//this->gLocalCamera->SumbitMousePosition(xpos, ypos);
	}
}

void MScene::Update(double dt) {
	if (this->mLoaded) {
		this->OnPreUpdate(dt);
		if (this->mEditorModeFlag) {
			this->mTimeOfDay->SetDynamic(false);
		}
		this->mTimeOfDay->Update();
		this->mSunLight->Update(dt);
		for (unsigned int i = 0; i < this->mActors.size(); i++) {
			this->mActors[i]->Update(dt);
		}
		if (!this->mEditorModeFlag) {
			if (gPlatform->GetKey(VK_ESCAPE) == 1) {
				reinterpret_cast<MEditor*>(gPlatform)->ExitGame();
			}
			this->UpdatePhysics(dt);
		}
		if (this->gLocalCamera && !this->mEditorModeFlag) {
			this->gLocalCamera->SubmitMousePosition(gPlatform->GetCursorPosX(), gPlatform->GetCursorPosY());
			glm::mat4 projection = gLocalCamera->GetProjectionMatrix();
			glm::mat4 view = gLocalCamera->GetViewMatrix();
			gPlatform->gDeferredPipeline->SetCameraPosition(gLocalCamera->mPosition);
			gPlatform->gDeferredPipeline->LoadMatrix(MMatrixType::PROJECTION, projection);
			gPlatform->gAtomspherePipeline->LoadMatrix(MMatrixType::PROJECTION, projection);
			gPlatform->gDeferredPipeline->LoadMatrix(MMatrixType::VIEW, view);
			gPlatform->gAtomspherePipeline->LoadMatrix(MMatrixType::VIEW, view);
		}
		else {
			this->gEditorCamera->SubmitMousePosition(gPlatform->GetCursorPosX(), gPlatform->GetCursorPosY());
			this->gEditorCamera->ProcessMovement();
			glm::mat4 projection = this->gEditorCamera->GetProjectionMatrix();
			glm::mat4 view = this->gEditorCamera->GetViewMatrix();
			gPlatform->gDeferredPipeline->SetCameraPosition(this->gEditorCamera->position);
			gPlatform->gDeferredPipeline->LoadMatrix(MMatrixType::PROJECTION, projection);
			gPlatform->gAtomspherePipeline->LoadMatrix(MMatrixType::PROJECTION, projection);
			gPlatform->gDeferredPipeline->LoadMatrix(MMatrixType::VIEW, view);
			gPlatform->gAtomspherePipeline->LoadMatrix(MMatrixType::VIEW, view);
		}
		this->OnPreRender(dt);
	}
}

void MScene::Render() {
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	gPlatform->gAtomspherePipeline->ClearBuffers();
	gPlatform->gDeferredPipeline->ClearBuffers();
	gPlatform->gDepthMappingPipeline->ClearBuffers();
	if (this->mLoaded) {
		this->mSunLight->Render();
		for (unsigned int i = 0; i < this->mActors.size(); i++) {
			this->mActors[i]->Render();
		}
		mAtmosphere->Render();
	}
}

void MScene::Release() {
	for (auto i = 0; i < this->mActors.size(); i++) {
		free(this->mActors[i]);
	}
}

void MScene::SetEditMode(MEditMode mode) {
	if (mEditorModeFlag) {
		mEditorAgent->SetEditMode(mode);
	}
}

void MScene::SaveSceneWithFullPath(const std::string& path) {
	tinyxml2::XMLDocument document;
	//write head information and information of TOD
	tinyxml2::XMLElement* root = document.NewElement("Scene");
	root->SetAttribute("name", this->mName.c_str());
	root->SetAttribute("script", this->mLuaScriptPath.c_str());
	tinyxml2::XMLElement* todElement = root->InsertNewChildElement("TimeOfDay");
	todElement->SetAttribute("dynamic", 1);
	todElement->SetAttribute("speed", mTimeOfDay->mSpeed);
	todElement->SetAttribute("xoffset", mTimeOfDay->mXOffset);
	todElement->SetAttribute("iSun", mTimeOfDay->mAtmosphereParameters.iSun);
	todElement->SetAttribute("rPlanet", mTimeOfDay->mAtmosphereParameters.rPlanet);
	todElement->SetAttribute("rAtmos", mTimeOfDay->mAtmosphereParameters.rAtmos);
	todElement->SetAttribute("kRlh", GenStringByVec3(mTimeOfDay->mAtmosphereParameters.kRlh));
	todElement->SetAttribute("kMie", mTimeOfDay->mAtmosphereParameters.kMie);
	todElement->SetAttribute("shRlh", mTimeOfDay->mAtmosphereParameters.shRlh);
	todElement->SetAttribute("shMie", mTimeOfDay->mAtmosphereParameters.shMie);
	todElement->SetAttribute("g", mTimeOfDay->mAtmosphereParameters.g);
	document.InsertEndChild(root);
	//write information of actors to the scene file
	for (auto i = 0u; i < mActors.size(); i++) {
		if (mActors[i]->mClassName == "RigidStatic") {
			MRigidStatic* rs = reinterpret_cast<MRigidStatic*>(mActors[i]);
			tinyxml2::XMLElement* currentElement = document.NewElement("Actor");
			currentElement->SetAttribute("class", "RigidStatic");
			currentElement->SetAttribute("name", rs->name.c_str());
			currentElement->SetAttribute("model", rs->mModelPath.c_str());
			currentElement->SetAttribute("material", rs->mMaterialPath.c_str());
			currentElement->SetAttribute("position", GenStringByVec3(glm::vec3(rs->pActor->getGlobalPose().p.x, rs->pActor->getGlobalPose().p.y, rs->pActor->getGlobalPose().p.z)));
			currentElement->SetAttribute("scale", GenStringByVec3(rs->scale));
			currentElement->SetAttribute("rotate", GenStringByVec3(rs->rotate));
			root->InsertEndChild(currentElement);
		}
		else if (mActors[i]->mClassName == "Entity") {
			MEntity* ent = reinterpret_cast<MEntity*>(mActors[i]);
			tinyxml2::XMLElement* currentElement = document.NewElement("Actor");
			currentElement->SetAttribute("class", "Entity");
			currentElement->SetAttribute("name", ent->name.c_str());
			currentElement->SetAttribute("def", ent->mDefFilePath.c_str());
			currentElement->SetAttribute("position", GenStringByVec3(ent->position));
			currentElement->SetAttribute("scale", GenStringByVec3(ent->scale));
			currentElement->SetAttribute("rotate", GenStringByVec3(ent->rotate));
			root->InsertEndChild(currentElement);
		}
	}
	//save the file
	int errorCode = document.SaveFile(path.c_str());
	if (errorCode != tinyxml2::XML_SUCCESS) {
		std::wstring errorInfo = std::wstring(L"Failed to save the scene.\nError Code ") + std::to_wstring(errorCode);
		MessageBox(NULL, errorInfo.c_str(), L"Editor Error", MB_ICONHAND);
	}
}
void MScene::StartGame() {
	mEditorModeFlag = false;
	gPlatform->SetCursorInvisible();
	this->OnCreate();
}

void MScene::ExitGame() {
	mEditorModeFlag = true;
	gPlatform->SetCursorVisible();
}

void MScene::SetLocalFirstPersonCamera(MFirstPersonCamera* camera) {
	gLocalCamera = camera;
}

void MScene::SetLocalCinematicCamera(MCinematicCamera* camera) {
	gLocalCamera = camera;
}

double MScene::GetWorldTime() {
	return mTimeOfDay->GetTime();
}

void MScene::SetWorldTime(double t) {
	mTimeOfDay->SetTime(t);
}

void MScene::InitializePhysics() {
	physx::PxSceneDesc sceneDesc(this->gPlatform->pDefaultScale);
	sceneDesc.userData = this;
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.broadPhaseType = physx::PxBroadPhaseType::eABP;
	sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(4);
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

	pScene = gPlatform->pPhysics->createScene(sceneDesc);
	assert(this->pScene);
	pControllerManager = PxCreateControllerManager(*pScene);
	assert(pControllerManager);
}

void MScene::UpdatePhysics(double delta_time) {
	this->pScene->simulate((float)delta_time);
	this->pScene->fetchResults(true);
}

void MScene::LoadScene() {
	tinyxml2::XMLElement* root = this->mSceneDoc.RootElement();
	tinyxml2::XMLElement* actorNode = root->FirstChildElement("Actor");

	this->mLuaScriptPath = root->Attribute("script");
	this->mName = root->Attribute("name");

	std::string actorName, className;

	while (actorNode) {
		actorName = actorNode->Attribute("name");
		className = actorNode->Attribute("class");

		if (className == "Player") {
			this->AddPlayer(actorName, actorNode);
		}
		else if (className == "RigidStatic") {
			this->AddRigidStatic(actorName, actorNode);
		}
		else if (className == "PointLight") {
			this->AddPointLight(actorName, actorNode);
		}
		else if (className == "DirectionLight") {
			this->AddDirectionLight(actorName, actorNode);
		}
		else if (className == "SpotLight") {
			this->AddSpotLight(actorName, actorNode);
		}
		else if (className == "Entity") {
			this->AddEntity(actorName, actorNode);
		}
		else if (className == "Skybox") {
			this->AddSkybox(actorName, actorNode);
		}
		else if (className == "Ragdoll") {
			this->AddRagdoll(actorName, actorNode);
		}
#ifdef _DEBUG
		std::cout << "Scene Loader " << actorName << " " << className << std::endl;
#endif // DEBUG

		actorNode = actorNode->NextSiblingElement("Actor");
	}

	tinyxml2::XMLElement* todNode = root->FirstChildElement("TimeOfDay");
	bool dynamic = todNode->BoolAttribute("dynamic");
	double speed = todNode->DoubleAttribute("speed");
	float xoffset = todNode->FloatAttribute("xoffset");
	float iSun = todNode->FloatAttribute("iSun");
	float rPlanet = todNode->FloatAttribute("rPlanet");
	float rAtmos = todNode->FloatAttribute("rAtmos");
	float kMie = todNode->FloatAttribute("kMie");
	float shRlh = todNode->FloatAttribute("shRlh");
	float shMie = todNode->FloatAttribute("shMie");
	float g = todNode->FloatAttribute("g");
	std::string skRlh = todNode->Attribute("kRlh");
	float kRlhx{}, kRlhy{}, kRlhz{};
	int n = GetVec3FromString(skRlh, &kRlhx, &kRlhy, &kRlhz);
	assert(n == 3);

	MAtmosphereParameters parameters;
	parameters.g = g;
	parameters.iSun = iSun;
	parameters.rPlanet = rPlanet;
	parameters.kMie = kMie;
	parameters.rAtmos = rAtmos;
	parameters.shMie = shMie;
	parameters.shRlh = shRlh;
	parameters.kRlh = glm::vec3(kRlhx, kRlhy, kRlhz);

	mTimeOfDay = new MTimeOfDay(gPlatform, this, dynamic, speed, xoffset, parameters);
	mSunLight = new MDirectionalLight(this, "Sun", "", glm::vec3(0.0f), glm::vec3(0.0f), true);
	mSunLight->SetTimeOfDayObject(mTimeOfDay);

	this->mEditorAgent = new MSceneEditorAgent(gPlatform, this);
	this->gEditorCamera = new MFreeCamera(gPlatform, 5.0f);
	//this->gLocalPlayer = reinterpret_cast<MEntity*>(this->mActorMap["LocalPlayer"]);
	//this->gLocalCamera = reinterpret_cast<MCameraComponent*>(gLocalPlayer->componentMap["DefaultCamera"]);
	//assert(this->mActorMap["LocalPlayer"] && gLocalPlayer->componentMap["DefaultCamera"]);

	mAtmosphere = new MAtmosphere(this, "Atmosphere");
	mAtmosphere->SetTimeOfDayObject(mTimeOfDay);

	this->InitializeLuaInstance();
	if (!mEditorModeFlag) {
		this->OnCreate();
	}

	this->mLoaded = true;
}

void MScene::AddPlayer(const std::string& name, tinyxml2::XMLElement* node) {
	std::string mDefFilePath = node->Attribute("def");
	std::string initialPosStr = node->Attribute("position");

	float pos_x = 0.0f, pos_y = 0.0f, pos_z = 0.0f;
	int num = GetVec3FromString(initialPosStr, &pos_x, &pos_y, &pos_z);
	assert(num == 3);

	MPlayer* object = new MPlayer(this, name, mDefFilePath, glm::vec3(pos_x, pos_y, pos_z));
	this->mActors.push_back(object);
	this->mActorMap[name] = object;
}

void MScene::AddRigidStatic(const std::string& name, tinyxml2::XMLElement* node) {
	std::string modelPath = node->Attribute("model");
	std::string materialPath = node->Attribute("material");
	std::string positionStr = node->Attribute("position");
	std::string scaleStr = node->Attribute("scale");
	std::string rotateStr = node->Attribute("rotate");

	float pos_x = 0.0f, pos_y = 0.0f, pos_z = 0.0f;
	int num = GetVec3FromString(positionStr, &pos_x, &pos_y, &pos_z);
	assert(num == 3);
	float scale_x = 0.0f, scale_y = 0.0f, scale_z = 0.0f;
	num = GetVec3FromString(scaleStr, &scale_x, &scale_y, &scale_z);
	assert(num == 3);
	float rotate_x = 0.0f, rotate_y = 0.0f, rotate_z = 0.0f;
	num = GetVec3FromString(rotateStr, &rotate_x, &rotate_y, &rotate_z);
	assert(num == 3);

	MRigidStatic* object = new MRigidStatic(this, name, modelPath, materialPath,
		glm::vec3(pos_x, pos_y, pos_z), 
		glm::vec3(scale_x, scale_y, scale_z), 
		glm::vec3(rotate_x, rotate_y, rotate_z));
	this->mActors.push_back(object);
	this->mActorMap[name] = object;
}

void MScene::AddPointLight(const std::string& name, tinyxml2::XMLElement* node) {
	std::string luaScriptPath = node->Attribute("script");
	std::string positionStr = node->Attribute("position");
	std::string colorStr = node->Attribute("color");
	float linear = node->FloatAttribute("linear");
	float quadratic = node->FloatAttribute("quadratic");
	int mCastShadow = node->IntAttribute("cast_shadow");

	float pos_x = 0.0f, pos_y = 1.0f, pos_z = 0.0f;
	int num = GetVec3FromString(positionStr, &pos_x, &pos_y, &pos_z);
	assert(num == 3);
	float col_r = 0.0f, col_g = 0.0f, col_b = 0.0f;
	num = GetVec3FromString(colorStr, &col_r, &col_g, &col_b);
	assert(num == 3);

#ifdef _DEBUG
	std::cout << "PointLight " << positionStr << " " << pos_x << ',' << pos_y << ',' << pos_z << std::endl;
#endif // DEBUG

	MPointLight* object = new MPointLight(this, name, luaScriptPath,
		glm::vec3(pos_x, pos_y, pos_z),
		glm::vec3(col_r, col_g, col_b), linear, quadratic, mCastShadow);
	this->mActors.push_back(object);
	this->mActorMap[name] = object;
}

void MScene::AddDirectionLight(const std::string& name, tinyxml2::XMLElement* node) {
	std::string luaScriptPath = node->Attribute("script");
	std::string directionStr = node->Attribute("direction");
	std::string colorStr = node->Attribute("color");
	int mCastShadow = node->IntAttribute("cast_shadow");

	float dir_x = 0.0f, dir_y = 1.0f, dir_z = 0.0f;
	int num = GetVec3FromString(directionStr, &dir_x, &dir_y, &dir_z);
	assert(num == 3);
	float col_r = 0.0f, col_g = 0.0f, col_b = 0.0f;
	num = GetVec3FromString(colorStr, &col_r, &col_g, &col_b);
	assert(num == 3);

#ifdef _DEBUG
	std::cout << "DirectionLight " << directionStr << " " << dir_x << ',' << dir_y << ',' << dir_z << std::endl;
#endif // DEBUG

	MDirectionalLight* object = new MDirectionalLight(this, name, luaScriptPath,
		glm::vec3(dir_x, dir_y, dir_z),
		glm::vec3(col_r, col_g, col_b),
		mCastShadow);
	this->mActors.push_back(object);
	this->mActorMap[name] = object;
}

void MScene::AddSpotLight(const std::string& name, tinyxml2::XMLElement* root) {

}

void MScene::AddEntity(const std::string& name, tinyxml2::XMLElement* node) {
	std::string mDefFilePath = node->Attribute("def");
	std::string positionStr = node->Attribute("position");
	std::string scaleStr = node->Attribute("scale");
	std::string rotateStr = node->Attribute("rotate");
	//
	float pos_x = 0.0f, pos_y = 0.0f, pos_z = 0.0f;
	int num = GetVec3FromString(positionStr, &pos_x, &pos_y, &pos_z);
	assert(num == 3);
	float scale_x = 0.0f, scale_y = 0.0f, scale_z = 0.0f;
	num = GetVec3FromString(scaleStr, &scale_x, &scale_y, &scale_z);
	assert(num == 3);
	float rotate_x = 0.0f, rotate_y = 0.0f, rotate_z = 0.0f;
	num = GetVec3FromString(rotateStr, &rotate_x, &rotate_y, &rotate_z);
	assert(num == 3);
	//
	MEntity* object = new MEntity(this, name, mDefFilePath, "",
		glm::vec3(pos_x, pos_y, pos_z),
		glm::vec3(scale_x, scale_y, scale_z),
		glm::vec3(rotate_x, rotate_y, rotate_z));
	this->mActors.push_back(object);
	this->mActorMap[name] = object;
	gPlatform->gLuaState->set(name, object);
}

void MScene::AddSkybox(const std::string& name, tinyxml2::XMLElement* node) {
	std::string right = node->Attribute("right");
	std::string left = node->Attribute("left");
	std::string top = node->Attribute("top");
	std::string bottom = node->Attribute("bottom");
	std::string back = node->Attribute("back");
	std::string front = node->Attribute("front");

	MSkybox* object = new MSkybox(this, name, top, bottom, front, back, left, right);
	this->mActors.push_back(object);
	this->mActorMap[name] = object;
}

void MScene::AddCharacterController(const std::string& name, tinyxml2::XMLElement* root)
{
}

void MScene::AddRagdoll(const std::string& name, tinyxml2::XMLElement* root) {

}

void MScene::GenerateFunctionNames() {
	nOnCreate = mName + std::string("_OnCreate");
	nOnPreUpdate = mName + std::string("_OnPreUpdate");
	nOnPreRender = mName + std::string("_OnPreRender");
	nOnDestroy = mName + std::string("_OnDestroy");
	nOnPostRender = mName + std::string("_OnPostRender");
}

void MScene::OnCreate() {
	(*gPlatform->gLuaState)[nOnCreate]();
}

void MScene::OnDestroy() {
	(*gPlatform->gLuaState)[nOnDestroy]();
}

void MScene::OnPreUpdate(double dt) {
	if (!mEditorModeFlag) {
		(*gPlatform->gLuaState)[nOnPreUpdate](dt);
	}
}

void MScene::OnPreRender(double dt) {
	if (!mEditorModeFlag) {
		(*gPlatform->gLuaState)[nOnPreRender](dt);
	}
}

void MScene::OnPostRender() {
	if (!mEditorModeFlag) {
		(*gPlatform->gLuaState)[nOnPostRender]();
	}
}

unsigned int MScene::GetPointLightID() {
	return this->currentPointLightID++;
}

unsigned int MScene::GetSpotLightID() {
	return this->currentSpotLightID++;
}

void MScene::RenderForDepthMapping() {
	if (this->mLoaded) {
		gPlatform->gDepthMappingPipeline->BeginRendering();
		for (unsigned int i = 0; i < this->mActors.size(); i++) {
			this->mActors[i]->RenderForDepthMapping();
		}
		gPlatform->gDepthMappingPipeline->EndRendering();
	}
}

void MScene::SpawnEntity(const std::string& name, const std::string& def) {
	MEntity* object = new MEntity(this, name, def, "",
		gLocalCamera->mPosition + glm::vec3(0.0f, 5.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f));
	this->mActors.push_back(object);
	this->mActorMap[name] = object;
	gPlatform->gLuaState->set(name, object);
}

void MScene::SpawnEntityEx(const std::string& name, const std::string& def, double xpos, double ypos, double zpos) {
	MEntity* object = new MEntity(this, name, def, "",
		glm::vec3(xpos, ypos, zpos),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f));
	this->mActors.push_back(object);
	this->mActorMap[name] = object;
	gPlatform->gLuaState->set(name, object);
}

void MScene::SpawnEntityAutoNameEx(const std::string& def, double xpos, double ypos, double zpos) {
	std::string name = GenerateGuidA();
	this->SpawnEntityEx(name, def, xpos, ypos, zpos);
}

void MScene::SetDynamicTimeOfDay(int flag) {
	mTimeOfDay->SetDynamic(flag);
}