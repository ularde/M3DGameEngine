#include "pch.h"
#include "GameConsole.h"
#include "Platforms.h"
#include "SharedFunc.h"
#include "BasicContainer.h"

MGameConsole::MGameConsole(MBasicPlatform* platform_) {
	this->gPlatform = platform_;
}

MGameConsole::~MGameConsole() {
	//delete[]this->consoleFont;
	//this->consoleFont = nullptr;
}

void MGameConsole::SetPlatform(MBasicPlatform* platform_) {
	this->gPlatform = platform_;
}

void MGameConsole::Update() {
	if ((!this->showGameConsole) && this->gPlatform->GetKey('`') == GLFW_PRESS) {
		this->showGameConsole = true;
	}
	else if (this->gPlatform->GetKeyInGameConsole(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		this->showGameConsole = false;
	}
}

void MGameConsole::Render() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	if (this->showGameConsole) {
		if (ImGui::Begin("Console")) {
			ImGui::Text("Output");
			conWinSize = ImGui::GetWindowSize();
			if (ImGui::BeginListBox("##0", ImVec2(conWinSize.x - 5.0f, conWinSize.y - 100.0f))) {
				for (unsigned int i = 0; i < this->consoleTextLines.size(); i++) {
					ImGui::TextWrapped(consoleTextLines[i]->text.c_str());
				}
				ImGui::EndListBox();
				static char buffer[M3D_MAX_COMMON_STRING_LENGTH] = { 0 };
				ImGui::InputText("##", buffer, M3D_MAX_COMMON_STRING_LENGTH);
				ImGui::SameLine();
				if (ImGui::Button("EXECUTE")) {
					ExecuteCommand(buffer);
				}
			}
		}
		ImGui::End();
	}
}

void MGameConsole::ExecuteCommand(const std::string & command) {
	char* value = (char*)malloc(M3D_MAX_GAMECONSOLE_VALUE_LENGTH * sizeof(char));

	if (strcmp(command.c_str(), ".console.cls") == 0) {
		this->ConsoleClearScreen();
	}
	else if (strcmp(command.c_str(), ".console.help") == 0) {
		this->ConsoleDisplayHelp();
	}
	else if (strcmp(command.c_str(), ".window.fullscr") == 0) {
		const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		this->gPlatform->gFramebufferWidth = vidmode->width;
		this->gPlatform->gFramebufferHeight = vidmode->height;
		this->gPlatform->gFullscreenFlag = true;
	}
	else if (sscanf(command.c_str(), ".engine.load_scene %s", value) == 1) {
		if (value != 0) {
			this->WriteLine(1.0f, 1.0f, 1.0f, "[INFO] GameConsole : Engine will load scene " + std::string(value));
			this->gPlatform->LoadScene(value);
		}
	}
	else {
		//Execute in virtual machine
		gPlatform->gLuaState->safe_script(command);
	}

	free(value);
}

void MGameConsole::WriteLine(float r, float g, float b, const std::string& text) {
	this->consoleTextLines.push_back(new MGameConsoleTextLine(r, g, b, text));
}

void MGameConsole::CaptureTextInput(unsigned int key) {
	if (this->showGameConsole) {
		this->MoveCursorRight();
		this->commandBuffer.insert(this->commandCursorPos - 1, { (char)key });
	}
}

void MGameConsole::CaptureControlKeyInput(int key, int scancode, int action, int mods) {

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
	this->WriteLine(1.0f, 1.0f, 1.0f, "M3DEngine Console Help :");
	this->WriteLine(1.0f, 1.0f, 1.0f, "Usage : Enter Lua command that you'd like to execute and then press ENTER to expect its execution.");
	this->WriteLine(1.0f, 1.0f, 1.0f, "Note : Following commands wont't be recognized as Lua commands because they have their own functions :");
	this->WriteLine(1.0f, 1.0f, 0.0f, ".console.cls : Clear all text in the console.");
	this->WriteLine(1.0f, 1.0f, 0.0f, ".console.help : Display this help information in the console."); 
	this->WriteLine(1.0f, 1.0f, 0.0f, ".engine.load_scene <name> : Load a scene.");
	this->WriteLine(1.0f, 1.0f, 0.0f, ".engine.load_form <name> : Load a form.");
	this->WriteLine(1.0f, 1.0f, 0.0f, ".window.fullscr : Enable fullscreen mode.");
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