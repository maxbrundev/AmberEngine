#include "Amberpch.h"

#include "AmberEngine/Windowing/Dialogs/MessageBox.h"

#include <windows.h>

#undef MessageBox

AmberEngine::Windowing::Dialogs::MessageBox::MessageBox(std::string p_title, std::string p_message, EMessageType p_messageType, EButtonLayout p_buttonLayout, bool p_autoSpawn) :
m_title(p_title),
m_message(p_message),
m_buttonLayout(p_buttonLayout),
m_messageType(p_messageType)
{
	if (p_autoSpawn)
		Spawn();
}

const AmberEngine::Windowing::Dialogs::MessageBox::EUserAction& AmberEngine::Windowing::Dialogs::MessageBox::GetUserAction() const
{
	return m_userResult;
}

void AmberEngine::Windowing::Dialogs::MessageBox::Spawn()
{
	int messageBoxID = MessageBoxA
	(
		nullptr,
		static_cast<LPCSTR>(m_message.c_str()),
		static_cast<LPCSTR>(m_title.c_str()),
		static_cast<UINT>(m_messageType) | static_cast<UINT>(m_buttonLayout) | MB_DEFBUTTON2
	);

	m_userResult = static_cast<EUserAction>(messageBoxID);
}
