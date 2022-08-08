#include "pch.h"
#include "GameConsole.h"
#include "Platforms.h"
#include "SharedFunc.h"

MGameConsole::MGameConsole(MBasicPlatform* platform_) {
	this->platform = platform_;
	this->consoleFont = new FTPixmapFont("C:\\WINDOWS\\Fonts\\arial.ttf");
	this->consoleFont->FaceSize(15);
}

MGameConsole::~MGameConsole() {
	//delete[]this->consoleFont;
	//this->consoleFont = nullptr;
}

void MGameConsole::SetPlatform(MBasicPlatform* platform_) {
	this->platform = platform_;
}

void MGameConsole::Update() {
	this->FPSTextBuffer = L"FPS : ";
	this->FPSTextBuffer += std::to_wstring(this->platform->GetFPS());
	if ((!this->showGameConsole) && this->platform->GetKey('`') == GLFW_PRESS) {
		this->showGameConsole = true;
	}
	else if (this->platform->GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		this->showGameConsole = false;
	}
	if (this->consoleTextLines.size() >= 14u) {
		this->ConsolePageDown();
	}
}

void MGameConsole::Render() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)this->platform->GetWindowWidth(), (GLdouble)this->platform->GetWindowHeight(), 0.0, -1.0, 1.0);
	if (this->showGameConsole) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glColor4f(0.0f, 1.0f, 1.0f, 0.4f);
		glBegin(GL_QUADS);
		glVertex2d(10.0, 30.0);
		glVertex2d(10.0, 250.0);
		glVertex2d(this->platform->GetWindowWidth() - 10.0, 250.0);
		glVertex2d(this->platform->GetWindowWidth() - 10.0, 30.0);
		glEnd();
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos2d(10.0, 250.0);
		this->consoleFont->Render(this->commandBuffer.c_str());
		//打印控制台文字（未添加滚动条功能） 理论最多14行
		for (unsigned int i = 0; i < this->consoleTextLines.size(); ++i) {
			glColor3f(this->consoleTextLines[i]->r, this->consoleTextLines[i]->g, this->consoleTextLines[i]->b);
			glRasterPos2d(10.0, 40.0 + ((double)i * 15.0));
			this->consoleFont->Render(this->consoleTextLines[i]->text.c_str());
		}
		//绘制提示符
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glVertex2d(this->graphicCursorPosX, 250.0);
		glVertex2d(this->graphicCursorPosX, 247.0);
		glVertex2d(this->graphicCursorPosX + 10.0, 247.0);
		glVertex2d(this->graphicCursorPosX + 10.0, 250.0);
		glEnd();
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2d(10.0, 20.0);
	this->consoleFont->Render(this->FPSTextBuffer.c_str());
	glRasterPos2d(0.0, 0.0);
}

void MGameConsole::ExecuteCommand(const std::wstring& command) {
	wchar_t* value_wcs = (wchar_t*)malloc(M3D_MAX_GAMECONSOLE_VALUE_LENGTH * sizeof(wchar_t));
	char* value_mbs = (char*)malloc(M3D_MAX_GAMECONSOLE_VALUE_LENGTH * sizeof(char));

	if (lstrcmp(command.c_str(), L".console.cls") == 0) {
		this->ConsoleClearScreen();
	}
	else if (lstrcmp(command.c_str(), L".console.help") == 0) {
		this->ConsoleDisplayHelp();
	}
	else if (lstrcmp(command.c_str(), L".window.fullscr") == 0) {
		this->platform->fullscreenFlag = true;
	}
	else if (swscanf(command.c_str(), L".engine.load_scene %s", value_wcs)) {
		if (value_wcs != 0) {
			this->WriteLine(1.0f, 1.0f, 1.0f, L"[INFO] GameConsole : Engine will load scene " + std::wstring(value_wcs));
		}
	}
	else {
		char* pycmd_buf = (char*)malloc(M3D_MAX_GAMECONSOLE_PYCMD_LENGTH * sizeof(char));
		wcstombs(pycmd_buf, command.c_str(), M3D_MAX_GAMECONSOLE_PYCMD_LENGTH);
		PyRun_SimpleString(pycmd_buf);
		free(pycmd_buf);
	}

	free(value_wcs);
	free(value_mbs);
}

void MGameConsole::WriteLine(float r, float g, float b, std::wstring text) {
	this->consoleTextLines.push_back(new MGameConsoleTextLine(r, g, b, text));
}

void MGameConsole::CaptureTextInput(unsigned int key) {
	if (this->showGameConsole) {
		this->MoveCursorRight();
		this->commandBuffer.insert(this->commandCursorPos - 1, { (wchar_t)key });
	}
}

void MGameConsole::CaptureControlKeyInput(int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (key == GLFW_KEY_BACKSPACE) {
			if (this->commandBuffer.size() != 0) {
				this->MoveCursorLeft();
				this->commandBuffer.erase((size_t)this->commandCursorPos, 1);
			}
		}
		else if (key == GLFW_KEY_DELETE) {
			if (this->commandBuffer.size() != 0 && this->commandCursorPos < this->commandBuffer.size()) {
				this->commandBuffer.erase((size_t)this->commandCursorPos + 1, 1);
			}
		}
		else if (key == GLFW_KEY_RIGHT) {
			this->MoveCursorRight();
		}
		else if (key == GLFW_KEY_LEFT) {
			this->MoveCursorLeft();
		}
		else if (key == GLFW_KEY_ENTER) {
			this->WriteLine(1.0f, 1.0f, 1.0f, L">" + this->commandBuffer);
			this->ExecuteCommand(this->commandBuffer);
			//
			this->commandBuffer.clear();
			this->commandBuffer.resize(0);
			this->ResetCursorLocation();
		}
	}
}

void MGameConsole::ConsolePageDown()
{
}

void MGameConsole::ConsolePageUp()
{
}

void MGameConsole::ConsoleClearScreen() {
	this->consoleTextLines.clear();
	this->consoleTextLines.resize(0);
}

void MGameConsole::ConsoleDisplayHelp() {
	this->WriteLine(1.0f, 1.0f, 1.0f, L"M3DEngine Console Help :");
	this->WriteLine(1.0f, 1.0f, 1.0f, L"Usage : Enter Python command that you'd like to execute and then press ENTER to expect its execution.");
	this->WriteLine(1.0f, 1.0f, 1.0f, L"Note : Following commands wont't be recognized as Python commands because they have their own functions :");
	this->WriteLine(1.0f, 1.0f, 0.0f, L".console.cls : Clear all text in the console.     .console.help : Display this help information in the console.");
	this->WriteLine(1.0f, 1.0f, 0.0f, L".engine.load_scene <name> : Load a scene.     .engine.load_form <name> : Load a form.");
	this->WriteLine(1.0f, 1.0f, 0.0f, L".window.fullscr : Enable fullscreen mode.");
}

void MGameConsole::MoveCursorLeft() {
	if (this->commandCursorPos != 0) {
		this->commandCursorPos--;
		this->graphicCursorPosX -= 10.0;
	}
}

void MGameConsole::MoveCursorRight() {
	if (this->commandCursorPos <= this->commandBuffer.size()) {
		this->commandCursorPos++;
		this->graphicCursorPosX += 10.0;
	}
}

void MGameConsole::ResetCursorLocation() {
	this->commandCursorPos = 0;
	this->graphicCursorPosX = 10.0;
}