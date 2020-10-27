#include "LightObject.h"

LightObject::LightObject() 
:
	lightType(LightType::Point),
	m_vertexShaderPath("objectDefaults/basic.vert"),
	m_fragmentShaderPath("objectDefaults/basic.frag"),
	m_meshPath("objectDefaults/light.obj"),
	position(glm::vec3(0.0f)),
	rotation(glm::vec3(0.0f)),
	scale(glm::vec3(1.0f)),
	color(glm::vec3(300.0f, 150.0f, 150.0f)),
	direction(glm::vec3(0.0f, -0.9f, -0.17f))
{
	m_Shader = std::make_unique<ShaderProgram>();
	m_Shader->loadShaders(m_vertexShaderPath, m_fragmentShaderPath);

	m_Mesh = std::make_unique<Mesh>();
	m_Mesh->LoadOBJ(m_meshPath);
}

LightObject::~LightObject() {

}

void LightObject::LoadMesh(const char* meshPath) {

}

void LightObject::LoadShader(const char* vertPath, const char* fragPath) {

}
