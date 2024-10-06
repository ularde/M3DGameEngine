#pragma once
#include <string>
#include <map>
#include <FTGL/ftgl.h>
#include "glad.h"
#include "Platforms.h"
#include "ShaderManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class MBasicPlatform;
class MVertexShaderProgram;
class MFragmentShaderProgram;

struct MGameConsoleTextLine {
	MGameConsoleTextLine(float r__, float g__, float b__, std::string text__) { this->text = text__; this->r = r__; this->g = g__; this->b = b__; }
	std::string text;
	float r, g, b;
};

class MGameConsole {
public:
	MGameConsole(MBasicPlatform* platform_);
	~MGameConsole();
	bool GetConsoleVisibility() { return showGameConsole; }

	void SetPlatform(MBasicPlatform* platform_);
	void Update();
	void Render();
	void ExecuteCommand(const std::string & command);
	void WriteLine(float r, float g, float b, const std::string& text);

	void CaptureTextInput(unsigned int key);
	void CaptureControlKeyInput(int key, int scancode, int action, int mods);

	void ConsolePageDown();
	void ConsolePageUp();
	void ConsoleClearScreen();
	void ConsoleDisplayHelp();
private:
	double graphicCursorPosX = 10.0;
	unsigned int commandCursorPos = 0;

	bool showGameConsole = false;

	ImVec2 conWinSize;
	MBasicPlatform* gPlatform = NULL;

	std::string commandBuffer;
	std::vector<MGameConsoleTextLine*> consoleTextLines;

	void MoveCursorLeft();
	void MoveCursorRight();
	void ResetCursorLocation();
protected:
};