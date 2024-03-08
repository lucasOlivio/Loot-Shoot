#include "pch.h"

#include "ComponentUI.h"

#include "Utils/UIWidgets.h"

#include "Engine/ECS/Components.h"
#include "Engine/ECS/SingletonComponents/Components.h"
#include "Engine/ECS/SingletonComponents/PhysicsLocator.h"

#include "Engine/Graphics/VAO/VAOManagerLocator.h"

namespace MyEngine
{
    std::map<ComponentId, ComponentUIHandler> ComponentUI::m_componentsUI = {};

    void ComponentUI::GenerateComponentsUI(Scene& scene, Entity entityId)
    {
        if (scene.GetEntityCount() == 1)
        {
            return;
        }

        // For all components that entity have get the UI generator from cache and draw UI
        EntityMask mask = scene.GetMask(entityId);
        for (int i = 0; i < MAX_COMPONENTS; i++)
        {
            if (!mask[i])
            {
                continue;
            }

            Separator();

            std::string removId = "Remove component##RemovCompEntt:" + std::to_string(entityId) + "Comp:" + std::to_string(i);
            if (ImGui::Button(removId.c_str()))
            {
                scene.RemoveComponent(entityId, i);
            }

            ImGui::SameLine();

            ComponentUIHandler handler = GetComponentUI(i);
            handler(scene, entityId);

            Separator();
        }

        if (entityId == 0)
        {
            // HACK: Singleton components come here for now
            ComponentType GridbroadPhaseType = scene.GetComponentType<GridBroadphaseComponent>();

            Separator();

            ImGui::SameLine();

            ComponentUIHandler handler = GetComponentUI(GridbroadPhaseType);
            handler(scene, entityId);

            Separator();
        }
    }

    void ComponentUI::MapComponentsUI(Scene& scene)
    {
        ComponentType TagType = scene.GetComponentType<TagComponent>();
        m_componentsUI[TagType] = m_TagUI;

        ComponentType CameraType = scene.GetComponentType<CameraComponent>();
        m_componentsUI[CameraType] = m_CameraUI;

        ComponentType GridbroadPhaseType = scene.GetComponentType<GridBroadphaseComponent>();
        m_componentsUI[GridbroadPhaseType] = m_GridbroadPhaseUI;

        ComponentType TransformType = scene.GetComponentType<TransformComponent>();
        m_componentsUI[TransformType] = m_TransformUI;

        ComponentType MovementType = scene.GetComponentType<MovementComponent>();
        m_componentsUI[MovementType] = m_MovementUI;

        ComponentType RotationType = scene.GetComponentType<RotationComponent>();
        m_componentsUI[RotationType] = m_RotationUI;

        ComponentType ParentType = scene.GetComponentType<ParentComponent>();
        m_componentsUI[ParentType] = m_ParentUI;

        ComponentType TextureType = scene.GetComponentType<TextureComponent>();
        m_componentsUI[TextureType] = m_TextureUI;

        ComponentType MaterialType = scene.GetComponentType<MaterialComponent>();
        m_componentsUI[MaterialType] = m_MaterialUI;

        ComponentType EmitterType = scene.GetComponentType<EmitterComponent>();
        m_componentsUI[EmitterType] = m_EmitterUI;

        ComponentType LightType = scene.GetComponentType<LightComponent>();
        m_componentsUI[LightType] = m_LightUI;

        ComponentType ModelType = scene.GetComponentType<ModelComponent>();
        m_componentsUI[ModelType] = m_ModelUI;

        ComponentType FrameBufferType = scene.GetComponentType<FrameBufferComponent>();
        m_componentsUI[FrameBufferType] = m_FrameBufferUI;

        ComponentType FrameBufferViewType = scene.GetComponentType<FrameBufferViewComponent>();
        m_componentsUI[FrameBufferViewType] = m_FrameBufferViewUI;

        ComponentType TransformAnimationType = scene.GetComponentType<TransformAnimationComponent>();
        m_componentsUI[TransformAnimationType] = m_TransformAnimationUI;

        ComponentType TilingType = scene.GetComponentType<TilingComponent>();
        m_componentsUI[TilingType] = m_TilingUI;

        ComponentType GravityType = scene.GetComponentType<GravityComponent>();
        m_componentsUI[GravityType] = m_GravityUI;

        ComponentType RigidBodyType = scene.GetComponentType<RigidBodyComponent>();
        m_componentsUI[RigidBodyType] = m_RigidBodyUI;

        ComponentType MeshColliderType = scene.GetComponentType<MeshColliderComponent>();
        m_componentsUI[MeshColliderType] = m_MeshColliderUI;

        ComponentType SphereColliderType = scene.GetComponentType<SphereColliderComponent>();
        m_componentsUI[SphereColliderType] = m_SphereColliderUI;

        ComponentType AABBColliderType = scene.GetComponentType<AABBColliderComponent>();
        m_componentsUI[AABBColliderType] = m_AABBColliderUI;

        ComponentType SoftBodyType = scene.GetComponentType<SoftBodyComponent>();
        m_componentsUI[SoftBodyType] = m_SoftBodyUI;

        ComponentType PlayerType = scene.GetComponentType<PlayerComponent>();
        m_componentsUI[PlayerType] = m_PlayerUI;

        ComponentType SteeringBehaviorType = scene.GetComponentType<SteeringBehaviorComponent>();
        m_componentsUI[SteeringBehaviorType] = m_SteeringBehaviorUI;
    }

