#include "Amberpch.h"

#include "AmberEditor/Panels/Console.h"

#include "AmberEditor/Core/EditorAction.h"
#include "AmberDebug/Logger.h"
#include "AmberUI/Widgets/Button.h"
#include "AmberUI/Widgets/CheckBox.h"
#include "AmberUI/Widgets/Separator.h"
#include "AmberUI/Widgets/Spacing.h"


std::pair<AmberRendering::Data::Color, std::string> GetWidgetSettingsFromLogData(const AmberDebug::LogData& p_logData)
{
	AmberRendering::Data::Color logColor;
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
	case AmberDebug::ELogLevel::LOG_DEFAULT:	return { { 1.f, 1.f, 1.f, 1.f }, logDateFormated };
	case AmberDebug::ELogLevel::LOG_INFO:		return { { 0.f, 1.f, 1.f, 1.f }, logDateFormated };
	case AmberDebug::ELogLevel::LOG_WARNING:	return { { 1.f, 1.f, 0.f, 1.f }, logDateFormated };
	case AmberDebug::ELogLevel::LOG_ERROR:		return { { 1.f, 0.f, 0.f, 1.f }, logDateFormated };
	}
}

AmberEditor::Panels::Console::Console(const std::string& p_title, bool p_opened,
	const AmberUI::Panels::PanelSettings& p_windowSettings) : AmberUI::Panels::APanelWindow(p_title, p_opened, p_windowSettings)
{
	AllowHorizontalScrollbar = true;

	auto& clearButton = CreateWidget<AmberUI::Widgets::Button>("Clear");
	clearButton.Size = { 50.f, 0.f };
	clearButton.IdleBackgroundColor = { 0.5f, 0.f, 0.f };
	clearButton.ClickedEvent += std::bind(&Console::Clear, this);
	clearButton.LineBreak = false;

	auto& clearOnPlay = CreateWidget<AmberUI::Widgets::CheckBox>(m_clearOnPlay, "Auto clear on play");

	CreateWidget<AmberUI::Widgets::Spacing>(5).LineBreak = false;

	auto& enableDefault = CreateWidget<AmberUI::Widgets::CheckBox>(true, "Default");
	auto& enableInfo = CreateWidget<AmberUI::Widgets::CheckBox>(true, "Info");
	auto& enableWarning = CreateWidget<AmberUI::Widgets::CheckBox>(true, "Warning");
	auto& enableError = CreateWidget<AmberUI::Widgets::CheckBox>(true, "Error");

	clearOnPlay.LineBreak = false;
	enableDefault.LineBreak = false;
	enableInfo.LineBreak = false;
	enableWarning.LineBreak = false;
	enableError.LineBreak = true;

	clearOnPlay.ValueChangedEvent += [this](bool p_value) { m_clearOnPlay = p_value; };
	enableDefault.ValueChangedEvent += std::bind(&Console::SetShowDefaultLogs, this, std::placeholders::_1);
	enableInfo.ValueChangedEvent += std::bind(&Console::SetShowInfoLogs, this, std::placeholders::_1);
	enableWarning.ValueChangedEvent += std::bind(&Console::SetShowWarningLogs, this, std::placeholders::_1);
	enableError.ValueChangedEvent += std::bind(&Console::SetShowErrorLogs, this, std::placeholders::_1);

	CreateWidget<AmberUI::Widgets::Separator>();

	m_logGroup = &CreateWidget<AmberUI::Widgets::Group>();
	m_logGroup->SetReverseDrawOrder();

	EDITOR_EVENT(PlayEvent) += std::bind(&Console::ClearOnPlay, this);

	AmberDebug::Logger::LogEvent += std::bind(&Console::OnLogIntercepted, this, std::placeholders::_1);
}

void AmberEditor::Panels::Console::OnLogIntercepted(const AmberDebug::LogData& p_logData)
{
	auto[logColor, logDate] = GetWidgetSettingsFromLogData(p_logData);

	auto& consoleItem1 = m_logGroup->CreateWidget<AmberUI::Widgets::TextColored>(logDate + "\t" + p_logData.message, logColor);

	consoleItem1.Enabled = IsAllowedByFilter(p_logData.logLevel);

	m_logTextWidgets[&consoleItem1] = p_logData.logLevel;
}

void AmberEditor::Panels::Console::ClearOnPlay()
{
	if (m_clearOnPlay)
		Clear();
}

void AmberEditor::Panels::Console::Clear()
{
	m_logTextWidgets.clear();
	m_logGroup->RemoveAllWidgets();
}

void AmberEditor::Panels::Console::FilterLogs()
{
	for (const auto&[widget, logLevel] : m_logTextWidgets)
		widget->Enabled = IsAllowedByFilter(logLevel);
}

bool AmberEditor::Panels::Console::IsAllowedByFilter(AmberDebug::ELogLevel p_logLevel)
{
	switch (p_logLevel)
	{
	case AmberDebug::ELogLevel::LOG_DEFAULT:	return m_showDefaultLog;
	case AmberDebug::ELogLevel::LOG_INFO:		return m_showInfoLog;
	case AmberDebug::ELogLevel::LOG_WARNING:	return m_showWarningLog;
	case AmberDebug::ELogLevel::LOG_ERROR:		return m_showErrorLog;
	}

	return false;
}

void AmberEditor::Panels::Console::SetShowDefaultLogs(bool p_value)
{
	m_showDefaultLog = p_value;
	FilterLogs();
}

void AmberEditor::Panels::Console::SetShowInfoLogs(bool p_value)
{
	m_showInfoLog = p_value;
	FilterLogs();
}

void AmberEditor::Panels::Console::SetShowWarningLogs(bool p_value)
{
	m_showWarningLog = p_value;
	FilterLogs();
}

void AmberEditor::Panels::Console::SetShowErrorLogs(bool p_value)
{
	m_showErrorLog = p_value;
	FilterLogs();
}
