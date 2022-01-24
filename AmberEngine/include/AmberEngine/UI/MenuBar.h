#pragma once

#include "AmberEngine/Core/Context.h"

namespace AmberEngine::UI
{
	class MenuBar
	{
	public:
		MenuBar(Core::Context& p_context);
		~MenuBar() = default;

		void Draw();

	private:
		Core::Context& m_context;
	};
}