    ComponentUIHandler ComponentUI::GetComponentUI(ComponentType component)
    {
        std::map<ComponentId, ComponentUIHandler>::iterator it = m_componentsUI.find(component);
        if (it == m_componentsUI.end())
        {
            return m_EmptyUI;
        }

        return it->second;
    }

    void ComponentUI::m_EmptyUI(Scene& scene, Entity entityId)
    {
    }

    void ComponentUI::m_TagUI(Scene& scene, Entity entityId)
    {
        Title("Tag:");

        bool hastag = false;
        TagComponent& tag = scene.Get<TagComponent>(entityId, hastag);
        if (!hastag)
        {
            return;
        }

        ImGui::Text("Name");
        ImGui::InputText("##Name", &tag.name);
    }

    void ComponentUI::m_CameraUI(Scene& scene, Entity entityId)
    {
        Title("Camera:");

        bool hascamera = false;
        CameraComponent& camera = scene.Get<CameraComponent>(entityId, hascamera);
        if (!hascamera)
        {
            return;
        }

        ImGui::Text("Up vector");
        ImGui::InputFloat3("##upVector", &camera.upVector.x);

        ImGui::Text("Distance");
        ImGui::InputFloat("##distance", &camera.distance);

        ImGui::Text("Height");
        ImGui::InputFloat("##height", &camera.height);

        ImGui::Text("Offset Target");
        ImGui::InputFloat("##offsetTarget", &camera.offsetTarget);

        ImGui::Text("fovy");
        ImGui::InputFloat("##fovy", &camera.fovy);

        ImGui::Text("zNear");
        ImGui::InputFloat("##zNear", &camera.zNear);

        ImGui::Text("zFar");
        ImGui::InputFloat("##zFar", &camera.zFar);
    }

    void ComponentUI::m_GridbroadPhaseUI(Scene& scene, Entity entityId)
    {
        Title("Gridbroadphase:");

        std::shared_ptr<GridBroadphaseComponent> pGridBroadphase = PhysicsLocator::GetGridBroadphase();
        if (!pGridBroadphase)
        {
            return;
        }

        ImGui::Text("Length per box");
        ImGui::InputFloat3("##Lengthbox", &pGridBroadphase->lengthPerBox.x);
    }

    void ComponentUI::m_TransformUI(Scene& scene, Entity entityId)
    {
        Title("Transform:");

        bool hastransform = false;
        TransformComponent& transform = scene.Get<TransformComponent>(entityId, hastransform);
        if (!hastransform)
        {
            return;
        }

        ImGui::Text("Position");
        ImGui::InputFloat("X##PositionX", &transform.position.x);
        ImGui::InputFloat("Y##PositionY", &transform.position.y);
        ImGui::InputFloat("Z##PositionZ", &transform.position.z);

        Separator();
        ImGui::Text("Rotation");
        glm::vec3 euler = glm::degrees(glm::eulerAngles(transform.orientation));
        if (ImGui::InputFloat("X##RotationX", &euler.x)) {
            transform.orientation = glm::quat(glm::radians(euler));
        }
        if (ImGui::InputFloat("Y##RotationY", &euler.y)) {
            transform.orientation = glm::quat(glm::radians(euler));
        }
        if (ImGui::InputFloat("Z##RotationZ", &euler.z)) {
            transform.orientation = glm::quat(glm::radians(euler));
        }

        Separator();
        ImGui::Text("Scale");
        ImGui::InputFloat("##Scale", &transform.scale);
    }

