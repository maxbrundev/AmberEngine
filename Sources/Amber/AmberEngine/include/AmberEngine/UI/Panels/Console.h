#pragma once
#include "APanelWindow.h"
#include "AmberEngine/Debug/ILogHandler.h"
#include "AmberEngine/UI/Widgets/Group.h"
#include "AmberEngine/UI/Widgets/TextColored.h"


namespace AmberEngine::UI::Panels
{
	class Console : public APanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		Console
		(
			const std::string& p_title,
			bool p_opened,
			const PanelSettings& p_windowSettings
		);

		/**
		* Method called when a log event occured
		* @param p_logData
		*/
		void OnLogIntercepted(const Debug::LogData& p_logData);

		/**
		* Called when the scene plays. It will clear the console if the "Clear on play" settings is on
		*/
		void ClearOnPlay();

		/**
		* Clear the console
		*/
		void Clear();

		/**
		* Filter logs using defined filters
		*/
		void FilterLogs();

		/**
		* Verify if a given log level is allowed by the current filter
		* @param p_logLevel
		*/
		bool IsAllowedByFilter(Debug::ELogLevel p_logLevel);

	private:
		void SetShowDefaultLogs(bool p_value);
		void SetShowInfoLogs(bool p_value);
		void SetShowWarningLogs(bool p_value);
		void SetShowErrorLogs(bool p_value);

	private:
		Widgets::Group* m_logGroup;
		std::unordered_map<Widgets::TextColored*, Debug::ELogLevel> m_logTextWidgets;

		bool m_clearOnPlay = true;
		bool m_showDefaultLog = true;
		bool m_showInfoLog = true;
		bool m_showWarningLog = true;
		bool m_showErrorLog = true;
	};
}
