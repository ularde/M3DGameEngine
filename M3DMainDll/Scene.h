#pragma once
#include <vector>
#include <map>

#include "BasicContainer.h"
#include "Platforms.h"

class MBasicPlatform;
class MBasicContainer;
class MActor;
class MCamera;
class MFreeCamera;
class MPlayer;
class MAtmosphere;
class MEntity;
class MCameraComponent;
class MTimeOfDay;
class MDirectionalLight;
class MSceneEditorAgent;
class MCinematicCamera;
class MFirstPersonCamera;

class MScene :public MBasicContainer {
public:
	MScene(bool immediately_load, MBasicPlatform* platform_, const std::string& path_);
	~MScene();
	virtual void InitializeLuaInstance()override;
	virtual void MouseCallback(GLFWwindow* window, double xpos, double ypos)override;
	virtual void Update(double dt)override;
	virtual void Render()override;
	virtual void Release()override;
	void TestAPI() { printf("Hello, API.\n"); }
	void SetEditMode(MEditMode mode);
	void SaveSceneWithFullPath(const std::string& path);
	void StartGame();
	void ExitGame();
	void SetLocalFirstPersonCamera(MFirstPersonCamera* camera);
	void SetLocalCinematicCamera(MCinematicCamera* camera);
	bool IsEditorMode() { return this->mEditorModeFlag; }
	double GetWorldTime();
	void SetWorldTime(double t);
	unsigned int GetPointLightID();
	unsigned int GetSpotLightID();
	void RenderForDepthMapping();
	void SpawnEntity(const std::string& name, const std::string& def);
	void SpawnEntityEx(const std::string& name, const std::string& def, double xpos, double ypos, double zpos);
	void SpawnEntityAutoNameEx(const std::string& def, double xpos, double ypos, double zpos);
	void SetDynamicTimeOfDay(int flag);
	void OnCreate();
	void OnDestroy();
	void OnPreUpdate(double dt);
	void OnPreRender(double dt);
	void OnPostRender();
	MTimeOfDay* mTimeOfDay = NULL;
	MEntity* gLocalPlayer = NULL;
	MFreeCamera* gEditorCamera = NULL;
	MCameraComponent* gLocalCamera = NULL;
	MDirectionalLight* mSunLight = NULL;
	MAtmosphere* mAtmosphere = NULL;
	MSceneEditorAgent* mEditorAgent = NULL;
	physx::PxScene* pScene = NULL;
	physx::PxControllerManager* pControllerManager = NULL;
	std::vector<MActor*> mActors;
	std::map<std::string, MActor*> mActorMap;
protected:
	bool mEditorModeFlag = false;
	bool mLoaded = false;
	tinyxml2::XMLDocument mSceneDoc;
	void InitializePhysics();
	void UpdatePhysics(double dt);
	void LoadScene();
	void AddPlayer(const std::string& name, tinyxml2::XMLElement* root);
	void AddRigidStatic(const std::string& name, tinyxml2::XMLElement* root);
	void AddPointLight(const std::string& name, tinyxml2::XMLElement* root);
	void AddDirectionLight(const std::string& name, tinyxml2::XMLElement* root);
	void AddSpotLight(const std::string& name, tinyxml2::XMLElement* root);
	void AddEntity(const std::string& name, tinyxml2::XMLElement* root);
	void AddSkybox(const std::string& name, tinyxml2::XMLElement* root);
	void AddCharacterController(const std::string& name, tinyxml2::XMLElement* root);
	void AddRagdoll(const std::string& name, tinyxml2::XMLElement* root);
	void GenerateFunctionNames();
private:
	unsigned int currentPointLightID = 1;
	unsigned int currentSpotLightID = 1;
	std::string nOnCreate, nOnPreUpdate,
		nOnPreRender, nOnDestroy,
		nOnPostRender;
};