#include "Amberpch.h"

#include "AmberEditor/Panels/HardwareInfoPanel.h"
#include "AmberTools/Analytics/Hardware/HardwareInfo.h"

#include "AmberEditor/Core/EditorAction.h"
#include "AmberUI/Widgets/PlotLines.h"
#include "AmberUI/Widgets/Text.h"

AmberEditor::Panels::HardwareInfoPanel::HardwareInfoPanel(const std::string& p_title, bool p_opened, const AmberUI::Panels::PanelSettings& p_windowSettings, float p_logFrequency, size_t p_maxElements) :
	AmberUI::Panels::APanelWindow(p_title, p_opened, p_windowSettings),
	m_logFrequency(p_logFrequency),
	m_maxElements(p_maxElements),
	m_hardwareInfo(new AmberTools::Analytics::Hardware::HardwareInfo(m_logFrequency))
{
	auto& renderer = *EDITOR_CONTEXT(renderer).get();

	// Display some static information about the graphics driver
	CreateWidget<AmberUI::Widgets::Text>("Vendor: " + renderer.GetDriver().GetString(GL_VENDOR));
	CreateWidget<AmberUI::Widgets::Text>("Hardware: " + renderer.GetDriver().GetString(GL_RENDERER));
	CreateWidget<AmberUI::Widgets::Text>("OpenGL Version: " + renderer.GetDriver().GetString(GL_VERSION));
	CreateWidget<AmberUI::Widgets::Text>("GLSL Version: " + renderer.GetDriver().GetString(GL_SHADING_LANGUAGE_VERSION));

	m_cpuUsage = &CreateWidget<AmberUI::Widgets::PlotLines>();
	m_gpuUsage = &CreateWidget<AmberUI::Widgets::PlotLines>();
	m_ramUsage = &CreateWidget<AmberUI::Widgets::PlotLines>();

	m_cpuUsage->MinScale = 0.0f;
	m_cpuUsage->MaxScale = 100.0f;
	m_cpuUsage->Size.y = 75.0f;
	m_cpuUsage->Data.resize(m_maxElements, 0);
	m_cpuUsage->Overlay = "CPU Usage (%)";

	m_gpuUsage->MinScale = 0.0f;
	m_gpuUsage->MaxScale = 100.0f;
	m_gpuUsage->Size.y = 75.0f;
	m_gpuUsage->Data.resize(m_maxElements, 0);
	m_gpuUsage->Overlay = "GPU Usage (%)";

	m_ramUsage->MinScale = 0.0f;
	m_ramUsage->MaxScale = 100.0f;
	m_ramUsage->Size.y = 75.0f;
	m_ramUsage->Data.resize(m_maxElements, 0);
	m_ramUsage->Overlay = "RAM Usage (%)";
}

AmberEditor::Panels::HardwareInfoPanel::~HardwareInfoPanel()
{
	delete m_hardwareInfo;
}

void AmberEditor::Panels::HardwareInfoPanel::Update(float p_deltaTime)
{
	m_hardwareInfo->Tick();

	p_updateTimer += p_deltaTime;

	while (p_updateTimer >= m_logFrequency)
	{
		AmberTools::Analytics::Hardware::HardwareReport report = m_hardwareInfo->GenerateReport();

		m_cpuUsage->Data.push_back(report.CPULoad);
		m_gpuUsage->Data.push_back(report.GPULoad);
		m_ramUsage->Data.push_back((report.RAMUsed / report.RAMMax) * 100.0f);

		if (m_cpuUsage->Data.size() > m_maxElements)
			m_cpuUsage->Data.erase(m_cpuUsage->Data.begin());

		if (m_gpuUsage->Data.size() > m_maxElements)
			m_gpuUsage->Data.erase(m_gpuUsage->Data.begin());

		if (m_ramUsage->Data.size() > m_maxElements)
			m_ramUsage->Data.erase(m_ramUsage->Data.begin());

		p_updateTimer -= m_logFrequency;
	}
}
