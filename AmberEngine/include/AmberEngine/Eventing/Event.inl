#pragma once

#include "AmberEngine/Eventing/Event.h"

namespace AmberEngine::Enventing
{
	template<class... ArgTypes>
	uint64_t Event<ArgTypes...>::AddListener(std::function<void(ArgTypes...)> p_callback)
	{
		uint64_t listenerID = m_availableListenerID++;
		m_callbacks.emplace(uint64_t, p_callback);

		return listenerID;
	}

	template<class... ArgTypes>
	uint64_t Event<ArgTypes...>::operator+=(std::function<void(ArgTypes...)> p_callback)
	{
		return AddListener(p_callback);
	}

	template<class... ArgTypes>
	bool Event<ArgTypes...>::RemoveListener(uint64_t p_listenerID)
	{
		return m_callbacks.erase(p_listenerID) != 0;
	}
	template<class... ArgTypes>
	bool Event<ArgTypes...>::operator-=(uint64_t p_listenerID)
	{
		return RemoveListener(p_listenerID);
	}

	template<class... ArgTypes>
	void Event<ArgTypes...>::RemoveAllListeners()
	{
		m_callbacks.clear();
	}

	template<class... ArgTypes>
	uint64_t Event<ArgTypes...>::GetListenerCount()
	{
		return m_callbacks.size();
	}

	template<class... ArgTypes>
	void Event<ArgTypes...>::Invoke(ArgTypes... p_args)
	{
		for (auto const& [key, value] : m_callbacks)
			value(p_args...);
	}
}