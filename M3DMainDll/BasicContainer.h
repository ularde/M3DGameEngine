#pragma once
#include <vector>
#include <string>

class MBasicContainer {
public:
	virtual void Tick() {}
	bool GetWhetherShouldQuit() { return shouldQuit; }
protected:
	bool shouldQuit = false;
};