    void ComponentUI::m_MovementUI(Scene& scene, Entity entityId)
    {
        Title("Movement:");

        bool hasmovement = false;
        MovementComponent& movement = scene.Get<MovementComponent>(entityId, hasmovement);
        if (!hasmovement)
        {
            return;
        }

        ImGui::Text("Velocity");
        ImGui::InputFloat("##VelocityX", &movement.velocity.x);
        ImGui::InputFloat("##VelocityY", &movement.velocity.y);
        ImGui::InputFloat("##VelocityZ", &movement.velocity.z);

        Separator();
        ImGui::Text("Acceleration");
        ImGui::InputFloat("##AccelerationX", &movement.acceleration.x);
        ImGui::InputFloat("##AccelerationY", &movement.acceleration.y);
        ImGui::InputFloat("##AccelerationZ", &movement.acceleration.z);

        Separator();
        ImGui::Text("Max speed");
        ImGui::InputFloat("##MovementMaxSpeed", &movement.maxSpeed);
        Separator();
        ImGui::Text("Drag");
        ImGui::InputFloat("##MovementDrag", &movement.drag);
    }

    void ComponentUI::m_RotationUI(Scene& scene, Entity entityId)
    {
        Title("Rotation:");

        bool hasrotation = false;
        RotationComponent& rotation = scene.Get<RotationComponent>(entityId, hasrotation);
        if (!hasrotation)
        {
            return;
        }

        ImGui::Text("Angular Velocity");
        ImGui::InputFloat("##AngularVelocityX", &rotation.velocity.x);
        ImGui::InputFloat("##AngularVelocityY", &rotation.velocity.y);
        ImGui::InputFloat("##AngularVelocityZ", &rotation.velocity.z);

        Separator();
        ImGui::Text("Angular Acceleration");
        ImGui::InputFloat("##AngularAccelerationX", &rotation.acceleration.x);
        ImGui::InputFloat("##AngularAccelerationY", &rotation.acceleration.y);
        ImGui::InputFloat("##AngularAccelerationZ", &rotation.acceleration.z);

        Separator();
        ImGui::InputFloat("##AngularMaxSpeed", &rotation.maxSpeed);
        Separator();
        ImGui::InputFloat("##AngularDrag", &rotation.drag);
    }

    void ComponentUI::m_ParentUI(Scene& scene, Entity entityId)
    {
        Title("Parent:");

        bool hasparent = false;
        ParentComponent& parent = scene.Get<ParentComponent>(entityId, hasparent);
        if (!hasparent)
        {
            return;
        }

        ImGui::Text("Parent Entity ID");
        ImGui::InputInt("##ParentEntityID", &parent.parentId);
    }

    void ComponentUI::m_TextureUI(Scene& scene, Entity entityId)
    {
        Title("Texture:");

        bool hastexture = false;
        TextureComponent& texture = scene.Get<TextureComponent>(entityId, hastexture);
        if (!hastexture)
        {
            return;
        }

        ImGui::Text("File Name");
        ImGui::InputText("##FileName", &texture.fileName);

        // Way to set it with a list
        ImGui::Text("Texture Type");
        ImGui::InputInt("##TextureType", reinterpret_cast<int*>(&texture.textureType));

        ImGui::Text("Texture Sides");
        for (size_t i = 0; i < texture.vecTextures.size(); ++i)
        {
            ImGui::InputText(("##TextureSide" + std::to_string(i)).c_str(), &texture.vecTextures[i]);
        }
        if (ImGui::Button("Add texture side"))
        {
            texture.vecTextures.push_back("");
        }
    }

