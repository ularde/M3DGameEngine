#include "pch.h"
#include "Platforms.h"
#include "ConsoleOperations.h"
#include "SharedFunc.h"
#include "ShaderManager.h"
#include "PhysXInterface.h"
#include "AssetManager.h"
#include "BasicContainer.h"
#include "Scene.h"
#include "UIForm.h"
#include "Class.h"
#include "Light.h"
#include "RigidStatic.h"
#include "RenderDeferredPipeline.h"
#include "RenderAtomspherePipeline.h"
#include "RenderOcclusionQueryPipeline.h"
#include "RenderEnvironmentPipeline.h"
#include "RenderDepthMappingPipeline.h"
#include "RenderGuiPipeline.h"
#include "Atmosphere.h"
#include "GameContext.h"
#include "GuiContext.h"
#include "ActorEditorAgent.h"
#include "SceneEditorAgent.h"
#include "Entity.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <GL/freeglut.h>

class MRenderDeferredPipeline;
class MRenderAtomspherePipeline;
class MRenderGuiPipeline;
class MDirectionalLight;
class MScene;
class MPointLight;
class MPlayer;
class MRigidStatic;

static HMODULE libMGL;
typedef void* (APIENTRYP PFNMWGLGETPROCADDRESS)(const char*);
static PFNMWGLGETPROCADDRESS mwglGetProcAddress;
typedef HGLRC(APIENTRYP PFNMWGLCREATECONTEXT)(HDC);
static PFNMWGLCREATECONTEXT mwglCreateContext;
typedef HGLRC(APIENTRYP PFNMWGLGETCURRENTCONTEXT)(void);
static PFNMWGLGETCURRENTCONTEXT mwglGetCurrentContext;
typedef BOOL(APIENTRYP PFNMWGLMAKECURRENT)(HDC, HGLRC);
static PFNMWGLMAKECURRENT mwglMakeCurrent;
typedef BOOL(APIENTRYP PFNMWGLDELETECONTEXT)(HGLRC);
static PFNMWGLDELETECONTEXT mwglDeleteContext;
typedef HDC(APIENTRYP PFNWGLGETCURRENTDC)(void);
static PFNWGLGETCURRENTDC mwglGetCurrentDC;
typedef BOOL(APIENTRYP PFNMWGLSWAPBUFFERS)(HDC);
static PFNMWGLSWAPBUFFERS mwglSwapBuffers;
typedef BOOL(APIENTRYP PFNMWGLSWAPLAYERBUFFERS)(HDC, UINT);
static PFNMWGLSWAPLAYERBUFFERS mwglSwapLayerBuffers;
typedef int(APIENTRYP PFNMWGLCHOOSEPIXELFORMAT)(HDC, PIXELFORMATDESCRIPTOR*);
static PFNMWGLCHOOSEPIXELFORMAT mwglChoosePixelFormat;
typedef BOOL(APIENTRYP PFNMWGLSETPIXELFORMAT)(HDC, int, PIXELFORMATDESCRIPTOR*);
static PFNMWGLSETPIXELFORMAT mwglSetPixelFormat;

static int InitializeMesa3D(void) {
	libMGL = LoadLibrary(L"mogl32.dll");
	if (libMGL != NULL) {
		mwglChoosePixelFormat = (PFNMWGLCHOOSEPIXELFORMAT)GetProcAddress(libMGL, "wglChoosePixelFormat");
		mwglSetPixelFormat = (PFNMWGLSETPIXELFORMAT)GetProcAddress(libMGL, "wglSetPixelFormat");
		mwglCreateContext = (PFNMWGLCREATECONTEXT)GetProcAddress(libMGL, "wglCreateContext");
		mwglMakeCurrent = (PFNMWGLMAKECURRENT)GetProcAddress(libMGL, "wglMakeCurrent");
		mwglDeleteContext = (PFNMWGLDELETECONTEXT)GetProcAddress(libMGL, "wglDeleteContext");
		mwglGetCurrentDC = (PFNWGLGETCURRENTDC)GetProcAddress(libMGL, "wglGetCurrentDC");
		mwglGetCurrentContext = (PFNMWGLGETCURRENTCONTEXT)GetProcAddress(libMGL, "wglGetCurrentContext");
		mwglGetProcAddress = (PFNMWGLGETPROCADDRESS)GetProcAddress(libMGL, "wglGetProcAddress");
		mwglSwapBuffers = (PFNMWGLSWAPBUFFERS)GetProcAddress(libMGL, "wglSwapBuffers");
		mwglSwapLayerBuffers = (PFNMWGLSWAPLAYERBUFFERS)GetProcAddress(libMGL, "wglSwapLayerBuffers");
		return mwglGetProcAddress != NULL;
	}
	return 0;
}

static void mclose_gl(void) {
	if (libMGL != NULL) {
		FreeLibrary((HMODULE)libMGL);
		libMGL = NULL;
	}
}

