#include "pch.h"

#include "EngineLocator.h"

namespace MyEngine
{
	std::shared_ptr<Engine> EngineLocator::m_pEngine = nullptr;

	std::shared_ptr<Engine> EngineLocator::Get() { return m_pEngine; }

	void EngineLocator::Set(std::shared_ptr<Engine> pEngine)
	{
		m_pEngine = pEngine;
	}
}