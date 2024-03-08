#include "pch.h"

#include "AnimationPlayerSystem.h"

#include "Engine/Core/Components/Components.h"

#include "Engine/Graphics/Components/GraphicsLocator.h"
#include "Engine/Graphics/Components/Components.h"

#include "Engine/ECS/Scene/SceneView.hpp"

#include "Engine/Events/EventBusLocator.hpp"
#include "Engine/Events/AnimationEvent.h"

#include "Engine/Utils/AnimationUtils.h"

namespace MyEngine
{
    void AnimationPlayerSystem::Init()
    {
    }

    void AnimationPlayerSystem::Start(std::shared_ptr<Scene> pScene)
    {
        std::shared_ptr<AnimationControllerComponent> pAnimController = GraphicsLocator::GetAnimationController();

        // Get the first and last key frame values and reset animations keyframes
        for (Entity entityId : SceneView<TransformComponent, TransformAnimationComponent>(*pScene))
        {
            TransformAnimationComponent& animation = pScene->Get<TransformAnimationComponent>(entityId);

            for (const PositionKeyFrame& keyframe : animation.positionKeyFrames)
            {
                if (keyframe.time < pAnimController->timeFirstKeyFrame)
                {
                    pAnimController->timeFirstKeyFrame = keyframe.time;
                }
                if (keyframe.time > pAnimController->timeLastKeyFrame)
                {
                    pAnimController->timeLastKeyFrame = keyframe.time;
                }
            }

            for (const RotationKeyFrame& keyframe : animation.rotationKeyFrames)
            {
                if (keyframe.time < pAnimController->timeFirstKeyFrame)
                {
                    pAnimController->timeFirstKeyFrame = keyframe.time;
                }
                if (keyframe.time > pAnimController->timeLastKeyFrame)
                {
                    pAnimController->timeLastKeyFrame = keyframe.time;
                }
            }

            for (const ScaleKeyFrame& keyframe : animation.scaleKeyFrames)
            {
                if (keyframe.time < pAnimController->timeFirstKeyFrame)
                {
                    pAnimController->timeFirstKeyFrame = keyframe.time;
                }
                if (keyframe.time > pAnimController->timeLastKeyFrame)
                {
                    pAnimController->timeLastKeyFrame = keyframe.time;
                }
            }
        }
    }

    void AnimationPlayerSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        std::shared_ptr<AnimationControllerComponent> pAnimController = GraphicsLocator::GetAnimationController();

        // First reset all animations if needed
        if (pAnimController->reset)
        {
            for (Entity entityId : SceneView<TransformComponent, TransformAnimationComponent>(*pScene))
            {
                TransformAnimationComponent& animation = pScene->Get<TransformAnimationComponent>(entityId);
                
                // If animation in reverse then go to last frame
                if (pAnimController->speed < 0)
                {
                    animation.time = pAnimController->timeLastKeyFrame;
                }
                else
                {
                    animation.time = pAnimController->timeFirstKeyFrame;
                }
            }

            pAnimController->reset = false;
        }

