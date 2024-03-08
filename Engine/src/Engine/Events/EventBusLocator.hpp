#pragma once

#include "NullEventBus.hpp"
#include "iEventBus.h"

namespace MyEngine
{
	template <typename T, typename F>
	class EventBusLocator
	{
	public:
		static std::shared_ptr<iEventBus<T, F>> Get() { return m_eventBus; }

		static void Set(std::shared_ptr<iEventBus<T, F>> eventBus)
		{
			if (eventBus == nullptr)
			{
				m_eventBus = m_nullService;
			}
			else
			{
				m_eventBus = eventBus;
			}
		}

	private:
		static std::shared_ptr<iEventBus<T, F>> m_eventBus;
		static std::shared_ptr<NullEventBus<T, F>> m_nullService;
	};

	template <typename T, typename F>
	std::shared_ptr<NullEventBus<T, F>> EventBusLocator<T, F>::m_nullService(new NullEventBus<T, F>);
	template <typename T, typename F>
	std::shared_ptr<iEventBus<T, F>> EventBusLocator<T, F>::m_eventBus = EventBusLocator<T, F>::m_nullService;
}
