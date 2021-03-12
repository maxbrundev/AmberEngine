#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/Context.h"
#include "AmberEngine/Core/Editor.h"

#include "AmberEngine/Managers/LayerManager.h"

//TODO: Remove the Delta Time / Time computation from this class.
//TODO: Integrate Clock class.
//TODO: Integrate new Resources Loaders and Managers.
//TODO: Integrate Logger class.
//TODO: Integrate Docking / Scene / Scene Manager / SceneView.
//TODO: Integrate Maths Lib.
//TODO: Integrate ECS.

namespace AmberEngine::Core
{
	class API_AMBERENGINE Application
	{
	public:
		Application(const AmberEngine::Settings::DeviceSettings& p_deviceSettings, const AmberEngine::Settings::WindowSettings& p_windowSettings, const AmberEngine::Settings::DriverSettings& p_driverSettings);
		virtual ~Application() = default;
		
		void Run();
		bool PushLayer(ALayer* p_layer);
		bool IsRunning();

		void UpdateDeltaTime();
		
	public:
		Context m_context;
		Editor m_editor;
		
	private:
		std::unique_ptr<Managers::LayerManager> m_layerManager;

		bool isRunning;

		float m_deltaTime = 0.0f;
		float m_lastTime = 0.0f;
	};
}
