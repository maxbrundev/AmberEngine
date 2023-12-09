#pragma once

#include "AmberEngine/UI/Panels/MenuBar.h"
#include "AmberEngine/UI/Panels/APanel.h"
#include "AmberEngine/UI/Panels/APanelWindow.h"
#include "AmberEngine/UI/Canvas.h"
#include "AmberEngine/Data/EditorConstants.h"

namespace AmberEngine::Core
{
	class PanelsManager
	{
	public:
		PanelsManager(AmberEngine::UI::Canvas& p_canvas);

		template<typename T, typename... Args>
		void CreatePanel(const std::string& p_id, Args&&... p_args)
		{
			if constexpr (std::is_base_of<AmberEngine::UI::Panels::APanelWindow, T>::value)
			{
				m_panels.emplace(p_id, std::make_unique<T>(p_id, std::forward<Args>(p_args)...));

				T& instance = *static_cast<T*>(m_panels.at(p_id).get());
				GetPanelAs<AmberEngine::UI::Panels::MenuBar>(Data::EditorConstants::EDITOR_PANEL_MENU_BAR_TITLE).RegisterPanel(instance.title, instance);
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
		std::unordered_map<std::string, std::unique_ptr<AmberEngine::UI::Panels::APanel>> m_panels;
		UI::Canvas& m_canvas;
	};
}
