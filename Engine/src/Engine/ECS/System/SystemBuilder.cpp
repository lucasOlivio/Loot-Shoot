#include "pch.h"

#include "SystemBuilder.h"

#include "Engine/Core/CoreSystem.h"
#include "Engine/Core/FPSSystem.h"
#include "Engine/Core/TimerSystem.h"
#include "Engine/Core/InputSystem.h"
#include "Engine/Core/StateSystem.h"

#include "Engine/Graphics/WindowSystem.h"
#include "Engine/Graphics/ModelRenderSystem.h"
#include "Engine/Graphics/Animations/AnimationSystem.h"
#include "Engine/Graphics/Animations/AnimationPlayerSystem.h"
#include "Engine/Graphics/Lights/LightSystem.h"
#include "Engine/Graphics/Shaders/ShaderSystem.h"
#include "Engine/Graphics/Particles/ParticleSystem.h"
#include "Engine/Graphics/UI/BaseUISystem.h"

#include "Engine/Physics/MovementSystem.h"
#include "Engine/Physics/RotationSystem.h"
#include "Engine/Physics/BroadPhaseSystem.h"
#include "Engine/Physics/NarrowPhaseSystem.h"

#include "Engine/Gameplay/SteeringBehaviorSystem.h"

namespace MyEngine
{
    std::map<std::string, SystemBuilder::SystemFactory> SystemBuilder::m_systemFactories
    {
        // Any other system from engine should come into this map
        // 
        // Core Systems
        {"CoreSystem", []() { return new CoreSystem(); }},
        {"FPSSystem", []() { return new FPSSystem(); }},
        {"TimerSystem", []() { return new TimerSystem(); }},
        {"InputSystem", []() { return new InputSystem(); }},
        {"StateSystem", []() { return new StateSystem(); }},
        // Graphics systems
        {"WindowSystem", []() { return new WindowSystem(); }},
        {"BaseUISystem", []() { return new BaseUISystem(); }},
        {"ShaderSystem", []() { return new ShaderSystem(); }},
        {"ModelRenderSystem", []() { return new ModelRenderSystem(); }},
        {"LightSystem", []() { return new LightSystem(); }},
        {"AnimationSystem", []() { return new AnimationSystem(); }},
        {"AnimationPlayerSystem", []() { return new AnimationPlayerSystem(); }},
        {"ParticleSystem", []() { return new ParticleSystem(); }},
        // Physics systems
        {"MovementSystem", []() { return new MovementSystem(); }},
        {"RotationSystem", []() { return new RotationSystem(); }},
        {"BroadPhaseSystem", []() { return new BroadPhaseSystem(); }},
        {"NarrowPhaseSystem", []() { return new NarrowPhaseSystem(); }},
        // Gameplay
        {"SteeringBehaviorSystem", []() { return new SteeringBehaviorSystem(); }}
    };

    void SystemBuilder::RegisterSystem(const std::string& name, SystemFactory factory)
    {
        m_systemFactories[name] = factory;
    }

    std::shared_ptr<iSystem> SystemBuilder::CreateSystem(const std::string& name)
    {
        auto it = m_systemFactories.find(name);
        if (it != m_systemFactories.end())
        {
            // Call the factory function to create an instance
            return std::shared_ptr<iSystem>(it->second());
        }
        else
        {
            LOG_ERROR("Unknown system '" + name);
            return nullptr;
        }
    }


}