MOfflineGame::MOfflineGame(unsigned int OS) {
	gType = MPlatformType::GAME;
	this->gLuaState = new sol::state();
	this->gLuaState->open_libraries(sol::lib::base);

	CoInitialize(NULL);

	this->LoadGlobalConfig();

	GetPrivateProfileString(L"Graphics", L"Driver", L"Default", gDriver, 1024, GetFullAssetPathW(L"system.ini"));
	gFullscreenFlag = GetPrivateProfileInt(L"Graphics", L"Fullscreen", 0, GetFullAssetPathW(L"system.ini"));
	gShadowQuality = GetPrivateProfileInt(L"Graphics", L"ShadowQuality", 0, GetFullAssetPathW(L"system.ini"));
	gAtmosphereQuality = GetPrivateProfileInt(L"Graphics", L"AtomsphereQuality", 0, GetFullAssetPathW(L"system.ini"));
	gFramebufferWidth = GetPrivateProfileInt(L"Graphics", L"WindowWidth", 0, GetFullAssetPathW(L"system.ini"));
	gFramebufferHeight = GetPrivateProfileInt(L"Graphics", L"WindowHeight", 0, GetFullAssetPathW(L"system.ini"));
	gSSDOFlag = GetPrivateProfileInt(L"Graphics", L"SSDO", 0, GetFullAssetPathW(L"system.ini"));

	if (wcsncmp(gDriver, L"Gallium", 1024) == 0) {
		gGalliumFlag = true;
	}

	std::string mainModulePath = gameConfigDoc->RootElement()->FirstChildElement()->Attribute("main_script");
	std::string windowTitle = gameConfigDoc->RootElement()->FirstChildElement()->Attribute("title");

	mMainModulePath = mainModulePath;

	switch (gAtmosphereQuality)
	{
	case 1:
		gAtmosphereFramebufferWidth = (int)gFramebufferWidth / 8;
		gAtmosphereFramebufferHeight = (int)gFramebufferHeight / 8;
		break;
	case 2:
		gAtmosphereFramebufferWidth = (int)gFramebufferWidth / 4;
		gAtmosphereFramebufferHeight = (int)gFramebufferHeight / 4;
		break;
	case 3:
		gAtmosphereFramebufferWidth = (int)gFramebufferWidth / 2;
		gAtmosphereFramebufferHeight = (int)gFramebufferHeight / 2;
		break;
	default:
		break;
	}
	
	glfwInit();
	
	if (!gGalliumFlag) {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		this->gWindow = glfwCreateWindow(this->gFramebufferWidth, this->gFramebufferHeight, windowTitle.c_str(), NULL, NULL);

		glfwMakeContextCurrent(this->gWindow);
		glfwSwapInterval(-1);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			MessageBeep(MB_ICONERROR);
			MessageBox(NULL, L"Failed to initialize OpenGL.", L"M3D GameEngine", MB_ICONERROR);
			glfwDestroyWindow(this->gWindow);
			glfwTerminate();
		}
	}
	else {
		if (InitializeMesa3D()) {
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			this->gWindow = glfwCreateWindow(this->gFramebufferWidth, this->gFramebufferHeight, windowTitle.c_str(), NULL, NULL);

			HWND hWnd = glfwGetWin32Window(this->gWindow);
			HDC dc = GetDC(hWnd);
			PIXELFORMATDESCRIPTOR pfd =
			{
				sizeof(PIXELFORMATDESCRIPTOR),
				1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,
				PFD_TYPE_RGBA,
				24,
				0,0,0,0,0,0,
				0,
				0,
				0,0,0,0,
				8,
				0,
				0,
				PFD_MAIN_PLANE,
				0,
				0,0,0
			};
			int pf;
			pf = ChoosePixelFormat(dc, &pfd);
			if (!pf) {
				MessageBox(NULL, L"Failed to get a proper pixel format.", L"M3D GameEngine", MB_ICONERROR);
			}
			else {
				SetPixelFormat(dc, pf, &pfd);
				HGLRC rc = mwglCreateContext(dc);
				assert(rc);
				mwglMakeCurrent(dc, rc);
				m_DC = mwglGetCurrentDC();
				
				if (!gladLoadGLLoader((GLADloadproc)mwglGetProcAddress)) {
					MessageBeep(MB_ICONERROR);
					MessageBox(NULL, L"Failed to initialize Gallium.", L"M3D GameEngine", MB_ICONERROR);
					glfwDestroyWindow(this->gWindow);
					glfwTerminate();
				}
			}
		}
	}

	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwSetWindowUserPointer(this->gWindow, reinterpret_cast<void*>(this));
	glfwSetCharCallback(this->gWindow, OfflineGame_CharCallback);
	glfwSetKeyCallback(this->gWindow, OfflineGame_KeyCallback);
	glfwSetFramebufferSizeCallback(this->gWindow, OfflineGame_FramebufferSizeCallback);
	glfwSetCursorPosCallback(this->gWindow, OfflineGame_CursorPosCallback);
	glfwSetScrollCallback(this->gWindow, OfflineGame_ScrollCallback);
	//glfwSetWindowSizeLimits(this->window, this->windowWidth, this->windowHeight, this->windowWidth, this->windowHeight);

	this->SplashScreen();

	glfwSwapInterval(0);

	stbi_set_flip_vertically_on_load(true);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsLight();
	ImGuiIO& io = ImGui::GetIO();
	ImFont* font = io.Fonts->AddFontFromFileTTF(GetFullAssetPathA("Fonts\\arial.ttf"), 18.0f);
	io.FontDefault = font;

	ImGui_ImplGlfw_InitForOpenGL(this->gWindow, true);
	ImGui_ImplOpenGL3_Init("#version 430 core");

	this->gAssetManager = new MAssetManager(this);
	this->gGameConsole = new MGameConsole(this);
	this->gGameContext = new MGameContext(this);
	this->InitializePipeline();
	this->InitializeMImGui();

	this->InitializePhysX();
	this->MakeLuaBindings();

	gLuaState->safe_script_file(GetFullAssetPathA(mMainModulePath));
	(*gLuaState)["Main_OnInit"]();
}


