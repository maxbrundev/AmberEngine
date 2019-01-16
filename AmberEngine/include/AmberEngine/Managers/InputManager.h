#pragma once


#include <unordered_map>

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Managers
{
	class API_AMBERENGINE InputManager
	{
	public:
		InputManager() = default;
		~InputManager() = default;

		void Update();

		bool IsKeyPressed(char p_key);
		bool IsKeyEventOccured(char p_key);

		std::unordered_map<char, bool> m_keyEvents;
		std::unordered_map<char, bool> m_keyStates;
	};
}
