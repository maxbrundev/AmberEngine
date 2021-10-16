#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/Context.h"

#include "AmberEngine/Views/SceneView.h"

namespace AmberEngine::Core
{
	class API_AMBERENGINE Editor
	{
	public:
		Editor(Context& p_context);
		~Editor() = default;

		void PreUpdate();
		void Update(float p_deltaTime);
		void PostUpdate();
		void RenderScene();
		
		void UpdateInput();

		void FreeCamera();
		void LockCamera();
		void ToggleCamera();
		void SetCameraPosition(const glm::vec3& p_position);
		
	public:
		AmberEngine::Core::Context&	m_context;

		AmberEngine::Views::SceneView m_sceneView;
		bool isCameraFree;
	};
}
