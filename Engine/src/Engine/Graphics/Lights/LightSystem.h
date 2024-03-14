#pragma once

#include "Engine/Core/Components/Components.h"
#include "Engine/Core/Resources/Shaders/ShaderManager.h"

#include "Engine/ECS/System/EntitySystem.h"

#include "Engine/Graphics/Components/Components.h"

namespace MyEngine
{
	class LightSystem : public EntitySystem
	{
	public:
		LightSystem() = default;
		virtual ~LightSystem() { };

		virtual std::string SystemName() { return "LightSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();

		virtual void SetSystemMask(std::shared_ptr<Scene> pScene);

	private:
		// Setup the UL locations and all light configurations
		void m_SetupLight(TransformComponent& transform, LightComponent& light, int lightIndex);

		void m_UpdatePositionUL(TransformComponent& transform, 
								LightComponent& light, 
								std::shared_ptr<ShaderManager> pShader);
		void m_UpdateDirectionUL(LightComponent& light, std::shared_ptr<ShaderManager> pShader);
		void m_UpdateDiffuseUL(LightComponent& light, std::shared_ptr<ShaderManager> pShader);
		void m_UpdateSpecularUL(LightComponent& light, std::shared_ptr<ShaderManager> pShader);
		void m_UpdateAttenUL(LightComponent& light, std::shared_ptr<ShaderManager> pShader);
		void m_UpdateParamsUL(LightComponent& light, std::shared_ptr<ShaderManager> pShader);
		void m_UpdateStatusUL(LightComponent& light, std::shared_ptr<ShaderManager> pShader);
	};
}
