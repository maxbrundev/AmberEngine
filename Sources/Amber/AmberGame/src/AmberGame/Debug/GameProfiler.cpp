#include "Amberpch.h"

#ifdef _DEBUG

#include "AmberGame/Debug/GameProfiler.h"

#include "AmberTools/Analytics/Profiling/ProfilerSpy.h"
#include "AmberUI/Widgets/Separator.h"
#include "AmberUI/Widgets/Text.h"

AmberGame::Debug::GameProfiler::GameProfiler(AmberWindowing::Context::Window& p_window, float p_frequency) :
	m_frequency(p_frequency),
	m_window(p_window)
{
	m_defaultHorizontalAlignment = AmberUI::EHorizontalAlignment::LEFT;
	m_defaultPosition = { 10.0f, 10.0f };

	m_elapsedFramesText = &CreateWidget<AmberUI::Widgets::TextColored>("", AmberRendering::Data::Color(1.f, 0.8f, 0.01f, 1.f));
	m_elapsedTimeText = &CreateWidget<AmberUI::Widgets::TextColored>("", AmberRendering::Data::Color(1.f, 0.8f, 0.01f, 1.f));
	m_separator = &CreateWidget<AmberUI::Widgets::Separator>();
	m_actionList = &CreateWidget<AmberUI::Widgets::Group>();

	m_actionList->CreateWidget<AmberUI::Widgets::Text>("Action | Total duration | Frame Duration | Frame load | Total calls");

	AmberTools::Analytics::Profiling::Profiler::Enable();
}

void AmberGame::Debug::GameProfiler::Update(float p_deltaTime)
{
	PROFILER_SPY("Game Profiler Update");

	m_position = { 10.0f, static_cast<float>(m_window.GetSize().second / 2) };

	m_timer += p_deltaTime;

	if (AmberTools::Analytics::Profiling::Profiler::IsEnabled())
	{
		m_profiler.Update(p_deltaTime);

		while (m_timer >= m_frequency)
		{
			AmberTools::Analytics::Profiling::ProfilerReport report = m_profiler.GenerateReport();
			m_profiler.ClearHistory();
			m_actionList->RemoveAllWidgets();

			m_elapsedFramesText->Content = "Elapsed frames: " + std::to_string(report.elapsedFrames);
			m_elapsedTimeText->Content = "Elapsed time: " + std::to_string(report.elaspedTime);

			m_actionList->CreateWidget<AmberUI::Widgets::Text>("Action | Total duration | Frame Duration | Frame load | Total calls");

			for (AmberTools::Analytics::Profiling::ProfilerReport::Action& action : report.actions)
			{
				AmberRendering::Data::Color color = CalculateActionColor(action.percentage);
				m_actionList->CreateWidget<AmberUI::Widgets::TextColored>(GenerateActionString(action), color);
			}

			m_timer -= m_frequency;
		}
	}
}

AmberRendering::Data::Color AmberGame::Debug::GameProfiler::CalculateActionColor(double p_percentage) const
{
	if (p_percentage <= 25.0f)
		return { 0.0f, 1.0f, 0.0f, 1.0f };
	else if (p_percentage <= 50.0f)
		return { 1.0f, 1.0f, 0.0f, 1.0f };
	else if (p_percentage <= 75.0f)
		return { 1.0f, 0.6f, 0.0f, 1.0f };
	else
		return { 1.0f, 0.0f, 0.0f, 1.0f };
}

std::string AmberGame::Debug::GameProfiler::GenerateActionString(AmberTools::Analytics::Profiling::ProfilerReport::Action& p_action)
{
	std::string result;

	result += "[" + p_action.name + "]";
	result += std::to_string(p_action.duration) + "s (total) | ";
	result += std::to_string(p_action.duration / p_action.calls) + "s (per call) | ";
	result += std::to_string(p_action.percentage) + "%% | ";
	result += std::to_string(p_action.calls) + " calls";

	return result;
}

#endif
