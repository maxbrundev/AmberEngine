#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Context/Driver.h"
#include "AmberEngine/Context/Device.h"
#include "AmberEngine/Context/Window.h"
#include "AmberEngine/Rendering/Core/Renderer.h"
#include "AmberEngine/UI/Core/UIManager.h"
#include "AmberEngine/UI/Panels/APanelWindow.h"
#include "AmberEngine/UI/Canvas.h"

namespace AmberEngine::Core
{
	class API_AMBERENGINE ProjectHub
	{
	public:
		ProjectHub();

		void SetupContext();

		std::tuple<bool, std::string, std::string> Run();

		void RegisterProject(const std::string& p_path);

	private:
		std::unique_ptr<AmberEngine::Context::Device> m_device;
		std::unique_ptr<AmberEngine::Context::Window> m_window;
		std::unique_ptr<AmberEngine::Context::Driver> m_driver;
		std::unique_ptr<AmberEngine::Rendering::Core::Renderer> m_renderer;
		std::unique_ptr<AmberEngine::UI::Core::UIManager> m_uiManager;
		std::unique_ptr<AmberEngine::UI::Panels::APanelWindow> m_mainPanel;

		AmberEngine::UI::Canvas m_canvas;

		std::string m_projectPath;
		std::string m_projectName;

		bool m_readyToGo = false;
	};
}
