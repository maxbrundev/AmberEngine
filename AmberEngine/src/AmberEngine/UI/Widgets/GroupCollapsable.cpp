#include "Amberpch.h"

#include "AmberEngine/UI/wIDGETS/GroupCollapsable.h"

AmberEngine::UI::Widgets::GroupCollapsable::GroupCollapsable(const std::string& p_name) : name(p_name)
{

}

void AmberEngine::UI::Widgets::GroupCollapsable::DrawImplementation()
{
	bool previouslyOpened = opened;

	if (ImGui::CollapsingHeader(name.c_str(), closable ? &opened : nullptr))
		Group::DrawImplementation();

	if (opened != previouslyOpened)
	{
		if (opened)
			OpenEvent.Invoke();
		else
			CloseEvent.Invoke();
	}
}
