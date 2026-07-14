#include "Amberpch.h"

#include "AmberUI/Widgets/InputText.h"

AmberUI::Widgets::InputText::InputText(const std::string& p_content, const std::string& p_label) :
DataWidget<std::string>(Content), Content(p_content), Label(p_label)
{
}

void AmberUI::Widgets::InputText::DrawImplementation()
{
	std::string previousContent = Content;

	Content.resize(256, '\0');
	bool enterPressed = ImGui::InputText((Label + m_widgetID).c_str(), &Content[0], 256, ImGuiInputTextFlags_EnterReturnsTrue | (SelectAllOnClick ? ImGuiInputTextFlags_AutoSelectAll : 0));
	Content = Content.c_str();

	if (Content != previousContent)
	{
		ContentChangedEvent.Invoke(Content);
		NotifyChange();
	}

	if (enterPressed)
	{
		EnterPressedEvent.Invoke(Content);
	}
}
