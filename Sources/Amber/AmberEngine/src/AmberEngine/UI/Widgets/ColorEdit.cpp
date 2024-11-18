#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/ColorEdit.h"

AmberEngine::UI::Widgets::ColorEdit::ColorEdit(bool p_enableAlpha, const Data::Color& p_defaultColor) :
DataWidget<Data::Color>(color),
enableAlpha(p_enableAlpha),
color(p_defaultColor)
{

}

void AmberEngine::UI::Widgets::ColorEdit::DrawImplementation()
{
	int flags = !enableAlpha ? ImGuiColorEditFlags_NoAlpha : 0;
	bool valueChanged = false;

	if (enableAlpha)
		valueChanged = ImGui::ColorEdit4(m_widgetID.c_str(), &color.r, flags);
	else
		valueChanged = ImGui::ColorEdit3(m_widgetID.c_str(), &color.r, flags);

	if (valueChanged)
	{
		ColorChangedEvent.Invoke(color);
		NotifyChange();
	}
}
