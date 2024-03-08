#pragma once

#include "Engine/Graphics/Components/SingletonComponents.h"

namespace MyEngine
{
	class GraphicsLocator
	{
	public:
		static std::shared_ptr<WindowComponent> GetWindow();
		static std::shared_ptr<AnimationControllerComponent> GetAnimationController();

		static void SetWindow(std::shared_ptr<WindowComponent> pWindow);
		static void SetAnimationController(std::shared_ptr<AnimationControllerComponent> animationController);

	private:
		static std::shared_ptr<WindowComponent> m_pWindow;
		static std::shared_ptr<AnimationControllerComponent> m_animationController;
	};
}
