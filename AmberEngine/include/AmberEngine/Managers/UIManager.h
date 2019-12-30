#pragma once

#include "AmberEngine/Context/Window.h"

#include "AmberEngine/API/Export.h"

namespace  AmberEngine::Managers
{
	/**
	* Temporary class
	*/
	
	class API_AMBERENGINE UIManager
	{
	private:
		Context::Window& m_window;

	public:
		UIManager(Context::Window& p_window);
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