    void ComponentUI::m_MaterialUI(Scene& scene, Entity entityId)
    {
        Title("Material:");

        bool hasmaterial = false;
        MaterialComponent& material = scene.Get<MaterialComponent>(entityId, hasmaterial);
        if (!hasmaterial)
        {
            return;
        }

        ImGui::Text("Name");
        ImGui::InputText("##MaterialName", &material.name);

        ImGui::Text("Offset");
        ImGui::InputFloat3("##Offset", &material.offset.x);

        ImGui::Text("Offset Move");
        ImGui::InputFloat3("##OffsetMove", &material.offsetMove.x);

        ImGui::Text("Offset Height Map");
        ImGui::InputFloat3("##OffsetHeightMap", &material.offsetHeightMap.x);

        ImGui::Text("Color Textures");
        for (size_t i = 0; i < material.colorTextures.size(); ++i)
        {
            ImGui::InputText(("##ColorTexture" + std::to_string(i)).c_str(), &material.colorTextures[i]);
        }

        if (ImGui::Button("Add color texture"))
        {
            material.colorTextures.push_back("");
        }

        ImGui::Text("Color Textures Ratios");
        ImGui::InputFloat3("##ColorTexturesRatios", &material.colorTexturesRatios.x);

        ImGui::Text("Use Height Map");
        ImGui::Checkbox("##UseHeightMap", &material.useHeightMap);

        ImGui::Text("Height Map Texture");
        ImGui::InputText("##HeightMapTexture", &material.heightMapTexture);

        ImGui::Text("Height Scale");
        ImGui::InputFloat("##HeightScale", &material.heightScale);

        ImGui::Text("Use Normal Texture");
        ImGui::Checkbox("##UseNormalTexture", &material.useNormalTexture);

        ImGui::Text("Normal Texture");
        ImGui::InputText("##NormalTexture", &material.normalTexture);

        ImGui::Text("Is Emissive");
        ImGui::Checkbox("##IsEmissive", &material.isEmissive);

        ImGui::Text("Use Alpha Texture");
        ImGui::Checkbox("##UseAlphaTexture", &material.useAlphaTexture);

        ImGui::Text("Alpha Texture");
        ImGui::InputText("##AlphaTexture", &material.alphaTexture);

        ImGui::Text("Use Cube Texture");
        ImGui::Checkbox("##UseCubeTexture", &material.useCubeTexture);

        ImGui::Text("Cube Texture");
        ImGui::InputText("##CubeTexture", &material.cubeTexture);
    }

    void ComponentUI::m_EmitterUI(Scene& scene, Entity entityId)
    {
        Title("Emitter:");

        bool hasemitter = false;
        EmitterComponent& emitter = scene.Get<EmitterComponent>(entityId, hasemitter);
        if (!hasemitter)
        {
            return;
        }

        ImGui::Text("Emit Rate:");
        ImGui::InputInt("Min##EmitRateMin", &emitter.emitRateMin);
        ImGui::InputInt("Max##EmitRateMax", &emitter.emitRateMax);

        ImGui::Text("Max Particles:");
        ImGui::InputInt("##MaxParticles", &emitter.maxParticles);

        ImGui::Text("Active:");
        ImGui::Checkbox("##IsActive", &emitter.isActive);

        ImGui::Text("Mesh Name:");
        ImGui::InputText("##MeshName", &(emitter.properties.meshName));

        ImGui::Text("Material:");
        ImGui::InputText("##Material", &(emitter.properties.material));

        ImGui::Text("Initial Color:");
        ImGui::InputFloat3("##ColorInitial", &(emitter.properties.colorInitial.x));

        ImGui::Text("Color Change:");
        ImGui::InputFloat3("##ColorChange", &(emitter.properties.colorChange.x));

        ImGui::Text("Constant Force:");
        ImGui::InputFloat3("##ConstForce", &(emitter.properties.constForce.x));

        ImGui::Text("Max Life Time:");
        ImGui::InputFloat("##MaxLifeTime", &(emitter.properties.maxLifeTime));

        ImGui::Text("Min Life Time:");
        ImGui::InputFloat("##MinLifeTime", &(emitter.properties.minLifeTime));

        ImGui::Text("Max Orientation (In quat):");
        ImGui::InputFloat4("##OriMax", &(emitter.properties.oriMax.w));

        ImGui::Text("Min Orientation (In quat):");
        ImGui::InputFloat4("##OriMin", &(emitter.properties.oriMin.w));

        ImGui::Text("Max Position:");
        ImGui::InputFloat3("##PosMax", &(emitter.properties.posMax.x));

        ImGui::Text("Min Position:");
        ImGui::InputFloat3("##PosMin", &(emitter.properties.posMin.x));

        ImGui::Text("Max Rotation (In degrees):");
        ImGui::InputFloat3("##RotMax", &(emitter.properties.rotMax.x));

        ImGui::Text("Min Rotation:");
        ImGui::InputFloat3("##RotMin", &(emitter.properties.rotMin.x));

        ImGui::Text("Max Scale:");
        ImGui::InputFloat("##ScaMax", &(emitter.properties.scaMax));

        ImGui::Text("Min Scale:");
        ImGui::InputFloat("##ScaMin", &(emitter.properties.scaMin));

        ImGui::Text("Max Velocity:");
        ImGui::InputFloat3("##VelMax", &(emitter.properties.velMax.x));

        ImGui::Text("Min Velocity:");
        ImGui::InputFloat3("##VelMin", &(emitter.properties.velMin.x));
    }

