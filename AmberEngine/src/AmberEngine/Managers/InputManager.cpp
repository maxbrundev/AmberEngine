#include "AmberEngine/Managers/InputManager.h"

/*void AmberEngine::Managers::InputManager::Update()
{
	m_keyEvents.clear();

	for (uint8_t i = 0; i < MAX_TOUCH_COUNT - 1; ++i)
	{
		bool keyState = GetAsyncKeyState(static_cast<int>(i)) & 0x8000;

		m_keyEvents[i] = keyState && !m_keyStates[i];
		m_keyStates[i] = keyState;
	}
}

bool AmberEngine::Managers::InputManager::IsKeyPressed(char p_key)
{
	return m_keyStates[p_key];
}

bool AmberEngine::Managers::InputManager::IsKeyEventOccured(char p_key)
{
	return m_keyEvents[p_key];
}*/

std::unordered_map<AmberEngine::Managers::InputManager::Key, bool> AmberEngine::Managers::InputManager::m_keyStates;
std::unordered_map<AmberEngine::Managers::InputManager::Key, bool> AmberEngine::Managers::InputManager::m_keyEvents;

GLdouble AmberEngine::Managers::InputManager::m_mouseLastX = 0;
GLdouble AmberEngine::Managers::InputManager::m_mouseLastY = 0;
GLdouble AmberEngine::Managers::InputManager::m_mouseXOffset = 0;
GLdouble AmberEngine::Managers::InputManager::m_mouseYOffset = 0;

bool AmberEngine::Managers::InputManager::m_firstMouse = true;
bool AmberEngine::Managers::InputManager::m_mouseButtonRight = false;
bool AmberEngine::Managers::InputManager::m_mouseButtonLeft = false;

void AmberEngine::Managers::InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Key keycode = static_cast<Key>(key);

	if (action == GLFW_PRESS)
	{
		m_keyEvents[keycode] = true;
		m_keyStates[keycode] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		m_keyEvents[keycode] = false;
		m_keyStates[keycode] = false;
	}
}

void AmberEngine::Managers::InputManager::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (m_firstMouse)
	{
		m_mouseLastX = xpos;
		m_mouseLastY = ypos;
		m_firstMouse = false;
	}

	m_mouseXOffset = xpos - m_mouseLastX;
	m_mouseYOffset = m_mouseLastY - ypos;

	m_mouseLastX = xpos;
	m_mouseLastY = ypos;
}

void AmberEngine::Managers::InputManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		m_mouseButtonRight = true;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		m_mouseButtonLeft = true;
}

bool AmberEngine::Managers::InputManager::GetKey(Key p_key)
{
	return m_keyStates.find(p_key) != m_keyStates.end() && m_keyStates[p_key];
}

bool AmberEngine::Managers::InputManager::GetKeyDown(Key p_key)
{
	return m_keyEvents.find(p_key) != m_keyEvents.end() && m_keyEvents[p_key];
}

bool AmberEngine::Managers::InputManager::GetKeyUp(Key p_key)
{
	return m_keyEvents.find(p_key) != m_keyEvents.end() && !m_keyEvents[p_key];
}

GLdouble AmberEngine::Managers::InputManager::MouseX()
{
	return static_cast<float>(m_mouseLastX);
}

GLdouble AmberEngine::Managers::InputManager::MouseY()
{
	return static_cast<float>(m_mouseLastY);
}

GLdouble AmberEngine::Managers::InputManager::MouseXOffset()
{
	return static_cast<float>(m_mouseXOffset);
}

GLdouble AmberEngine::Managers::InputManager::MouseYOffset()
{
	return static_cast<float>(m_mouseYOffset);
}

bool AmberEngine::Managers::InputManager::MouseButton(unsigned int p_button)
{
	if (p_button == 0)
		return m_mouseButtonLeft;
	if (p_button == 1)
		return m_mouseButtonRight;

	return false;
}

void AmberEngine::Managers::InputManager::Update()
{
	m_keyEvents.clear();
	m_mouseButtonRight = false;
	m_mouseButtonLeft = false;
}