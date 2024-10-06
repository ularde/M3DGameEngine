#include "pch.h"
#include "ShaderManager.h"
#include "SharedFunc.h"

std::string tempShaderCode;

void MShaderStage::RegisterUniformVariable(const std::string &vName) {
	this->uniformLocationMap[vName.c_str()] = glGetUniformLocation(this->programID,vName.c_str());
	this->gPlatform->ConsoleWriteLine(0.0f, 1.0f, 0.0f, std::string("[NOTE] OpenGL : uniform variable ") + std::string(vName) + std::string(" location = ") + std::to_string(this->uniformLocationMap[vName.c_str()]));
}

void MShaderStage::UniformBool(const std::string &vName, const GLboolean data) {
	glProgramUniform1i(this->programID, glGetUniformLocation(this->programID, vName.c_str()), data);
}

void MShaderStage::UniformInt(const std::string &vName, const GLint data) {
	glProgramUniform1i(this->programID, glGetUniformLocation(this->programID, vName.c_str()), data);
}

void MShaderStage::UniformUint(const std::string &vName, const GLuint data) {
	glProgramUniform1ui(this->programID, glGetUniformLocation(this->programID, vName.c_str()), data);
}

void MShaderStage::UniformFloat(const std::string &vName, const GLfloat data) {
	glProgramUniform1f(this->programID, glGetUniformLocation(this->programID, vName.c_str()), data);
}

void MShaderStage::UniformDouble(const std::string &vName, const GLfloat data) {
	glProgramUniform1d(this->programID, glGetUniformLocation(this->programID, vName.c_str()), data);
}

void MShaderStage::UniformVec2(const std::string &vName, const glm::vec2 &vec) {
	glProgramUniform2fv(this->programID, glGetUniformLocation(this->programID, vName.c_str()), 1, &vec[0]);
}

void MShaderStage::UniformVec3(const std::string &vName, const glm::vec3 &vec) {
	glProgramUniform3fv(this->programID, glGetUniformLocation(this->programID, vName.c_str()), 1, &vec[0]);
}

void MShaderStage::UniformVec4(const std::string& vName, const glm::vec4& vec) {
	glProgramUniform4fv(this->programID, glGetUniformLocation(this->programID, vName.c_str()), 1, &vec[0]);
}

