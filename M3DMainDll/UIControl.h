#pragma once
#include "Class.h"
#include "UIForm.h"

class MScriptableObject;
class MUIForm;

class MUIControl :public MScriptableObject {
public:
	MUIForm* mParent = NULL;
	std::string name;
	virtual void Save() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
protected:
};