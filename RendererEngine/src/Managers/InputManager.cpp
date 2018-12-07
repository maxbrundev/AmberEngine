#include "pch.h"

#include "Managers/InputManager.h"

void RenderEngine::Managers::InputManager::Update()
{
	m_keyEvents.clear();

	for (uint8_t i = 0; i < MAX_TOUCH_COUNT - 1; ++i)
	{
		bool keyState = GetAsyncKeyState(static_cast<int>(i)) & 0x8000;

		m_keyEvents[i] = keyState && !m_keyStates[i];
		m_keyStates[i] = keyState;
	}
}

bool RenderEngine::Managers::InputManager::IsKeyPressed(char p_key)
{
	return m_keyStates[p_key];
}

bool RenderEngine::Managers::InputManager::IsKeyEventOccured(char p_key)
{
	return m_keyEvents[p_key];
}