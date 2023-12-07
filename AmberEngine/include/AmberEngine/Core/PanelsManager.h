#pragma once

#include "AmberEngine/UI/Canvas.h"
#include "AmberEngine/UI/Panels/MenuBar.h"

namespace AmberEngine::UI
{
	class APanel;
	class APanelWindow;
}

namespace AmberEngine::Core
{
	class PanelsManager
	{
	public:
		PanelsManager(UI::Canvas& p_canvas);

		template<typename T, typename... Args>
		void CreatePanel(const std::string& p_id, Args&&... p_args)
		{
			if constexpr (std::is_base_of<UI::Panels::APanelWindow, T>::value)
			{
				m_panels.emplace(p_id, std::make_unique<T>(p_id, std::forward<Args>(p_args)...));

				T& instance = *static_cast<T*>(m_panels.at(p_id).get());
				GetPanelAs<UI::Panels::MenuBar>("MenuBar").RegisterPanel(instance.title, instance);
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
		std::unordered_map<std::string, std::unique_ptr<UI::APanel>> m_panels;
		UI::Canvas& m_canvas;
	};
}
