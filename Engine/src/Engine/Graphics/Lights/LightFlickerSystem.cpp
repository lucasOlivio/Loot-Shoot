#include "pch.h"

#include "LightFlickerSystem.h"

#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/ECS/Components.h"

#include "Engine/Graphics/Components/Components.h"

#include "Engine/Utils/Random.h"

namespace MyEngine
{
    void LightFlickerSystem::Init()
    {
    }

    void LightFlickerSystem::Start(std::shared_ptr<Scene> pScene)
    {
    }

    void LightFlickerSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        // Update flickering offset
        for (Entity entityId : SceneView<LightComponent>(*pScene))
        {
            LightComponent& light = pScene->Get<LightComponent>(entityId);

            if (light.params.w == 1)
            {
                // Modulate the light attenuation by changing the offset proportion randomly over time
                float max = 1.2f;
                float min = 0.8f;

                uint32_t seed = (uint32_t)rand();
                light.flickerOffset = glm::vec4(Random::Vec3(seed, min, max), 0);
            }
        }
    }

    void LightFlickerSystem::Render(std::shared_ptr<Scene> pScene)
    {
    }

    void LightFlickerSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

    void LightFlickerSystem::Shutdown()
    {
    }
}