MOfflineGame::~MOfflineGame() {
	(*gLuaState)["Main_OnDestroy"]();

	delete[]this->gAssetManager;
	delete[]this->gameConfigDoc;
	delete[]this->gGameConsole;

	this->gGameConsole = nullptr;
	this->gameConfigDoc = nullptr;

	glfwDestroyWindow(this->gWindow);
	glfwTerminate();
}

int MOfflineGame::GetKey(int key) {
	if (gGameConsole->GetConsoleVisibility()) {
		return -1;
	}
	else {
		return glfwGetKey(this->gWindow, key);
	}
}

int MOfflineGame::GetKeyInGameConsole(int key) {
	if (gGameConsole->GetConsoleVisibility()) {
		return glfwGetKey(this->gWindow, key);
	}
	else {
		return -1;
	}
}

void MOfflineGame::Update() {
	this->currentFrameTime = glfwGetTime();
	this->deltaTime = this->currentFrameTime - this->lastFrameTime;
	this->lastFrameTime = this->currentFrameTime;

	if (this->gFullscreenFlag) {
		const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowSize(this->gWindow, vidmode->width, vidmode->height);
		glfwSetWindowMonitor(this->gWindow, glfwGetPrimaryMonitor(), 0, 0, vidmode->width, vidmode->height, GLFW_DONT_CARE);
	}
	this->gShouldExit = glfwWindowShouldClose(this->gWindow);

	if (!gGameConsole->GetConsoleVisibility()) {
		glfwGetCursorPos(this->gWindow, &this->mCursorPosX, &this->mCursorPosY);
	}

	glViewport(0, 0, gFramebufferWidth, gFramebufferHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	if (gCurrentContainer) {
		gCurrentContainer->Update(this->deltaTime);
		gCurrentContainer->Render();
	}

	gDeferredPipeline->DoLightingPass();
	gDeferredPipeline->DoIndirectLightingPass();
	gDeferredPipeline->DoCompositionPass();

	//Post Render
	if (gCurrentContainer && gMode == MPlatformMode::SCENE) {
		reinterpret_cast<MScene*>(gCurrentContainer)->OnPostRender();
	}
	//console
	this->gGameConsole->Update();
	this->gGameConsole->Render();
	//ImGui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	this->gGuiContext->RenderText("arial", std::string("FPS ") + std::to_string(int(this->FPS)), 10.0f, this->gFramebufferHeight - 25.0f, glm::vec3(1.0f, 1.0f, 1.0f), 0.5);

	(*gLuaState)["Main_OnTickFinish"]();

	this->FPS = 1.0 / abs(this->deltaTime);

	//reset scroll offset values
	this->mScrollOffsetX = 0.0;
	this->mScrollOffsetY = 0.0;

	if (!gGalliumFlag) {
		glfwSwapBuffers(this->gWindow);
	}
	else {
		mwglSwapBuffers(m_DC);
	}

	glfwPollEvents();
}

void MOfflineGame::ConsoleWriteLine(float r, float g, float b, const std::string& text) {
	this->gGameConsole->WriteLine(r, g, b, text);
}

bool MOfflineGame::GetMouseLeftButtonDown() {
	int n = glfwGetMouseButton(this->gWindow, GLFW_MOUSE_BUTTON_LEFT);
	switch (n)
	{
	case GLFW_PRESS:
		return true;
		break;
	case GLFW_RELEASE:
		return false;
		break;
	default:
		break;
	}
	return false;
}

bool MOfflineGame::GetMouseRightButtonDown() {
	int n = glfwGetMouseButton(this->gWindow, GLFW_MOUSE_BUTTON_RIGHT);
	switch (n)
	{
	case GLFW_PRESS:
		return true;
		break;
	case GLFW_RELEASE:
		return false;
		break;
	default:
		break;
	}
	return false;
}

bool MOfflineGame::GetMouseLeftButtonUp() {
	int n = glfwGetMouseButton(this->gWindow, GLFW_MOUSE_BUTTON_LEFT);
	switch (n)
	{
	case GLFW_PRESS:
		return false;
		break;
	case GLFW_RELEASE:
		return true;
		break;
	default:
		break;
	}
	return true;
}

bool MOfflineGame::GetMouseRightButtonUp() {
	int n = glfwGetMouseButton(this->gWindow, GLFW_MOUSE_BUTTON_RIGHT);
	switch (n)
	{
	case GLFW_PRESS:
		return false;
		break;
	case GLFW_RELEASE:
		return true;
		break;
	default:
		break;
	}
	return true;
}

void MOfflineGame::CharCallback(unsigned int codepoint) {
	this->gGameConsole->CaptureTextInput(codepoint);
}

void MOfflineGame::KeyCallback(int key, int scancode, int action, int mods) {
	this->gGameConsole->CaptureControlKeyInput(key, scancode, action, mods);
}

void MOfflineGame::FramebufferSizeCallback(int width, int height) {
	gFramebufferWidth = width;
	gFramebufferHeight = height;
	switch (gAtmosphereQuality)
	{
	case 1:
		gAtmosphereFramebufferWidth = (int)gFramebufferWidth / 8;
		gAtmosphereFramebufferHeight = (int)gFramebufferHeight / 8;
		break;
	case 2:
		gAtmosphereFramebufferWidth = (int)gFramebufferWidth / 4;
		gAtmosphereFramebufferHeight = (int)gFramebufferHeight / 4;
		break;
	case 3:
		gAtmosphereFramebufferWidth = (int)gFramebufferWidth / 2;
		gAtmosphereFramebufferHeight = (int)gFramebufferHeight / 2;
		break;
	default:
		break;
	}
	gForwardPipeline->UpdateFramebufferSize();
	gDeferredPipeline->UpdateFramebufferSize();
	gAtomspherePipeline->UpdateFramebufferSize();
}

void MOfflineGame::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	if (this->gCurrentContainer) {
		this->gCurrentContainer->MouseCallback(window, xpos, ypos);
	}
}

