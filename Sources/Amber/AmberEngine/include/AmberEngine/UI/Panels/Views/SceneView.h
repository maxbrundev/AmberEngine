#pragma once

#include "AmberEngine/UI/Panels/Views/AView.h"

#include "AmberEngine/Core/CameraController.h"
#include "AmberEngine/Core/SceneSystem/SceneManager.h"

namespace AmberEngine::UI::Panels
{
	class SceneView : public AView
	{
	public:
		SceneView(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings);

		void Update(float p_deltaTime) override;
		void RenderSceneForActorPicking();
		void HandleActorPicking();
		void RenderScene(uint8_t p_defaultRenderState);

		AmberEngine::Core::CameraController& GetCameraController();

	protected:
		void RenderImplementation() override;

	private:
		void HandleRectangleSelectInputs();
		void HandleRectangleSelect();
		void UpdateSelectedActorsTransform();

	private:
		AmberEngine::Core::SceneSystem::SceneManager& m_sceneManager;
		AmberEngine::Core::CameraController m_cameraController;
		AmberEngine::Core::GizmoBehaviour m_gizmoOperations;
		AmberEngine::Core::EGizmoOperation m_currentOperation = AmberEngine::Core::EGizmoOperation::TRANSLATE;
		Buffers::FrameBuffer m_actorPickingFramebuffer;
		std::optional<std::reference_wrapper<AmberEngine::Core::ECS::Actor>> m_highlightedActor;
		std::optional<AmberEngine::Core::GizmoBehaviour::EDirection> m_highlightedGizmoDirection;

		glm::vec2 m_selectionStartPosition;
		glm::vec2 m_selectionEndPosition;

		bool m_isSelecting = false;

		Maths::Transform m_selectionTransform;

		glm::vec3 m_previousSelectionActorPosition;

		struct SelectedActorData
		{
			AmberEngine::Core::ECS::Actor* actor;
			glm::vec3 initialLocalPosition;
			glm::quat initialLocalRotation;
			glm::vec3 initialLocalScale;
		};

		std::vector<SelectedActorData> m_selectedActorsData;
	};

}
