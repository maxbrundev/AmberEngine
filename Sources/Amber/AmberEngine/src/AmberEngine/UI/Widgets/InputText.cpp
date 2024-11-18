#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/InputText.h"

AmberEngine::UI::Widgets::InputText::InputText(const std::string& p_content, const std::string& p_label) :
DataWidget<std::string>(content), content(p_content), label(p_label)
{
}

void AmberEngine::UI::Widgets::InputText::DrawImplementation()
{
	std::string previousContent = content;

	content.resize(256, '\0');
	bool enterPressed = ImGui::InputText((label + m_widgetID).c_str(), &content[0], 256, ImGuiInputTextFlags_EnterReturnsTrue | (selectAllOnClick ? ImGuiInputTextFlags_AutoSelectAll : 0));
	content = content.c_str();

	if (content != previousContent)
	{
		ContentChangedEvent.Invoke(content);
		NotifyChange();
	}

	if (enterPressed)
	{
		EnterPressedEvent.Invoke(content);
	}
}