void MOfflineGame::ScrollCallback(double xoffset, double yoffset) {
	mScrollOffsetX = xoffset;
	mScrollOffsetY = yoffset;
}

void MOfflineGame::SetCursorInvisible() {
	mCursorDisabled = true;
	glfwSetInputMode(this->gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void MOfflineGame::SetCursorVisible() {
	mCursorDisabled = false;
	glfwSetInputMode(this->gWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


void OfflineGame_CharCallback(GLFWwindow* window, unsigned int codepoint) {
	MOfflineGame* instance = reinterpret_cast<MOfflineGame*>(glfwGetWindowUserPointer(window));
	instance->CharCallback(codepoint);
}

void OfflineGame_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	MOfflineGame* instance = reinterpret_cast<MOfflineGame*>(glfwGetWindowUserPointer(window));
	instance->KeyCallback(key, scancode, action, mods);
}

void OfflineGame_FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	MOfflineGame* instance = reinterpret_cast<MOfflineGame*>(glfwGetWindowUserPointer(window));
	instance->FramebufferSizeCallback(width, height);
}

void OfflineGame_CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	MOfflineGame* instance = reinterpret_cast<MOfflineGame*>(glfwGetWindowUserPointer(window));
	instance->CursorPosCallback(window, xpos, ypos);
}

void OfflineGame_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	MOfflineGame* instance = reinterpret_cast<MOfflineGame*>(glfwGetWindowUserPointer(window));
	instance->ScrollCallback(xoffset, yoffset);
}

void MOfflineGame::SplashScreen() {
	/*
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	int imgWidth, imgHeight, imgChannelCount;
	//
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imgData = stbi_load(GetFullAssetPathA("Textures\\SplashScreen.png"), &imgWidth, &imgHeight, &imgChannelCount, 0);
	//
	unsigned int texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//
	if (imgData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	}
	else {
		MessageBox(NULL, L"Failed to load splash screen image.", L"Engine Error", MB_ICONERROR);
	}
	stbi_image_free(imgData);
	//
	glUseProgram(0);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	for (int i = 0; i < 90; ++i) {
		glColor3f((float)i / 90.0f, (float)i / 90.0f, (float)i / 90.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(-1.0f, 1.0f);
		glEnd();
		glfwSwapBuffers(this->window);
		glfwPollEvents();
	}
	for (int i = 90; i > 0; --i) {
		glColor3f((float)i / 90.0f, (float)i / 90.0f, (float)i / 90.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(-1.0f, 1.0f);
		glEnd();
		glfwSwapBuffers(this->window);
		glfwPollEvents();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	*/
}

void MBasicPlatform::InitializePhysX() {
	//initialize log
	physxLog = fopen(".\\PhysX.log", "w");
	if (!physxLog) {
		MessageBox(NULL, L"Failed to open file PhysX.log. Engine will exit.", L"Engine Error", MB_ICONERROR);
		exit(-1);
	}
	//initialize PhysX
	this->pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, this->gAllocatorCallback, this->gErrorCallback);
	if (!this->pFoundation) {
		MessageBox(NULL, L"Failed to initialize PhysX foundation. Engine will exit.", L"Engine Error", MB_ICONERROR);
		exit(-1);
	}
	//
	bool recordMemoryAllocations = true;
	//PVD
	this->pPvd = physx::PxCreatePvd(*this->pFoundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	this->pPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
	//PxPhysics
	this->pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *this->pFoundation, this->pDefaultScale, recordMemoryAllocations, this->pPvd);
	if (!this->pPhysics) {
		MessageBox(NULL, L"Failed to initialize PhysX PxPhysics. Engine will exit.", L"Engine Error", MB_ICONERROR);
		exit(-1);
	}
	//PxCooking
	this->pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *this->pFoundation, physx::PxCookingParams(this->pDefaultScale));
	if (!this->pCooking) {
		MessageBox(NULL, L"Failed to initialize PhysX PxCooking. Engine will exit.", L"Engine Error", MB_ICONERROR);
		exit(-1);
	}
	//initialize physx materials
	gMetalMaterial = pPhysics->createMaterial(0.4f, 0.4f, 0.1f);
	gStoneMaterial = pPhysics->createMaterial(0.4f, 0.4f, 0.0f);
	gWoodMaterial = pPhysics->createMaterial(0.4f, 0.4f, 0.2f);
	gPlasticMaterial = pPhysics->createMaterial(0.4f, 0.4f, 0.0f);
	gMudMaterial = pPhysics->createMaterial(0.4f, 0.4f, 0.0f);
}

void MBasicPlatform::InitializePipeline() {
	gDeferredPipeline = new MRenderDeferredPipeline(this);
	gAtomspherePipeline = new MRenderAtomspherePipeline(this);
	gDepthMappingPipeline = new MRenderDepthMappingPipeline(this);
	gForwardPipeline = new MRenderForwardPipeline(this);
	gGuiPipeline = new MRenderGuiPipeline(this);
}

void MBasicPlatform::InitializeMImGui() {
	if (FT_Init_FreeType(&mFreetypeLib)) {
		MessageBox(NULL, L"Failed to initialize Freetype.", L"ERROR", MB_ICONHAND);
	}
	gGuiContext = new MGuiContext(this, mFreetypeLib, gGuiPipeline);
}

