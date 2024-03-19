#include "pch.h"

#include "WindowSystem.h"

#include "Engine/Graphics/Components/GraphicsLocator.h"

#include "Engine/Events/EventsFacade.h"

namespace MyEngine
{
    void WindowSystem::Init()
    {
        glfwSetErrorCallback(GlfwErrorCallback);
        if (!glfwInit())
        {
            LOG_ERROR("Error initializing GLFW!\n");
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

        // Set width and height from maximized window
        std::shared_ptr<WindowComponent> pWindow = GraphicsLocator::GetWindow();

        pWindow->LockWrite();
        pWindow->pGLFWWindow = glfwCreateWindow(pWindow->width,
                                                pWindow->height,
                                                pWindow->name.c_str(), NULL, NULL);
        if (!pWindow->pGLFWWindow)
        {
            LOG_ERROR("Error creating window '" + pWindow->name + "'!\n");
            glfwTerminate();

            pWindow->UnlockWrite();
            return;
        }

        // Update width and height for maximized window
        glfwGetFramebufferSize(pWindow->pGLFWWindow, &(pWindow->width), &(pWindow->height));
        glfwMakeContextCurrent(pWindow->pGLFWWindow);
        pWindow->UnlockWrite();

        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        glfwSwapInterval(1);

        // While drawing a pixel, see if the pixel that's already there is closer or not?
        glEnable(GL_DEPTH_TEST);
        // Activate transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // set to not draw "back facing" triangles
        glCullFace(GL_BACK);
    }

    void WindowSystem::Start(std::shared_ptr<Scene> pScene)
    {
    }

    void WindowSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
    }

    void WindowSystem::Render(std::shared_ptr<Scene> pScene)
    {
    }

    void WindowSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

    void WindowSystem::Shutdown()
    {
        std::shared_ptr<WindowComponent> pWindow = GraphicsLocator::GetWindow();
        if (pWindow->pGLFWWindow) {
            glfwDestroyWindow(pWindow->pGLFWWindow);
        }
        glfwTerminate();
    }
}
