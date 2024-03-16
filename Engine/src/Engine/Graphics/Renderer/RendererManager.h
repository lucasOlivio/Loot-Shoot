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

		// Add particle to rendering pipeline
		virtual void AddToRender(const sRenderParticleInfo& renderInfo);

		// Add model to rendering pipeline
		virtual void AddToRender( const sRenderModelInfo& renderInfo);

		// Render all models listed
		virtual void RenderAll(std::shared_ptr<Scene> pScene);

		// Clear all model listing
		virtual void ClearRender();

	private:
		std::vector<sRenderModelInfo> m_vecRenderInfos;
		std::vector<sRenderParticleInfo> m_vecRenderParticleInfos;

		// Render all models from list models
		void m_RenderList(const std::vector<sRenderModelInfo>& renderInfos);
		void m_RenderList(const std::vector<sRenderParticleInfo>& renderInfos);

		// Calculate projection and view mats and bind to shader
		void m_UpdateCamera(std::shared_ptr<Scene> pScene);
	};
}
