#include "test_imguizmo.h"


extern uint32_t GetAmountOfEntities();

extern void G_SetManager(EntityManager* mgr);
EntityManager* G_GetManager();

namespace test {
    test_imguizmo::test_imguizmo() {
        editor = new EditorGUI;
        EntityManager* manager = new EntityManager;
        G_SetManager(manager);

        entity = new Entity("Main Camera");
        entity->addComponent<FPSCamera>();
        auto& cam = entity->getComponent<FPSCamera>().usingImGuiWindow = true;
        entity->getComponent<Transform>().position = glm::vec3(0, 0, 0);
        G_GetManager()->addEntity(entity);

        for (int16_t i = 0; i < 3; i++) {
            std::string name = "basic sphere ";
            name.append(std::to_string(i));
            entity = new Entity(name.c_str());
            G_GetManager()->addEntity(entity);
            entity->addComponent<ShaderProgram>();
            entity->getComponent<ShaderProgram>().SetBaseColor(glm::vec3(3 / (i + 1), 0, 0));
            entity->addComponent<Mesh>("mesh/pipe.obj");
            entity->addComponent<script_simplebehaviours>();
            entity->getComponent<Transform>().position = glm::vec3(i*2 + -2,-2,7);
        }

        fbo.GenerateFrameBuffer(editor->lastFrameWindowSize.x, editor->lastFrameWindowSize.y);
    }

    test_imguizmo::~test_imguizmo() {
        fbo.UnBind();
        glViewport(0, 0, G_GetWindowWidth(), G_GetWindowHeight());
    }

    void test_imguizmo::OnUpdate(double deltaTime) {
        G_GetManager()->OnUpdate(deltaTime);
    }

    void test_imguizmo::OnRender() {
        editor->OnGizmoRender();

        if (editor->windowSizeChangeFlag) {editor->UpdateFrameBufferTextureSize(fbo.GetRenderBuffer());}
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {return;}

        fbo.Bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        G_GetManager()->OnRender();

        fbo.UnBind();
    }

    void test_imguizmo::OnExit() {
        G_GetManager()->OnExit();
    }

    void test_imguizmo::OnImGuiRender() {
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        editor->RenderScene(fbo.GetRenderBuffer());
        editor->RenderHierarchy();
        editor->RenderProject();
        editor->RenderConsole();
        editor->RenderMainMenuBar();
        editor->RenderActiveInspector();
    }
}

