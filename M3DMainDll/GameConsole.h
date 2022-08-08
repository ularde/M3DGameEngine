#pragma once
#include <string>
#include <map>
#include <FTGL/ftgl.h>
#include "glad.h"
#include "Platforms.h"
#include "ShaderManager.h"

class MBasicPlatform;
class MVertexShaderProgram;
class MFragmentShaderProgram;

struct MGameConsoleTextLine {
	MGameConsoleTextLine(float r__, float g__, float b__, std::wstring text__) { this->text = text__; this->r = r__; this->g = g__; this->b = b__; }
	std::wstring text;
	float r, g, b;
};

class MGameConsole {
public:
	MGameConsole(MBasicPlatform* platform_);
	~MGameConsole();
	void SetPlatform(MBasicPlatform* platform_);
	void Update();
	void Render();
	void ExecuteCommand(const std::wstring& command);
	void WriteLine(float r, float g, float b, std::wstring text);

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
	MBasicPlatform* platform = NULL;
	FTFont* consoleFont = NULL;
	std::wstring FPSTextBuffer;
	std::wstring commandBuffer;
	std::vector<MGameConsoleTextLine*> consoleTextLines;

	void MoveCursorLeft();
	void MoveCursorRight();
	void ResetCursorLocation();
protected:
};