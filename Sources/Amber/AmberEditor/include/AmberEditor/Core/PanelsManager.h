#pragma once

#include "AmberEditor/Panels/MenuBar.h"
#include "AmberUI/Panels/APanel.h"
#include "AmberUI/Panels/APanelWindow.h"
#include "AmberUI/Canvas.h"
#include "AmberEditor/Data/EditorConstants.h"

namespace AmberEditor::Core
{
	class PanelsManager
	{
	public:
		PanelsManager(AmberUI::Canvas& p_canvas);

		template<typename T, typename... Args>
		void CreatePanel(const std::string& p_id, Args&&... p_args)
		{
			if constexpr (std::is_base_of<AmberUI::Panels::APanelWindow, T>::value)
			{
				m_panels.emplace(p_id, std::make_unique<T>(p_id, std::forward<Args>(p_args)...));

				T& instance = *static_cast<T*>(m_panels.at(p_id).get());
				GetPanelAs<AmberEditor::Panels::MenuBar>(Data::EditorConstants::EDITOR_PANEL_MENU_BAR_TITLE).RegisterPanel(instance.Title, instance);
			}
			else
			{
				m_panels.emplace(p_id, std::make_unique<T>(std::forward<Args>(p_args)...));
			}

			m_canvas.AddPanel(*m_panels[p_id]);
		}

		template<typename T>
		T& GetPanelAs(const std::string& p_id)
		{
			return *static_cast<T*>(m_panels[p_id].get());
		}

	private:
		std::unordered_map<std::string, std::unique_ptr<AmberUI::Panels::APanel>> m_panels;
		AmberUI::Canvas& m_canvas;
	};
}
