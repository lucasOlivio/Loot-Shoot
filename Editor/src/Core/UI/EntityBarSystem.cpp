#include "pch.h"

#include "EntityBarSystem.h"

#include "Utils/EntitySelector.h"
#include "Utils/ComponentUI.h"

#include "Engine/ECS/Components.h"
#include "Engine/ECS/Scene/SceneManagerLocator.h"
#include "Engine/ECS/SingletonComponents/CoreLocator.h"

#include "Engine/Events/EventBusLocator.hpp"

#include "Engine/Utils/Files.h"

namespace MyEngine
{
    void EntityBarSystem::Init()
    {
    }

    void EntityBarSystem::Start(std::shared_ptr<Scene> pScene)
    {
        ComponentUI::MapComponentsUI(*pScene);
    }

    void EntityBarSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
    }

    void EntityBarSystem::Render(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        Entity entityId = EntitySelector::GetSelectedEntity();
        std::string sliderTitle = "Entity #" + std::to_string(entityId);

        ImGui::AlignTextToFramePadding();
        if (ImGui::Button("REMOVE ENTITY"))
        {
            m_RemoveEntity(*pScene, entityId);
        }
        ImGui::SameLine();
        ImGui::Spacing();
        ImGui::SameLine();
        if (ImGui::Button("DUPLICATE ENTITY"))
        {
            m_CreateEntity(*pScene, entityId);
        }
        ImGui::SameLine();
        ImGui::Spacing();
        ImGui::SameLine();
        if (ImGui::Button("NEW ENTITY"))
        {
            m_CreateEntity(*pScene);
        }
        ImGui::SameLine();
        ImGui::Spacing();
        ImGui::SameLine();
        ImGui::Spacing();
        ImGui::SameLine();
        if (ImGui::Button("<"))
        {
            EntitySelector::PrevEntity(*pScene);
        }
        ImGui::SameLine();
        if (ImGui::Button(">"))
        {
            EntitySelector::NextEntity(*pScene);
        }
        ImGui::SameLine();
        bool treeEntity = ImGui::TreeNodeEx(sliderTitle.c_str(), ImGuiTreeNodeFlags_AllowItemOverlap);

        bool hasTag = false;
        TagComponent& tag = pScene->Get<TagComponent>(entityId, hasTag);
        if (hasTag)
        {
            ImGui::SameLine();
            ImGui::Text((" | TAG: " + tag.name).c_str());
        }

        ImGui::SameLine();
        ImGui::Spacing();
        ImGui::SameLine();
        m_AddComponentsList(*pScene, entityId);
        if (ImGui::Button("ADD COMPONENT"))
        {
            ImGui::OpenPopup("AddComponentsPopup");
        }

        ImGui::Columns(4, nullptr, false);
        if (treeEntity)
        {
            ComponentUI::GenerateComponentsUI(*pScene, entityId);
            ImGui::TreePop();
        }
        ImGui::Columns(1);
    }

    void EntityBarSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

    void EntityBarSystem::Shutdown()
    {
    }

    void EntityBarSystem::m_CreateEntity(Scene& scene)
    {
        Entity entityId = scene.CreateEntity(true);
        EntitySelector::SetSelectedEntity(entityId);
    }

    void EntityBarSystem::m_CreateEntity(Scene& scene, Entity entityId)
    {
        Entity newEntityId = scene.CreateEntity(entityId);
        EntitySelector::SetSelectedEntity(newEntityId);
    }

    void EntityBarSystem::m_RemoveEntity(Scene& scene, Entity entityId)
    {
        scene.RemoveEntity(entityId);
    }

    void EntityBarSystem::m_AddComponentsList(Scene& scene, Entity entityId)
    {
        if (ImGui::BeginPopup("AddComponentsPopup"))
        {
            static bool addTag = false;
            static bool addCamera = false;
            static bool addTransform = false;
            static bool addMovement = false;
            static bool addRotation = false;
            static bool addParent = false;
            static bool addTexture = false;
            static bool addMaterial = false;
            static bool addLight = false;
            static bool addModel = false;
            static bool addFrameBuffer = false;
            static bool addFrameBufferView = false;
            static bool addEmitter = false;
            static bool addTransformAnimation = false;
            static bool addTiling = false;
            static bool addGravity = false;
            static bool addRigidBody = false;
            static bool addMeshCollider = false;
            static bool addSphereCollider = false;
            static bool addAABBCollider = false;
            static bool addSoftBody = false;
            static bool addPlayer = false;
            static bool addSteeringBehavior = false;

            ImGui::Checkbox("Tag Component", &addTag);
            ImGui::Checkbox("Camera Component", &addCamera);
            ImGui::Checkbox("Transform Component", &addTransform);
            ImGui::Checkbox("Movement Component", &addMovement);
            ImGui::Checkbox("Rotation Component", &addRotation);
            ImGui::Checkbox("Parent Component", &addParent);
            ImGui::Checkbox("Texture Component", &addTexture);
            ImGui::Checkbox("Material Component", &addMaterial);
            ImGui::Checkbox("Light Component", &addLight);
            ImGui::Checkbox("Model Component", &addModel);
            ImGui::Checkbox("FrameBuffer Component", &addFrameBuffer);
            ImGui::Checkbox("FrameBufferView Component", &addFrameBufferView);
            ImGui::Checkbox("Emitter Component", &addEmitter);
            ImGui::Checkbox("TransformAnimation Component", &addTransformAnimation);
            ImGui::Checkbox("Tiling Component", &addTiling);
            ImGui::Checkbox("Gravity Component", &addGravity);
            ImGui::Checkbox("RigidBody Component", &addRigidBody);
            ImGui::Checkbox("MeshCollider Component", &addMeshCollider);
            ImGui::Checkbox("SphereCollider Component", &addSphereCollider);
            ImGui::Checkbox("AABBCollider Component", &addAABBCollider);
            ImGui::Checkbox("SoftBody Component", &addSoftBody);
            ImGui::Checkbox("Player Component", &addPlayer);
            ImGui::Checkbox("SteeringBehavior Component", &addSteeringBehavior);

            if (ImGui::Button("Add Selected Components"))
            {
                if (addTag) m_AddComponent<TagComponent>(scene, entityId);
                if (addCamera) m_AddComponent<CameraComponent>(scene, entityId);
                if (addTransform) m_AddComponent<TransformComponent>(scene, entityId);
                if (addMovement) m_AddComponent<MovementComponent>(scene, entityId);
                if (addRotation) m_AddComponent<RotationComponent>(scene, entityId);
                if (addParent) m_AddComponent<ParentComponent>(scene, entityId);
                if (addTexture) m_AddComponent<TextureComponent>(scene, entityId);
                if (addMaterial) m_AddComponent<MaterialComponent>(scene, entityId);
                if (addLight) m_AddComponent<LightComponent>(scene, entityId);
                if (addModel) m_AddComponent<ModelComponent>(scene, entityId);
                if (addFrameBuffer) m_AddComponent<FrameBufferComponent>(scene, entityId);
                if (addFrameBufferView) m_AddComponent<FrameBufferViewComponent>(scene, entityId);
                if (addEmitter) m_AddComponent<EmitterComponent>(scene, entityId);
                if (addTransformAnimation) m_AddComponent<TransformAnimationComponent>(scene, entityId);
                if (addTiling) m_AddComponent<TilingComponent>(scene, entityId);
                if (addGravity) m_AddComponent<GravityComponent>(scene, entityId);
                if (addRigidBody) m_AddComponent<RigidBodyComponent>(scene, entityId);
                if (addMeshCollider) m_AddComponent<MeshColliderComponent>(scene, entityId);
                if (addSphereCollider) m_AddComponent<SphereColliderComponent>(scene, entityId);
                if (addAABBCollider) m_AddComponent<AABBColliderComponent>(scene, entityId);
                if (addSoftBody) m_AddComponent<SoftBodyComponent>(scene, entityId);
                if (addPlayer) m_AddComponent<PlayerComponent>(scene, entityId);
                if (addSteeringBehavior) m_AddComponent<SteeringBehaviorComponent>(scene, entityId);

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
}
