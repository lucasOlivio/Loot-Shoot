#include "pch.h"

#include "Engine.h"

#include "Engine/Core/Configs/ConfigSerializerFactory.h"
#include "Engine/Core/Components/CoreLocator.h"
#include "Engine/Core/Resources/ResourceManagerFactory.h"
#include "Engine/Core/Threads/ThreadPool.h"
#include "Engine/Core/Threads/ThreadPoolLocator.h"

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
#include "Engine/Utils/GameplayUtils.h"
#include "Engine/Utils/Random.h"

#include "Engine/Physics/Components/PhysicsLocator.h"

// timeBeginPeriod
#include <mmsystem.h> 
#pragma comment(lib, "winmm.lib")

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

        // Setting up services
        RendererManagerLocator::Set(m_rendererManager);
        ParticleManagerLocator::Set(m_particleManager);
        ThreadPoolLocator::GetOrCreate("update");
        ThreadPoolLocator::GetOrCreate("render");

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

        // Run update thread separated from render thread
        HANDLE updateHandle = CreateThread(NULL, 0, Engine::m_Update, this, 0, NULL);

        while (m_isRunning)
        {
            Render();
        }

        // Wait for the threads to finish
        DWORD dwWaitResult = WaitForSingleObject(updateHandle, INFINITE);
        if (dwWaitResult != WAIT_OBJECT_0)
        {
            LOG_ERROR("Wait failed or thread exited unexpectedly!");
        }

        // Close the threads
        CloseHandle(updateHandle);
    }

    bool Engine::IsRunning()
    {
        return m_isRunning;
    }

    void Engine::Update()
    {
        std::shared_ptr<iThreadPool> updateThreadPool = ThreadPoolLocator::GetOrCreate("update");

        while (m_isRunning)
        {
            float deltaTime = m_GetDeltaTime();

            for (int i = 0; i < m_vecSystems.size(); i++)
            {
                updateThreadPool->EnqueueTask([this, i, deltaTime]()
                {
                    m_vecSystems[i]->Update(m_pScene, deltaTime);
                });
                //m_vecSystems[i]->Update(m_pScene, deltaTime);
            }

            while (updateThreadPool->RunningTasks() > 0)
            {
                Sleep(0);
            }

            m_ClearFrame();

            // Calculate the time taken to process the frame
            float endTime = static_cast<float>(glfwGetTime());
            float frameTime = endTime - m_lastTime;

            // If the frame was processed faster than the min frame time, wait for the remaining time
            if (frameTime < MIN_FRAME_DURATION)
            {
                float diff = MIN_FRAME_DURATION - frameTime;
                DWORD sleepTime = static_cast<DWORD>(diff * 1000.0f);

                // Activate high precision sleeping using timeBeginPeriod
                // The OS usually can only Sleep for minimum of 15ms, if needed less we use this
                timeBeginPeriod(1);
                Sleep(sleepTime);
                timeEndPeriod(1);
            }
            else
            {
                Sleep(0);
            }
        }
    }

    void Engine::Render()
    {
        std::shared_ptr<iThreadPool> renderThreadPool = ThreadPoolLocator::GetOrCreate("render");

        m_BeginFrame();

        for (int i = 0; i < m_vecSystems.size(); i++)
        {
            renderThreadPool->EnqueueTask([this, i]()
            {
                m_vecSystems[i]->Render(m_pScene);
            });
            //m_vecSystems[i]->Render(m_pScene);
        }

        // Wait all systems send their render commands
        while (renderThreadPool->RunningTasks() > 0)
        {
            Sleep(0);
        }
        
        // Render all on queue
        std::shared_ptr<iRendererManager> pRenderer = RendererManagerLocator::Get();

        pRenderer->RenderAll(m_pScene);
        pRenderer->ClearRender();

        m_EndFrame();
    }

    void Engine::Shutdown()
    {
        ThreadPoolLocator::CloseAllPools();

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

    float Engine::m_GetDeltaTime()
    {
        float currentTime = static_cast<float>(glfwGetTime());
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

    void Engine::m_TriggerWindowClose()
    {
        WindowCloseEvent windowEvent = WindowCloseEvent();
        PUBLISH_WINDOW_CLOSE_EVENT(windowEvent);
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

        if (glfwWindowShouldClose(pWindow->pGLFWWindow))
        {
            m_TriggerWindowClose();
        }

        // Update window title
        if (pWindow->prevName != pWindow->name)
        {
            glfwSetWindowTitle(pWindow->pGLFWWindow, pWindow->name.c_str());
            pWindow->prevName = pWindow->name;
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

    DWORD __stdcall Engine::m_Update(LPVOID lpParam)
    {
        Engine* pEngine = static_cast<Engine*>(lpParam);
        pEngine->Update();

        return 0;
    }
}