void MBasicPlatform::LoadGlobalConfig() {
	this->gameConfigDoc = new tinyxml2::XMLDocument();
	if (this->gameConfigDoc->LoadFile(".\\..\\..\\Game\\Config\\global.xml") != tinyxml2::XML_SUCCESS) {
		MessageBeep(MB_ICONERROR);
		MessageBox(NULL, L"Failed to read document Game\\Config\\global.xml.", L"M3D GameEngine", MB_ICONERROR);
	}
}

void MBasicPlatform::LoadScene(const std::string& path) {
	//only in game mode
	if (gCurrentContainer) {
		if (gCurrentContainer->mClassName == "Scene") {
			if (!reinterpret_cast<MScene*>(gCurrentContainer)->IsEditorMode()) {
				//release old scene
				this->gCurrentContainer->Release();
				free(this->gCurrentContainer);
				//load new scene
				this->gCurrentContainer = NULL;
				this->gCurrentContainer = new MScene(true, this, GetFullAssetPathA(path));
				assert(this->gCurrentContainer);
				this->gMode = MPlatformMode::SCENE;
				this->gGameContext->UpdateEntityTable();
			}
		}
		//such as a form
		else {
			//release old scene
			this->gCurrentContainer->Release();
			free(this->gCurrentContainer);
			//load new scene
			this->gCurrentContainer = NULL;
			this->gCurrentContainer = new MScene(true, this, GetFullAssetPathA(path));
			assert(this->gCurrentContainer);
			this->gMode = MPlatformMode::SCENE;
			this->gGameContext->UpdateEntityTable();
		}
	}
	else if (gCurrentContainer == NULL) {
		//load new scene
		this->gCurrentContainer = NULL;
		this->gCurrentContainer = new MScene(true, this, GetFullAssetPathA(path));
		assert(this->gCurrentContainer);
		this->gMode = MPlatformMode::SCENE;
		this->gGameContext->UpdateEntityTable();
	}
	gLuaState->set("CurrentScene", (MScene*)this->gCurrentContainer);
}

void MBasicPlatform::LoadUIForm(const std::string& path) {
	this->gMode = MPlatformMode::FORM;
}

void MBasicPlatform::UpdateDepthMappingPipeline() {
	gDepthMappingPipeline->UpdateDepthMappingSize();
}

void MBasicPlatform::UpdateFullscreenState() {

}

