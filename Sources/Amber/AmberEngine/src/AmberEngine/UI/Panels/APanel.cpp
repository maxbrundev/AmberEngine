#include "Amberpch.h"

#include "AmberEngine/UI/Panels/APanel.h"

AmberEngine::UI::Panels::APanel::APanel()
{
}

void AmberEngine::UI::Panels::APanel::Draw()
{
	if (Enabled)
	{
		m_callbackQueue.Process();
		DrawImplementation();
	}
}
