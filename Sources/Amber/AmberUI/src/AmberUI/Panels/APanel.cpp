#include "Amberpch.h"

#include "AmberUI/Panels/APanel.h"

AmberUI::Panels::APanel::APanel()
{
}

void AmberUI::Panels::APanel::Draw()
{
	if (Enabled)
	{
		DrawImplementation();
	}
}