void MBasicPlatform::MakeLuaBindings() {
	gLuaState->new_usertype<glm::vec2>("vec2",
		"x", &glm::vec2::x,
		"y", &glm::vec2::y);
	gLuaState->new_usertype<glm::ivec2>("ivec2",
		"x", &glm::ivec2::x,
		"y", &glm::ivec2::y);
	gLuaState->new_usertype<glm::vec3>("vec3",
		"x", &glm::vec3::x,
		"y", &glm::vec3::y,
		"z", &glm::vec3::z);
	gLuaState->new_usertype<glm::vec4>("vec4",
		"x", &glm::vec4::x,
		"y", &glm::vec4::y,
		"z", &glm::vec4::z,
		"w", &glm::vec4::w);
	gLuaState->new_usertype<MScene>("Scene",
		"SpawnEntity", &MScene::SpawnEntity,
		"SpawnEntityEx", &MScene::SpawnEntityEx,
		"SpawnEntityAutoNameEx", &MScene::SpawnEntityAutoNameEx,
		"SetLocalCameraFPC", &MScene::SetLocalFirstPersonCamera,
		"SetLocalCameraCC", &MScene::SetLocalCinematicCamera,
		"TestAPI", &MScene::TestAPI);
	gLuaState->new_usertype<MEntity>("Entity",
		"GetCinematicCamera", &MEntity::GetCinematicCamera,
		"GetFirstPersonCamera", &MEntity::GetFirstPersonCamera,
		"GetRigidBodyPhysicsProxyBox", &MEntity::GetRigidBodyPhysicsProxyBox,
		"GetRigidBodyPhysicsProxySphere", &MEntity::GetRigidBodyPhysicsProxySphere);
	gLuaState->new_usertype<MFirstPersonCamera>("FirstPersonCamera",
		"position", &MFirstPersonCamera::mPosition,
		"front", &MFirstPersonCamera::mFront,
		"up", &MFirstPersonCamera::mUpDirection);
	gLuaState->new_usertype<MRigidBodyPhysicsProxyBox>("RigidBodyPhysicsProxyBox",
		"AddForce", &MRigidBodyPhysicsProxyBox::AddForce,
		"AddTorque", &MRigidBodyPhysicsProxyBox::AddTorque,
		"SetPosition", &MRigidBodyPhysicsProxyBox::SetPosition,
		"Validate", &MRigidBodyPhysicsProxyBox::Validate,
		"Invalidate", &MRigidBodyPhysicsProxyBox::Invalidate);
	gLuaState->new_usertype<MCinematicCamera>("CinematicCamera",
		"fovy", &MCinematicCamera::mFovy,
		"front", &MCinematicCamera::mFront,
		"position", &MCinematicCamera::mPosition,
		"up", &MCinematicCamera::mUpDirection);
	gLuaState->new_usertype<MRigidBodyPhysicsProxySphere>("RigidBodyPhysicsProxySphere",
		"AddForce", &MRigidBodyPhysicsProxySphere::AddForce,
		"AddTorque", &MRigidBodyPhysicsProxySphere::AddTorque,
		"SetPosition", &MRigidBodyPhysicsProxySphere::SetPosition,
		"Validate", &MRigidBodyPhysicsProxySphere::Validate,
		"Invalidate", &MRigidBodyPhysicsProxySphere::Invalidate);
	gLuaState->new_usertype<MButtonEvent>("ButtonEvent",
		"click", &MButtonEvent::click,
		"dbclick", &MButtonEvent::dbclick,
		"mouseOn", &MButtonEvent::mouseOn);
	gLuaState->set_function("GetTime", Game_GetTime);
	gLuaState->set_function("GetCameraPosition", CinematicCamera_GetPosition);
	gLuaState->set_function("GetCameraFront", CinematicCamera_GetFront);
	gLuaState->set_function("GetCameraUp", CinematicCamera_GetUp);
	gLuaState->set_function("GetCameraFovy", CinematicCamera_GetFovy);
	gLuaState->set_function("SetCameraPosition", CinematicCamera_SetPosition);
	gLuaState->set_function("SetCameraFront", CinematicCamera_SetFront);
	gLuaState->set_function("SetCameraUp", CinematicCamera_SetUp);
	gLuaState->set_function("SetCameraFovy", CinematicCamera_SetFovy);
	gLuaState->set_function("SetCursorInvisible", Game_SetCursorInvisible);
	gLuaState->set_function("SetCursorVisible", Game_SetCursorVisible);
	gLuaState->set_function("SetLocalCamera", Game_SetLocalCamera);
	gLuaState->set_function("BindEntity", Game_BindEntity);
	gLuaState->set_function("BindThisEntity", Game_BindThisEntity);
	gLuaState->set_function("BindThisEntityComponent", Entity_BindThisEntityComponent);
	gLuaState->set_function("ConsoleWriteLine", Game_ConsoleWriteLine);
	gLuaState->set_function("LoadScene", Game_LoadScene);
	gLuaState->set_function("SpawnEntity", Game_SpawnEntity);
	gLuaState->set_function("SpawnEntityAutoName", Game_SpawnEntityAutoName);
	gLuaState->set_function("SpawnEntityEx", Game_SpawnEntityEx);
	gLuaState->set_function("SpawnEntityAutoNameEx", Game_SpawnEntityAutoNameEx);
	gLuaState->set_function("GetKey", Game_GetKey);
	gLuaState->set_function("GetMouseLButtonDown", Game_GetMouseLButtonDown);
	gLuaState->set_function("GetMouseRButtonDown", Game_GetMouseRButtonDown);
	gLuaState->set_function("GetMouseLButtonUp", Game_GetMouseLButtonUp);
	gLuaState->set_function("GetMouseRButtonUp", Game_GetMouseRButtonUp);
	gLuaState->set_function("GetCursorPosX", Game_GetCursorPosX);
	gLuaState->set_function("GetCursorPosY", Game_GetCursorPosY);
	gLuaState->set_function("GetScrollOffsetX", Game_GetScrollOffsetX);
	gLuaState->set_function("GetScrollOffsetY", Game_GetScrollOffsetY);
	gLuaState->set_function("GetWindowSize", Game_GetWindowSize);
	gLuaState->set_function("BindEntityComponent", Entity_BindEntityComponent);
	gLuaState->set_function("Invalidate", RigidDynamic_Invalidate);
	gLuaState->set_function("Validate", RigidDynamic_Validate);
	gLuaState->set_function("AddForce", RigidDynamic_AddForce);
	gLuaState->set_function("AddTorque", RigidDynamic_AddTorque);
	gLuaState->set_function("SetPosition", RigidDynamic_SetPosition);
	gLuaState->set_function("GetPosition", RigidDynamic_GetPosition);
	gLuaState->set_function("GetWorldTime", Game_GetWorldTime);
	gLuaState->set_function("SetWorldTime", Game_SetWorldTime);
	gLuaState->set_function("SetDynamicTimeOfDay", Game_SetDynamicTimeOfDay);
	gLuaState->set_function("CastRayByCursorPosition", Game_CastRayByCursorPosition);
	gLuaState->set_function("GetRaycastingBlockPosition", Game_GetRaycastingBlockPosition);
	gLuaState->set_function("CreateVec2", Game_CreateVec2);
	gLuaState->set_function("CreateVec3", Game_CreateVec3);
	gLuaState->set_function("CreateVec4", Game_CreateVec4);
	gLuaState->set_function("SetGlobalVariableInt", Game_SetGlobalVariableInt);
	gLuaState->set_function("SetGlobalVariableFloat", Game_SetGlobalVariableFloat);
	gLuaState->set_function("SetGlobalVariableDouble", Game_SetGlobalVariableDouble);
	gLuaState->set_function("SetGlobalVariableString", Game_SetGlobalVariableString);
	gLuaState->set_function("GetGlobalVariableInt", Game_GetGlobalVariableInt);
	gLuaState->set_function("GetGlobalVariableFloat", Game_GetGlobalVariableFloat);
	gLuaState->set_function("GetGlobalVariableDouble", Game_GetGlobalVariableDouble);
	gLuaState->set_function("GetGlobalVariableString", Game_GetGlobalVariableString);
	gLuaState->set_function("GuiAddFace", Gui_AddFace);
	gLuaState->set_function("GuiRenderText", Gui_RenderText);
	gLuaState->set_function("GuiRenderButton", Gui_RenderButton);
	gLuaState->set_function("Exit", exit);
	this->gLuaState->open_libraries(sol::lib::base, sol::lib::table, sol::lib::math);
}

