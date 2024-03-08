#pragma once

#include "Engine/Core/Components/Components.h"

#include "Engine/ECS/System/iSystem.h"
#include "Engine/ECS/Components.h"

#include "Engine/Graphics/Shaders/ShaderManager.h"

namespace MyEngine
{
	class LightSystem : public iSystem
	{
	public:
		LightSystem() = default;
		virtual ~LightSystem() { };

		virtual std::string SystemName() { return "LightSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene) {};

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();

	private:
		// Setup the UL locations and all light configurations
		void m_SetupLight(TransformComponent& transform, LightComponent& light, int lightIndex);

		void m_UpdatePositionUL(TransformComponent& transform, 
								LightComponent& light, 
								std::shared_ptr<iShaderProgram> pShader);
		void m_UpdateDirectionUL(LightComponent& light, std::shared_ptr<iShaderProgram> pShader);
		void m_UpdateDiffuseUL(LightComponent& light, std::shared_ptr<iShaderProgram> pShader);
		void m_UpdateSpecularUL(LightComponent& light, std::shared_ptr<iShaderProgram> pShader);
		void m_UpdateAttenUL(LightComponent& light, std::shared_ptr<iShaderProgram> pShader);
		void m_UpdateParamsUL(LightComponent& light, std::shared_ptr<iShaderProgram> pShader);
		void m_UpdateStatusUL(LightComponent& light, std::shared_ptr<iShaderProgram> pShader);
	};
}
