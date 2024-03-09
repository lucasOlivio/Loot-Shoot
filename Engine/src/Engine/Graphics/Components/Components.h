#pragma once

#include "Engine/Core/Resources/Meshes/Mesh.h"

#include "Engine/ECS/iComponent.h"

#include "Engine/Graphics/Textures/TextureProperties.h"
#include "Engine/Graphics/Animations/AnimationProperties.h"
#include "Engine/Graphics/Particles/ParticlesProperties.h"

#include <glm/glm.hpp>
#include <string>
#include <set>

namespace MyEngine
{
	struct TextureComponent : public iComponent
	{
		std::string fileName;
		eTextureType textureType;

		// Cube map list of texture sides
		std::vector<std::string> vecTextures;
	};

	struct MaterialComponent : public iComponent
	{
		std::string name;

		glm::vec3 offset; // fixed offset

		glm::vec3 currOffset;
		glm::vec3 offsetMove;

		glm::vec3 currOffsetHeightMap;
		glm::vec3 offsetHeightMap;

		std::vector<std::string> colorTextures;
		glm::vec3 colorTexturesRatios;

		bool useHeightMap;
		std::string heightMapTexture;
		float heightScale;

		bool useNormalTexture;
		std::string normalTexture;

		bool useSpecularTexture;
		std::string specularTexture;

		bool useDiscardTexture;
		std::string discardTexture;

		bool useCubeTexture;
		std::string cubeTexture;

		bool isEmissive;

		// Material transparency
		bool useAlphaTexture;
		std::string alphaTexture;
	};

	struct LightComponent : public iComponent
	{
		// Flickering control
		glm::vec4 flickerOffset;

		std::string ulBasePath;

		// Controls relative to transform component
		glm::vec4 positionOffset;
		glm::vec4 directionOffset;
		// multiplied by the direction to put the light at N distance from the front of the object
		float distanceOffset;

		// Light parameters
		glm::vec4 diffuse;   // Colour of the light (used for diffuse)
		glm::vec4 specular;  // rgb = highlight colour, w = power
		glm::vec4 atten;	 // x = constant, y = linear, z = quadratic, w = DistanceCutOff
		glm::vec4 initialAtten;
		glm::vec4 direction; // Spot, directional lights
		glm::vec4 params;	 // x = Light::LIGHT_TYPE, y = inner angle, z = outer angle, w = is flickery
		bool status;	     // x = 0 for off, 1 for on

		// Parameters uniform locations
		GLint position_UL;
		GLint diffuse_UL;
		GLint specular_UL;
		GLint atten_UL;
		GLint direction_UL;
		GLint params_UL;
		GLint status_UL;
	};

	struct ModelComponent : public iComponent
	{
		std::vector<std::string> models;
		std::vector<std::shared_ptr<sMeshInfo>> pMeshes;
		std::set<uint> FBOIDs = { 0 };

		std::string material;

		double elapsedTime;
		int currMesh;

		glm::vec3 defaultColor;

		bool useSoftBodyVertex; // HACK: Use the softbody copy from VAO instead of original
		bool useDefaultColor;
		bool useTransparency;
		bool isWireframe;
		bool isDynamic;
		bool doNotLight;
		bool useColorTexture;
		bool isActive;
	};

	struct TilingComponent : public iComponent
	{
		glm::vec3 axis;
		glm::vec3 offset;
	};

	struct TransformAnimationComponent : public iComponent
	{
		std::vector<PositionKeyFrame> positionKeyFrames;
		std::vector<ScaleKeyFrame> scaleKeyFrames;
		std::vector<RotationKeyFrame> rotationKeyFrames;

		int currStartPosKF = -1;
		int currEndPosKF = -1;

		int currStartRotKF = -1;
		int currEndRotKF = -1;

		int currStartScaKF = -1;
		int currEndScaKF = -1;

		float time;
		bool isActive;
	};

	struct EmitterComponent : public iComponent
	{
		EmitterProps properties = EmitterProps();

		// Time since active
		float timer;
		float timeLastEmit;

		// Number of particles emitted per second
		int emitRateMin;
		int emitRateMax;
		int maxParticles;

		// Total number of particles emitted
		int totalEmitPart = 0;

		std::shared_ptr<sMeshInfo> pMesh;

		bool isActive = false;
	};
}
