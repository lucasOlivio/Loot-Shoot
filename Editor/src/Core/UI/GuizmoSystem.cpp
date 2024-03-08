#include "pch.h"

#include "GuizmoSystem.h"

#include "Utils/EntitySelector.h"
#include "Utils/UIWidgets.h"

namespace MyEngine
{
    void GuizmoSystem::Init()
    {
    }

    void GuizmoSystem::Start(std::shared_ptr<Scene> pScene)
    {
    }

    void GuizmoSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
    }

    void GuizmoSystem::Render(std::shared_ptr<Scene> pScene)
    {
        Entity entityId = EntitySelector::GetSelectedEntity();

        Guizmo(pScene, entityId);
    }

    void GuizmoSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

    void GuizmoSystem::Shutdown()
    {
    }
}
