#pragma once
#include "BasicContainer.h"
#include "UIActors.h"

class MUIForm :public MBasicContainer {
public:
	MUIForm(const char* xmlPath);
	~MUIForm();
	void Tick();
protected:
	tinyxml2::XMLDocument* xmlDoc = NULL;
	std::vector<MUIActor> formActors;
};