#pragma once
#include <string>
#include "Class.h"

#include <PxPhysicsAPI.h>

class MScriptableObject;
class MScene;
class MActorEditorAgent;

class MActor :public MScriptableObject {
public:
	std::string name;
	MScene* mParent = NULL;
	MActorEditorAgent* mEditorAgent = NULL;
	virtual void Save() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void RenderForDepthMapping() = 0;
protected:
};