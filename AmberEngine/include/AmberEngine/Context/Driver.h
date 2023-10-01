#pragma once

#include <GL/glew.h>

#include "AmberEngine/Settings/DriverSettings.h"

namespace  AmberEngine::Context
{
	class Driver
	{
	public:
		Driver(const Settings::DriverSettings& p_driverSettings);
		~Driver() = default;

		void DisplayDriverInfo() const;

		std::string_view m_api_version() const;
		std::string_view m_library_version() const;
		std::string_view m_render1() const;
		std::string_view m_vendor1() const;
		std::string_view m_shading_language_version1() const;
		
	private:
		void InitGlew();
		static void __stdcall GLDebugMessageCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char* message, const void* userParam);

	private:
		bool isActive;

		std::string_view m_apiVersion;
		std::string_view m_libraryVersion;
		std::string_view m_render;
		std::string_view m_vendor;
		std::string_view m_shading_language_version;

	};
}
