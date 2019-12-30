#pragma once

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Context
{
	enum class API_AMBERENGINE EDeviceError
	{
		 AE_GLFW_NOT_INITIALIZED		= 0x00010001,
		 AE_GLFW_NO_CURRENT_CONTEXT	= 0x00010002,
		 AE_GLFW_INVALID_ENUM			= 0x00010003,
		 AE_GLFW_INVALID_VALUE			= 0x00010004,
		 AE_GLFW_OUT_OF_MEMORY			= 0x00010005,
		 AE_GLFW_API_UNAVAILABLE		= 0x00010006,
		 AE_GLFW_VERSION_UNAVAILABLE	= 0x00010007,
		 AE_GLFW_PLATFORM_ERROR		= 0x00010008,
		 AE_GLFW_FORMAT_UNAVAILABLE	= 0x00010009,
		 AE_GLFW_NO_WINDOW_CONTEXT		= 0x0001000A
	};
}