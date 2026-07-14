#pragma once

#ifdef _DEBUG

#include "AmberRendering/Context/Driver.h"
#include "AmberWindowing/Context/Window.h"

#include "AmberUI/Panels/PanelUndecorated.h"

namespace AmberGame::Debug
{
	class DriverInfo : public AmberUI::Panels::PanelUndecorated
	{
	public:
		DriverInfo(AmberRendering::Context::Driver& p_driver, AmberWindowing::Context::Window& p_window);
	};
}

#endif
