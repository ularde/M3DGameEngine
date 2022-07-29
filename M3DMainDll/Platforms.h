#pragma once
#include <vector>
#include <atomic>
#include <SDL.h>
#include <M3DS/M3DS.h>

class MBasicPlatform {
public:
	//MBasicPlatform(){}
	//~MBasicPlatform(){}
	virtual void Tick(){}
	bool GetWhetherShouldExit() { return this->shouldExit; }
protected:
	bool shouldExit = false;
	tinyxml2::XMLDocument* gameConfigDoc = NULL;
	M3DS_Module* mainModule = NULL;
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