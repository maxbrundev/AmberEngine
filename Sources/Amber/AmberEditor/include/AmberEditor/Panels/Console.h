#pragma once
#include "AmberUI/Panels/APanelWindow.h"
#include "AmberDebug/ILogHandler.h"
#include "AmberUI/Widgets/Group.h"
#include "AmberUI/Widgets/TextColored.h"


namespace AmberEditor::Panels
{
	class Console : public AmberUI::Panels::APanelWindow
	{
	public:
		Console
		(
			const std::string& p_title,
			bool p_opened,
			const AmberUI::Panels::PanelSettings& p_windowSettings
		);

		void OnLogIntercepted(const AmberDebug::LogData& p_logData);

		void ClearOnPlay();

		void Clear();

		void FilterLogs();

		bool IsAllowedByFilter(AmberDebug::ELogLevel p_logLevel);

	private:
		void SetShowDefaultLogs(bool p_value);
		void SetShowInfoLogs(bool p_value);
		void SetShowWarningLogs(bool p_value);
		void SetShowErrorLogs(bool p_value);

	private:
		AmberUI::Widgets::Group* m_logGroup;
		std::unordered_map<AmberUI::Widgets::TextColored*, AmberDebug::ELogLevel> m_logTextWidgets;

		bool m_clearOnPlay = true;
		bool m_showDefaultLog = true;
		bool m_showInfoLog = true;
		bool m_showWarningLog = true;
		bool m_showErrorLog = true;
	};
}
