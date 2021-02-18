#include "LightObject.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

LightObject::LightObject() :
	lightType(LightType::Point),
	color(glm::vec3(300.0f, 150.0f, 150.0f))
{}

LightObject::~LightObject() {}
void LightObject::OnRender(){}
void LightObject::OnUpdate(double deltaTime){}

glm::mat4 LightObject::LightSpaceMatrix() {
	glm::mat4 lightProjection = glm::mat4(1);
	glm::mat4 lightView = glm::mat4(1);
	glm::mat4 lightSpaceMatrix = glm::mat4(1);

		lightProjection = glm::ortho(
			v4lightProjection[0], 
			v4lightProjection[1], 
			v4lightProjection[2], 
			v4lightProjection[3], 
			near_plane, 
			far_plane);

		lightView = glm::lookAt
		(entity->getComponent<Transform>().position, 
			lookAtTarget, 
			glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		return lightSpaceMatrix;
}


void LightObject::OnImGuiRender(){
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	ImGui::Begin("Inspector");{
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Light Object", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
			ImGui::DragFloat4("Projection Size", &v4lightProjection.x, -0.1f, 0.1f);
			ImGui::DragFloat("Near plane", &near_plane, -0.05f, 0.05f);
			ImGui::DragFloat("far plane", &far_plane, -0.05f, 0.05f);
			ImGui::DragFloat("Light Intensity", &lightIntensity, -0.05f, 0.05f);
			ImGui::DragFloat("Shadow Intensity", &shadowIntensity, -0.05f, 0.05f);
			ImGui::DragFloat3("LookAtPosition", &lookAtTarget.x);
			ImGui::ColorPicker3("Light Color", &color.r);
			ImGui::ColorPicker3("Light Color", &shadowColour.r);

		}

		ImGui::Separator();
	}
	ImGui::End();
}

