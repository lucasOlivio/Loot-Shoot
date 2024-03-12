#pragma once

#include "Engine/Core/StateProperties.h"

#include "Engine/ThreadSafe.h"

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <string>

namespace MyEngine
{
	struct ConfigPathComponent : public ThreadSafe
	{
		std::string pathModels;
		std::string pathAudio;
		std::string pathShaders;
		std::string pathScripts;
		std::string pathTextures;
		std::string pathScenes;

		// Path to debug models inside models default folder
		std::string pathDebugSquare;
		std::string pathDebugSphere;
	};

	struct TimerComponent : public ThreadSafe
	{
		float timeTrack; // counts up to 1 milisecond

		int miliseconds;
		int seconds;
	};

	struct FrameCounterComponent : public ThreadSafe
	{
		int frameCount;
		float fpsTimer;
		float fps;
	};

	struct KeyInputComponent : public ThreadSafe
	{
		// One for each key code from glfw
		bool key[350];
	};

	struct MouseInputComponent : public ThreadSafe
	{
		// One for each mouse button code from glfw
		bool button[7];

		float posX;
		float posY;

		float lastPosX;
		float lastPosY;

		float sensitivity;

		bool mouseCaptured;
	};

	struct GameStateComponent : public ThreadSafe
	{
		// All the systems that should be running 
		std::vector<std::string> mainSystems; // Always running
		std::map<eGameStates, std::vector<std::string>> mapStateSystems;

		eGameStates prevState; // Only state system should modify this
		eGameStates currState; // Any system can modify this to stop/run simulation
	};
}
