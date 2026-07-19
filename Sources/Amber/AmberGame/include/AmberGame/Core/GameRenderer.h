#pragma once

#include "AmberCore/ECS/Components/CCamera.h"
#include "AmberCore/SceneSystem/SceneManager.h"

#include "AmberGame/Core/Context.h"

namespace AmberGame::Core
{
	class GameRenderer
	{
	public:
		GameRenderer(Context& p_context);

		void RenderScene();

		void UpdateEngineUBO(AmberCore::ECS::Components::CCamera& p_mainCamera);

		void UpdateLights(AmberCore::SceneSystem::Scene& p_scene);

		void UpdateLightsInFrustum(AmberCore::SceneSystem::Scene& p_scene, const AmberRendering::Data::Frustum& p_frustum);

	private:
		Context& m_context;
		AmberCore::Resources::Material m_emptyMaterial;
	};
}
