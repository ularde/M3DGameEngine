#include "pch.h"
#include "TimeOfDay.h"
#include "Scene.h"

MTimeOfDay::MTimeOfDay(MBasicPlatform* platform, MScene* parent, bool isDynamic, double speed, float xoffset, const MAtmosphereParameters& ap) {
	gPlatform = platform;
	mParent = parent;
	mAtmosphereParameters = ap;
	mDynamicFlag = isDynamic;
	mSpeed = speed;
	mXOffset = xoffset;
	mInitialTime = glfwGetTime();
	Update();
}

MTimeOfDay::~MTimeOfDay()
{
}

void MTimeOfDay::Update() {
	glm::vec3 direction(sin(mXOffset) * cos(GetTime() / mSpeed),
		sin(GetTime() / mSpeed),
		cos(mXOffset) * cos(GetTime() / mSpeed));
	direction = glm::normalize(direction);
	mColor = atmosphere(
		direction,
		glm::vec3(0, 6372e3, 0),
		direction,
		mAtmosphereParameters.iSun,
		mAtmosphereParameters.rPlanet,
		mAtmosphereParameters.rAtmos,
		mAtmosphereParameters.kRlh,
		mAtmosphereParameters.kMie,
		mAtmosphereParameters.shRlh,
		mAtmosphereParameters.shMie,
		mAtmosphereParameters.g
	);
	mDirection = direction;
}


double MTimeOfDay::GetTime() {
	if (mDynamicFlag) {
		mCurrentTime = glfwGetTime() - mInitialTime;
		return mCurrentTime;
	}
	else {
		return mCurrentTime;
	}
}

void MTimeOfDay::SetTime(double t) {
	if (mDynamicFlag) {
		mInitialTime = glfwGetTime() - t;
	}
	else {
		mCurrentTime = t;
	}
}

void MTimeOfDay::SetDynamic(bool flag) {
	if (!mDynamicFlag) {
		mInitialTime = glfwGetTime() + mCurrentTime;
	}
	mDynamicFlag = flag;
}