    void ComponentUI::m_LightUI(Scene& scene, Entity entityId)
    {
        Title("Light:");

        bool haslight = false;
        LightComponent& light = scene.Get<LightComponent>(entityId, haslight);
        if (!haslight)
        {
            return;
        }

        // Flickering control
        ImGui::Text("Flicker Offset");
        ImGui::InputFloat4("##FlickerOffset", &light.flickerOffset.x);

        // ulBasePath
        ImGui::Text("UL Base Path");
        ImGui::InputText("##ULBasePath", &light.ulBasePath);

        // Controls relative to transform component
        ImGui::Text("Position Offset");
        ImGui::InputFloat4("##PositionOffset", &light.positionOffset.x);

        ImGui::Text("Direction Offset");
        ImGui::InputFloat4("##DirectionOffset", &light.directionOffset.x);

        ImGui::Text("Distance Offset");
        ImGui::InputFloat("##DistanceOffset", &light.distanceOffset);

        // Light parameters
        ImGui::Text("Diffuse (R, G, B, A)");
        ImGui::InputFloat4("##Diffuse", &light.diffuse.x);

        ImGui::Text("Specular (R, G, B, Power)");
        ImGui::InputFloat4("##Specular", &light.specular.x);

        ImGui::Text("Attenuation (Constant, Linear, Quadratic, Distance CutOff)");
        ImGui::InputFloat4("##Attenuation", &light.atten.x);

        ImGui::Text("Direction");
        ImGui::InputFloat4("##Direction", &light.direction.x);

        ImGui::Text("Parameters (Type, Inner Angle, Outer Angle, Is Flickery)");
        ImGui::InputFloat4("##Params", &light.params.x);

        ImGui::Text("Status");
        ImGui::Checkbox("##Status", &light.status);
    }

    void ComponentUI::m_ModelUI(Scene& scene, Entity entityId)
    {
        Title("Model:");

        std::shared_ptr<iVAOManager> pVAOManager = VAOManagerLocator::Get();
        bool hasmodel = false;
        ModelComponent& model = scene.Get<ModelComponent>(entityId, hasmodel);
        if (!hasmodel)
        {
            return;
        }

        // Model paths
        ImGui::Text("Models");
        for (size_t i = 0; i < model.models.size(); ++i)
        {
            if (ImGui::InputText(("##Model" + std::to_string(i)).c_str(), &model.models[i]))
            {
                sMesh* pMesh = pVAOManager->LoadModelIntoVAO(model.models[i], false);
                if (!pMesh)
                {
                    continue;
                }
                model.pMeshes[i] = pMesh;
            }
        }

        if (ImGui::Button("Add Mesh"))
        {
            model.models.push_back("cube.ply");

            sMesh* pMesh = pVAOManager->LoadModelIntoVAO(model.models.back(), false);
            model.pMeshes.push_back(pMesh);
        }

        ImGui::Text("(use material to add transparency texture)");
        ImGui::Checkbox("Use transparency", &model.useTransparency);

        // Color
        ImGui::Checkbox("Use default color", &model.useDefaultColor);
        ImGui::Text("Default color:");
        ImGui::InputFloat3("##ModelDefaultcolor", &model.defaultColor[0]);

        // Material
        ImGui::Text("Material");
        ImGui::InputText("##MaterialInput", &model.material);

        // Flags
        ImGui::Checkbox("Wireframe", &model.isWireframe);
        ImGui::Checkbox("Dynamic", &model.isDynamic);
        ImGui::Checkbox("Do Not Light", &model.doNotLight);
        ImGui::Checkbox("Use Color Texture", &model.useColorTexture);

        ImGui::Checkbox("Is Active", &model.isActive);

        // FBOIDs
        ImGui::Text("FBO IDs:");
        std::set<uint> fboIdsCopy = model.FBOIDs;
        for (uint id : fboIdsCopy)
        {
            ImGui::PushID(id);
            uint modifiedId = id; // Create a copy of the ID for modification
            ImGui::InputScalar(("##FBOID_" + std::to_string(id)).c_str(), ImGuiDataType_U32, &modifiedId);
            ImGui::SameLine();
            if (ImGui::Button(("Remove##FBOID_" + std::to_string(id)).c_str()))
            {
                model.FBOIDs.erase(id); // Remove the original ID
                break; // Exit loop to avoid modifying container while iterating
            }

            ImGui::PopID();

            if (modifiedId != id) {
                model.FBOIDs.erase(id); // Remove the original ID
                model.FBOIDs.insert(modifiedId); // Insert the modified ID
            }
        }

        if (ImGui::Button("Add FBO ID"))
        {
            int newIndex = *(model.FBOIDs.rbegin()) + 1; // Last index +1
            model.FBOIDs.insert(newIndex);
        }
    }

