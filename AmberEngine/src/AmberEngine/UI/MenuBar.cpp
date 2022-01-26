#include "Amberpch.h"

#include "AmberEngine/UI/MenuBar.h"

#include "AmberEngine/Context/Window.h"

AmberEngine::UI::MenuBar::MenuBar(Core::Context& p_context) : m_context(p_context)
{
}

void AmberEngine::UI::MenuBar::Draw()
{
	ImGui::BeginMainMenuBar();
	
	DisplayFileMenu();
	DisplayDrawModeMenu();

	ImGui::EndMainMenuBar();
}

void AmberEngine::UI::MenuBar::DisplayFileMenu() const
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Exit"))
		{
			Context::Window::CloseEvent.Invoke(true);
		}

		ImGui::EndMenu();
	}
}

void AmberEngine::UI::MenuBar::DisplayDrawModeMenu()
{
	if (ImGui::BeginMenu("Draw Mode"))
	{
		if (ImGui::Checkbox("Shaded", &m_isShadeDrawMode))
		{
			m_isShadeDrawMode = true;
			m_isWireframeDrawMode = false;
			m_isNormalDebugDrawMode = false;

			m_context.renderer->PolygonModeFill();

			m_context.m_scene.SetDebugNormal(false);
		}

		if (ImGui::Checkbox("Wireframe", &m_isWireframeDrawMode))
		{
			m_isWireframeDrawMode = true;
			m_isShadeDrawMode = false;
			m_isNormalDebugDrawMode = false;

			m_context.renderer->PolygonModeLine();

			m_context.m_scene.SetDebugNormal(false);
		}

		if (ImGui::Checkbox("Normals", &m_isNormalDebugDrawMode))
		{
			m_isNormalDebugDrawMode = true;
			m_isWireframeDrawMode = false;
			m_isShadeDrawMode = false;

			m_context.renderer->PolygonModeFill();
			m_context.m_scene.SetDebugNormal(true);
		}

		ImGui::EndMenu();
	}
}
