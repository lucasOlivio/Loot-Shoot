#pragma once

#include "Engine/Core/Engine.h"

namespace MyEngine
{
	class EngineLocator
	{
	public:
		static std::shared_ptr<Engine> Get();

		static void Set(std::shared_ptr<Engine> pEngine);

	private:
		static std::shared_ptr<Engine> m_pEngine;
	};
}
