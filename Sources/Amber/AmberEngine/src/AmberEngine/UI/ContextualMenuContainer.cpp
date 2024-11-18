#include "Amberpch.h"

#include "AmberEngine/UI/ContextualMenuContainer.h"

void AmberEngine::UI::ContextualMenuContainer::DrawContextualMenu()
{
	if(m_contextualMenu)
		m_contextualMenu->Draw();
}
