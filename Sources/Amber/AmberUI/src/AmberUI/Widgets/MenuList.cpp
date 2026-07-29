#include "Amberpch.h"

#include "AmberUI/Widgets/MenuList.h"

#include "imgui_internal.h"

AmberUI::Widgets::MenuList::MenuList(const std::string& p_name, bool p_locked) :
Name(p_name), Locked(p_locked)
{
}

void AmberUI::Widgets::MenuList::Close()
{
	ImGuiContext& context = *ImGui::GetCurrentContext();

	if (context.OpenPopupStack.Size > 0)
	{
		ImGui::ClosePopupToLevel(context.OpenPopupStack.Size - 1, true);
	}
}

void AmberUI::Widgets::MenuList::DrawImplementation()
{
	if (ImGui::BeginMenu(Name.c_str(), !Locked))
	{
		if (!m_opened)
		{
			ClickedEvent.Invoke();
			m_opened = true;
		}

		DrawWidgets();
		ImGui::EndMenu();
	}
	else
	{
		m_opened = false;
	}
}
