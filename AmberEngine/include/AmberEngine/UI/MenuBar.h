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

		std::function<void(bool)> NormalsColorsVisualizationCallback;

	private:
		void DisplayFileMenu() const;
		void DisplayDrawModeMenu();

	private:
		Core::Context& m_context;
		bool m_isShadeDrawMode     = true;
		bool m_isWireframeDrawMode = false;
		bool m_isNormalDebugDrawMode = false;
	};
}