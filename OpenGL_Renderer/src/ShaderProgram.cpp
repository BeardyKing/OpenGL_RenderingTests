#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "glm/gtc/type_ptr.hpp"

ShaderProgram::ShaderProgram() 
	: m_Handle(0)
{
	loadShaders("objectDefaults/basic.vert", "objectDefaults/basic.frag");
}

ShaderProgram::ShaderProgram(const char* _vsFileName, const char* _fsFileName)
	: m_Handle(0)
{
	loadShaders(_vsFileName, _fsFileName);
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(m_Handle);
}

void ShaderProgram::OnImGuiRender()
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	ImGui::Begin("Inspector"); {
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Shader Program", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
			ImGui::Indent();
			if (ImGui::CollapsingHeader("Vertex Shader", ImGuiTreeNodeFlags_AllowItemOverlap)) {
			ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly;
			ImGui::InputTextMultiline("EDITOR_VS", &editor_fragmentShader[0], editor_fragmentShader.size(), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);
			}

			if (ImGui::CollapsingHeader("Fragment Shader",ImGuiTreeNodeFlags_AllowItemOverlap)) {
				ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly;
				ImGui::InputTextMultiline("EDITOR_FS", &editor_vertexShader[0], editor_vertexShader.size(), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);
			}
			ImGui::Unindent();
		}
		ImGui::Separator();
	}
	ImGui::End();
}

bool ShaderProgram::loadShaders (const char* vsFileName, const char* fsFileName) {
	std::string vsString = fileToString(vsFileName);
	std::string fsString = fileToString(fsFileName);
	const GLchar* vsSourcePtr = vsString.c_str();
	const GLchar* fsSourcePtr = fsString.c_str();

	//_EDITOR_

	editor_vertexShader = vsString;
	editor_fragmentShader = fsString;

	//_EDITOR_

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(vs, 1, &vsSourcePtr, NULL);
	glShaderSource(fs, 1, &fsSourcePtr, NULL);

	glCompileShader(vs);
	checkCompileErrors(vs, VERTEX);
	glCompileShader(fs);
	checkCompileErrors(fs, FRAGMENT);

	m_Handle = glCreateProgram();
	glAttachShader(m_Handle, vs);
	glAttachShader(m_Handle, fs);
	glLinkProgram(m_Handle);

	checkCompileErrors(m_Handle, PROGRAM);

	glDeleteShader(vs);
	glDeleteShader(fs);

	m_UniformLocations.clear();

	return true;
}

void ShaderProgram::use() {
	if (m_Handle > 0){
		glUseProgram(m_Handle);
	}
}

std::string ShaderProgram::fileToString(const std::string& fileName) {
	std::stringstream ss;
	std::ifstream file;

	try{
		file.open(fileName, std::ios::in);

		if (!file.fail()){
			ss << file.rdbuf();
		}
		
		file.close();
	}
	catch (std::exception ex){
		std::cerr << "Error reading shader filename" << std::endl;
	}

	//std::cout << ss.str() << std::endl;

	return ss.str();
}

void ShaderProgram::checkCompileErrors(GLuint shader, ShaderType type) {
	int status = 0;

	if (type == PROGRAM){
		glGetProgramiv(m_Handle, GL_LINK_STATUS, &status);
		if (status == GL_FALSE){
			GLint length = 0;
			glGetProgramiv(m_Handle, GL_INFO_LOG_LENGTH, &length);

			std::string errorLog(length, ' ');
			glGetProgramInfoLog(m_Handle, length, &length, &errorLog[0]);
			std::cerr << "ERROR! Program failed to link : " << errorLog << std::endl;
		}
	}
	else{ // VERTEX OR FRAGMENT COMPILING
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE){
			GLint length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

			std::string errorLog(length, ' ');
			glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
			std::cerr << "ERROR! Shdaer failed to compile : " << errorLog << std::endl;
		}
	}
}

GLuint ShaderProgram::getProgram()const {
	return m_Handle;
}


GLint ShaderProgram::getUniformLocation(const GLchar* name) { // check if uniform has already been found
	std::map<std::string, GLint>::iterator it = m_UniformLocations.find(name);

	if (it == m_UniformLocations.end()){
		m_UniformLocations[name] = glGetUniformLocation(m_Handle, name);
	}

	return m_UniformLocations[name];
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec2& v) {
	GLint loc = getUniformLocation(name);
	glUniform2f(loc, v.x, v.y);
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec3& v) {
	GLint loc = getUniformLocation(name);
	glUniform3f(loc, v.x, v.y, v.z);
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec4& v) {
	GLint loc = getUniformLocation(name);
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void ShaderProgram::setUniform(const GLchar* name, const glm::mat4& m) {
	GLint loc = getUniformLocation(name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}

void ShaderProgram::setUniform(const GLchar* name, const GLfloat& f) {
	GLfloat loc = getUniformLocation(name);
	glUniform1f(loc, f);
}

void ShaderProgram::setUniformSampler(const GLchar* name, const GLint& slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	GLfloat loc = getUniformLocation(name);
	glUniform1i(loc, slot);
}

void ShaderProgram::setUniform(const GLchar* name, const GLint& i) {
	GLint loc = getUniformLocation(name);
	glUniform1i(loc, i);
}

