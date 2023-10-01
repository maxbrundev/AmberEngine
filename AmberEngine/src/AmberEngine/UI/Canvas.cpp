#include "Amberpch.h"

#include "AmberEngine/UI/Canvas.h"

AmberEngine::UI::Canvas::~Canvas()
{
	RemoveAllPanels();
}

void AmberEngine::UI::Canvas::AddPanel(APanel& p_panel)
{
	m_panels.push_back(std::ref(p_panel));
}

void AmberEngine::UI::Canvas::RemovePanel(APanel& p_panel)
{
	const auto& predicate = [&p_panel](std::reference_wrapper<APanel>& p_item)
	{
		return &p_panel == &p_item.get();
	};

	m_panels.erase(std::remove_if(m_panels.begin(), m_panels.end(), predicate), m_panels.end());
}

void AmberEngine::UI::Canvas::RemoveAllPanels()
{
	m_panels.clear();
}

void AmberEngine::UI::Canvas::MakeDockspace(bool p_value)
{
	m_isDockspace = p_value;
}

void AmberEngine::UI::Canvas::Draw() const
{
	if (!m_panels.empty())
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (m_isDockspace)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			ImGui::Begin("##dockspace", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking);
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
			ImGui::SetWindowPos({ 0.f, 0.f });
			ImVec2 displaySize = ImGui::GetIO().DisplaySize;
			ImGui::SetWindowSize({ (float)displaySize.x, (float)displaySize.y });
			ImGui::End();

			ImGui::PopStyleVar(3);
		}

		for (auto& panel : m_panels)
			panel.get().Draw();

		ImGui::Render();
	}
}

bool AmberEngine::UI::Canvas::IsDockspace() const
{
	return m_isDockspace;
}
