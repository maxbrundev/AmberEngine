#include "Amberpch.h"

#include "AmberEngine/Inputs/InputManager.h"

AmberEngine::Inputs::InputManager::InputManager(Context::Window& p_window) : m_window(p_window)
{
	m_keyPressedEventListerId		= m_window.KeyPressedEvent.AddListener(std::bind(&InputManager::OnKeyPressed, this, std::placeholders::_1));
	m_keyReleasedEventListerId		= m_window.KeyReleasedEvent.AddListener(std::bind(&InputManager::OnKeyReleased, this, std::placeholders::_1));
	m_mousePressedEventListerId		= m_window.MouseButtonPressedEvent.AddListener(std::bind(&InputManager::OnMouseButtonPressed, this, std::placeholders::_1));
	m_mouseReleasedEventListerId	= m_window.MouseButtonReleasedEvent.AddListener(std::bind(&InputManager::OnMouseButtonReleased, this, std::placeholders::_1));
}

AmberEngine::Inputs::InputManager::~InputManager()
{
	m_window.KeyPressedEvent.RemoveListener(m_keyPressedEventListerId);
	m_window.KeyReleasedEvent.RemoveListener(m_keyReleasedEventListerId);
	m_window.MouseButtonPressedEvent.RemoveListener(m_mousePressedEventListerId);
	m_window.MouseButtonReleasedEvent.RemoveListener(m_mouseReleasedEventListerId);
}

void AmberEngine::Inputs::InputManager::clearEvents()
{
	m_keyEvents.clear();
	m_mouseEvents.clear();
}

AmberEngine::Inputs::EKeyState AmberEngine::Inputs::InputManager::GetKey(EKey p_key) const
{
	int key = glfwGetKey(m_window.GetGlfwWindow(), static_cast<int>(p_key));

	switch (key)
	{
	case GLFW_PRESS:
		return EKeyState::KEY_DOWN;
		break;
	case GLFW_RELEASE:
		return EKeyState::KEY_UP;
		break;
	}

	return EKeyState::KEY_UP;
}

AmberEngine::Inputs::EMouseButtonState AmberEngine::Inputs::InputManager::GetMouseButton(EMouseButton p_button) const
{
	int mouseButton = glfwGetMouseButton(m_window.GetGlfwWindow(), static_cast<int>(p_button));

	switch (mouseButton)
	{
	case GLFW_PRESS:
		return EMouseButtonState::MOUSE_DOWN;
		break;
	case GLFW_RELEASE:
		return EMouseButtonState::MOUSE_UP;
		break;
	}

	return EMouseButtonState::MOUSE_UP;
}

bool AmberEngine::Inputs::InputManager::IsKeyPressed(EKey p_key) const
{
	return m_keyEvents.find(p_key) != m_keyEvents.end() && m_keyEvents.at(p_key) == EKeyState::KEY_DOWN;
}

bool AmberEngine::Inputs::InputManager::IsKeyReleased(EKey p_key) const
{
	return m_keyEvents.find(p_key) != m_keyEvents.end() && m_keyEvents.at(p_key) == EKeyState::KEY_UP;
}

bool AmberEngine::Inputs::InputManager::IsMouseButtonPressed(EMouseButton p_button) const
{
	return m_mouseEvents.find(p_button) != m_mouseEvents.end() && m_mouseEvents.at(p_button) == EMouseButtonState::MOUSE_DOWN;
}

bool AmberEngine::Inputs::InputManager::IsMouseButtonReleased(EMouseButton p_button) const
{
	return m_mouseEvents.find(p_button) != m_mouseEvents.end() && m_mouseEvents.at(p_button) == EMouseButtonState::MOUSE_UP;
}

void AmberEngine::Inputs::InputManager::OnKeyPressed(int p_key)
{
	m_keyEvents[static_cast<EKey>(p_key)] = EKeyState::KEY_DOWN;
}

void AmberEngine::Inputs::InputManager::OnKeyReleased(int p_key)
{
	m_keyEvents[static_cast<EKey>(p_key)] = EKeyState::KEY_UP;
}

void AmberEngine::Inputs::InputManager::OnMouseButtonPressed(int p_button)
{
	m_mouseEvents[static_cast<EMouseButton>(p_button)] = EMouseButtonState::MOUSE_DOWN;
}

void AmberEngine::Inputs::InputManager::OnMouseButtonReleased(int p_button)
{
	m_mouseEvents[static_cast<EMouseButton>(p_button)] = EMouseButtonState::MOUSE_UP;
}
