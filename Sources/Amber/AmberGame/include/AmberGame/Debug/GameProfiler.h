#pragma once

#ifdef _DEBUG

#include "AmberTools/Analytics/Profiling/Profiler.h"

#include "AmberUI/Panels/PanelUndecorated.h"
#include "AmberUI/Widgets/Group.h"
#include "AmberUI/Widgets/TextColored.h"

#include "AmberWindowing/Context/Window.h"

namespace AmberGame::Debug
{
	class GameProfiler : public AmberUI::Panels::PanelUndecorated
	{
	public:
		GameProfiler(AmberWindowing::Context::Window& p_window, float p_frequency);

		void Update(float p_deltaTime);

	private:
		AmberRendering::Data::Color CalculateActionColor(double p_percentage) const;
		std::string GenerateActionString(AmberTools::Analytics::Profiling::ProfilerReport::Action& p_action);

	private:
		float m_frequency;
		float m_timer = 0.f;

		AmberTools::Analytics::Profiling::Profiler m_profiler;

		AmberWindowing::Context::Window& m_window;
		AmberUI::Widgets::AWidget* m_separator;
		AmberUI::Widgets::TextColored* m_elapsedFramesText;
		AmberUI::Widgets::TextColored* m_elapsedTimeText;
		AmberUI::Widgets::Group* m_actionList;
	};
}

#endif
