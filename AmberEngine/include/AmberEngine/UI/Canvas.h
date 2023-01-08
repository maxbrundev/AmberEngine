#pragma once

#include "AmberEngine/API/Export.h"

#include "Panels/APanel.h"

namespace AmberEngine::UI
{
	class API_AMBERENGINE Canvas
	{
	public:
		Canvas() = default;
		~Canvas();

		void AddPanel(APanel& p_panel);
		void RemovePanel(APanel& p_panel);
		void RemoveAllPanels();
		void MakeDockspace(bool p_value);

		bool IsDockspace() const;

		void Draw();

	private:
		std::vector<std::reference_wrapper<APanel>> m_panels;
		bool m_isDockspace = false;
	};
}
