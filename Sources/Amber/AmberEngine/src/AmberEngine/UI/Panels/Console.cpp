#include "Amberpch.h"

#include "AmberEngine/UI/Panels/Console.h"

#include "AmberEngine/Core/EditorAction.h"
#include "AmberEngine/Debug/Logger.h"
#include "AmberEngine/UI/Widgets/Button.h"
#include "AmberEngine/UI/Widgets/CheckBox.h"
#include "AmberEngine/UI/Widgets/Separator.h"
#include "AmberEngine/UI/Widgets/Spacing.h"


std::pair<AmberEngine::Data::Color, std::string> GetWidgetSettingsFromLogData(const AmberEngine::Debug::LogData& p_logData)
{
	AmberEngine::Data::Color logColor;
	std::string logHeader;
	std::string logDateFormated = "[";
	bool isSecondPart = false;
	std::for_each(p_logData.date.begin(), p_logData.date.end(), [&logDateFormated, &isSecondPart](char c)
	{
		if (isSecondPart)
			logDateFormated.push_back(c == '-' ? ':' : c);

		if (c == '_')
			isSecondPart = true;
	});

	logDateFormated += "] ";

	switch (p_logData.logLevel)
	{
	default:
	case AmberEngine::Debug::ELogLevel::LOG_DEFAULT:	return { { 1.f, 1.f, 1.f, 1.f }, logDateFormated };
	case AmberEngine::Debug::ELogLevel::LOG_INFO:		return { { 0.f, 1.f, 1.f, 1.f }, logDateFormated };
	case AmberEngine::Debug::ELogLevel::LOG_WARNING:	return { { 1.f, 1.f, 0.f, 1.f }, logDateFormated };
	case AmberEngine::Debug::ELogLevel::LOG_ERROR:		return { { 1.f, 0.f, 0.f, 1.f }, logDateFormated };
	}
}

AmberEngine::UI::Panels::Console::Console(const std::string& p_title, bool p_opened,
	const PanelSettings& p_windowSettings) : APanelWindow(p_title, p_opened, p_windowSettings)
{
	allowHorizontalScrollbar = true;

	auto& clearButton = CreateWidget<Widgets::Button>("Clear");
	clearButton.size = { 50.f, 0.f };
	clearButton.idleBackgroundColor = { 0.5f, 0.f, 0.f };
	clearButton.ClickedEvent += std::bind(&Console::Clear, this);
	clearButton.lineBreak = false;

	auto& clearOnPlay = CreateWidget<Widgets::CheckBox>(m_clearOnPlay, "Auto clear on play");

	CreateWidget<Widgets::Spacing>(5).lineBreak = false;

	auto& enableDefault = CreateWidget<Widgets::CheckBox>(true, "Default");
	auto& enableInfo = CreateWidget<Widgets::CheckBox>(true, "Info");
	auto& enableWarning = CreateWidget<Widgets::CheckBox>(true, "Warning");
	auto& enableError = CreateWidget<Widgets::CheckBox>(true, "Error");

	clearOnPlay.lineBreak = false;
	enableDefault.lineBreak = false;
	enableInfo.lineBreak = false;
	enableWarning.lineBreak = false;
	enableError.lineBreak = true;

	clearOnPlay.ValueChangedEvent += [this](bool p_value) { m_clearOnPlay = p_value; };
	enableDefault.ValueChangedEvent += std::bind(&Console::SetShowDefaultLogs, this, std::placeholders::_1);
	enableInfo.ValueChangedEvent += std::bind(&Console::SetShowInfoLogs, this, std::placeholders::_1);
	enableWarning.ValueChangedEvent += std::bind(&Console::SetShowWarningLogs, this, std::placeholders::_1);
	enableError.ValueChangedEvent += std::bind(&Console::SetShowErrorLogs, this, std::placeholders::_1);

	CreateWidget<Widgets::Separator>();

	m_logGroup = &CreateWidget<Widgets::Group>();
	m_logGroup->SetReverseDrawOrder();

	EDITOR_EVENT(PlayEvent) += std::bind(&Console::ClearOnPlay, this);

	Debug::Logger::LogEvent += std::bind(&Console::OnLogIntercepted, this, std::placeholders::_1);
}

void AmberEngine::UI::Panels::Console::OnLogIntercepted(const Debug::LogData& p_logData)
{
	auto[logColor, logDate] = GetWidgetSettingsFromLogData(p_logData);

	auto& consoleItem1 = m_logGroup->CreateWidget<Widgets::TextColored>(logDate + "\t" + p_logData.message, logColor);

	consoleItem1.enabled = IsAllowedByFilter(p_logData.logLevel);

	m_logTextWidgets[&consoleItem1] = p_logData.logLevel;
}

void AmberEngine::UI::Panels::Console::ClearOnPlay()
{
	if (m_clearOnPlay)
		Clear();
}

void AmberEngine::UI::Panels::Console::Clear()
{
	m_logTextWidgets.clear();
	m_logGroup->RemoveAllWidgets();
}

void AmberEngine::UI::Panels::Console::FilterLogs()
{
	for (const auto&[widget, logLevel] : m_logTextWidgets)
		widget->enabled = IsAllowedByFilter(logLevel);
}

bool AmberEngine::UI::Panels::Console::IsAllowedByFilter(Debug::ELogLevel p_logLevel)
{
	switch (p_logLevel)
	{
	case Debug::ELogLevel::LOG_DEFAULT:	return m_showDefaultLog;
	case Debug::ELogLevel::LOG_INFO:		return m_showInfoLog;
	case Debug::ELogLevel::LOG_WARNING:	return m_showWarningLog;
	case Debug::ELogLevel::LOG_ERROR:		return m_showErrorLog;
	}

	return false;
}

void AmberEngine::UI::Panels::Console::SetShowDefaultLogs(bool p_value)
{
	m_showDefaultLog = p_value;
	FilterLogs();
}

void AmberEngine::UI::Panels::Console::SetShowInfoLogs(bool p_value)
{
	m_showInfoLog = p_value;
	FilterLogs();
}

void AmberEngine::UI::Panels::Console::SetShowWarningLogs(bool p_value)
{
	m_showWarningLog = p_value;
	FilterLogs();
}

void AmberEngine::UI::Panels::Console::SetShowErrorLogs(bool p_value)
{
	m_showErrorLog = p_value;
	FilterLogs();
}
