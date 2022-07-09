#include "pch.h"
#include "Platforms.h"
#include "ConsoleOperations.h"
#include "ProblemCodes.h"

MOfflineGame::MOfflineGame(unsigned int OS) {
	this->ReadAmbientConfig();
	//获取配置信息
	const char *fullscreenOrigin = gameConfigDoc->RootElement()->FirstChildElement()->Attribute("fullscreen");
	const char* heightOrigin = gameConfigDoc->RootElement()->FirstChildElement()->Attribute("height");
	const char* widthOrigin = gameConfigDoc->RootElement()->FirstChildElement()->Attribute("width");
	this->windowTitle = gameConfigDoc->RootElement()->FirstChildElement()->Attribute("title");
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
	//初始化SDL和OpenGL上下文
	SDL_Init(SDL_INIT_EVERYTHING);
	this->window = SDL_CreateWindow(this->windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->windowWidth, this->windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	this->GLContext = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(-1);
	//获取OpenGL函数指针
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		MessageBeep(MB_ICONERROR);
		MessageBox(NULL, L"Failed to initialize OpenGL.", L"M3D GameEngine", MB_ICONERROR);
		SDL_GL_DeleteContext(this->GLContext);
		SDL_DestroyWindow(this->window);
		SDL_Quit();
	}
	//SDL事件
}


MOfflineGame::~MOfflineGame() {
	delete[]this->gameConfigDoc;
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}

void MOfflineGame::Tick() {
	SDL_PollEvent(&this->currentSDLEvent);
	//
	if (this->currentSDLEvent.type == SDL_QUIT) {
		this->shouldExit = true;
	}
	//
	glViewport(0, 0, windowWidth, windowHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(this->window);
}

void MBasicPlatform::ReadAmbientConfig() {
	this->gameConfigDoc = new tinyxml2::XMLDocument();
	if (!this->gameConfigDoc->LoadFile(".\\..\\..\\Game\\Config\\ambient.xml") == tinyxml2::XML_SUCCESS) {
		MessageBeep(MB_ICONERROR);
		MessageBox(NULL, L"Failed to read document Game\\Config\\ambient.xml.", L"M3D GameEngine", MB_ICONERROR);
	}
}
