#include "Amberpch.h"

#include "AmberEngine\UI\MenuBar.h"

#include "AmberEngine\Context\Window.h"

AmberEngine::UI::MenuBar::MenuBar(Core::Context& p_context) : m_context(p_context)
{
}

void AmberEngine::UI::MenuBar::Draw()
{
	ImGui::BeginMainMenuBar();
	
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Exit"))
		{
			Context::Window::CloseEvent.Invoke(true);
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Draw Mode"))
	{
		if (ImGui::MenuItem("Shaded"))
		{
			m_context.renderer->PolygonModeFill();
		}

		if (ImGui::MenuItem("Wireframe"))
		{
			m_context.renderer->PolygonModeLine();
		}

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
	
}
