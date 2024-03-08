#pragma once

#include "Engine/Physics/Components/SingletonComponents.h"

namespace MyEngine
{
	class PhysicsLocator
	{
	public:
		static std::shared_ptr<NarrowPhaseTestsComponent> GetNarrowPhaseTests();
		static std::shared_ptr<GridBroadphaseComponent> GetGridBroadphase();
		static std::shared_ptr<FrameCollisionComponent> GetFrameCollision();

		static void SetNarrowPhaseTests(std::shared_ptr<NarrowPhaseTestsComponent> pNarrowPhaseTests);
		static void SetGridBroadphase(std::shared_ptr<GridBroadphaseComponent> pGridBroadphase);
		static void SetFrameCollision(std::shared_ptr<FrameCollisionComponent> pFrameCollision);

	private:
		static std::shared_ptr<NarrowPhaseTestsComponent> m_pNarrowPhaseTests;
		static std::shared_ptr<GridBroadphaseComponent> m_pGridBroadphase;
		static std::shared_ptr<FrameCollisionComponent> m_pFrameCollision;
	};
}
