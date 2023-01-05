#include "Amberpch.h"

#include "AmberEngine/UI/MenuBar.h"

#include "AmberEngine/Context/Window.h"
#include "AmberEngine/Data/Constants.h"

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
	if (ImGui::BeginMenu(Data::Constants::EDITOR_PANEL_MENU_BAR_FILE_NAME.c_str()))
	{
		if (ImGui::MenuItem(Data::Constants::EDITOR_PANEL_MENU_ITEM_EXIT_NAME.c_str()))
		{
			Context::Window::CloseEvent.Invoke(true);
		}

		ImGui::EndMenu();
	}
}

void AmberEngine::UI::MenuBar::DisplayDrawModeMenu()
{
	if (ImGui::BeginMenu(Data::Constants::EDITOR_PANEL_MENU_BAR_DRAW_MODE_NAME.c_str()))
	{
		if (ImGui::MenuItem(Data::Constants::EDITOR_PANEL_MENU_ITEM_SHADED_NAME.c_str(), nullptr, &m_isShadeDrawMode))
		{
			m_isShadeDrawMode = true;
			m_isWireframeDrawMode = false;
			m_isNormalDebugDrawMode = false;

			m_context.renderer->PolygonModeFill();

			NormalsColorsVisualizationCallback(false);
		}

		if (ImGui::MenuItem(Data::Constants::EDITOR_PANEL_MENU_ITEM_WIREFRAME_NAME.c_str(), nullptr, &m_isWireframeDrawMode))
		{
			m_isWireframeDrawMode = true;
			m_isShadeDrawMode = false;
			m_isNormalDebugDrawMode = false;

			m_context.renderer->PolygonModeLine();
			NormalsColorsVisualizationCallback(false);
		}

		if (ImGui::MenuItem(Data::Constants::EDITOR_PANEL_MENU_ITEM_NORMALS_NAME.c_str(),nullptr, &m_isNormalDebugDrawMode))
		{
			m_isNormalDebugDrawMode = true;
			m_isWireframeDrawMode = false;
			m_isShadeDrawMode = false;

			m_context.renderer->PolygonModeFill();
			NormalsColorsVisualizationCallback(true);
		}

		ImGui::EndMenu();
	}
}
