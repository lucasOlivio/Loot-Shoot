#include "pch.h"

#include "Engine.h"

#include "Engine/Core/Configs/ConfigSerializerFactory.h"
#include "Engine/Core/Components/CoreLocator.h"

#include "Engine/Debug/Components/DebugLocator.h"

#include "Engine/Events/EventBus.hpp"
#include "Engine/Events/EventBusLocator.hpp"

#include "Engine/ECS/Scene/SceneManager.h"
#include "Engine/ECS/Scene/SceneManagerLocator.h"
#include "Engine/ECS/System/SystemBuilder.h"    

#include "Engine/Graphics/VAO/VAOManagerLocator.h"
#include "Engine/Graphics/VAO/VAOManager.h"
#include "Engine/Graphics/Shaders/ShaderManager.h"
#include "Engine/Graphics/Shaders/ShaderManagerLocator.h"
#include "Engine/Graphics/Textures/cBasicTextureManager.h"
#include "Engine/Graphics/Textures/TextureManagerLocator.h"
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

    Engine::Engine() : m_ebWindow(new EventBus<eWindowEvents, WindowCloseEvent>()),
                       m_ebRigidCollision(new EventBus<eCollisionEvents, RigidBodyCollisionEvent>()),
                       m_ebSoftCollision(new EventBus<eCollisionEvents, SoftBodyCollisionEvent>()),
                       m_ebKeyboard(new EventBus<eInputEvents, KeyboardEvent>()),
                       m_ebMouse(new EventBus<eInputEvents, MouseEvent>()),
                       m_ebSceneChange(new EventBus<eSceneEvents, SceneChangeEvent>()),
                       m_ebStartedState(new EventBus<eGameStateEvents, GameStartedEvent>()),
                       m_ebStoppedState(new EventBus<eGameStateEvents, GameStoppedEvent>()),
                       m_ebRunningState(new EventBus<eGameStateEvents, GameRunningEvent>()),
                       m_ebGameOverState(new EventBus<eGameStateEvents, GameOverEvent>()),
                       m_ebPosKeyFrame(new EventBus<eAnimationEvents, PositionKeyFrameEvent>()),
                       m_ebRotKeyFrame(new EventBus<eAnimationEvents, RotationKeyFrameEvent>()),
                       m_ebScaleKeyFrame(new EventBus<eAnimationEvents, ScaleKeyFrameEvent>()),
                       m_sceneManager(new SceneManager()),
                       m_VAOManager(new VAOManager()),
                       m_shaderManager(new ShaderManager()),
                       m_textureManager(new cBasicTextureManager()),
                       m_rendererManager(new RendererManager()),
                       m_particleManager(new ParticleManager())

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

        // Setting up events
        EventBusLocator<eWindowEvents, WindowCloseEvent>::Set(m_ebWindow);
        EventBusLocator<eCollisionEvents, RigidBodyCollisionEvent>::Set(m_ebRigidCollision);
        EventBusLocator<eCollisionEvents, SoftBodyCollisionEvent>::Set(m_ebSoftCollision);
        EventBusLocator<eInputEvents, KeyboardEvent>::Set(m_ebKeyboard);
        EventBusLocator<eInputEvents, MouseEvent>::Set(m_ebMouse);
        EventBusLocator<eSceneEvents, SceneChangeEvent>::Set(m_ebSceneChange);
        EventBusLocator<eGameStateEvents, GameStartedEvent>::Set(m_ebStartedState);
        EventBusLocator<eGameStateEvents, GameStoppedEvent>::Set(m_ebStoppedState);
        EventBusLocator<eGameStateEvents, GameRunningEvent>::Set(m_ebRunningState);
        EventBusLocator<eGameStateEvents, GameOverEvent>::Set(m_ebGameOverState);
        EventBusLocator<eAnimationEvents, PositionKeyFrameEvent>::Set(m_ebPosKeyFrame);
        EventBusLocator<eAnimationEvents, RotationKeyFrameEvent>::Set(m_ebRotKeyFrame);
        EventBusLocator<eAnimationEvents, ScaleKeyFrameEvent>::Set(m_ebScaleKeyFrame);

        // Setting up resources managers        
        SceneManagerLocator::Set(m_sceneManager);
        VAOManagerLocator::Set(m_VAOManager);
        ShaderManagerLocator::Set(m_shaderManager);
        TextureManagerLocator::Set(m_textureManager);
        RendererManagerLocator::Set(m_rendererManager);
        ParticleManagerLocator::Set(m_particleManager);

        // Global events of engine interest
        m_ebSceneChange->Subscribe(eSceneEvents::CHANGE, [this](const SceneChangeEvent& event) { OnSceneChange(event); });
        m_ebWindow->Subscribe(eWindowEvents::WINDOW_CLOSE, [this](const WindowCloseEvent& event) { OnWindowClose(event); });

        // Load resources
        std::shared_ptr<ConfigPathComponent> pConfigPaths = CoreLocator::GetConfigPath();
        m_textureManager->SetBasePath(pConfigPaths->pathTextures);
        m_shaderManager->SetBasePath(pConfigPaths->pathShaders);
        m_VAOManager->SetBasePath(pConfigPaths->pathModels);
        m_sceneManager->SetBasePath(pConfigPaths->pathScenes);
    }

    void Engine::Run(std::string initialSceneName, bool startSimulation)
    {
        // HACK: State system must be first of all systems to trigger the appropiate system changes before other systems
        // make irreversible changes to the scene
        std::shared_ptr<GameStateComponent> pState = CoreLocator::GetGameState();
        pState->mainSystems.insert(pState->mainSystems.begin(), "StateSystem");
        AddSystem("CoreSystem");

        // TODO: Now each resource is been loaded by the systems, 
        // but they should be loaded all here and have separate files
        m_sceneManager->ChangeScene(initialSceneName);

        // Just need to set current state and the update will trigger the event change
        pState->currState = eGameStates::STARTED;
        if (startSimulation)
        {
            pState->currState = eGameStates::RUNNING;
        }

        m_isRunning = true;

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
            m_vecSystems[i]->Update(m_currentScene, deltaTime);
        }

        m_ClearFrame();
    }

    void Engine::Render()
    {
        m_BeginFrame();

        for (int i = 0; i < m_vecSystems.size(); i++)
        {
            m_vecSystems[i]->Render(m_currentScene);
        }

        m_EndFrame();
    }

    void Engine::Shutdown()
    {
        for (int i = 0; i < m_vecSystems.size(); i++)
        {
            std::shared_ptr<iSystem> pSystem = m_vecSystems[i];
            pSystem->End(m_currentScene);

            pSystem->Shutdown();
        }

        m_vecSystems.clear();
    }

    void Engine::LoadConfigurations()
    {
        iConfigSerializer* pConfigSerializer = ConfigSerializerFactory::CreateConfigSerializer(DEFAULT_CONFIG);
        std::shared_ptr<ConfigPathComponent> pConfigPath = CoreLocator::GetConfigPath();

        pConfigSerializer->DeserializeConfig(DEFAULT_CONFIG);
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

    void Engine::OnSceneChange(const SceneChangeEvent& event)
    {
        // End old scene
        if (m_currentScene)
        {
            EndSystems(m_currentScene);
        }

        // Start new scene
        m_currentScene = event.pNewScene;
        StartSystems(m_currentScene);
    }

    void Engine::OnWindowClose(const WindowCloseEvent& event)
    {
        m_isRunning = false;
    }

    void Engine::m_ClearFrame()
    {
        std::shared_ptr<iSceneManager> pSceneManager = SceneManagerLocator::Get();
        m_currentScene->m_DestroyEntities();
        m_currentScene->m_DestroyComponents();
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