    void ComponentUI::m_FrameBufferUI(Scene& scene, Entity entityId)
    {
        Title("Frame buffer:");

        std::shared_ptr<iVAOManager> pVAOManager = VAOManagerLocator::Get();
        bool hasframeBuffer = false;
        FrameBufferComponent& frameBuffer = scene.Get<FrameBufferComponent>(entityId, hasframeBuffer);
        if (!hasframeBuffer)
        {
            return;
        }

        ImGui::Text("Camera ID");
        ImGui::InputInt("##CameraId", &frameBuffer.cameraId);

        ImGui::Text("Width");
        ImGui::InputInt("##FBOWidth", &frameBuffer.width);

        ImGui::Text("Height");
        ImGui::InputInt("##FBOHeight", &frameBuffer.height);
    }

    void ComponentUI::m_FrameBufferViewUI(Scene& scene, Entity entityId)
    {
        Title("Frame buffer view:");

        std::shared_ptr<iVAOManager> pVAOManager = VAOManagerLocator::Get();
        bool hasframeBufferView = false;
        FrameBufferViewComponent& frameBufferView = scene.Get<FrameBufferViewComponent>(entityId, hasframeBufferView);
        if (!hasframeBufferView)
        {
            return;
        }

        int FBOID = static_cast<int>(frameBufferView.FBOID);
        ImGui::Text("FBO ID");
        ImGui::InputInt("##FBOID", &FBOID);
        frameBufferView.FBOID = FBOID;

        int filter = static_cast<int>(frameBufferView.filter);
        ImGui::Text("Filter");
        ImGui::InputInt("##Filter", &filter);
        frameBufferView.filter = static_cast<eFBFilter>(filter);
    }

