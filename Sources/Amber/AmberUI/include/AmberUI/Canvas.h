#pragma once


#include "AmberUI/Panels/APanel.h"

namespace AmberUI
{
	class Canvas
	{
	public:
		Canvas() = default;
		~Canvas();

		Canvas(const Canvas& other) = delete;
		Canvas(Canvas&& other) = delete;
		Canvas& operator=(const Canvas& other) = delete;
		Canvas& operator=(Canvas&& other) = delete;

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
