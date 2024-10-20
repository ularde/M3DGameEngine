#pragma once
#include "Actor.h"
#include <string>

class MScene;
class MActor;
class MCamera;
class MTimeOfDay;

class MPointLight :public MActor {
public:
	MPointLight(MScene* scene_, const std::string& name_, const std::string& pymodule_path, 
		const glm::vec3& position, const glm::vec3& color,
		float linear_, float quadratic_, bool cast_shadow);
	~MPointLight();
	glm::vec3 mPosition, mColor;
	float linear = 0.0f, quadratic = 0.0f;
	virtual void Save()override;
	virtual void InitializeLuaInstance()override;
	virtual void Update(double delta_time)override;
	unsigned int GetLightID() { return this->lightID; }
public:
	void SetPosition(double x, double y, double z) { mPosition.x = x; mPosition.y = y; mPosition.z = z; }
	void SetColor(double r, double g, double b) { mColor.x = r; mColor.y = g; mColor.z = b; }

private:
	unsigned int lightID = 1;
	std::string vnLightPos, vnLightAmbient,
				vnLightDiffuse, vnLightSpecular,
				vnLightQuadratic,vnLightLinear;

	unsigned int mDepthTex;
	unsigned int mDepthFBO;
	bool mCastShadow = true;
};

class MDirectionalLight :public MActor {
public:
	MDirectionalLight(MScene* scene_, const std::string& name_, const std::string& pymodule_path,
		const glm::vec3& direction, const glm::vec3& color, bool cast_shadow);
	~MDirectionalLight();
	glm::vec3 mDirection, mColor;
    void SetTimeOfDayObject(MTimeOfDay* tod) { mTimeOfDay = tod; }
	virtual void Save()override {}
	virtual void InitializeLuaInstance()override;
	virtual void Update(double dt)override;
	unsigned int GetLightID() { return this->lightID; }
private:
    MTimeOfDay* mTimeOfDay = NULL;
	unsigned int lightID = 1;
	bool mCastShadow = true;
};