    void ComponentUI::m_TransformAnimationUI(Scene& scene, Entity entityId)
    {
        Title("Animation Component:");

        bool hastransformAnimation = false;
        TransformAnimationComponent& transformAnimation = scene.Get<TransformAnimationComponent>(entityId, hastransformAnimation);
        if (!hastransformAnimation)
        {
            return;
        }

        // Edit time
        ImGui::InputFloat("Time", &transformAnimation.time);
        ImGui::Checkbox("Is Active##AnimationIsActive", &transformAnimation.isActive);

        // Edit position keyframes
        ImGui::Text("Position Keyframes:");
        for (size_t i = 0; i < transformAnimation.positionKeyFrames.size(); ++i)
        {
            PositionKeyFrame& keyFrame = transformAnimation.positionKeyFrames[i];
            ImGui::InputFloat(("Time##AnimationPos" + std::to_string(i)).c_str(), &keyFrame.time);
            ImGui::InputFloat3(("Position##AnimationPos" + std::to_string(i)).c_str(), &keyFrame.value[0]);
            ImGui::InputInt(("EaseType##AnimationPos" + std::to_string(i)).c_str(), reinterpret_cast<int*>(&keyFrame.easeType));

            ImGui::Text("Is key event:");
            ImGui::Checkbox(("##AnimationPosKeyEvent" + std::to_string(i)).c_str(), &keyFrame.isKeyEvent);

            // Button to remove this keyframe
            if (ImGui::Button(("Remove##AnimationPos" + std::to_string(i)).c_str()))
            {
                transformAnimation.positionKeyFrames.erase(transformAnimation.positionKeyFrames.begin() + i);
                --i;
            }
        }

        if (ImGui::Button("Add Position Keyframe"))
        {
            PositionKeyFrame newKeyFrame;
            transformAnimation.positionKeyFrames.push_back(newKeyFrame);
        }

        Separator();

        // Edit scale keyframes
        ImGui::Text("Scale Keyframes:");
        for (size_t i = 0; i < transformAnimation.scaleKeyFrames.size(); ++i)
        {
            ScaleKeyFrame& keyFrame = transformAnimation.scaleKeyFrames[i];
            ImGui::InputFloat(("Time##AnimationScale" + std::to_string(i)).c_str(), &keyFrame.time);
            ImGui::InputFloat(("Scale##AnimationScale" + std::to_string(i)).c_str(), &keyFrame.value);
            ImGui::InputInt(("EaseType##AnimationScale" + std::to_string(i)).c_str(), reinterpret_cast<int*>(&keyFrame.easeType));

            ImGui::Text("Is key event:");
            ImGui::Checkbox(("##AnimationScaleKeyEvent" + std::to_string(i)).c_str(), &keyFrame.isKeyEvent);

            // Button to remove this keyframe
            if (ImGui::Button(("Remove##AnimationScale" + std::to_string(i)).c_str()))
            {
                transformAnimation.scaleKeyFrames.erase(transformAnimation.scaleKeyFrames.begin() + i);
                --i;
            }
        }

        if (ImGui::Button("Add Scale Keyframe"))
        {
            ScaleKeyFrame newKeyFrame;
            transformAnimation.scaleKeyFrames.push_back(newKeyFrame);
        }

        Separator();

        // Edit rotation keyframes
        ImGui::Text("Rotation Keyframes:");
        for (size_t i = 0; i < transformAnimation.rotationKeyFrames.size(); ++i)
        {
            RotationKeyFrame& keyFrame = transformAnimation.rotationKeyFrames[i];
            ImGui::InputFloat(("Time##AnimationRot" + std::to_string(i)).c_str(), &keyFrame.time);

            ImGui::Text("Rotation");
            glm::vec3 euler = glm::degrees(glm::eulerAngles(keyFrame.value));
            if (ImGui::InputFloat(("X##AnimationRotationX" + std::to_string(i)).c_str(), &euler.x)) {
                keyFrame.value = glm::quat(glm::radians(euler));
            }
            if (ImGui::InputFloat(("Y##AnimationRotationY" + std::to_string(i)).c_str(), &euler.y)) {
                keyFrame.value = glm::quat(glm::radians(euler));
            }
            if (ImGui::InputFloat(("Z##AnimationRotationZ" + std::to_string(i)).c_str(), &euler.z)) {
                keyFrame.value = glm::quat(glm::radians(euler));
            }

            ImGui::InputInt(("EaseType##AnimationRot" + std::to_string(i)).c_str(), reinterpret_cast<int*>(&keyFrame.easeType));

            ImGui::Text("Is key event:");
            ImGui::Checkbox(("##AnimationRotKeyEvent" + std::to_string(i)).c_str(), &keyFrame.isKeyEvent);

            // Button to remove this keyframe
            if (ImGui::Button(("Remove##AnimationRot" + std::to_string(i)).c_str()))
            {
                transformAnimation.rotationKeyFrames.erase(transformAnimation.rotationKeyFrames.begin() + i);
                --i;
            }
        }

        if (ImGui::Button("Add Rotation Keyframe"))
        {
            RotationKeyFrame newKeyFrame;
            transformAnimation.rotationKeyFrames.push_back(newKeyFrame);
        }

        Separator();
    }

    void ComponentUI::m_TilingUI(Scene& scene, Entity entityId)
    {
        Title("Tiling:");

        bool hastiling = false;
        TilingComponent& tiling = scene.Get<TilingComponent>(entityId, hastiling);
        if (!hastiling)
        {
            return;
        }

        // Axis
        ImGui::Text("Axis");
        ImGui::InputFloat3("##TilingAxis", &tiling.axis.x);

        // Offset
        ImGui::Text("Offset");
        ImGui::InputFloat3("##TilingOffset", &tiling.offset.x);
    }

    void ComponentUI::m_GravityUI(Scene& scene, Entity entityId)
    {
        Title("Gravity:");

        bool hasgravity = false;
        GravityComponent& gravity = scene.Get<GravityComponent>(entityId, hasgravity);
        if (!hasgravity)
        {
            return;
        }

        // Acceleration
        ImGui::Text("Acceleration");
        ImGui::InputFloat3("##GravityAcceleration", &gravity.acceleration.x);
    }

