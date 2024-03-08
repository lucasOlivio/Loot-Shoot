#include "pch.h"

#include "AnimationSystem.h"

#include "Engine/Core/Components/Components.h"

#include "Engine/Graphics/Components/Components.h"

#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/ECS/Components.h"

#include "Engine/Utils/AnimationUtils.h"

namespace MyEngine
{
    void AnimationSystem::Init()
    {
    }

    void AnimationSystem::Start(std::shared_ptr<Scene> pScene)
    {
        for (Entity entityId : SceneView<TransformComponent, TransformAnimationComponent>(*pScene))
        {
            TransformAnimationComponent& animation = pScene->Get<TransformAnimationComponent>(entityId);

            animation.time = 0.0f;
        }
    }

    void AnimationSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        for (Entity entityId : SceneView<TransformComponent, TransformAnimationComponent>(*pScene))
        {
            TransformComponent& transform = pScene->Get<TransformComponent>(entityId);
            TransformAnimationComponent& animation = pScene->Get<TransformAnimationComponent>(entityId);

            float currTime = animation.time;

            // Position
            AnimationUtils::InterpolateAndApply<PositionKeyFrame, glm::vec3>(animation.positionKeyFrames, 
                                                      currTime, animation.currStartPosKF, animation.currEndPosKF, transform.position);

            // Rotation
            AnimationUtils::InterpolateAndApply<RotationKeyFrame, glm::quat>(animation.rotationKeyFrames, 
                                                      currTime, animation.currStartRotKF, animation.currEndRotKF, transform.orientation);

            // Scale
            AnimationUtils::InterpolateAndApply<ScaleKeyFrame, float>(animation.scaleKeyFrames, 
                                                      currTime, animation.currStartScaKF, animation.currEndScaKF, transform.scale);
        }
    }

    void AnimationSystem::Render(std::shared_ptr<Scene> pScene)
    {
    }

    void AnimationSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

    void AnimationSystem::Shutdown()
    {
    }
}
