#pragma once

#include <functional>

namespace AmberEngine::Eventing
{
	template<class... ArgTypes>
	class Event
	{
	public:
		uint64_t AddListener(std::function<void(ArgTypes...)> p_callback);
		uint64_t operator+=(std::function<void(ArgTypes...)> p_callback);

		bool RemoveListener(uint64_t p_listenerID);
		bool operator-=(uint64_t p_listenerID);

		void RemoveAllListeners();

		uint64_t GetListenerCount();

		void Invoke(ArgTypes... p_args);

	private:
		std::unordered_map<uint64_t, std::function<void(ArgTypes...)>> m_callbacks;
		uint64_t m_availableListenerID = 0;
	};
}

#include "AmberEngine/Eventing/Event.inl"