#include "Amberpch.h"

#include "AmberEngine/UI/Panels/APanel.h"

AmberEngine::UI::APanel::APanel()
{
}

void AmberEngine::UI::APanel::Draw()
{
	if (enabled)
		DrawImplementation();
}
