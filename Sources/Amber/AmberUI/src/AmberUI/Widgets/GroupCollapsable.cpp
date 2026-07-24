#include "Amberpch.h"

#include "AmberUI/wIDGETS/GroupCollapsable.h"

AmberUI::Widgets::GroupCollapsable::GroupCollapsable(const std::string& p_name) : Name(p_name)
{
}

void AmberUI::Widgets::GroupCollapsable::DrawImplementation()
{
	const bool previouslyOpened = Opened;

	ImGui::SetNextItemOpen(!Collapsed);

	const bool isOpen = ImGui::CollapsingHeader(Name.c_str(), Closable ? &Opened : nullptr);

	if (isOpen)
		Group::DrawImplementation();

	Collapsed = !isOpen;

	if (Opened != previouslyOpened)
	{
		if (Opened)
			OpenEvent.Invoke();
		else
			CloseEvent.Invoke();
	}
}
