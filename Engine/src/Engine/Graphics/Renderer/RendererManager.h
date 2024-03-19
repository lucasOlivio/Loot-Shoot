#pragma once

#include "Engine/Core/Resources/Meshes/Mesh.h"
#include "Engine/Graphics/Renderer/iRendererManager.h"

#include "Engine/ThreadSafe.h"

#include <map>
#include <vector>

namespace MyEngine
{
	class RendererManager : public iRendererManager,
							public ThreadSafe
	{
	public:
		RendererManager();
		virtual ~RendererManager() {};

		// Add model to rendering pipeline
		virtual void AddToRender(const sRenderModelInfo& renderInfo);

		// Render all models listed
		virtual void RenderAll(std::shared_ptr<Scene> pScene);

		// Clear all model listing
		virtual void ClearRender();

	private:
		// Array buffer for all meshes to render information
		uint m_bufferId;

		std::vector<sRenderModelInfo> m_vecRenderModelInfos;

		// Calculate projection and view mats and bind to shader
		void m_UpdateCamera(std::shared_ptr<Scene> pScene);
	};
}
