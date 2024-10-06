#pragma once
#include "glad.h" 
#include "ShaderManager.h"
#include "Platforms.h"

class MFreeCamera;
class MShaderStage;
class MVertexShaderProgram;
class MFragmentShaderProgram;


static MVertexShaderProgram* vs = NULL;
static MFragmentShaderProgram* fs = NULL;

void InitTest(MBasicPlatform* platform);
void RunTest(MBasicPlatform* platform);