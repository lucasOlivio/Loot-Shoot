#include "pch.h"

#include "TransformParentSystem.h"
#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/ECS/Components.h"
#include "Engine/Utils/TransformUtils.h"

namespace MyEngine
{
    void TransformParentSystem::Init()
    {
    }

    void TransformParentSystem::Start(std::shared_ptr<Scene> pScene)
    {
    }

    void TransformParentSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        // Update each local to world transforms
        for (Entity entityId : SceneView<TransformComponent>(*pScene))
        {
            TransformComponent& transform = pScene->Get<TransformComponent>(entityId);

            bool hasParent = false;
            ParentComponent& parent = pScene->Get<ParentComponent>(entityId, hasParent);

            if (!hasParent)
            {
                // Entity have no parent so world and local transforms are the same
                transform.worldPosition = transform.position;
                transform.worldOrientation = transform.orientation;
                transform.worldScale = transform.scale;
                
                continue;
            }

            TransformComponent& transformParent = pScene->Get<TransformComponent>(parent.parentId);

            glm::mat4 matParent = glm::mat4(1.0f);
            TransformUtils::GetTransform(transformParent.worldPosition, 
                                         transformParent.worldOrientation,
                                         transformParent.worldScale, 
                                         matParent);

            transform.worldPosition = matParent * glm::vec4(transform.position, 1.0f);
            transform.worldScale = transform.scale * transformParent.scale;
            transform.worldOrientation = transform.orientation * transformParent.worldOrientation;
        }
    }

    void TransformParentSystem::Render(std::shared_ptr<Scene> pScene)
    {
    }

    void TransformParentSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

    void TransformParentSystem::Shutdown()
    {
    }
}
