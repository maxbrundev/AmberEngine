#pragma once

#include <AmberEngine/Core/Context.h>
#include <AmberEngine/Core/Editor.h>

namespace Example
{
	class Application
	{
	public:
		Application(const AmberEngine::Settings::DeviceSettings& p_deviceSettings, const AmberEngine::Settings::WindowSettings& p_windowSettings, const AmberEngine::Settings::DriverSettings& p_driverSettings);
		~Application() = default;

		void Setup();
		void Run();
		bool IsRunning() const;

	private:
		AmberEngine::Core::Context m_context;
		AmberEngine::Core::Editor m_editor;

		bool isRunning;
	};
}