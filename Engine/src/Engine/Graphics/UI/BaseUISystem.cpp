#include "pch.h"

#include "BaseUISystem.h"
#include "Engine/Graphics/Components/GraphicsLocator.h"

namespace MyEngine
{
    void BaseUISystem::Init()
    {
        // Initialize all Imgui integration with GLFW and OpenGL
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        std::shared_ptr<WindowComponent> pWindow = GraphicsLocator::GetWindow();

        // Dark theme for the window
        ImGui::StyleColorsDark();

        bool response = ImGui_ImplGlfw_InitForOpenGL(pWindow->pGLFWWindow, true);
        if (!response)
        {
            LOG_ERROR("Error initializing IMGUI with GLFW!");
            return;
        }

        response = ImGui_ImplOpenGL3_Init("#version 420");
        if (!response)
        {
            LOG_ERROR("Error initializing IMGUI with OpenGL!");
            return;
        }

        return;
    }

    void BaseUISystem::Start(std::shared_ptr<Scene> pScene)
    {
    }

    void BaseUISystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
    }

    void BaseUISystem::Render(std::shared_ptr<Scene> pScene)
    {
    }

    void BaseUISystem::End(std::shared_ptr<Scene> pScene)
    {
    }

    void BaseUISystem::Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}
