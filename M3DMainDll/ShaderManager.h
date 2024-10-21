#pragma once
#include "glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>

#include "Platforms.h"

class MBasicPlatform;

enum class MShaderType {
	PROGRAM,
	VERTEX,
	FRAGMENT,
	GEOMETRY,
	COMPUTE
};

class MShaderProgram {
public:
	MShaderProgram(MBasicPlatform* gPlatform);
	~MShaderProgram();
	void AttachShader(MShaderType type, const std::string& path);
	void LinkProgram();
	void Use();
	void UniformBool(const std::string& vName, const GLboolean data);
	void UniformInt(const std::string& vName, const GLint data);
	void UniformUint(const std::string& vName, const GLuint data);
	void UniformFloat(const std::string& vName, const GLfloat data);
	void UniformDouble(const std::string& vName, const GLfloat data);
	void UniformIVec2(const std::string& vName, const glm::ivec2& vec);
	void UniformVec2(const std::string& vName, const glm::vec2& vec);
	void UniformVec3(const std::string& vName, const glm::vec3& vec);
	void UniformVec4(const std::string& vName, const glm::vec4& vec);
	void UniformMat2(const std::string& vName, const glm::mat2& mat);
	void UniformMat3(const std::string& vName, const glm::mat3& mat);
	void UniformMat4(const std::string& vName, const glm::mat4& mat);
protected:
	GLuint mVertexShaderID = 0;
	GLuint mFragmentShaderID = 0;
	GLuint mProgramID = 0;
	MBasicPlatform* mPlatform;
	void LoadShaderFile(const std::string& path);
	GLuint CheckCompileStatus(MShaderType type);
};

class MShaderStage {
public:
	void RegisterUniformVariable(const std::string &vName);
	void UniformBool(const std::string& vName, const GLboolean data);
	void UniformInt(const std::string& vName, const GLint data);
	void UniformUint(const std::string& vName, const GLuint data);
	void UniformFloat(const std::string& vName, const GLfloat data);
	void UniformDouble(const std::string& vName, const GLfloat data);
	void UniformVec2(const std::string& vName, const glm::vec2& vec);
	void UniformVec3(const std::string& vName, const glm::vec3& vec);
	void UniformVec4(const std::string& vName, const glm::vec4& vec);
	void UniformMat2(const std::string& vName, const glm::mat2& mat);
	void UniformMat3(const std::string& vName, const glm::mat3& mat);
	void UniformMat4(const std::string& vName, const glm::mat4& mat);
	void UniformPxMat44(const std::string& vName, const physx::PxMat44& mat);
protected:
	int CheckCompileStatus();
	unsigned int shaderID = 0;
	unsigned int programID = 0;
	unsigned int pipelineID = 0;
	MBasicPlatform* gPlatform = NULL;
	std::string path;
	std::string code_str;
	wchar_t path_wcs[4096] = { 0 };
	std::unordered_map<const char*, GLint> uniformLocationMap;

	void ReadShaderFileContent();
};

class MVertexShaderProgram :public MShaderStage {
public:
	MVertexShaderProgram(GLuint pipelineID_, MBasicPlatform* platform__, std::string path__);
	~MVertexShaderProgram();
	void UseProgramStage();
};

class MFragmentShaderProgram :public MShaderStage {
public:
	MFragmentShaderProgram(GLuint pipelineID_, MBasicPlatform* platform__, std::string path__);
	~MFragmentShaderProgram();
	void UseProgramStage();
};

class MGeometryShader :public MShaderStage {
public:
	MGeometryShader(MBasicPlatform* platform__, std::string path__);
	~MGeometryShader();
	void UseProgramStage();
};