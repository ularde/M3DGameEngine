#pragma once
#include <vector>
#include <string>
#include "Class.h"

class MBasicContainer :public MClass {
public:
	virtual void Tick() {}
	bool GetWhetherShouldQuit() { return shouldQuit; }
protected:
	bool shouldQuit = false;
};