        // Then update animation timers
        for (Entity entityId : SceneView<TransformComponent, TransformAnimationComponent>(*pScene))
        {
            TransformAnimationComponent& animation = pScene->Get<TransformAnimationComponent>(entityId);
            if (!animation.isActive)
            {
                continue;
            }

            int startPos1 = 0;
            int endPos1 = 0;
            int startScale1 = 0;
            int endScale1 = 0;
            int startRot1 = 0;
            int endRot1 = 0;
            int startPos2 = 0;
            int endPos2 = 0;
            int startScale2 = 0;
            int endScale2 = 0;
            int startRot2 = 0;
            int endRot2 = 0;

            // Get keyframes before time increase
            AnimationUtils::GetKeyFrames<PositionKeyFrame>(animation.time, animation.positionKeyFrames,
								                           startPos1, endPos1);
            AnimationUtils::GetKeyFrames<ScaleKeyFrame>(animation.time, animation.scaleKeyFrames,
								                           startScale1, endScale1);
            AnimationUtils::GetKeyFrames<RotationKeyFrame>(animation.time, animation.rotationKeyFrames,
								                           startRot1, endRot1);

            animation.time += (deltaTime * pAnimController->speed);

            // Get keyframes after time increase
            AnimationUtils::GetKeyFrames<PositionKeyFrame>(animation.time, animation.positionKeyFrames,
								                           startPos2, endPos2);
            AnimationUtils::GetKeyFrames<ScaleKeyFrame>(animation.time, animation.scaleKeyFrames,
								                           startScale2, endScale2);
            AnimationUtils::GetKeyFrames<RotationKeyFrame>(animation.time, animation.rotationKeyFrames,
								                           startRot2, endRot2);

            // Set current keyframes
            animation.currStartPosKF = startPos2;
            animation.currStartRotKF = startRot2;
            animation.currStartScaKF = startScale2;
            animation.currEndPosKF = endPos2;
            animation.currEndRotKF = endRot2;
            animation.currEndScaKF = endScale2;

            // Check if passed frame and if is event frame
            if (endPos1 > -1 && animation.positionKeyFrames[endPos1].isKeyEvent)
            {
                if (startPos1 != endPos2 && endPos1 == startPos2)
                {
                    m_TriggerPosKeyFrameEvent(entityId, pScene, startPos1, endPos1, endPos2);
                }
            }

            if (endScale1 > -1 && animation.scaleKeyFrames[endScale1].isKeyEvent)
            {
                if (startScale1 != endScale2 && endScale1 == startScale2)
                {
                    m_TriggerScaleKeyFrameEvent(entityId, pScene, startScale1, endScale1, endScale2);
                }
            }

            if (endRot1 > -1 && animation.rotationKeyFrames[endRot1].isKeyEvent)
            {
                if (startRot1 != endRot2 && endRot1 == startRot2)
                {
                    m_TriggerRotKeyFrameEvent(entityId, pScene, startRot1, endRot1, endRot2);
                }
            }
        }
    }

    void AnimationPlayerSystem::Render(std::shared_ptr<Scene> pScene)
    {
    }

    void AnimationPlayerSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

    void AnimationPlayerSystem::Shutdown()
    {
    }

    void AnimationPlayerSystem::m_TriggerPosKeyFrameEvent(Entity entityId, std::shared_ptr<Scene> pScene,
        int oldKeyFrame,
        int keyFrame, int nextKeyFrame)
    {
        std::shared_ptr<iEventBus<eAnimationEvents, PositionKeyFrameEvent>> pEventBus = EventBusLocator<eAnimationEvents, PositionKeyFrameEvent>::Get();

        PositionKeyFrameEvent keyEvent = PositionKeyFrameEvent();
        keyEvent.entityId = entityId;
        keyEvent.pScene = pScene;
        keyEvent.oldKeyFrame = oldKeyFrame;
        keyEvent.keyFrame = keyFrame;
        keyEvent.nextKeyFrame = nextKeyFrame;
        pEventBus->Publish(keyEvent);
    }

    void AnimationPlayerSystem::m_TriggerRotKeyFrameEvent(Entity entityId, std::shared_ptr<Scene> pScene,
        int oldKeyFrame,
        int keyFrame, int nextKeyFrame)
    {
        std::shared_ptr<iEventBus<eAnimationEvents, RotationKeyFrameEvent>> pEventBus = EventBusLocator<eAnimationEvents, RotationKeyFrameEvent>::Get();

        RotationKeyFrameEvent keyEvent = RotationKeyFrameEvent();
        keyEvent.entityId = entityId;
        keyEvent.pScene = pScene;
        keyEvent.oldKeyFrame = oldKeyFrame;
        keyEvent.keyFrame = keyFrame;
        keyEvent.nextKeyFrame = nextKeyFrame;
        pEventBus->Publish(keyEvent);
    }

    void AnimationPlayerSystem::m_TriggerScaleKeyFrameEvent(Entity entityId, std::shared_ptr<Scene> pScene,
										                  int oldKeyFrame, 
										                  int keyFrame, int nextKeyFrame)
    {
        std::shared_ptr<iEventBus<eAnimationEvents, ScaleKeyFrameEvent>> pEventBus = EventBusLocator<eAnimationEvents, ScaleKeyFrameEvent>::Get();

        ScaleKeyFrameEvent keyEvent = ScaleKeyFrameEvent();
        keyEvent.entityId = entityId;
        keyEvent.pScene = pScene;
        keyEvent.oldKeyFrame = oldKeyFrame;
        keyEvent.keyFrame = keyFrame;
        keyEvent.nextKeyFrame = nextKeyFrame;
        pEventBus->Publish(keyEvent);
    }
}
