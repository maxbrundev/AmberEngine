#include "Amberpch.h"

#include "AmberEngine/UI/Panels/HardwareInfoPanel.h"
#include "AmberEngine/Analytics/Hardware/HardwareInfo.h"

#include "AmberEngine/Core/EditorAction.h"
#include "AmberEngine/UI/Widgets/PlotLines.h"
#include "AmberEngine/UI/Widgets/Text.h"

AmberEngine::UI::Panels::HardwareInfoPanel::HardwareInfoPanel(const std::string& p_title, bool p_opened,
                                                    const PanelSettings& p_windowSettings, float p_logFrequency, size_t p_maxElements) :
	APanelWindow(p_title, p_opened, p_windowSettings),
	m_logFrequency(p_logFrequency),
	m_maxElements(p_maxElements),
	m_hardwareInfo(new AmberEngine::Analytics::Hardware::HardwareInfo(m_logFrequency))
{
	auto& renderer = *EDITOR_CONTEXT(renderer).get();

	// Display some static information about the graphics driver
	CreateWidget<Widgets::Text>("Vendor: " + renderer.GetDriver().GetString(GL_VENDOR));
	CreateWidget<Widgets::Text>("Hardware: " + renderer.GetDriver().GetString(GL_RENDERER));
	CreateWidget<Widgets::Text>("OpenGL Version: " + renderer.GetDriver().GetString(GL_VERSION));
	CreateWidget<Widgets::Text>("GLSL Version: " + renderer.GetDriver().GetString(GL_SHADING_LANGUAGE_VERSION));

	m_cpuUsage = &CreateWidget<Widgets::PlotLines>();
	m_gpuUsage = &CreateWidget<Widgets::PlotLines>();
	m_ramUsage = &CreateWidget<Widgets::PlotLines>();

	m_cpuUsage->minScale = 0.0f;
	m_cpuUsage->maxScale = 100.0f;
	m_cpuUsage->size.y = 75.0f;
	m_cpuUsage->data.resize(m_maxElements, 0);
	m_cpuUsage->overlay = "CPU Usage (%)";

	m_gpuUsage->minScale = 0.0f;
	m_gpuUsage->maxScale = 100.0f;
	m_gpuUsage->size.y = 75.0f;
	m_gpuUsage->data.resize(m_maxElements, 0);
	m_gpuUsage->overlay = "GPU Usage (%)";

	m_ramUsage->minScale = 0.0f;
	m_ramUsage->maxScale = 100.0f;
	m_ramUsage->size.y = 75.0f;
	m_ramUsage->data.resize(m_maxElements, 0);
	m_ramUsage->overlay = "RAM Usage (%)";
}

AmberEngine::UI::Panels::HardwareInfoPanel::~HardwareInfoPanel()
{
	delete m_hardwareInfo;
}

void AmberEngine::UI::Panels::HardwareInfoPanel::Update(float p_deltaTime)
{
	m_hardwareInfo->Tick();

	p_updateTimer += p_deltaTime;

	while (p_updateTimer >= m_logFrequency)
	{
		Analytics::Hardware::HardwareReport report = m_hardwareInfo->GenerateReport();

		m_cpuUsage->data.push_back(report.CPULoad);
		m_gpuUsage->data.push_back(report.GPULoad);
		m_ramUsage->data.push_back((report.RAMUsed / report.RAMMax) * 100.0f);

		if (m_cpuUsage->data.size() > m_maxElements)
			m_cpuUsage->data.erase(m_cpuUsage->data.begin());

		if (m_gpuUsage->data.size() > m_maxElements)
			m_gpuUsage->data.erase(m_gpuUsage->data.begin());

		if (m_ramUsage->data.size() > m_maxElements)
			m_ramUsage->data.erase(m_ramUsage->data.begin());

		p_updateTimer -= m_logFrequency;
	}
}
