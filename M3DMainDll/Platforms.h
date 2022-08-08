#pragma once
#include <vector>
#include <atomic>
#include "GameConsole.h"
#include "BackgroundQuad.h"
#include "Class.h"
#include "Camera.h"
#include "Test.h"
#include "AssetManager.h"

class MFreeCamera;
class MGameConsole;
class MBackgroundQuad;
class MVertexShaderProgram;
class MFragmentShaderProgram;
class MAssetManager;

void OfflineGame_CharCallback(GLFWwindow* window, unsigned int codepoint);
void OfflineGame_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void OfflineGame_WindowSizeCallback(GLFWwindow* window, int width, int height);
void OfflineGame_CursorPosCallback(GLFWwindow* window, double xpos, double ypos);

class MBasicPlatform {
public:
	MAssetManager* assetManager = NULL;
	bool fullscreenFlag = false;
	int windowWidth = 800;
	int windowHeight = 600;

	virtual void Tick() = 0;
	virtual int GetWindowWidth() = 0;
	virtual int GetWindowHeight() = 0;
	double GetFPS() { return this->FPS; }
	double GetDeltaTime() { return this->deltaTime; }
	virtual double GetTime() = 0;
	virtual int GetKey(int key) = 0;
	virtual void ConsoleWriteLine(float r, float g, float b, std::wstring text) = 0;
	virtual void SetCursorInvisible() = 0;
	bool GetWhetherShouldExit() { return this->shouldExit; }
	double GetCursorPosX() { return this->cursorPos_x; }
	double GetCursorPosY() { return this->cursorPos_y; }
	//
	MFreeCamera* camera = NULL;
	//

	GLuint pipelineID = 0;
	float mouseSensitivity = 0.1f;
protected:
	double lastFrameTime = 0.0, currentFrameTime = 0.0;
	double FPS = 0.0;
	double deltaTime = 0.0;
	bool shouldExit = false;
	double cursorPos_x, cursorPos_y;

	tinyxml2::XMLDocument* gameConfigDoc = NULL;
	MBackgroundQuad* backgroundQuad = NULL;
	MGameConsole* gameConsole = NULL;

	void InitializePipeline();
	void ReadGlobalConfig();
};

class MOfflineGame :public MBasicPlatform {
public:
	MOfflineGame(unsigned int OS);
	~MOfflineGame();
	int GetWindowWidth() { return this->windowWidth; }
	int GetWindowHeight() { return this->windowHeight; }
	double GetTime() { return glfwGetTime(); }
	int GetKey(int key);
	void Tick();
	void ConsoleWriteLine(float r, float g, float b, std::wstring text);
	void CharCallback(unsigned int codepoint);
	void KeyCallback(int key, int scancode, int action, int mods);
	void WindowSizeCallback(int width, int height);
	void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	void SetCursorInvisible();
private:
	GLFWwindow* window;
	void SplashScreen();
protected:
};

class MEditor :public MOfflineGame {
public:
	MEditor(unsigned int OS);
	~MEditor();
	void Tick();
private:
protected:
};