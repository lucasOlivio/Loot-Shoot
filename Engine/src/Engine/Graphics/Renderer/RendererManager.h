#pragma once

#include "Engine/Graphics/Renderer/iRendererManager.h"

#include "Engine/Graphics/Materials/iMaterialManager.h"
#include "Engine/Graphics/Shaders/iShaderProgram.h"

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

		void m_RenderList(std::shared_ptr<iMaterialManager> pMaterialManager, 
						  const std::vector<sRenderModelInfo>& renderInfos);
	};
}
