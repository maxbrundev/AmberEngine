#include "Amberpch.h"

#include "AmberUI/wIDGETS/GroupCollapsable.h"

AmberUI::Widgets::GroupCollapsable::GroupCollapsable(const std::string& p_name) : Name(p_name)
{

}

void AmberUI::Widgets::GroupCollapsable::DrawImplementation()
{
	bool previouslyOpened = Opened;

	if (ImGui::CollapsingHeader(Name.c_str(), Closable ? &Opened : nullptr))
		Group::DrawImplementation();

	if (Opened != previouslyOpened)
	{
		if (Opened)
			OpenEvent.Invoke();
		else
			CloseEvent.Invoke();
	}
}