MEditor::MEditor(unsigned int OS) {
	gType = MPlatformType::EDITOR;
	this->gLuaState = new sol::state();
	this->gLuaState->open_libraries(sol::lib::base);

	CoInitialize(NULL);

	this->LoadGlobalConfig();

	gFullscreenFlag = GetPrivateProfileInt(L"Graphics", L"Fullscreen", 0, GetFullAssetPathW(L"system.ini"));
	gShadowQuality = GetPrivateProfileInt(L"Graphics", L"ShadowQuality", 0, GetFullAssetPathW(L"system.ini"));
	gFramebufferHeight = GetPrivateProfileInt(L"Graphics", L"WindowHeight", 0, GetFullAssetPathW(L"system.ini"));
	gFramebufferWidth = GetPrivateProfileInt(L"Graphics", L"WindowWidth", 0, GetFullAssetPathW(L"system.ini"));
	gAtmosphereQuality = GetPrivateProfileInt(L"Graphics", L"AtmosphereQuality", 0, GetFullAssetPathW(L"system.ini"));
	gSSDOFlag = GetPrivateProfileInt(L"Graphics", L"SSDO", 0, GetFullAssetPathW(L"system.ini"));

	std::string mainModulePath = gameConfigDoc->RootElement()->FirstChildElement()->Attribute("main_script");
	std::string windowTitle = gameConfigDoc->RootElement()->FirstChildElement()->Attribute("title");

	mMainModulePath = mainModulePath;

	switch (gAtmosphereQuality)
	{
	case 1:
		gAtmosphereFramebufferWidth = (int)gFramebufferWidth / 8;
		gAtmosphereFramebufferHeight = (int)gFramebufferHeight / 8;
		break;
	case 2:
		gAtmosphereFramebufferWidth = (int)gFramebufferWidth / 4;
		gAtmosphereFramebufferHeight = (int)gFramebufferHeight / 4;
		break;
	case 3:
		gAtmosphereFramebufferWidth = (int)gFramebufferWidth / 2;
		gAtmosphereFramebufferHeight = (int)gFramebufferHeight / 2;
		break;
	default:
		break;
	}

	glfwInit();

	int argc = 1;
	char* argv[] = { (char*)"M3DEditor.exe" };
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	if (!gladLoadGL()) {
		MessageBeep(MB_ICONERROR);
		MessageBox(NULL, L"Failed to initialize OpenGL.", L"M3D GameEngine", MB_ICONERROR);
	}

	stbi_set_flip_vertically_on_load(true);

	this->gAssetManager = new MAssetManager(this);
	this->gGameConsole = new MGameConsole(this);
	this->gGameContext = new MGameContext(this);
	this->InitializePipeline();
	this->InitializeMImGui();

	this->InitializePhysX();
	this->MakeLuaBindings();

	gLuaState->safe_script_file(GetFullAssetPathA(mMainModulePath));
	(*this->gLuaState)["Main_OnInit"]();
}

MEditor::~MEditor()
{
}

void MEditor::SetEditMode(MEditMode mode) {
	if (gCurrentContainer && gMode == MPlatformMode::SCENE) {
		reinterpret_cast<MScene*>(gCurrentContainer)->SetEditMode(mode);
	}
}

double MEditor::GetTime() {
	return glfwGetTime();
}

int MEditor::GetKey(int key) {
	return mKeyStatueTable[key];
}

int MEditor::GetKeyInGameConsole(int key) {
	return -1;
}

bool MEditor::GetMouseLeftButtonDown() {
	int n = mglfwGetMouseButton(0);
	switch (n)
	{
	case 1:
		return true;
		break;
	case 0:
		return false;
		break;
	default:
		break;
	}
	return false;
}

bool MEditor::GetMouseRightButtonDown() {
	int n = mglfwGetMouseButton(1);
	switch (n)
	{
	case 1:
		return true;
		break;
	case 0:
		return false;
		break;
	default:
		break;
	}
	return false;
}

bool MEditor::GetMouseLeftButtonUp() {
	int n = mglfwGetMouseButton(0);
	switch (n)
	{
	case 1:
		return false;
		break;
	case 0:
		return true;
		break;
	default:
		break;
	}
	return true;
}

bool MEditor::GetMouseRightButtonUp() {
	int n = mglfwGetMouseButton(1);
	switch (n)
	{
	case 1:
		return false;
		break;
	case 0:
		return true;
		break;
	default:
		break;
	}
	return true;
}

