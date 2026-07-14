#pragma once


#include "AmberRendering/Context/Driver.h"
#include "AmberWindowing/Context/Device.h"
#include "AmberWindowing/Context/Window.h"
#include "AmberRendering/Core/Renderer.h"
#include "AmberUI/Core/UIManager.h"
#include "AmberUI/Panels/APanelWindow.h"
#include "AmberUI/Canvas.h"

namespace AmberEditor::Core
{
	class ProjectHub
	{
	public:
		ProjectHub();

		void SetupContext();

		std::tuple<bool, std::string, std::string> Run();

		void RegisterProject(const std::string& p_path);

	private:
		std::unique_ptr<AmberWindowing::Context::Device> m_device;
		std::unique_ptr<AmberWindowing::Context::Window> m_window;
		std::unique_ptr<AmberRendering::Context::Driver> m_driver;
		std::unique_ptr<AmberRendering::Core::Renderer> m_renderer;
		std::unique_ptr<AmberUI::Core::UIManager> m_uiManager;
		std::unique_ptr<AmberUI::Panels::APanelWindow> m_mainPanel;

		AmberUI::Canvas m_canvas;

		std::string m_projectPath;
		std::string m_projectName;

		bool m_readyToGo = false;
	};
}
