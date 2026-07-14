#include "Amberpch.h"

#include "AmberUI/ContextualMenuContainer.h"

void AmberUI::ContextualMenuContainer::DrawContextualMenu()
{
	if(m_contextualMenu)
		m_contextualMenu->Draw();
}