void MEditor::Update() {
	this->currentFrameTime = glfwGetTime();
	this->deltaTime = this->currentFrameTime - this->lastFrameTime;
	this->lastFrameTime = this->currentFrameTime;

	glViewport(0, 0, gFramebufferWidth, gFramebufferHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	if (gCurrentContainer) {
		gCurrentContainer->Update(this->deltaTime);
		gCurrentContainer->Render();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, gDefaultFBO);
	glDisable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	gDeferredPipeline->DoLightingPass();
	gDeferredPipeline->DoIndirectLightingPass();
	gDeferredPipeline->DoCompositionPass();

	if (!gCurrentContainer) {
		this->gGuiContext->RenderText("arial", std::string("FPS ") + std::to_string(int(this->FPS)), 10.0f, this->gFramebufferHeight - 25.0f, glm::vec3(1.0f, 1.0f, 1.0f), 0.5);
	}
	//Post Render
	if (gCurrentContainer && gMode == MPlatformMode::SCENE) {
		static MScene* scene = reinterpret_cast<MScene*>(gCurrentContainer);
		if (scene->IsEditorMode()) {
			if (scene->mEditorAgent) {
				scene->mEditorAgent->Tick();
			}
			this->gGuiContext->RenderText("arial", std::string("FPS ") + std::to_string(int(this->FPS)), 10.0f, this->gFramebufferHeight - 25.0f, glm::vec3(1.0f, 1.0f, 1.0f), 0.5);
		}
		else {
			scene->OnPostRender();
			this->gGuiContext->RenderText("arial", std::string("FPS ") + std::to_string(int(this->FPS)), 10.0f, this->gFramebufferHeight - 25.0f, glm::vec3(1.0f, 1.0f, 1.0f), 0.5);
			(*gLuaState)["Main_OnTickFinish"]();
		}
	}

	this->FPS = 1.0 / abs(this->deltaTime);

	//reset scroll offset values
	this->mScrollOffsetX = 0.0;
	this->mScrollOffsetY = 0.0;
}

void MEditor::ConsoleWriteLine(float r, float g, float b, const std::string& text)
{
}

void MEditor::SetCursorInvisible() {
	mCursorDisabled = true;
	const RAWINPUTDEVICE rid = { 0x01,0x02,0,gHWND };
	if (!RegisterRawInputDevices(&rid, 1, sizeof(rid))) {
		MessageBox(NULL, L"RegisterRawInputDevices failed.", L"M3D Editor", MB_ICONHAND);
	}
	RECT rect;
	GetClientRect(gHWND, &rect);
	ClientToScreen(gHWND, (POINT*)&rect.left);
	ClientToScreen(gHWND, (POINT*)&rect.right);
	ClipCursor(&rect);
	while (ShowCursor(FALSE) >= 0);
}

void MEditor::SetCursorVisible() {
	mCursorDisabled = false;
	const RAWINPUTDEVICE rid = { 0x01,0x02,RIDEV_REMOVE,NULL };
	if (!RegisterRawInputDevices(&rid, 1, sizeof(rid))) {
		MessageBox(NULL, L"RegisterRawInputDevices failed.", L"M3D Editor", MB_ICONHAND);
	}
	ClipCursor(NULL);
	while (ShowCursor(TRUE) <= 0);
}

void MEditor::StartGame() {
	if (gCurrentContainer && this->gMode == MPlatformMode::SCENE) {
		reinterpret_cast<MScene*>(gCurrentContainer)->StartGame();
	}
}

void MEditor::ExitGame() {
	if (gCurrentContainer && this->gMode == MPlatformMode::SCENE) {
		reinterpret_cast<MScene*>(gCurrentContainer)->ExitGame();
	}
}

void MEditor::LoadSceneWithFullPath(const std::string& path) {
	if (this->gCurrentContainer) {
		this->gCurrentContainer->Release();
		free(this->gCurrentContainer);
	}
	this->gCurrentContainer = NULL;
	this->gCurrentContainer = new MScene(true, this, path);
	assert(this->gCurrentContainer);
	this->gMode = MPlatformMode::SCENE;
	this->gGameContext->UpdateEntityTable();
	gLuaState->set("CurrentScene", (MScene*)this->gCurrentContainer);
}

void MEditor::SaveSceneWithFullPath(const std::string& path) {
	if (this->gCurrentContainer) {
		static MScene* scene = reinterpret_cast<MScene*>(gCurrentContainer);
		scene->SaveSceneWithFullPath(path.c_str());
	}
}

void MEditor::SubmitKeyboardInput(int key, int mode) {
	mKeyStatueTable[key] = mode;
}

void MEditor::SubmitCursorPosInput(double x, double y) {
	mCursorPosX = x;
	mCursorPosY = y;
}

void MEditor::SubmitMouseButtonFlags(unsigned short flags) {
	mMouseButtonFlags = flags;
}

void MEditor::SubmitMouseButtonInput(int button, int flag) {
	mMouseButtons[button] = flag;
}

void MEditor::SubmitScrollOffset(double x, double y) {
	mScrollOffsetX = x;
	mScrollOffsetY = y;
}

void MEditor::KeyCallback(int key, int scancode, int action, int mods) {

}

void MEditor::FramebufferSizeCallback(int width, int height) {
	gFramebufferWidth = width;
	gFramebufferHeight = height;
	switch (gAtmosphereQuality)
	{
	case 1:
		gAtmosphereFramebufferWidth = (int)gFramebufferWidth / 8;
		gAtmosphereFramebufferHeight = (int)gFramebufferHeight / 8;
		break;
	case 2:
		gAtmosphereFramebufferWidth = (int)gFramebufferWidth / 4;
		gAtmosphereFramebufferHeight = (int)gFramebufferHeight / 4;
		break;
	case 3:
		gAtmosphereFramebufferWidth = (int)gFramebufferWidth / 2;
		gAtmosphereFramebufferHeight = (int)gFramebufferHeight / 2;
		break;
	default:
		break;
	}
	gForwardPipeline->UpdateFramebufferSize();
	gDeferredPipeline->UpdateFramebufferSize();
	gAtomspherePipeline->UpdateFramebufferSize();
}

void MEditor::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
}

void MEditor::CreateRigidStatic(const std::string& model) {
	if (gCurrentContainer) {
		static MScene* scene = reinterpret_cast<MScene*>(gCurrentContainer);
		scene->mEditorAgent->CreateRigidStatic(model);
	}
}

void MEditor::CreateEntity(const std::string& def) {
	if (gCurrentContainer) {
		static MScene* scene = reinterpret_cast<MScene*>(gCurrentContainer);
		scene->mEditorAgent->CreateEntity(def);
	}
}

int MEditor::mglfwGetMouseButton(int button) {
	return mMouseButtons[button];
}
