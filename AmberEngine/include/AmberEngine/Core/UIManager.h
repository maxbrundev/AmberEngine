#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Context/Window.h"

//TODO: Refacto.

namespace AmberEngine::Core
{
	class API_AMBERENGINE UIManager
	{
	private:
		AmberEngine::Context::Window& m_window;

	public:
		UIManager(AmberEngine::Context::Window& p_window);
		~UIManager();

		void BeginFrame();
		void EndFrame();
		void BeginWindow(const std::string& p_title);
		void EndWindow();
		void Render();

		void DisplayMenu();
		void DisplayDeviceInfos();
	};
}