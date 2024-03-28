#pragma once

#include "Engine/ECS/System/EntitySystem.h"

namespace MyEngine
{
	// TODO: This could be all just transform, not separated by "position, rotation scale",
	// this way gets too much repetition
	// 
	// Handles the time for all animations and key frame events triggering, 
	// decoupled from animation system to allow more options on how to run the animations
	class AnimationPlayerSystem : public EntitySystem
	{
	public:
		AnimationPlayerSystem() = default;
		virtual ~AnimationPlayerSystem() { };

		virtual std::string SystemName() { return "AnimationPlayerSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();

		virtual void SetSystemMask(std::shared_ptr<Scene> pScene);

	private:
		void m_TriggerPosKeyFrameEvent(Entity entityId, std::shared_ptr<Scene> pScene,
										int oldKeyFrame, 
										int keyFrame, int nextKeyFrame);
		void m_TriggerRotKeyFrameEvent(Entity entityId, std::shared_ptr<Scene> pScene,
										int oldKeyFrame, 
										int keyFrame, int nextKeyFrame);
		void m_TriggerScaleKeyFrameEvent(Entity entityId, std::shared_ptr<Scene> pScene,
										int oldKeyFrame, 
										int keyFrame, int nextKeyFrame);
	};
}
