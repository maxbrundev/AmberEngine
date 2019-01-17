#pragma once

#include "AmberEngine/Context/Device.h"

#include "AmberEngine/API/Export.h"

namespace  AmberEngine::Managers
{
	class API_AMBERENGINE UIManager
	{
	private:
		Core::Device& m_device;
		float m_clearColor[4];

	public:
		UIManager(Core::Device& p_context);
		~UIManager();

		void PreUpdate();
		void Update();
		void PostUpdate();
		void DisplayMenuBar();
		void Close();
	};
}