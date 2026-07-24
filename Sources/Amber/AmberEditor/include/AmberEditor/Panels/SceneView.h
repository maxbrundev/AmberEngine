#pragma once

#include "AmberEditor/Panels/AView.h"

#include "AmberEditor/Core/CameraController.h"
#include "AmberCore/SceneSystem/SceneManager.h"

namespace AmberEditor::Panels
{
	class SceneView : public AView
	{
	public:
		SceneView(const std::string& p_title, bool p_opened, AmberUI::Panels::PanelSettings p_panelSettings);

		void Update(float p_deltaTime) override;
		void RenderSceneForActorPicking();
		void HandleActorPicking();
		void RenderScene(uint8_t p_defaultRenderState);

		AmberEditor::Core::CameraController& GetCameraController();

	protected:
		void RenderImplementation() override;

	private:
		void HandleRectangleSelectInputs();
		void HandleRectangleSelect();
		void UpdateSelectedActorsTransform();

	private:
		AmberCore::SceneSystem::SceneManager& m_sceneManager;
		AmberEditor::Core::CameraController m_cameraController;
		AmberEditor::Core::GizmoBehaviour m_gizmoOperations;
		AmberEditor::Core::EGizmoOperation m_currentOperation = AmberEditor::Core::EGizmoOperation::TRANSLATE;
		AmberRendering::Buffers::FrameBuffer m_actorPickingFramebuffer;
		std::optional<std::reference_wrapper<AmberCore::ECS::Actor>> m_highlightedActor;
		std::optional<AmberEditor::Core::GizmoBehaviour::EDirection> m_highlightedGizmoDirection;

		glm::vec2 m_selectionStartPosition;
		glm::vec2 m_selectionEndPosition;

		bool m_isSelecting = false;

		AmberMaths::Transform m_selectionTransform;

		struct SelectedActorData
		{
			AmberCore::ECS::Actor* Actor;
			glm::vec3 OffsetPosition;
			glm::vec3 OffsetScale;
			glm::quat OffsetRotation;
		};

		std::vector<SelectedActorData> m_selectedActorsData;
	};

}
