#pragma once

#include "Engine/ECS/SingletonComponents/Components.h"

namespace MyEngine
{
	class DebugLocator
	{
	public:
		static std::shared_ptr<DebugSquareComponent> GetSquare();
		static std::shared_ptr<DebugSphereComponent> GetSphere();

		static void SetSquare(std::shared_ptr<DebugSquareComponent> pDebugSquare);
		static void SetSphere(std::shared_ptr<DebugSphereComponent> pDebugSphere);

	private:
		static std::shared_ptr<DebugSquareComponent> m_pDebugSquare;
		static std::shared_ptr<DebugSphereComponent> m_pDebugSphere;
	};
}
