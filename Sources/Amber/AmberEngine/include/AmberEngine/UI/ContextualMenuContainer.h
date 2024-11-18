#pragma once

#include "AmberEngine/UI/Widgets/AContextualMenu.h"

namespace AmberEngine::UI
{
	class ContextualMenuContainer
	{
	public:
		ContextualMenuContainer() = default;
		virtual ~ContextualMenuContainer() = default;

		void DrawContextualMenu();

		template <typename T, typename ... Args>
		T& CreateContextualMenu(Args&&... p_args)
		{
			m_contextualMenu = std::make_unique<T>(p_args...);
			return *static_cast<T*>(m_contextualMenu.get());
		}

	public:
		std::unique_ptr<Widgets::AContextualMenu> m_contextualMenu;
	};
}
