#pragma once

#include "iSceneSerializer.h"

#include "Engine/Core/Components/Components.h"
#include "Engine/Core/Components/SingletonComponents.h"

#include "Engine/Gameplay/Components/Components.h"

#include "Engine/Graphics/Components/Components.h"
#include "Engine/Graphics/Components/SingletonComponents.h"

#include "Engine/Physics/Components/Components.h"
#include "Engine/Physics/Components/SingletonComponents.h"

#include <rapidjson/document.h>

namespace MyEngine
{
	class SceneSerializerJSON : public iSceneSerializer
	{
	public:
		SceneSerializerJSON();
		virtual ~SceneSerializerJSON();

		virtual bool DeserializeScene(const std::string& filePath, Scene& sceneOut);

		virtual bool SerializeScene(const std::string& filePath, Scene& sceneIn);

	private:
		rapidjson::Document m_doc;

		bool m_LoadDoc(const std::string& filePath);
		bool m_SaveDoc(const std::string& filePath);

		// Parse values From the respective data structures into the doc
		bool m_ParseSceneToDoc(Scene& sceneIn);
		bool m_ParseTagToDoc(rapidjson::Value& jsonObject, 
							 TagComponent& tagIn,
							 rapidjson::Document::AllocatorType& allocator);
		bool m_ParseTransformToDoc(rapidjson::Value& jsonObject,
								   TransformComponent& transformIn,
								   rapidjson::Document::AllocatorType& allocator);
		bool m_ParseMovementToDoc(rapidjson::Value& jsonObject, 
								  MovementComponent& movementIn,
								  rapidjson::Document::AllocatorType& allocator);
		bool m_ParseRotationToDoc(rapidjson::Value& jsonObject, 
								  RotationComponent& rotationIn,
								  rapidjson::Document::AllocatorType& allocator);
		bool m_ParseParentToDoc(rapidjson::Value& jsonObject, 
								  ParentComponent& parentIn,
								  rapidjson::Document::AllocatorType& allocator);
		bool m_ParseTextureToDoc(rapidjson::Value& jsonObject, 
								  TextureComponent& textureIn,
								  rapidjson::Document::AllocatorType& allocator);
		bool m_ParseMaterialToDoc(rapidjson::Value& jsonObject, 
								  MaterialComponent& materialIn,
								  rapidjson::Document::AllocatorType& allocator);
		bool m_ParseLightToDoc(rapidjson::Value& jsonObject, 
								  LightComponent& lightIn,
								  rapidjson::Document::AllocatorType& allocator);
		bool m_ParseModelToDoc(rapidjson::Value& jsonObject, 
								  ModelComponent& modelIn,
								  rapidjson::Document::AllocatorType& allocator);
		bool m_ParseTransformAnimationToDoc(rapidjson::Value& jsonObject, 
											TransformAnimationComponent& animationIn,
											rapidjson::Document::AllocatorType& allocator);
		bool m_ParseTilingToDoc(rapidjson::Value& jsonObject, 
								  TilingComponent& tilingIn,
								  rapidjson::Document::AllocatorType& allocator);
		bool m_ParseEmitterToDoc(rapidjson::Value& jsonObject, 
								  EmitterComponent& emitterIn,
								  rapidjson::Document::AllocatorType& allocator);
		bool m_ParseRigidBodyToDoc(rapidjson::Value& jsonObject, 
								  RigidBodyComponent& rigidBodyIn,
								  rapidjson::Document::AllocatorType& allocator);
		bool m_ParseCameraToDoc(rapidjson::Value& jsonObject, 
								  CameraComponent& cameraIn,
								  rapidjson::Document::AllocatorType& allocator);
		bool m_ParseGridBroadphaseToDoc(rapidjson::Value& jsonObject,
								  GridBroadphaseComponent& gridBroadphaseIn,
								  rapidjson::Document::AllocatorType& allocator);
		bool m_ParsePlayerToDoc(rapidjson::Value& jsonObject,
								  PlayerComponent& playerIn,
								  rapidjson::Document::AllocatorType& allocator);
		bool m_ParseSteeringBehaviorToDoc(rapidjson::Value& jsonObject,
								  SteeringBehaviorComponent& steeringIn,
								  rapidjson::Document::AllocatorType& allocator);

		// Parse values from doc to the respective data structure
		bool m_ParseDocToScene(Scene& sceneOut);
		bool m_ParseDocToTag(rapidjson::Value& jsonObject, 
							 TagComponent& tagOut);
		bool m_ParseDocToTransform(rapidjson::Value& jsonObject, 
								   TransformComponent& transformOut);
		bool m_ParseDocToMovement(rapidjson::Value& jsonObject, 
								  MovementComponent& movementOut);
		bool m_ParseDocToRotation(rapidjson::Value& jsonObject, 
								  RotationComponent& rotationOut);
		bool m_ParseDocToParent(rapidjson::Value& jsonObject, 
								  ParentComponent& parentOut);
		bool m_ParseDocToTexture(rapidjson::Value& jsonObject, 
								  TextureComponent& textureOut);
		bool m_ParseDocToMaterial(rapidjson::Value& jsonObject, 
								  MaterialComponent& materialOut);
		bool m_ParseDocToLight(rapidjson::Value& jsonObject, 
							   LightComponent& lightOut);
		bool m_ParseDocToModel(rapidjson::Value& jsonObject, 
							   ModelComponent& modelOut);
		bool m_ParseDocToTransformAnimation(rapidjson::Value& jsonObject, 
							   TransformAnimationComponent& animationOut);
		bool m_ParseDocToTiling(rapidjson::Value& jsonObject, 
							   TilingComponent& tilingOut);
		bool m_ParseDocToEmitter(rapidjson::Value& jsonObject, 
							   EmitterComponent& emitterOut);
		bool m_ParseDocToRigidBody(rapidjson::Value& jsonObject, 
							   RigidBodyComponent& rigidbodyOut);
		bool m_ParseDocToCamera(rapidjson::Value& jsonObject, 
							   CameraComponent& cameraOut);
		bool m_ParseDocToWindow(rapidjson::Value& jsonObject, 
							   WindowComponent& windowOut);
		bool m_ParseDocToConfigPath(rapidjson::Value& jsonObject, 
							   ConfigPathComponent& configPathOut);
		bool m_ParseDocToGridBroadphase(rapidjson::Value& jsonObject,
							   GridBroadphaseComponent& gridBroadphaseOut);
		bool m_ParseDocToPlayer(rapidjson::Value& jsonObject,
							   PlayerComponent& playerOut);
		bool m_ParseDocToSteeringBehavior(rapidjson::Value& jsonObject,
							   SteeringBehaviorComponent& steeringOut);
	};
}