void MShaderStage::UniformMat2(const std::string& vName, const glm::mat2& mat) {
	glProgramUniformMatrix2fv(this->programID, glGetUniformLocation(this->programID, vName.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void MShaderStage::UniformMat3(const std::string &vName, const glm::mat3 &mat) {
	glProgramUniformMatrix3fv(this->programID, glGetUniformLocation(this->programID, vName.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void MShaderStage::UniformMat4(const std::string& vName, const glm::mat4& mat) {
	glProgramUniformMatrix4fv(this->programID, glGetUniformLocation(this->programID, vName.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void MShaderStage::UniformPxMat44(const std::string& vName, const physx::PxMat44& mat) {
	glProgramUniformMatrix4fv(this->programID, glGetUniformLocation(this->programID, vName.c_str()), 1, GL_FALSE, mat.front());
}

void MShaderStage::ReadShaderFileContent() {
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		shaderFile.open(GetFullAssetPathA(this->path));
		std::stringstream contentStream;
		contentStream << shaderFile.rdbuf();
		shaderFile.close();
		this->code_str = contentStream.str();
	}
	catch (std::ifstream::failure& error) {
		this->gPlatform->ConsoleWriteLine(1.0f, 0.0f, 0.0f, std::string("[ERROR] ShaderManager : Failed to read shader file : ", (const char*)error.what()));
	}
}

int MShaderStage::CheckCompileStatus() {
	static GLint status;
	static GLchar infoLog[M3D_MAX_SHADER_INFOLOG_LENGTH] = { 0 };

	glGetProgramiv(this->programID, GL_LINK_STATUS, &status);
	glGetProgramInfoLog(this->programID, M3D_MAX_SHADER_INFOLOG_LENGTH, NULL, infoLog);

	if (!status) {
		this->gPlatform->ConsoleWriteLine(1.0f, 0.0f, 0.0f, "[ERROR] OpenGL : Failed to compile shader " + std::string(this->path) + ".");
		this->gPlatform->ConsoleWriteLine(1.0f, 0.0f, 0.0f, infoLog);
	}
	else {
		this->gPlatform->ConsoleWriteLine(1.0f, 1.0f, 1.0f, "[INFO] OpenGL : Shader " + std::string(this->path) + " has been compiled successfully.");
		this->gPlatform->ConsoleWriteLine(1.0f, 1.0f, 0.0f, infoLog);
	}

	return status;
}

MVertexShaderProgram::MVertexShaderProgram(GLuint pipelineID_, MBasicPlatform* platform__, std::string path__) {
	this->gPlatform = platform__;
	this->path = path__;
	this->pipelineID = pipelineID_;
	mbstowcs(this->path_wcs, this->path.c_str(), M3D_MAX_SHADER_INFOLOG_LENGTH);

	this->ReadShaderFileContent();
	const char* code = this->code_str.c_str();

	this->programID = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &code);
	this->CheckCompileStatus();
}

MVertexShaderProgram::~MVertexShaderProgram() {

}

void MVertexShaderProgram::UseProgramStage() {
	glUseProgramStages(this->pipelineID, GL_VERTEX_SHADER_BIT, this->programID);
}


MFragmentShaderProgram::MFragmentShaderProgram(GLuint pipelineID_, MBasicPlatform* platform__, std::string path__) {
	this->gPlatform = platform__;
	this->path = path__;
	this->pipelineID = pipelineID_;
	mbstowcs(this->path_wcs, this->path.c_str(), M3D_MAX_SHADER_INFOLOG_LENGTH);

	this->ReadShaderFileContent();
	const char* code = this->code_str.c_str();

	this->programID = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &code);
	this->CheckCompileStatus();
}

MFragmentShaderProgram::~MFragmentShaderProgram() {

}

void MFragmentShaderProgram::UseProgramStage() {
	glUseProgramStages(this->pipelineID, GL_FRAGMENT_SHADER_BIT, this->programID);
}

MGeometryShader::MGeometryShader(MBasicPlatform* platform__, std::string path__) {

}

MGeometryShader::~MGeometryShader() {

}

void MGeometryShader::UseProgramStage()
{
}

MShaderProgram::MShaderProgram(MBasicPlatform* gPlatform) {
	mPlatform = gPlatform;
	mProgramID = glCreateProgram();
}

MShaderProgram::~MShaderProgram()
{
}

void MShaderProgram::AttachShader(MShaderType type, const std::string& path) {
	LoadShaderFile(path);
	const char* code = tempShaderCode.c_str();

	switch (type)
	{
	case MShaderType::PROGRAM:
		break;
	case MShaderType::VERTEX:
		mVertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(mVertexShaderID, 1, &code, 0);
		glCompileShader(mVertexShaderID);
		glAttachShader(mProgramID, mVertexShaderID);
		break;
	case MShaderType::FRAGMENT:
		mFragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(mFragmentShaderID, 1, &code, 0);
		glCompileShader(mFragmentShaderID);
		glAttachShader(mProgramID, mFragmentShaderID);
		break;
	case MShaderType::GEOMETRY:
		break;
	case MShaderType::COMPUTE:
		break;
	default:
		break;
	}

	CheckCompileStatus(type);
}

void MShaderProgram::LinkProgram() {
	glLinkProgram(mProgramID);
	CheckCompileStatus(MShaderType::PROGRAM);
}

void MShaderProgram::Use() {
	glUseProgram(mProgramID);
}

void MShaderProgram::UniformBool(const std::string& vName, const GLboolean data) {
	glProgramUniform1i(mProgramID, glGetUniformLocation(mProgramID, vName.c_str()), data);
}

void MShaderProgram::UniformInt(const std::string& vName, const GLint data) {
	glProgramUniform1i(mProgramID, glGetUniformLocation(mProgramID, vName.c_str()), data);
}

void MShaderProgram::UniformUint(const std::string& vName, const GLuint data) {
	glProgramUniform1ui(mProgramID, glGetUniformLocation(mProgramID, vName.c_str()), data);
}

void MShaderProgram::UniformFloat(const std::string& vName, const GLfloat data) {
	glProgramUniform1f(mProgramID, glGetUniformLocation(mProgramID, vName.c_str()), data);
}

void MShaderProgram::UniformDouble(const std::string& vName, const GLfloat data) {
	glProgramUniform1d(mProgramID, glGetUniformLocation(mProgramID, vName.c_str()), data);
}

void MShaderProgram::UniformVec2(const std::string& vName, const glm::vec2& vec) {
	glProgramUniform2f(mProgramID, glGetUniformLocation(mProgramID, vName.c_str()), vec.x, vec.y);
}

void MShaderProgram::UniformVec3(const std::string& vName, const glm::vec3& vec) {
	glProgramUniform3f(mProgramID, glGetUniformLocation(mProgramID, vName.c_str()), vec.x, vec.y, vec.z);
}

void MShaderProgram::UniformVec4(const std::string& vName, const glm::vec4& vec) {
	glProgramUniform4f(mProgramID, glGetUniformLocation(mProgramID, vName.c_str()), vec.x, vec.y, vec.z, vec.w);
}

void MShaderProgram::UniformMat2(const std::string& vName, const glm::mat2& mat) {
	glProgramUniformMatrix2fv(mProgramID, glGetUniformLocation(mProgramID, vName.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void MShaderProgram::UniformMat3(const std::string& vName, const glm::mat3& mat) {
	glProgramUniformMatrix3fv(mProgramID, glGetUniformLocation(mProgramID, vName.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void MShaderProgram::UniformMat4(const std::string& vName, const glm::mat4& mat) {
	glProgramUniformMatrix4fv(mProgramID, glGetUniformLocation(mProgramID, vName.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void MShaderProgram::LoadShaderFile(const std::string& path) {
	std::ifstream file;
	std::string code;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		file.open(GetFullAssetPathA(path));
		std::stringstream contentStream;
		contentStream << file.rdbuf();
		file.close();
		code = contentStream.str();
		tempShaderCode = code;
	}
	catch (std::ifstream::failure& error) {
		static wchar_t error_wcs[M3D_MAX_IFSTREAM_ERROR_LENGTH] = { 0 };
		mPlatform->ConsoleWriteLine(1.0f, 0.0f, 0.0f, std::string("[ERROR] ShaderManager : Failed to read shader file : ", (const char*)error.what()));
	}
}

GLuint MShaderProgram::CheckCompileStatus(MShaderType type) {
	static GLint status;
	static GLchar infoLog[M3D_MAX_SHADER_INFOLOG_LENGTH] = { 0 };

	switch (type)
	{
	case MShaderType::PROGRAM:
		glGetProgramiv(mProgramID, GL_LINK_STATUS, &status);
		glGetProgramInfoLog(mProgramID, M3D_MAX_SHADER_INFOLOG_LENGTH, NULL, infoLog);
		break;
	case MShaderType::VERTEX:
		glGetShaderiv(mVertexShaderID, GL_COMPILE_STATUS, &status);
		glGetShaderInfoLog(mVertexShaderID, M3D_MAX_SHADER_INFOLOG_LENGTH, NULL, infoLog);
		break;
	case MShaderType::FRAGMENT:
		glGetShaderiv(mFragmentShaderID, GL_COMPILE_STATUS, &status);
		glGetShaderInfoLog(mFragmentShaderID, M3D_MAX_SHADER_INFOLOG_LENGTH, NULL, infoLog);
		break;
	case MShaderType::GEOMETRY:
		break;
	case MShaderType::COMPUTE:
		break;
	default:
		break;
	}

	printf("%s\n", infoLog);

	return status;
}
