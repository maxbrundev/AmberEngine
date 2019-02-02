#pragma once

#include "AmberEngine/Context/Device.h"

#include "AmberEngine/API/Export.h"

namespace  AmberEngine::Managers
{
	class API_AMBERENGINE UIManager
	{
	private:
		Context::Device& m_device;

	public:
		UIManager(Context::Device& p_device);
		~UIManager();

		void BeginFrame();
		void EndFrame();
		void BeginWindow(const std::string& p_title);
		void EndWindow();
		void Render();

		void DisplayDeviceInfos();
	};
}