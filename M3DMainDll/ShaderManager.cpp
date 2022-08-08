#include "pch.h"
#include "ShaderManager.h"
#include "SharedFunc.h"

void MShaderStage::RegisterUniformVariable(const std::string &vName) {
	this->uniformLocationMap[vName.c_str()] = glGetUniformLocation(this->programID,vName.c_str());

	wchar_t vName_wcs[512] = { 0 };
	mbstowcs(vName_wcs, vName.c_str(), 512);
	this->platform->ConsoleWriteLine(0.0f, 1.0f, 0.0f, std::wstring(L"[NOTE] OpenGL : uniform variable ") + std::wstring(vName_wcs) + std::wstring(L" location = ") + std::to_wstring(this->uniformLocationMap[vName.c_str()]));
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
		static wchar_t error_wcs[M3D_MAX_IFSTREAM_ERROR_LENGTH] = { 0 };
		mbstowcs(error_wcs, error.what(), M3D_MAX_IFSTREAM_ERROR_LENGTH);
		this->platform->ConsoleWriteLine(1.0f, 0.0f, 0.0f, std::wstring(L"[ERROR] ShaderManager : Failed to read shader file : ", (const wchar_t*)error_wcs));
	}
}

int MShaderStage::CheckCompileStatus() {
	static GLint status;
	static GLchar infoLog[M3D_MAX_SHADER_INFOLOG_LENGTH] = { 0 };
	static wchar_t infoLog_wcs[M3D_MAX_SHADER_INFOLOG_LENGTH] = { 0 };

	glGetProgramiv(this->programID, GL_LINK_STATUS, &status);
	glGetProgramInfoLog(this->programID, M3D_MAX_SHADER_INFOLOG_LENGTH, NULL, infoLog);

	mbstowcs(infoLog_wcs, infoLog, M3D_MAX_SHADER_INFOLOG_LENGTH);

	if (!status) {
		this->platform->ConsoleWriteLine(1.0f, 0.0f, 0.0f, L"[ERROR] OpenGL : Failed to compile shader " + std::wstring(this->path_wcs) + L".");
		this->platform->ConsoleWriteLine(1.0f, 0.0f, 0.0f, infoLog_wcs);
	}
	else {
		this->platform->ConsoleWriteLine(1.0f, 1.0f, 1.0f, L"[INFO] OpenGL : Shader " + std::wstring(this->path_wcs) + L" has been compiled successfully.");
		this->platform->ConsoleWriteLine(1.0f, 1.0f, 0.0f, infoLog_wcs);
	}

	return status;
}

MVertexShaderProgram::MVertexShaderProgram(MBasicPlatform* platform__, std::string path__) {
	this->platform = platform__;
	this->path = path__;
	this->pipelineID = this->platform->pipelineID;
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


MFragmentShaderProgram::MFragmentShaderProgram(MBasicPlatform* platform__, std::string path__) {
	this->platform = platform__;
	this->path = path__;
	this->pipelineID = this->platform->pipelineID;
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
