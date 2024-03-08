#include "pch.h"

#include "GraphicsLocator.h"

namespace MyEngine
{
    std::shared_ptr<WindowComponent> GraphicsLocator::m_pWindow(new WindowComponent());
    std::shared_ptr<AnimationControllerComponent> GraphicsLocator::m_animationController(new AnimationControllerComponent());

    std::shared_ptr<WindowComponent> GraphicsLocator::GetWindow()
    {
        return m_pWindow;
    }

    std::shared_ptr<AnimationControllerComponent> GraphicsLocator::GetAnimationController()
    {
        return m_animationController;
    }

    void GraphicsLocator::SetWindow(std::shared_ptr<WindowComponent> pWindow)
    {
        m_pWindow = pWindow;
    }

    void GraphicsLocator::SetAnimationController(std::shared_ptr<AnimationControllerComponent> animationController)
    {
        m_animationController = animationController;
    }
}
