#include "pch.h"

#include "Engine.h"

#include "Engine/Core/Configs/ConfigSerializerFactory.h"
#include "Engine/Core/Components/CoreLocator.h"
#include "Engine/Core/Resources/ResourceManagerFactory.h"

#include "Engine/Events/EventsFacade.h"

#include "Engine/Debug/Components/DebugLocator.h"

#include "Engine/ECS/System/SystemBuilder.h"    
#include "Engine/ECS/Scene/SceneSerializerFactory.h"

#include "Engine/Graphics/Renderer/RendererManager.h"
#include "Engine/Graphics/Renderer/RendererManagerLocator.h"
#include "Engine/Graphics/Particles/ParticleManager.h"
#include "Engine/Graphics/Particles/ParticleManagerLocator.h"
#include "Engine/Graphics/Components/GraphicsLocator.h"

#include "Engine/Utils/InputUtils.h"

#include "Engine/Physics/Components/PhysicsLocator.h"

namespace MyEngine
{
    using itSystems = std::map<std::string, std::shared_ptr<iSystem>>::iterator;
    using pairSystems = std::pair<std::string, std::shared_ptr<iSystem>>;

    Engine::Engine() : m_rendererManager(new RendererManager()),
                       m_particleManager(new ParticleManager()),
                       m_pScene(new Scene())

    {
    }

    Engine::~Engine()
    {
    }

    float Engine::GetDeltaTime()
    {
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - m_lastTime;
        m_lastTime = currentTime;

        // Clamp delta time to the maximum frame time
        if (deltaTime > FRAME_DURATION)
        {
            deltaTime = FRAME_DURATION;
        }

        // Add the frame time to the list
        m_frameTimes.push_back(deltaTime);

        // Limit the number of frames
        const size_t maxFrameCount = FRAME_RATE; // Store frame times for a second
        if (m_frameTimes.size() > maxFrameCount)
        {
            m_frameTimes.erase(m_frameTimes.begin());
        }

        // Calculate the average frame time
        float averageFrameTime = 0;
        for (float time : m_frameTimes)
        {
            averageFrameTime += time;
        }
        averageFrameTime /= m_frameTimes.size();

        return averageFrameTime;
    }

    void Engine::AddSystem(std::string systemName)
    {
        std::shared_ptr<iSystem> system = GetSystem(systemName);
        if (system)
        {
            // System already added
            return;
        }

        system = SystemBuilder::CreateSystem(systemName);
        if (!system)
        {
            return;
        }

        m_vecSystems.push_back(system);
        system->Init();
    }

    void Engine::AddSystem(std::string systemName, std::shared_ptr<Scene> pScene)
    {
        std::shared_ptr<iSystem> system = GetSystem(systemName);
        if (system)
        {
            // System already added
            return;
        }

        AddSystem(systemName);
        GetSystem(systemName)->Start(pScene);
    }

    void Engine::RemoveSystem(std::string systemName, std::shared_ptr<Scene> pScene)
    {
        for (int i = 0; i < m_vecSystems.size(); i++)
        {
            std::shared_ptr<iSystem> system = m_vecSystems[i];
            if (system->SystemName() != systemName)
            {
                continue;
            }

            // Clean system before deleting
            system->End(pScene);
            system->Shutdown();

            m_vecSystems.erase(m_vecSystems.begin() + i);

            break;
        }

        return;
    }

    std::shared_ptr<iSystem> Engine::GetSystem(std::string systemName)
    {
        for (int i = 0; i < m_vecSystems.size(); i++)
        {
            std::shared_ptr<iSystem> pSystem = m_vecSystems[i];
            if (pSystem->SystemName() == systemName)
            {
                return pSystem;
            }
        }

        return nullptr;
    }

    void Engine::Init()
    {
        LoadConfigurations();

        // Setting up resources managers        
        RendererManagerLocator::Set(m_rendererManager);
        ParticleManagerLocator::Set(m_particleManager);

        // Global events of engine interest
        SUBSCRIBE_WINDOW_CLOSE_EVENT([this](const WindowCloseEvent& event) { OnWindowClose(event); });

        // Load resources
        std::shared_ptr<ConfigPathComponent> pConfigPaths = CoreLocator::GetConfigPath();
        std::shared_ptr<iResourceManager> pMeshManager = ResourceManagerFactory::GetOrCreate(eResourceTypes::MESH);
        std::shared_ptr<iResourceManager> pTextureManager = ResourceManagerFactory::GetOrCreate(eResourceTypes::TEXTURE);
        std::shared_ptr<iResourceManager> pShaderManager = ResourceManagerFactory::GetOrCreate(eResourceTypes::SHADER);

        pMeshManager->SetBasePath(pConfigPaths->pathModels);
        pTextureManager->SetBasePath(pConfigPaths->pathTextures);
        pShaderManager->SetBasePath(pConfigPaths->pathShaders);
    }

