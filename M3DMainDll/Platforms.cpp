#include "pch.h"
#include "Platforms.h"
#include "ConsoleOperations.h"
#include "ProblemCodes.h"

MOfflineGame::MOfflineGame(unsigned int OS) {
	this->ReadAmbientConfig();
	//获取配置信息
	const char* fullscreenOrigin = gameConfigDoc->RootElement()->FirstChildElement()->Attribute("fullscreen");
	const char* heightOrigin = gameConfigDoc->RootElement()->FirstChildElement()->Attribute("height");
	const char* widthOrigin = gameConfigDoc->RootElement()->FirstChildElement()->Attribute("width");
	const char* mainModulePath = gameConfigDoc->RootElement()->FirstChildElement()->Attribute("main_script");
	const char* windowTitle = gameConfigDoc->RootElement()->FirstChildElement()->Attribute("title");
	if (widthOrigin && heightOrigin) {
		if (!sscanf(widthOrigin, "%d", &this->windowWidth) || !sscanf(heightOrigin, "%d", &this->windowHeight)) {
			M3DConsole_PrintWarning("MainDll Configuration", M3D_CFG_BADATTRIBUTION);
		}
	}
	if (fullscreenOrigin) {
		if (strcmp("0", fullscreenOrigin) == NULL) {
			this->fullscreenFlag = false;
		}
	}
	else {
		M3DConsole_PrintWarning("MainDll Configuration", M3D_CFG_BADATTRIBUTION);
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
	//初始化GLFW和OpenGL上下文
	if (!glfwInit()) {

	}
	this->window = glfwCreateWindow(this->windowWidth, this->windowHeight, windowTitle, NULL, NULL);
	glfwMakeContextCurrent(this->window);
	glfwSwapInterval(-1);
	//获取OpenGL函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		MessageBeep(MB_ICONERROR);
		MessageBox(NULL, L"Failed to initialize OpenGL.", L"M3D GameEngine", MB_ICONERROR);
		glfwDestroyWindow(this->window);
		glfwTerminate();
	}
	this->InitTestInfoFonts();
}


MOfflineGame::~MOfflineGame() {
	delete[]this->gameConfigDoc;
	//delete[]this->mainModule;
	glfwDestroyWindow(this->window);
	glfwTerminate();
}

void MOfflineGame::Tick() {
	this->shouldExit = glfwWindowShouldClose(this->window);
	glfwGetWindowSize(this->window, &(this->windowWidth), &(this->windowHeight));
	//this->mainModule->RunModule();
	glViewport(0, 0, windowWidth, windowHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	this->DisplayTestInfo();
	glfwSwapBuffers(this->window);
	glfwPollEvents();
}

void MOfflineGame::InitTestInfoFonts() {
	this->infoFont_EN = new FTGLPixmapFont(".\\..\\..\\Game\\Fonts\\arial.ttf");
	this->infoFont_CN = new FTGLPixmapFont(".\\..\\..\\Game\\Fonts\\Deng.ttf");
	this->infoFont_UY = new FTGLPixmapFont(".\\..\\..\\Game\\Fonts\\Microsoft-Uighur.ttf");
	this->infoFont_EN->FaceSize(20);
	this->infoFont_CN->FaceSize(20);
	this->infoFont_UY->FaceSize(20);
}

void MOfflineGame::DisplayTestInfo() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, this->windowWidth, this->windowHeight, 0.0, -1.0, 1.0);
	glRasterPos2d(100.0, 100.0);
	this->infoFont_EN->Render(L"English M3D Game Engine An open source game engine");
	glRasterPos2d(100.0, 70.0);
	this->infoFont_CN->Render(L"简体中文 M3D Game Engine 一个开源游戏引擎");
	glRasterPos2d(100.0, 40.0);
	this->infoFont_UY->Render(L"ئۇيغۇر");
}

void MBasicPlatform::ReadAmbientConfig() {
	this->gameConfigDoc = new tinyxml2::XMLDocument();
	if (!this->gameConfigDoc->LoadFile(".\\..\\..\\Game\\Config\\ambient.xml") == tinyxml2::XML_SUCCESS) {
#ifdef _DEBUG
		this->gameConfigDoc->LoadFile("D:\\Repos\\M3DGameEngine\\Game\\Config\\ambient.xml");
#else
		MessageBeep(MB_ICONERROR);
		MessageBox(NULL, L"Failed to read document Game\\Config\\ambient.xml.", L"M3D GameEngine", MB_ICONERROR);
#endif
	}
}
