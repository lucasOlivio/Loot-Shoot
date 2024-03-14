#include "pch.h"

#include "GameplayUtils.h"

#include "Engine/Core/Components/Components.h"
#include "Engine/Core/Resources/ResourceManagerFactory.h"

#include "Engine/Graphics/Components/Components.h"

#include "Engine/Physics/Components/Components.h"

#include "Engine/Utils/TransformUtils.h"
#include "Engine/Utils/Math.h"

namespace MyEngine
{
    const std::string CUBE_MODEL = "cube.ply";

	void GameplayUtils::CalculateSteeringDirections(const glm::vec3& myPosition, const glm::vec3& targetPosition, 
                                                   glm::quat& myOrientation, glm::vec3& myVelocity, 
                                                   const glm::vec3& targetVelocity, bool isFleeing, 
                                                   float speed, float maxDistance, 
                                                   float futureTime, StopConditionFunction stopCondition)
    {
        // Calculate direction to/from target
        glm::vec3 futurePosition = targetPosition + (targetVelocity * futureTime);
        glm::vec3 direction = futurePosition - myPosition;
        if (isFleeing)
        {
            direction = -direction;
        }

        // Normalize direction
        glm::vec3 lockedAxis = glm::vec3(1.0f, 0.0f, 1.0f);
        glm::vec3 directionNormal = glm::normalize(direction * lockedAxis);

        // Update orientation
        glm::vec3 currForward = TransformUtils::GetForwardVector(myOrientation);
        float angle = Angle(currForward, direction, UP_VECTOR);
        glm::vec3 yaw = angle * UP_VECTOR;
        myOrientation = TransformUtils::AdjustOrientation(myOrientation, yaw);

        // Update movement direction
        float distToTarget = glm::distance(futurePosition, myPosition);
        if (stopCondition(distToTarget, maxDistance))
        {
            myVelocity = glm::vec3(0.0f);

            return;
        }

        // HACK: Additional condition from m_ApproachTarget
        if (!isFleeing && distToTarget < maxDistance)
        {
            directionNormal = -directionNormal;
        }

        myVelocity = directionNormal * speed;
    }

    Entity GameplayUtils::CreateCube(float radius, 
                                    const glm::vec3& position, 
								    const glm::vec3& velocity, 
								    const glm::vec3& acceleration,
								    std::shared_ptr<Scene> pScene)
    {
        Entity cubeId = pScene->CreateEntityWithComponents<TransformComponent, MovementComponent,
                                                            RigidBodyComponent>();

        TransformComponent& transform = pScene->Get<TransformComponent>(cubeId);
        transform.position = position;
        transform.orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        transform.scale = 1.0f;

        MovementComponent& movement = pScene->Get<MovementComponent>(cubeId);
        movement.velocity = velocity;
        movement.acceleration = acceleration;
        movement.drag = 0.0f;
        movement.maxSpeed = glm::length(velocity);

        RigidBodyComponent& rigidBody = pScene->Get<RigidBodyComponent>(cubeId);
        rigidBody.bodyType = eBody::ACTIVE;
        rigidBody.radius = radius;

        //ModelComponent& model = pScene->Get<ModelComponent>(cubeId);
        //model.isActive = true;
        //model.doNotLight = false;
        //model.isWireframe = false;
        //model.isDynamic = false;
        //model.material = "";
        //model.useColorTexture = false;
        //model.useDefaultColor = true;
        //model.defaultColor = RED;
        //model.model = CUBE_MODEL;

        //// Loading mesh
        //std::shared_ptr<iResourceManager> pMeshManager = ResourceManagerFactory::GetOrCreate(eResourceTypes::MESH);
        //size_t index = pMeshManager->LoadResource(CUBE_MODEL);
        //model.pMesh = std::static_pointer_cast<sMeshInfo>(pMeshManager->GetResource(index));

        return cubeId;
    }
}
