#pragma once
#include "Actor.h"

class MUIActor :public MActor {
public:
protected:
};

class MButton :public MUIActor{
public:
	MButton(const char *caption, double pos_x, double pos_y, double pos_z, unsigned int color_r, unsigned int color_g, unsigned int color_b);
	~MButton();
	void Tick();
};

class MLabel :public MUIActor {
public:
	MLabel(const char* caption, double pos_x, double pos_y, double pos_z, unsigned int color_r, unsigned int color_g, unsigned int color_b);
	~MLabel();
	void Tick();
};