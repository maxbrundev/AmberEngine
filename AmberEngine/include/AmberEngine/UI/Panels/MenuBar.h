#pragma once

namespace AmberEngine::UI
{
	class MenuBar
	{
	public:
		MenuBar();
		~MenuBar() = default;

		void Draw();

		std::function<void(bool)> NormalsColorsVisualizationCallback;

	private:
		void DisplayFileMenu() const;
		void DisplayDrawModeMenu();

	private:
		bool m_isShadeDrawMode       = true;
		bool m_isWireframeDrawMode   = false;
		bool m_isNormalDebugDrawMode = false;
	};
}