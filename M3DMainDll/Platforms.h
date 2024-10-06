#pragma once
#include <vector>
#include <atomic>

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "GameConsole.h"
#include "BackgroundQuad.h"
#include "PhysXInterface.h"

class MBasicContainer;
class MGameConsole;
class MBackgroundQuad;
class MVertexShaderProgram;
class MFragmentShaderProgram;
class MAssetManager;
class MUserErrorCallback;
class MPxAllocatorCallback;
class MGameContext;
class MRenderDeferredPipeline;
class MRenderAtomspherePipeline;
class MRenderDepthMappingPipeline;
class MRenderEnvironmentPipeline;
class MRenderOcclusionQueryPipeline;

void OfflineGame_CharCallback(GLFWwindow* window, unsigned int codepoint);
void OfflineGame_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void OfflineGame_FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void OfflineGame_CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void OfflineGame_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

inline void* edGetProcAddress = NULL;
inline HWND gHWND = 0;

class MBasicPlatform {
public:
	GLuint gDefaultFBO = 0;
	MAssetManager* gAssetManager = NULL;
	bool gFullscreenFlag = false;
	bool mCursorDisabled = false;
	int gShadowQuality = 1;
	int gFramebufferWidth = 800, gFramebufferHeight = 600;
	int gAtmosphereFramebufferWidth = 800, gAtmosphereFramebufferHeight = 600;
	int gAtmosphereQuality = 1;
	sol::state* gLuaState = 0;
	void MakeLuaBindings();
	virtual void Update() = 0;
	virtual int GetWindowWidth() = 0;
	virtual int GetWindowHeight() = 0;
	double GetFPS() { return this->FPS; }
	double GetDeltaTime() { return this->deltaTime; }
	virtual double GetTime() = 0;
	virtual int GetKey(int key) = 0;
	virtual int GetKeyInGameConsole(int key) = 0;
	virtual bool GetMouseLeftButtonDown() = 0;
	virtual bool GetMouseRightButtonDown() = 0;
	virtual bool GetMouseLeftButtonUp() = 0;
	virtual bool GetMouseRightButtonUp() = 0;
	virtual void ConsoleWriteLine(float r, float g, float b, const std::string& text) = 0;
	virtual void SetCursorInvisible() = 0;
	virtual void SetCursorVisible() = 0;
	virtual void KeyCallback(int key, int scancode, int action, int mods) = 0;
	virtual void FramebufferSizeCallback(int width, int height) = 0;
	virtual void CursorPosCallback(GLFWwindow* window, double xpos, double ypos) = 0;
	bool GetWhetherShouldExit() { return this->gShouldExit; }
	double GetCursorPosX() { return this->mCursorPosX; }
	double GetCursorPosY() { return this->mCursorPosY; }
	double GetScrollOffsetX() { return this->mScrollOffsetX; }
	double GetScrollOffsetY() { return this->mScrollOffsetY; }
	void LoadScene(const std::string& path);
	void LoadUIForm(const std::string& path);
	MPlatformMode gMode;
	MBasicContainer* gCurrentContainer = NULL;
	MRenderDeferredPipeline* gDeferredPipeline = NULL;
	MRenderAtomspherePipeline* gAtomspherePipeline = NULL;
	MRenderDepthMappingPipeline* gDepthMappingPipeline = NULL;
	Assimp::Importer assimpImporter;
	MUserErrorCallback gErrorCallback;
	MPxAllocatorCallback gAllocatorCallback;
	physx::PxFoundation* pFoundation = NULL;
	physx::PxPvd* pPvd = NULL;
	physx::PxPhysics* pPhysics = NULL;
	physx::PxCooking* pCooking = NULL;
	physx::PxTolerancesScale pDefaultScale = physx::PxTolerancesScale();
	physx::PxMaterial* gMetalMaterial = NULL, * gStoneMaterial = NULL,
		* gWoodMaterial = NULL, * gPlasticMaterial = NULL,
		* gMudMaterial = NULL;
	MGameConsole* gGameConsole = NULL;
	MGameContext* gGameContext = NULL;
	float mouseSensitivity = 0.1f;
protected:
	double lastFrameTime = 0.0, currentFrameTime = 0.0;
	double FPS = 0.0;
	double deltaTime = 0.0;
	bool gShouldExit = false;
	double mCursorPosX, mCursorPosY;
	double mScrollOffsetX, mScrollOffsetY;
	int mLButtonFlag = 0, mRButtonFlag = 0;
	std::unordered_map<int, int> mKeyStatueTable;
	std::string mMainModulePath;
	tinyxml2::XMLDocument* gameConfigDoc = NULL;
	void InitializePhysX();
	void InitializePipeline();
	void LoadGlobalConfig();
};

class MOfflineGame :public MBasicPlatform {
public:
	MOfflineGame(unsigned int OS);
	~MOfflineGame();
	int GetWindowWidth() { return this->gFramebufferWidth; }
	int GetWindowHeight() { return this->gFramebufferHeight; }
	double GetTime() { return glfwGetTime(); }
	virtual int GetKey(int key)override;
	virtual int GetKeyInGameConsole(int key)override;
	virtual void Update()override;
	virtual void ConsoleWriteLine(float r, float g, float b, const std::string& text)override;
	virtual bool GetMouseLeftButtonDown()override;
	virtual bool GetMouseRightButtonDown()override;
	virtual bool GetMouseLeftButtonUp()override;
	virtual bool GetMouseRightButtonUp()override;
	void CharCallback(unsigned int codepoint);
	void KeyCallback(int key, int scancode, int action, int mods);
	void FramebufferSizeCallback(int width, int height);
	void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	void ScrollCallback(double xoffset, double yoffset);
	void SetCursorInvisible();
	virtual void SetCursorVisible()override;
private:
	GLFWwindow* window;
	void SplashScreen();
protected:
};

class MEditor :public MBasicPlatform {
public:
	MEditor(unsigned int OS);
	~MEditor();
	void SetEditMode(MEditMode mode);
	void SetDefaultFBO(GLuint ID) { gDefaultFBO = ID; }
	int GetWindowWidth() { return this->gFramebufferWidth; }
	int GetWindowHeight() { return this->gFramebufferHeight; }
	virtual double GetTime()override;
	virtual int GetKey(int key)override;
	virtual int GetKeyInGameConsole(int key)override;
	virtual bool GetMouseLeftButtonDown()override;
	virtual bool GetMouseRightButtonDown()override;
	virtual bool GetMouseLeftButtonUp()override;
	virtual bool GetMouseRightButtonUp()override;
	virtual void Update()override;
	virtual void ConsoleWriteLine(float r, float g, float b, const std::string& text)override;
	virtual void SetCursorInvisible()override;
	virtual void SetCursorVisible()override;
	void StartGame();
	void ExitGame();
	void LoadSceneWithFullPath(const std::string& path);
	void SaveSceneWithFullPath(const std::string& path);
	void SubmitKeyboardInput(int key, int mode);
	void SubmitCursorPosInput(double x, double y);
	void SubmitMouseButtonFlags(unsigned short flags);
	void SubmitMouseButtonInput(int button, int flag);
	void SubmitScrollOffset(double x, double y);
	void KeyCallback(int key, int scancode, int action, int mods);
	void FramebufferSizeCallback(int width, int height);
	void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	void CreateRigidStatic(const std::string& model);
	void CreateEntity(const std::string& def);
private:
protected:
	int mglfwGetMouseButton(int button);
	unsigned short mMouseButtonFlags = 0u;
	int mMouseButtons[2] = { 0 };
};