#pragma once

#include "Engine/Graphics/Renderer/iRendererManager.h"

namespace MyEngine
{
	class NullRendererManager : public iRendererManager
	{
	public:
		NullRendererManager() {};
		virtual ~NullRendererManager() {};

		virtual void AddToRender(const sRenderParticleInfo& renderInfo)
		{
			LOG_ERROR("Renderer manager not initialized!");
		}

		virtual void AddToRender(const sRenderModelInfo& renderInfo)
		{
			LOG_ERROR("Renderer manager not initialized!");
		}

		virtual void AddToRenderTransparent(const sRenderModelInfo& renderInfo)
		{
			LOG_ERROR("Renderer manager not initialized!");
		}

		virtual void RenderAll(std::shared_ptr<Scene> pScene)
		{
			LOG_ERROR("Renderer manager not initialized!");
		}

		virtual void ClearRender()
		{
			LOG_ERROR("Renderer manager not initialized!");
		}
	};
}
