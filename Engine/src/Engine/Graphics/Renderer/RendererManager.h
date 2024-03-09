#pragma once

#include "Engine/Graphics/Renderer/iRendererManager.h"

#include <map>
#include <vector>

namespace MyEngine
{
	class RendererManager : public iRendererManager
	{
	public:
		RendererManager();
		virtual ~RendererManager() {};

		// Add model to rendering pipeline into the respective FBO
		virtual void AddToRender( const sRenderModelInfo& renderInfo);

		// Render all models mapped into their respective FBOs
		virtual void RenderAllModels(std::shared_ptr<Scene> pScene);

		// Clear all model mapping from all FBOs
		virtual void ClearRender();

	private:
		std::vector<sRenderModelInfo> m_vecRenderInfos;

		// Render all models from list models
		void m_RenderList(const std::vector<sRenderModelInfo>& renderInfos);

		// Calculate projection and view mats and bind to shader
		void m_UpdateCamera(std::shared_ptr<Scene> pScene);
	};
}
