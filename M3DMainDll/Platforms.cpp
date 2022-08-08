#include "pch.h"
#include "Platforms.h"
#include "ConsoleOperations.h"
#include "SharedFunc.h"
#include "ShaderManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

MOfflineGame::MOfflineGame(unsigned int OS) {
	Py_Initialize();
	CoInitialize(NULL);

	this->ReadGlobalConfig();
	//获取配置信息
	const char* fullscreenOrigin = gameConfigDoc->RootElement()->FirstChildElement()->Attribute("fullscreen");
	const char* heightOrigin = gameConfigDoc->RootElement()->FirstChildElement()->Attribute("height");
	const char* widthOrigin = gameConfigDoc->RootElement()->FirstChildElement()->Attribute("width");
	const char* mainModulePath = gameConfigDoc->RootElement()->FirstChildElement()->Attribute("main_script");
	const char* windowTitle = gameConfigDoc->RootElement()->FirstChildElement()->Attribute("title");
	if (widthOrigin && heightOrigin) {
		if (!sscanf(widthOrigin, "%d", &this->windowWidth) || !sscanf(heightOrigin, "%d", &this->windowHeight)) {
			this->gameConsole->WriteLine(1.0f, 1.0f, 0, L"[WARNING] MainDll : Check content of \\Game\\Config\\ambient.xml.");
		}
	}
	if (fullscreenOrigin) {
		if (strcmp("0", fullscreenOrigin) == NULL) {
			this->fullscreenFlag = false;
		}
		else {
			this->fullscreenFlag = true;
		}
	}
	else {
		this->gameConsole->WriteLine(1.0f, 0.0f, 0.0f, L"[ERROR] MainDll : Bad attribution(s). Check content of \\Game\\Config\\ambient.xml.");
	}
	if (mainModulePath) {
		//this->mainModule = new M3DS_Module();
		//if (!this->mainModule->LoadModule(mainModulePath)) {
		//	M3DConsole_PrintError(mainModulePath, M3D_M3DS_BADMAINMODULE);
		//}
	}
	else {
		//this->mainModule = new M3DS_Module();
		M3DConsole_PrintError(mainModulePath, M3D_M3DS_BADMAINMODULE);
	}
	
	glfwInit();
	this->window = glfwCreateWindow(this->windowWidth, this->windowHeight, windowTitle, NULL, NULL);
	glfwMakeContextCurrent(this->window);
	glfwSwapInterval(-1);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		MessageBeep(MB_ICONERROR);
		MessageBox(NULL, L"Failed to initialize OpenGL.", L"M3D GameEngine", MB_ICONERROR);
		glfwDestroyWindow(this->window);
		glfwTerminate();
	}

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwSetWindowUserPointer(this->window, reinterpret_cast<void*>(this));
	glfwSetCharCallback(this->window, OfflineGame_CharCallback);
	glfwSetKeyCallback(this->window, OfflineGame_KeyCallback);
	glfwSetWindowSizeCallback(this->window, OfflineGame_WindowSizeCallback);
	glfwSetCursorPosCallback(this->window, OfflineGame_CursorPosCallback);

	//闪屏显示LOGO
	this->SplashScreen();

	this->assetManager = new MAssetManager(this);
	this->gameConsole = new MGameConsole(this);
	this->InitializePipeline();
	this->backgroundQuad = new MBackgroundQuad(this);
	InitTest(this);
}


MOfflineGame::~MOfflineGame() {
	delete[]this->assetManager;
	delete[]this->gameConfigDoc;
	delete[]this->gameConsole;
	delete[]this->backgroundQuad;
	this->gameConsole = nullptr;
	this->gameConfigDoc = nullptr;
	this->backgroundQuad = nullptr;
	glfwDestroyWindow(this->window);
	glfwTerminate();
	Py_Finalize();
	CoUninitialize();
}

int MOfflineGame::GetKey(int key) {
	return glfwGetKey(this->window, key);
}

