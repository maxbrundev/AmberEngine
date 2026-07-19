#pragma once
#include "AmberUI/Panels/APanelWindow.h"
#include "AmberTools/Analytics/Profiling/Profiler.h"
#include "AmberTools/Analytics/Profiling/ProfilerReport.h"
#include "AmberRendering/Data/Color.h"
#include "AmberUI/Widgets/Button.h"
#include "AmberUI/Widgets/Column.h"
#include "AmberUI/Widgets/TextColored.h"

namespace AmberEditor::Panels
{
	class ProfilerPanel : public AmberUI::Panels::APanelWindow
	{
	public:
		ProfilerPanel
		(
			const std::string& p_title,
			bool p_opened,
			const AmberUI::Panels::PanelSettings& p_windowSettings,
			float p_frequency
		);

		void Update(float p_deltaTime);

		void Enable(bool p_value, bool p_disableLog = false);

	private:
		AmberRendering::Data::Color CalculateActionColor(double p_percentage) const;
		std::string GenerateActionString(AmberTools::Analytics::Profiling::ProfilerReport::Action& p_action);

	private:
		enum class EProfilingMode
		{
			DEFAULT,
			CAPTURE
		};

		float m_frequency;
		float m_timer = 0.f;
		float m_fpsTimer = 0.f;
		EProfilingMode m_profilingMode = EProfilingMode::DEFAULT;

		AmberTools::Analytics::Profiling::Profiler m_profiler;

		AmberUI::Widgets::AWidget* m_separator;
		AmberUI::Widgets::Button* m_captureResumeButton;
		AmberUI::Widgets::TextColored* m_fpsText;
		AmberUI::Widgets::TextColored* m_elapsedFramesText;
		AmberUI::Widgets::TextColored* m_elapsedTimeText;
		AmberUI::Widgets::Columns<5>* m_actionList;
	};
}
