#pragma once
#include <vector>
#include "Class.h"

class MActor :public MPythonBoundObject {
public:
	virtual void Tick(double delta_time) = 0;
protected:
};