void MOfflineGame::Tick() {
	this->currentFrameTime = glfwGetTime();
	this->deltaTime = this->currentFrameTime - this->lastFrameTime;
	this->lastFrameTime = this->currentFrameTime;

	if (this->fullscreenFlag) {
		glfwSetWindowMonitor(this->window, glfwGetPrimaryMonitor(), 0, 0, this->windowWidth, this->windowHeight, GLFW_DONT_CARE);
	}
	this->shouldExit = glfwWindowShouldClose(this->window);
	glfwGetCursorPos(this->window, &this->cursorPos_x, &this->cursorPos_y);
	glfwGetWindowSize(this->window, &(this->windowWidth), &(this->windowHeight));

	glViewport(0, 0, windowWidth, windowHeight);

	this->backgroundQuad->BeginRender();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RunTest(this);
	this->backgroundQuad->EndRender();

	this->backgroundQuad->RenderQuad();
	this->gameConsole->Update();
	this->gameConsole->Render();

	this->FPS = 1.0 / abs(this->deltaTime);

	glfwSwapBuffers(this->window);
	glfwPollEvents();
}

void MOfflineGame::ConsoleWriteLine(float r, float g, float b, std::wstring text) {
	this->gameConsole->WriteLine(r, g, b, text);
}

void MOfflineGame::CharCallback(unsigned int codepoint) {
	this->gameConsole->CaptureTextInput(codepoint);
}

void MOfflineGame::KeyCallback(int key, int scancode, int action, int mods) {
	this->gameConsole->CaptureControlKeyInput(key, scancode, action, mods);
}

void MOfflineGame::WindowSizeCallback(int width, int height) {
	this->backgroundQuad->UpdateWindowSize(width, height);
}

void MOfflineGame::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	camera->Update(xpos, ypos);
}

void MOfflineGame::SetCursorInvisible() {
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


void OfflineGame_CharCallback(GLFWwindow* window, unsigned int codepoint) {
	MOfflineGame* instance = reinterpret_cast<MOfflineGame*>(glfwGetWindowUserPointer(window));
	instance->CharCallback(codepoint);
}

void OfflineGame_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	MOfflineGame* instance = reinterpret_cast<MOfflineGame*>(glfwGetWindowUserPointer(window));
	instance->KeyCallback(key, scancode, action, mods);
}

void OfflineGame_WindowSizeCallback(GLFWwindow* window, int width, int height) {
	MOfflineGame* instance = reinterpret_cast<MOfflineGame*>(glfwGetWindowUserPointer(window));
	instance->WindowSizeCallback(width, height);
}

void OfflineGame_CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	MOfflineGame* instance = reinterpret_cast<MOfflineGame*>(glfwGetWindowUserPointer(window));
	instance->CursorPosCallback(window, xpos, ypos);
}

void MOfflineGame::SplashScreen() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	int imgWidth, imgHeight, imgChannelCount;
	//设置加载时翻转图片（全局）
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imgData = stbi_load(GetFullAssetPathA("Textures\\SplashScreen.bmp"), &imgWidth, &imgHeight, &imgChannelCount, 0);
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
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
	for (int i = 0; i < 200; ++i) {
		glColor3f((float)i / 200.0f, (float)i / 200.0f, (float)i / 200.0f);
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
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void MBasicPlatform::InitializePipeline() {
	glGenProgramPipelines(1, &this->pipelineID);
}

void MBasicPlatform::ReadGlobalConfig() {
	this->gameConfigDoc = new tinyxml2::XMLDocument();
	if (!this->gameConfigDoc->LoadFile(".\\..\\..\\Game\\Config\\global.xml") == tinyxml2::XML_SUCCESS) {
#ifdef _DEBUG
		this->gameConfigDoc->LoadFile("D:\\Repos\\M3DGameEngine\\Game\\Config\\global.xml");
#else
		MessageBeep(MB_ICONERROR);
		MessageBox(NULL, L"Failed to read document Game\\Config\\global.xml.", L"M3D GameEngine", MB_ICONERROR);
#endif
	}
}
