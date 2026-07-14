#include "Amberpch.h"

#include "AmberUI/Widgets/ColorEdit.h"

AmberUI::Widgets::ColorEdit::ColorEdit(bool p_enableAlpha, const AmberRendering::Data::Color& p_defaultColor) :
DataWidget<AmberRendering::Data::Color>(Color),
EnableAlpha(p_enableAlpha),
Color(p_defaultColor)
{

}

void AmberUI::Widgets::ColorEdit::DrawImplementation()
{
	int flags = !EnableAlpha ? ImGuiColorEditFlags_NoAlpha : 0;
	bool valueChanged = false;

	if (EnableAlpha)
		valueChanged = ImGui::ColorEdit4(m_widgetID.c_str(), &Color.r, flags);
	else
		valueChanged = ImGui::ColorEdit3(m_widgetID.c_str(), &Color.r, flags);

	if (valueChanged)
	{
		ColorChangedEvent.Invoke(Color);
		NotifyChange();
	}
}
