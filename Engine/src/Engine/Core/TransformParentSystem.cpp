#include "pch.h"

#include "TransformParentSystem.h"

#include "Engine/Core/Components/Components.h"

#include "Engine/Utils/TransformUtils.h"

namespace MyEngine
{
    void TransformParentSystem::Init()
    {
    }

    void TransformParentSystem::Start(std::shared_ptr<Scene> pScene)
    {
        EntitySystem::Start(pScene);
    }

    void TransformParentSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        // Update each local to world transforms
        for (Entity entityId : m_vecEntities)
        {
            TransformComponent& transform = pScene->Get<TransformComponent>(entityId);

            bool hasParent = false;
            ParentComponent& parent = pScene->Get<ParentComponent>(entityId, hasParent);

            if (!hasParent)
            {
                // Entity have no parent so world and local transforms are the same
                transform.LockWrite();
                transform.worldPosition = transform.position;
                transform.worldOrientation = transform.orientation;
                transform.worldScale = transform.scale;
                transform.UnlockWrite();
                
                continue;
            }

            parent.LockRead();
            bool hasTransformParent = false;
            TransformComponent& transformParent = pScene->Get<TransformComponent>(parent.parentId, hasTransformParent);
            parent.UnlockRead();
            if (hasTransformParent)
            {
                transformParent.LockRead();
                glm::mat4 matParent = glm::mat4(1.0f);
                TransformUtils::GetTransform(transformParent.worldPosition,
                    transformParent.worldOrientation,
                    transformParent.worldScale,
                    matParent);

                transform.LockWrite();
                transform.worldPosition = matParent * glm::vec4(transform.position, 1.0f);
                transform.worldScale = transform.scale * transformParent.scale;
                transform.worldOrientation = transform.orientation * transformParent.worldOrientation;
                transform.UnlockWrite();
                transformParent.UnlockRead();
            }
            else
            {
                // Parents transform was removed
                transform.LockWrite();
                transform.worldPosition = transform.position;
                transform.worldOrientation = transform.orientation;
                transform.worldScale = transform.scale;
                transform.UnlockWrite();
            }
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

    void TransformParentSystem::SetSystemMask(std::shared_ptr<Scene> pScene)
    {
        ComponentType transformType = pScene->GetComponentType<TransformComponent>();

        m_systemMask.set(transformType);
    }
}
