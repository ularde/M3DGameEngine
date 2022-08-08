#pragma once
#include "Actor.h"
#include "Platforms.h"
#include "Camera.h"

class MActor;
class MBasicPlatform;
class MOfflineGame;

class MBasicPlayer :public MActor {
public:
protected:
	MCamera* camera = NULL;
};