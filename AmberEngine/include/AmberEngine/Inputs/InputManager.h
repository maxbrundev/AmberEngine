#pragma once

#include <unordered_map>

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Inputs/EKey.h"
#include "AmberEngine/Inputs/EKeyState.h"
#include "AmberEngine/Inputs/EMouseButton.h"
#include "AmberEngine/Inputs/EMouseButtonState.h"
#include "AmberEngine/Context/Window.h"

namespace AmberEngine::Inputs
{
	class API_AMBERENGINE InputManager
	{
	private:
		Context::Window& m_window;

		uint64_t m_keyPressedEventListerId;
		uint64_t m_keyReleasedEventListerId;
		uint64_t m_mousePressedEventListerId;
		uint64_t m_mouseReleasedEventListerId;

		std::unordered_map<EKey, EKeyState> m_keyEvents;
		std::unordered_map<EMouseButton, EMouseButtonState> m_mouseEvents;
		
	public:
		InputManager(Context::Window& p_window);
		~InputManager();

		void clearEvents();

		EKeyState GetKey(EKey p_key) const;
		EMouseButtonState GetMouseButton(EMouseButton p_button) const;
		
		bool IsKeyPressed(EKey p_key) const;
		bool IsKeyReleased(EKey p_key) const;

		bool IsMouseButtonPressed(EMouseButton p_button) const;
		bool IsMouseButtonReleased(EMouseButton p_button) const;

	private:
		void OnKeyPressed(int p_key);
		void OnKeyReleased(int p_key);
		void OnMouseButtonPressed(int p_button);
		void OnMouseButtonReleased(int p_button);
	};
}
