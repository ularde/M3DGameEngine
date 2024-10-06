#pragma once
#include <vector>
#include <string>
#include "Class.h"
#include "tinyxml2.h"

class MScriptableObject;

class MBasicContainer :public MScriptableObject {
public:
	std::string path;

	virtual void MouseCallback(GLFWwindow* window, double xpos, double ypos) = 0;
	virtual void Update(double delta_time) = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;

	bool GetWhetherShouldQuit() { return shouldQuit; }
protected:
	bool shouldQuit = false;
};