#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/Context.h"

namespace AmberEngine::Core
{
	class API_AMBERENGINE EditorRenderer
	{
	public:
		EditorRenderer(Context& p_context);
		void InitMaterials();
		void RenderScene(const glm::vec3& p_cameraPosition, const LowRenderer::Camera& p_camera);
		void RenderUI();
		void UpdateLights(SceneSystem::Scene& p_scene);

	private:
		Context& m_context;
		Resources::Material m_emptyMaterial;
		Resources::Material m_defaultMaterial;
	};
}
