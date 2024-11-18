#pragma once
#include "APanelWindow.h"
#include "AmberEngine/Analytics/Profiling/Profiler.h"
#include "AmberEngine/Analytics/Profiling/ProfilerReport.h"
#include "AmberEngine/Data/Color.h"
#include "AmberEngine/UI/Widgets/Button.h"
#include "AmberEngine/UI/Widgets/Column.h"
#include "AmberEngine/UI/Widgets/TextColored.h"

namespace AmberEngine::UI::Panels
{
	class ProfilerPanel : public APanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		* @param p_frequency
		*/
		ProfilerPanel
		(
			const std::string& p_title,
			bool p_opened,
			const PanelSettings& p_windowSettings,
			float p_frequency
		);

		/**
		* Update profiling information
		* @param p_deltaTime
		*/
		void Update(float p_deltaTime);

		/**
		* Enable or disable the profiler
		* @param p_value
		* @param p_disableLog
		*/
		void Enable(bool p_value, bool p_disableLog = false);

	private:
	    Data::Color CalculateActionColor(double p_percentage) const;
		std::string GenerateActionString(Analytics::Profiling::ProfilerReport::Action& p_action);

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

		Analytics::Profiling::Profiler m_profiler;

		Widgets::AWidget* m_separator;
		Widgets::Button* m_captureResumeButton;
		Widgets::TextColored* m_fpsText;
		Widgets::TextColored* m_elapsedFramesText;
		Widgets::TextColored* m_elapsedTimeText;
		Widgets::Columns<5>* m_actionList;
	};
}
