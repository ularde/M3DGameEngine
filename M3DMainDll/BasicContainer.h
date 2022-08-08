#pragma once
#include <vector>
#include <string>
#include "Class.h"

class MBasicContainer :public MPythonBoundObject {
public:
	virtual void Tick() {}
	bool GetWhetherShouldQuit() { return shouldQuit; }
protected:
	bool shouldQuit = false;
};