    void Engine::Run(bool startSimulation)
    {
        std::shared_ptr<GameStateComponent> pState = CoreLocator::GetGameState();
        // Just need to set current state and the update will trigger the event change
        pState->currState = eGameStates::STARTED;
        if (startSimulation)
        {
            pState->currState = eGameStates::RUNNING;
        }

        // HACK: State system must be first of all systems to trigger the appropiate system changes before other systems
        // make irreversible changes to the scene
        pState->mainSystems.insert(pState->mainSystems.begin(), "StateSystem");
        AddSystem("CoreSystem");

        m_isRunning = true;

        StartSystems(m_pScene);

        while (m_isRunning)
        {
            Update();

            Render();
        }
    }

    void Engine::Update()
    {
        float deltaTime = GetDeltaTime();

        for (int i = 0; i < m_vecSystems.size(); i++)
        {
            m_vecSystems[i]->Update(m_pScene, deltaTime);
        }

        m_ClearFrame();
    }

    void Engine::UpdateFixed()
    {
    }

    void Engine::Render()
    {
        m_BeginFrame();

        for (int i = 0; i < m_vecSystems.size(); i++)
        {
            m_vecSystems[i]->Render(m_pScene);
        }

        m_EndFrame();
    }

    void Engine::Shutdown()
    {
        for (int i = 0; i < m_vecSystems.size(); i++)
        {
            std::shared_ptr<iSystem> pSystem = m_vecSystems[i];
            pSystem->End(m_pScene);

            pSystem->Shutdown();
        }

        m_vecSystems.clear();
    }

    void Engine::LoadConfigurations()
    {
        iConfigSerializer* pConfigSerializer = ConfigSerializerFactory::CreateConfigSerializer(DEFAULT_CONFIG);
        std::shared_ptr<ConfigPathComponent> pConfigPath = CoreLocator::GetConfigPath();

        pConfigSerializer->DeserializeConfig(DEFAULT_CONFIG);
        delete pConfigSerializer;


        iSceneSerializer* pSceneSerializer = SceneSerializerFactory::CreateSceneSerializer(DEFAULT_SCENE);

        pSceneSerializer->DeserializeScene(pConfigPath->pathScenes + DEFAULT_SCENE, *(m_pScene));
        delete pSceneSerializer;
    }

    void Engine::StartSystems(std::shared_ptr<Scene> pScene)
    {
        for (int i = 0; i < m_vecSystems.size(); i++)
        {
            std::shared_ptr<iSystem> pSystem = m_vecSystems[i];
            pSystem->Start(pScene);
        }
    }

    void Engine::EndSystems(std::shared_ptr<Scene> pScene)
    {
        for (int i = 0; i < m_vecSystems.size(); i++)
        {
            std::shared_ptr<iSystem> pSystem = m_vecSystems[i];
            pSystem->End(pScene);
        }
    }

    void Engine::ShutdownSystems()
    {
        for (int i = 0; i < m_vecSystems.size(); i++)
        {
            std::shared_ptr<iSystem> pSystem = m_vecSystems[i];
        pSystem->Shutdown();
        }
    }

    void Engine::OnWindowClose(const WindowCloseEvent& event)
    {
        m_isRunning = false;
    }

    void Engine::m_ClearFrame()
    {
        m_pScene->m_DestroyEntities();
        m_pScene->m_DestroyComponents();
    }

    void Engine::m_BeginFrame()
    {
        std::shared_ptr<WindowComponent> pWindow = GraphicsLocator::GetWindow();

        if (!pWindow->pGLFWWindow)
        {
            // No window created
            return;
        }

        // Clear frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // ImGui init frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        window_flags |= ImGuiWindowFlags_NoBackground;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);

        // Start Imgui window context
        ImGui::Begin("ImGuiMainWindow", NULL, window_flags);

        ImGuizmo::BeginFrame();

        ImGuiIO& io = ImGui::GetIO();
        ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

        ImGuizmo::Enable(true);

        // Set default font size
        ImGui::SetWindowFontScale(TEXT_FONT_SIZE);
    }

    void Engine::m_EndFrame()
    {
        std::shared_ptr<WindowComponent> pWindow = GraphicsLocator::GetWindow();

        if (!pWindow->pGLFWWindow)
        {
            // No window created
            return;
        }

        // ImGui endframe
        ImGui::End();

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)pWindow->width, (float)pWindow->height);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        // GLFW endframe
        glfwSwapBuffers(pWindow->pGLFWWindow);
        glfwPollEvents();
    }
}
