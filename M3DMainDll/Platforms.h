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
	SDL_Event currentSDLEvent;
	bool shouldExit = false;
	tinyxml2::XMLDocument *gameConfigDoc;
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
	std::string windowTitle;
	SDL_Window *window;
	SDL_GLContext GLContext;
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