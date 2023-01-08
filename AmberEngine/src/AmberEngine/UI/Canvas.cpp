#include "Amberpch.h"

#include "AmberEngine/UI/Canvas.h"

AmberEngine::UI::Canvas::~Canvas()
{
	RemoveAllPanels();
}

void AmberEngine::UI::Canvas::AddPanel(APanel& p_panel)
{
	m_panels.push_back(std::ref(p_panel));
}

void AmberEngine::UI::Canvas::RemovePanel(APanel& p_panel)
{
	const auto& predicate = [&p_panel](std::reference_wrapper<APanel>& p_item)
	{
		return &p_panel == &p_item.get();
	};

	m_panels.erase(std::remove_if(m_panels.begin(), m_panels.end(), predicate), m_panels.end());
}

void AmberEngine::UI::Canvas::RemoveAllPanels()
{
	m_panels.clear();
}

void AmberEngine::UI::Canvas::MakeDockspace(bool p_value)
{
	m_isDockspace = p_value;
}

bool AmberEngine::UI::Canvas::IsDockspace() const
{
	return m_isDockspace;
}
