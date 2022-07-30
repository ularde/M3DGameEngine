#pragma once
#include <vector>
#include <atomic>
#include <SDL.h>

class MBasicPlatform {
public:
	//MBasicPlatform(){}
	//~MBasicPlatform(){}
	virtual void Tick(){}
	bool GetWhetherShouldExit() { return this->shouldExit; }
protected:
	bool shouldExit = false;
	tinyxml2::XMLDocument* gameConfigDoc = NULL;
protected:
	//∂¡»°≈‰÷√
	void ReadAmbientConfig();
};

class MOfflineGame :public MBasicPlatform {
public:
	MOfflineGame(unsigned int OS);
	~MOfflineGame();
	void Tick();
private:
	void InitTestInfoFonts();
	void DisplayTestInfo();
	FTFont* infoFont_EN = NULL;
	FTFont* infoFont_CN = NULL;
	FTFont* infoFont_UY = NULL;

	GLFWwindow* window;
	bool fullscreenFlag = false;
	int windowWidth = 800;
	int windowHeight = 600;
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