#ifndef TEST_DOCK_H
#define TEST_DOCK_H

#include "test.h"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#include "LightObject.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Mesh.h"
#include "Camera.h"

namespace test {
	class test_Dock : public Test
	{
	public:
		test_Dock();
		~test_Dock();

		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		GLuint FramebufferName = 0;
		GLuint renderedTexture;

		GLuint framebuffer;
		GLuint fbo;

		ImTextureID tex;
		ImVec2 lastFrameWindowSize;

		LightObject m_LightObject;
		FPSCamera m_fpsCamera;
		Texture2D m_Texture2D;
	};
}

#endif // !TEST_DOCK_H