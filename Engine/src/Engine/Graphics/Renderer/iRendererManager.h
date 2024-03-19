#pragma once

#include "Engine/ECS/Scene/Scene.h"

#include "Engine/Graphics/GraphicsProperties.h"

namespace MyEngine
{
	class iRendererManager
	{
	public:
		iRendererManager() {};
		virtual ~iRendererManager() {};

		// Add model to rendering pipeline
		virtual void AddToRender(const sRenderModelInfo& renderInfo) = 0;

		// Render all models
		virtual void RenderAll(std::shared_ptr<Scene> pScene) = 0;

		virtual void ClearRender() = 0;
	};
}
