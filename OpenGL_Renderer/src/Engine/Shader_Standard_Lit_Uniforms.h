#ifndef SHADER_STANDARD_LIT_UNIFORMS_H
#define SHADER_STANDARD_LIT_UNIFORMS_H

#include "Shader_UniformSetter.h"
#include "../Engine/ECS_List.h"
#include "../Texture2D.h"
#include <vector>

GLuint GetShadowMap();
bool RenderShadowMap();


namespace uniform {
	class Shader_Standard_Lit_Uniform : public Shader_Uniforms
	{
	public:
		Shader_Standard_Lit_Uniform();
		~Shader_Standard_Lit_Uniform();

		void LoadTextures(Entity& _shader);

		void SetUniformMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, ShaderProgram& _shader, Camera& _camera);
		void SetUniformCustom(ShaderProgram& _shader)override;
		void OnImGuiRender()override;

	public: //helperFunction
		void SetBaseColor(glm::vec3 _colour) override;
		void SetTextureScale(glm::vec2 _scale) override;
	private:
		std::unique_ptr <Texture2D[]> _pbr_textures;
		int numberOfTextures = 0;
	public:
		glm::vec3 m_baseColor = glm::vec3(1.0f, 0.38f, 0.38f);
		glm::vec2 m_TextureTiling = glm::vec2(1);
		bool castShadows = true;
		bool recieveShadows = true;
		float specular = 0;
	};

	void Shader_Standard_Lit_Uniform::SetBaseColor(glm::vec3 _color) { m_baseColor = _color; }
	void Shader_Standard_Lit_Uniform::SetTextureScale(glm::vec2 _scale) { m_TextureTiling = _scale; }

	void Shader_Standard_Lit_Uniform::SetUniformMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, ShaderProgram& _shader, Camera& _camera) {
		//glm::vec3 cameraPosition = GetManager()->FindActiveCamera()->getComponent<Transform>().position; // Also viable
		glm::mat4 _newModel = glm::mat4(1.0f);
		_shader.setUniform("model", model);
		_shader.setUniform("view", view);
		_shader.setUniform("projection", projection);
		_shader.setUniform("camPos", _camera.GetPosition());


	}

	void Shader_Standard_Lit_Uniform::SetUniformCustom(ShaderProgram& _shader) {

auto camPos = GetManager()->FindActiveCamera()->getComponent<Transform>().position;

		_shader.use();

		_pbr_textures[0].bind(0);
		_pbr_textures[1].bind(1);
		_pbr_textures[2].bind(2);
		_pbr_textures[3].bind(3);
		_pbr_textures[4].bind(4);

		_shader.setUniformSampler("albedoMap",		0);		// 0 = albedo
		_shader.setUniformSampler("normalMap",		1);		// 1 = normal
		_shader.setUniformSampler("metallicMap",	2);		// 2 = metalic
		_shader.setUniformSampler("roughnessMap",	3);		// 3 = roughness
		_shader.setUniformSampler("aoMap",			4);		// 4 = ambient 
		_shader.setUniformSampler("shadowMap",		5);		// 5 = shadow
		//glActiveTexture(GL_TEXTURE0 + 5);
		//glBindTexture(GL_TEXTURE_2D, GetShadowMap());

		auto m_lights = GetManager()->FindLights();

		_shader.setUniform("amountOfLights", (GLint)m_lights.size());
		_shader.setUniform("textureScale", m_TextureTiling);
		_shader.setUniform("_shadowIntensity", m_lights[0]->getComponent<LightObject>().shadowIntensity);
		_shader.setUniform("_lightIntensity", m_lights[0]->getComponent<LightObject>().lightIntensity);
		_shader.setUniform("_lightColor", m_lights[0]->getComponent<LightObject>().color);
		
		//std::cout << m_lights[0]->getComponent<LightObject>().color.x << "," 
		//<< m_lights[0]->getComponent<LightObject>().color.y << " , " 
		//<< m_lights[0]->getComponent<LightObject>().color.z << std::endl;
		
		for (unsigned int i = 0; i < m_lights.size(); ++i) {
			std::string str1 = "lightPositions[" + std::to_string(i) + "]";
			_shader.setUniform(str1.c_str(), m_lights[i]->getComponent<Transform>().position);	// set shader uniform for lightPosition[i]

			std::string str2 = "lightColors[" + std::to_string(i) + "]";
			_shader.setUniform(str2.c_str(), m_lights[i]->getComponent<LightObject>().color);		// set shader uniform for lightColors[i]
		}




		_shader.use();

		if (RenderShadowMap() && castShadows) {
			_shader.setUniform("view", glm::mat4(1));
			_shader.setUniform("projection", m_lights[0]->getComponent<LightObject>().LightSpaceMatrix());
		}
		if (recieveShadows) {
			_shader.setUniform("lightSpaceMatrix", m_lights[0]->getComponent<LightObject>().LightSpaceMatrix());
		}

		//_shader.setUniformSampler("diffuseTexture", 0);	// 0 = albedo
		_shader.setUniform("viewPos", camPos);
		//_shader.setUniform("lightPos", m_lights[0]->getComponent<Transform>().position);
	}

	void Shader_Standard_Lit_Uniform::OnImGuiRender() {
		ImGui::SetNextTreeNodeOpen(true, ImGuiTreeNodeFlags_DefaultOpen);
		if (ImGui::TreeNode("Surface Options")){
			ImGui::Columns(2, "Surface_Options_Columns", false);
			
			ImGui::Selectable("Cast Shadows");
			ImGui::NextColumn();
			ImGui::Checkbox("##Cast Shadows", &castShadows);
			
			ImGui::NextColumn();
			
			ImGui::Selectable("Recieve Shadows");
			ImGui::NextColumn();
			ImGui::Checkbox("##Recieve Shadows", &recieveShadows);
			
			ImGui::Columns(1);
			ImGui::TreePop();

		}
		ImGui::Separator();
		static ImVec4 albedo_color = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
		static ImVec4 normal_color = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
		static ImVec4 metallic_color = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
		static ImVec4 roughness_color = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
		static ImVec4 occlusion_color = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);

		ImGui::SetNextTreeNodeOpen(true, ImGuiTreeNodeFlags_DefaultOpen);
		if (ImGui::TreeNode("Surface Inputs")) {
			ImGui::Columns(2, "Surface_Inputs_Columns", false);
			//albedoMap",	
			//	normalMap",	
			//	metallicMap",
			//	roughnessMap"
			//	aoMap",		
			//--------------Albedo Map--------------//
			static uint16_t albedoMap_Image_Scalar = 1;
			if(ImGui::ImageButton((void*)_pbr_textures[0].GetTexture(), ImVec2(12 * albedoMap_Image_Scalar, 12 * albedoMap_Image_Scalar))) {
				albedoMap_Image_Scalar = (albedoMap_Image_Scalar == 1) ? 10 : 1;
			}
			ImGui::SameLine();
			ImGui::Selectable("Albedo Map");
			ImGui::NextColumn();
			if (ImGui::ColorButton("albedo_color", albedo_color)) {
				ImGui::SetNextWindowPos(ImVec2(ImGui::GetMousePos().x - 320, ImGui::GetMousePos().y - 120));
				ImGui::OpenPopup("albedo_color_picker");
			}
			if (ImGui::BeginPopup("albedo_color_picker", ImGuiSelectableFlags_DontClosePopups)){
				ImGui::ColorPicker4("##picker", &albedo_color.x, ImGuiColorEditFlags_None , NULL);
				ImGui::EndPopup();
			}
			ImGui::NextColumn();
			//--------------Normal Map--------------//
			 static uint16_t normalMap_Image_Scalar = 1;
			if (ImGui::ImageButton((void*)_pbr_textures[1].GetTexture(), ImVec2(12 * normalMap_Image_Scalar, 12 * normalMap_Image_Scalar))) {
				normalMap_Image_Scalar = (normalMap_Image_Scalar == 1) ? 10 : 1;
			}
			ImGui::SameLine();
			ImGui::Selectable("Normal Map");
			ImGui::NextColumn();
			if (ImGui::ColorButton("Normal_color", normal_color)) {
				ImGui::SetNextWindowPos(ImVec2(ImGui::GetMousePos().x - 320, ImGui::GetMousePos().y - 120));
				ImGui::OpenPopup("Normal_color_picker");
			}
			if (ImGui::BeginPopup("Normal_color_picker", ImGuiSelectableFlags_DontClosePopups)) {
				ImGui::ColorPicker4("##picker", &normal_color.x, ImGuiColorEditFlags_None, NULL);
				ImGui::EndPopup();
			}
			ImGui::NextColumn();
			//--------------Metallic Map--------------//
			static uint16_t metallicMap_Image_Scalar = 1;
			if (ImGui::ImageButton((void*)_pbr_textures[2].GetTexture(), ImVec2(12 * metallicMap_Image_Scalar, 12 * metallicMap_Image_Scalar))) {
				metallicMap_Image_Scalar = (metallicMap_Image_Scalar == 1) ? 10 : 1;
			}
			ImGui::SameLine();
			ImGui::Selectable("Metallic Map");
			ImGui::NextColumn();
			if (ImGui::ColorButton("Metallic_color", metallic_color)) {
				ImGui::SetNextWindowPos(ImVec2(ImGui::GetMousePos().x - 320, ImGui::GetMousePos().y - 120));
				ImGui::OpenPopup("Metallic_color_picker");
			}
			if (ImGui::BeginPopup("Metallic_color_picker", ImGuiSelectableFlags_DontClosePopups)) {
				ImGui::ColorPicker4("##picker", &metallic_color.x, ImGuiColorEditFlags_None, NULL);
				ImGui::EndPopup();
			}
			ImGui::NextColumn();
			//--------------Roughness Map--------------//
			static uint16_t RoughnessMap_Image_Scalar = 1;
			if (ImGui::ImageButton((void*)_pbr_textures[3].GetTexture(), ImVec2(12 * RoughnessMap_Image_Scalar, 12 * RoughnessMap_Image_Scalar))) {
				RoughnessMap_Image_Scalar = (RoughnessMap_Image_Scalar == 1) ? 10 : 1;
			}
			ImGui::SameLine();
			ImGui::Selectable("Roughness Map");

			ImGui::NextColumn();
			if (ImGui::ColorButton("Roughness_color", roughness_color)) {
				ImGui::SetNextWindowPos(ImVec2(ImGui::GetMousePos().x - 320, ImGui::GetMousePos().y - 120));
				ImGui::OpenPopup("Roughness_color_picker");
			}
			if (ImGui::BeginPopup("Roughness_color_picker", ImGuiSelectableFlags_DontClosePopups)) {
				ImGui::ColorPicker4("##picker", &roughness_color.x, ImGuiColorEditFlags_None, NULL);
				ImGui::EndPopup();
			}
			ImGui::NextColumn();
			//--------------Occlusion Map--------------//
			static uint16_t OcclusionMap_Image_Scalar = 1;
			if (ImGui::ImageButton((void*)_pbr_textures[4].GetTexture(), ImVec2(12 * OcclusionMap_Image_Scalar, 12 * OcclusionMap_Image_Scalar))) {
				OcclusionMap_Image_Scalar = (OcclusionMap_Image_Scalar == 1) ? 10 : 1;
			}
			ImGui::SameLine();
			ImGui::Selectable("Occlusion Map");
			ImGui::NextColumn();
			if (ImGui::ColorButton("Occlusion", occlusion_color)) {
				ImGui::SetNextWindowPos(ImVec2(ImGui::GetMousePos().x - 320, ImGui::GetMousePos().y - 120));
				ImGui::OpenPopup("Occlusion_color_picker");
			}
			if (ImGui::BeginPopup("Occlusion_color_picker", ImGuiSelectableFlags_DontClosePopups)) {
				ImGui::ColorPicker4("##picker", &occlusion_color.x, ImGuiColorEditFlags_None, NULL);
				ImGui::EndPopup();
			}
			ImGui::NextColumn();
			//--------------Shadow Map--------------// // TODO Update to support multiple shadowmaps
			static uint16_t ShadowMap_Image_Scalar = 1;
			if (ImGui::ImageButton((void*)GetShadowMap(), ImVec2(12 * ShadowMap_Image_Scalar, 12 * ShadowMap_Image_Scalar))) {
				ShadowMap_Image_Scalar = (ShadowMap_Image_Scalar == 1) ? 10 : 1;
			}
			ImGui::SameLine();
			ImGui::Selectable("Shadow Map");



			ImGui::Columns(1);
			ImGui::TreePop();
		}
		ImGui::Separator();

		ImGui::SetNextTreeNodeOpen(true, ImGuiTreeNodeFlags_DefaultOpen);
		if (ImGui::TreeNode("Advanced_Options")) {
			ImGui::Columns(2, "Advanced_Options_Columns", false);
			
			ImGui::Selectable("Texture Tiling");
			ImGui::NextColumn();
			ImGui::DragFloat2("Texture Tiling", &m_TextureTiling.x, -0.0125f, 0.0125f);
			
			ImGui::Columns(1);
			ImGui::TreePop();

		}
			ImGui::Separator();
		//ImGui::Indent();
		//ImGui::Separator();
		//if (ImGui::CollapsingHeader("Public Uniforms", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
		//	ImGui::Indent();
		//
		//
		//
		//	for (size_t i = 0; i < numberOfTextures; i++) {
		//		std::string name;
		//		name = "tex : " + std::to_string(i);
		//		if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
		//			int adjustedWidth = ImGui::GetContentRegionAvailWidth() * 1024 / 1024;
		//			ImGui::Image((void*)_pbr_textures[i].GetTexture(), ImVec2(ImGui::GetContentRegionAvailWidth(), adjustedWidth));
		//		}
		//	}
		//	ImGui::Unindent();
		//}
		//ImGui::Unindent();
	}

	void Shader_Standard_Lit_Uniform::LoadTextures(Entity& _shader) {
		auto tex = _shader.getComponent<ShaderProgram>().GetTextures();
		numberOfTextures = tex.size();
		_pbr_textures = std::make_unique<Texture2D[]>(numberOfTextures);

		for (size_t i = 0; i < numberOfTextures; i++) {
			_pbr_textures[i].loadTexture(tex[i], true);
		}
	}

	Shader_Standard_Lit_Uniform::Shader_Standard_Lit_Uniform() {}
	Shader_Standard_Lit_Uniform::~Shader_Standard_Lit_Uniform() {}
}

#endif // !SHADER_STANDARD_LIT_UNIFORMS_H