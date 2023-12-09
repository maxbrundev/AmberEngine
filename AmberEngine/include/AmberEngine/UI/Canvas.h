#pragma once

#include "AmberEngine/UI/Panels/APanel.h"

namespace AmberEngine::UI
{
	class Canvas
	{
	public:
		Canvas() = default;
		~Canvas();

		void AddPanel(Panels::APanel& p_panel);
		void RemovePanel(Panels::APanel& p_panel);
		void RemoveAllPanels();
		void MakeDockspace(bool p_value);
		void Draw() const;
		void PostDraw() const;
		bool IsDockspace() const;

	private:
		bool m_isDockspace = false;

		std::vector<std::reference_wrapper<Panels::APanel>> m_panels;
	};
}
