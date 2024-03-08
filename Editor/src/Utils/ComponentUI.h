#pragma once

#include "Engine/ECS/Base.h"
#include "Engine/ECS/Scene/Scene.h"

#include <map>
#include <string>
#include <functional>

namespace MyEngine
{
	using ComponentUIHandler = std::function<void(Scene&, Entity)>;

	class ComponentUI
	{
	public:
		static void GenerateComponentsUI(Scene& scene, Entity entityId);

		// Store components id -> UIhandler to all components in cache
		static void MapComponentsUI(Scene& scene);

		static ComponentUIHandler GetComponentUI(ComponentType component);

	private:
		static std::map<ComponentId, ComponentUIHandler> m_componentsUI;

		static void m_EmptyUI(Scene& scene, Entity entityId);

		static void m_TagUI(Scene& scene, Entity entityId);
		static void m_CameraUI(Scene& scene, Entity entityId);
		static void m_GridbroadPhaseUI(Scene& scene, Entity entityId);
		static void m_TransformUI(Scene& scene, Entity entityId);
		static void m_MovementUI(Scene& scene, Entity entityId);
		static void m_RotationUI(Scene& scene, Entity entityId);
		static void m_ParentUI(Scene& scene, Entity entityId);
		static void m_TextureUI(Scene& scene, Entity entityId);
		static void m_MaterialUI(Scene& scene, Entity entityId);
		static void m_EmitterUI(Scene& scene, Entity entityId);
		static void m_LightUI(Scene& scene, Entity entityId);
		static void m_ModelUI(Scene& scene, Entity entityId);
		static void m_FrameBufferUI(Scene& scene, Entity entityId);
		static void m_FrameBufferViewUI(Scene& scene, Entity entityId);
		static void m_TransformAnimationUI(Scene& scene, Entity entityId);
		static void m_TilingUI(Scene& scene, Entity entityId);
		static void m_GravityUI(Scene& scene, Entity entityId);
		static void m_RigidBodyUI(Scene& scene, Entity entityId);
		static void m_MeshColliderUI(Scene& scene, Entity entityId);
		static void m_SphereColliderUI(Scene& scene, Entity entityId);
		static void m_AABBColliderUI(Scene& scene, Entity entityId);
		static void m_SoftBodyUI(Scene& scene, Entity entityId);
		static void m_PlayerUI(Scene& scene, Entity entityId);
		static void m_SteeringBehaviorUI(Scene& scene, Entity entityId);

	};
}
