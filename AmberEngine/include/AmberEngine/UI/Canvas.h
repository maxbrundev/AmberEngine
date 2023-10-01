#pragma once

#include "Panels/APanel.h"

namespace AmberEngine::UI
{
	class Canvas
	{
	public:
		Canvas() = default;
		~Canvas();

		void AddPanel(APanel& p_panel);
		void RemovePanel(APanel& p_panel);
		void RemoveAllPanels();
		void MakeDockspace(bool p_value);
		void Draw() const;
		bool IsDockspace() const;

	private:
		bool m_isDockspace = false;

		std::vector<std::reference_wrapper<APanel>> m_panels;
	};
}
