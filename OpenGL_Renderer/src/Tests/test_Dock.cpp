#include "test_Dock.h"


extern uint32_t GetAmountOfEntities();

extern void G_SetManager(EntityManager* mgr);
EntityManager* G_GetManager();

namespace test {
	test_Dock::test_Dock(){
        editor = new EditorGUI;
        EntityManager* manager = new EntityManager;
        G_SetManager(manager);

        entity = new Entity("Main Camera");
        entity->addComponent<FPSCamera>();
        auto& cam = entity->getComponent<FPSCamera>().usingImGuiWindow = true;
        entity->getComponent<Transform>().position = glm::vec3(0, 0, -18);
        G_GetManager()->addEntity(entity);

        for (size_t i = 0; i < 100; i++){
            std::string name = "basic sphere ";
            name.append(std::to_string(i));
            entity = new Entity(name.c_str());
            G_GetManager()->addEntity(entity);
            entity->addComponent<ShaderProgram>();
            entity->getComponent<ShaderProgram>().SetBaseColor(glm::vec3(glm::ballRand((float)i / 2)));
            if (i % 2) {
                entity->addComponent<Mesh>("objectDefaults/cube.obj");
            }
            else if (i % 3) {
                entity->addComponent<Mesh>("mesh/pipe.obj");
            }
            else {
                entity->addComponent<Mesh>();
            }
            entity->getComponent<Transform>().position = glm::vec3(glm::ballRand((float)i / 2));
            entity->getComponent<Transform>().rotation = glm::vec3(glm::ballRand((float)i / 2));
        }

        fbo.GenerateFrameBuffer(editor->lastFrameWindowSize.x, editor->lastFrameWindowSize.y);
    }

	test_Dock::~test_Dock() {
        fbo.UnBind();
        glViewport(0, 0, G_GetWindowWidth(), G_GetWindowHeight());
    }

	void test_Dock::OnUpdate(double deltaTime) {
        G_GetManager()->OnUpdate(deltaTime);
    }

	void test_Dock::OnRender() {
        if (editor->windowSizeChangeFlag){
            editor->UpdateFrameBufferTextureSize(fbo.GetRenderBuffer());
        }
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            return;
        }

        fbo.Bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        G_GetManager()->OnRender();

        fbo.UnBind();
    }

    void test_Dock::OnExit() {
        G_GetManager()->OnExit();
    }

    void test_Dock::OnImGuiRender() {
    
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        editor->RenderScene(fbo.GetRenderBuffer());
        editor->RenderHierarchy();
        editor->RenderProject();
        editor->RenderConsole();
        editor->RenderMainMenuBar();
        editor->RenderActiveInspector();
    }
}

