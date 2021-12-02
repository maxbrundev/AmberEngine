#include "Amberpch.h"

#include "AmberEngine\UI\MenuBar.h"

#include "AmberEngine\Context\Window.h"

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

	ImGui::EndMainMenuBar();
	
}
