#pragma once
#include "Actor.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

class MCamera;
class MActor;
class MScene;
class MCameraComponent;
class MTimeOfDay;

class MAtmosphere :public MActor {
public:
	MAtmosphere(MScene* parent, const std::string& name_);
	~MAtmosphere();
	void SetTimeOfDayObject(MTimeOfDay* tod);
	virtual void Save()override{}
	virtual void InitializeLuaInstance()override;
	virtual void Update(double dt)override;
	void UpdateParameters();
protected:
	MTimeOfDay* mTimeOfDay = NULL;
	MCameraComponent* gLocalCamera = NULL;
	void RenderSphere();
};