    void ComponentUI::m_RigidBodyUI(Scene& scene, Entity entityId)
    {
        Title("Rigid Body:");

        bool hasrigidBody = false;
        RigidBodyComponent& rigidBody = scene.Get<RigidBodyComponent>(entityId, hasrigidBody);
        if (!hasrigidBody)
        {
            return;
        }

        // Body Type
        ImGui::Text("Body Type");
        ImGui::InputInt("##BodyType", reinterpret_cast<int*>(&rigidBody.bodyType));

        // Shape Type
        ImGui::Text("Shape Type");
        ImGui::InputInt("##ShapeType", reinterpret_cast<int*>(&rigidBody.shapeType));
    }

    void ComponentUI::m_MeshColliderUI(Scene& scene, Entity entityId)
    {
        Title("Mesh Collider:");

        bool hasmeshCollider = false;
        MeshColliderComponent& meshCollider = scene.Get<MeshColliderComponent>(entityId, hasmeshCollider);
        if (!hasmeshCollider)
        {
            return;
        }

        // Name
        ImGui::Text("Name");
        ImGui::InputText("##MeshColliderName", &meshCollider.name);
    }

    void ComponentUI::m_SphereColliderUI(Scene& scene, Entity entityId)
    {
        Title("Sphere Collider:");

        bool hassphereCollider = false;
        SphereColliderComponent& sphereCollider = scene.Get<SphereColliderComponent>(entityId, hassphereCollider);
        if (!hassphereCollider)
        {
            return;
        }

        // Radius
        ImGui::Text("Radius");
        ImGui::InputFloat("##SphereColliderRadius", &sphereCollider.radius);
    }

    void ComponentUI::m_AABBColliderUI(Scene& scene, Entity entityId)
    {
        Title("AABB Collider:");

        bool hasaABBCollider = false;
        AABBColliderComponent& aABBCollider = scene.Get<AABBColliderComponent>(entityId, hasaABBCollider);
        if (!hasaABBCollider)
        {
            return;
        }

        // Min
        ImGui::Text("Min");
        ImGui::InputFloat3("##AABBColliderMin", &aABBCollider.min.x);

        // Max
        ImGui::Text("Max");
        ImGui::InputFloat3("##AABBColliderMax", &aABBCollider.max.x);
    }

    void ComponentUI::m_SoftBodyUI(Scene& scene, Entity entityId)
    {
        Title("SoftBody:");

        bool hassoftBody = false;
        SoftBodyComponent& softBody = scene.Get<SoftBodyComponent>(entityId, hassoftBody);
        if (!hassoftBody)
        {
            return;
        }

        ImGui::Text("Mesh name");
        ImGui::InputText("##SoftBodyMesh", &softBody.meshName);

        ImGui::Text("Default spring strength");
        ImGui::InputFloat("##SoftBodyDefaultSpringStrength", &softBody.defaultSpringStrength);

        ImGui::Text("Default particle mass");
        ImGui::InputFloat("##SoftBodyParticleMass", &softBody.defaultParticleMass);
    }

    void ComponentUI::m_PlayerUI(Scene& scene, Entity entityId)
    {
        Title("Player:");

        bool hasplayer = false;
        PlayerComponent& player = scene.Get<PlayerComponent>(entityId, hasplayer);
        if (!hasplayer)
        {
            return;
        }

        ImGui::Text("Speed");
        ImGui::InputFloat("##PlayerSpeed", &player.speed);
    }

    void ComponentUI::m_SteeringBehaviorUI(Scene& scene, Entity entityId)
    {
        Title("Steering Behavior:");

        bool hassteeringBehavior = false;
        SteeringBehaviorComponent& steeringBehavior = scene.Get<SteeringBehaviorComponent>(entityId, hassteeringBehavior);
        if (!hassteeringBehavior)
        {
            return;
        }

        ImGui::Text("Steering type");
        ImGui::InputInt("##SteeringBehaviorType", reinterpret_cast<int*>(&steeringBehavior.steeringType));

        ImGui::Text("Target id");
        ImGui::InputInt("##SteeringBehaviorTargetId", &steeringBehavior.targetId);

        ImGui::Text("Speed");
        ImGui::InputFloat("##SteeringBehaviorSpeed", &steeringBehavior.speed);

        ImGui::Text("Max distance");
        ImGui::InputFloat("##SteeringBehaviorMaxDistance", &steeringBehavior.maxDistance);

        ImGui::Text("Future time");
        ImGui::InputFloat("##SteeringBehaviorFutureTime", &steeringBehavior.futureTime);
    }
}
