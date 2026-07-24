#include "Amberpch.h"

#include "AmberEditor/Panels/SceneView.h"

#include "AmberTools/Analytics/Profiling/Profiler.h"
#include "AmberTools/Analytics/Profiling/ProfilerSpy.h"
#include "AmberEditor/Core/Editor.h"
#include "AmberEditor/Core/EditorAction.h"
#include "AmberEditor/Panels/GameView.h"
#include "AmberEditor/Settings/EditorSettings.h"
#include "AmberCore/SceneSystem/SceneManager.h"
#include "AmberCore/SceneSystem/Scene.h"

#include "AmberTools/Global/ServiceLocator.h"
#include "AmberTools/Utils/PathParser.h"

AmberEditor::Panels::SceneView::SceneView(const std::string& p_title, bool p_opened, AmberUI::Panels::PanelSettings p_panelSettings) : 
AView(p_title, p_opened, p_panelSettings),
m_sceneManager(AmberTools::Global::ServiceLocator::Get<AmberCore::SceneSystem::SceneManager>()),
m_cameraController(m_camera, m_cameraPosition, m_cameraRotation, true)
{
	m_camera.SetClearColor({ 0.098f, 0.098f, 0.098f });
	m_camera.SetFar(5000.0f);

	m_image->AddDragDropTarget("File").DataReceivedEvent += [this](std::pair<std::string, AmberUI::Widgets::Group*> p_data)
	{
		std::string path = p_data.first;

		switch (AmberTools::Utils::PathParser::GetFileType(path))
		{
		case AmberTools::Utils::PathParser::EFileType::SCENE:
			EDITOR_EXEC(LoadSceneFromDisk(path));
			break;
		case AmberTools::Utils::PathParser::EFileType::MODEL:
			EDITOR_EXEC(CreateActorWithModel(path, true));
			break;
		}
	};

	EDITOR_EVENT(ActorSelectionEvent) += [&]()
	{
		auto& selectedActors = EDITOR_EXEC(GetSelectedActors());

		const size_t selectedActorsCount = selectedActors.size();
		
		m_selectedActorsData.clear();

		if (selectedActorsCount > 1)
		{
			glm::vec3 center(0.0f);

			for (auto actor : selectedActors)
			{
				center += actor->transform.GetWorldPosition();
			}

			center /= static_cast<float>(selectedActors.size());

			m_selectionTransform.SetWorldPosition(center);
			m_selectionTransform.SetWorldScale({1.0f, 1.0f, 1.0f});
			m_selectionTransform.SetWorldRotation(glm::quat());

			for (auto actor : selectedActors)
			{
				SelectedActorData selectedActorData;

				selectedActorData.Actor = actor;
				selectedActorData.OffsetPosition = actor->transform.GetWorldPosition() - center;
				selectedActorData.OffsetScale = actor->transform.GetWorldScale();
				selectedActorData.OffsetRotation = actor->transform.GetWorldRotation();
				m_selectedActorsData.push_back(selectedActorData);
			}
		}
	};
}

void AmberEditor::Panels::SceneView::Update(float p_deltaTime)
{
	AView::Update(p_deltaTime);

	if(IsHovered())
	{
		m_cameraController.Update(p_deltaTime);
	}

	if (IsFocused() && !m_cameraController.IsRightMousePressed())
	{
		if (EDITOR_CONTEXT(inputManager)->IsKeyPressed(AmberWindowing::Inputs::EKey::KEY_W))
		{
			m_currentOperation = AmberEditor::Core::EGizmoOperation::TRANSLATE;
		}

		if (EDITOR_CONTEXT(inputManager)->IsKeyPressed(AmberWindowing::Inputs::EKey::KEY_E))
		{
			m_currentOperation = AmberEditor::Core::EGizmoOperation::ROTATE;
		}

		if (EDITOR_CONTEXT(inputManager)->IsKeyPressed(AmberWindowing::Inputs::EKey::KEY_R))
		{
			m_currentOperation = AmberEditor::Core::EGizmoOperation::SCALE;
		}
	}

	HandleRectangleSelectInputs();
	UpdateSelectedActorsTransform();
}

void AmberEditor::Panels::SceneView::RenderSceneForActorPicking()
{
	auto& baseRenderer = *EDITOR_CONTEXT(renderer).get();

	auto[winWidth, winHeight] = GetSafeSize();

	m_actorPickingFramebuffer.Resize(winWidth, winHeight);
	m_actorPickingFramebuffer.Bind();

	baseRenderer.SetClearColor(1.0f, 1.0f, 1.0f);
	baseRenderer.Clear();

	m_editorRenderer.RenderSceneForActorPicking();

	if (EDITOR_EXEC(IsAnyActorSelected()))
	{
		auto& selectedActors = EDITOR_EXEC(GetSelectedActors());

		const size_t selectedActorsCount = selectedActors.size();

		baseRenderer.Clear(false, true, false);

		if (selectedActorsCount == 1)
		{
			m_editorRenderer.RenderGizmo(selectedActors.front()->transform.GetWorldPosition(), selectedActors.front()->transform.GetWorldRotation(), m_currentOperation, true);
		}
		else if (selectedActorsCount > 1)
		{
			m_editorRenderer.RenderGizmo(m_selectionTransform.GetWorldPosition(), m_selectionTransform.GetWorldRotation(), m_currentOperation, true);
		}
	}

	m_actorPickingFramebuffer.Unbind();
}

bool IsResizing()
{
	auto cursor = ImGui::GetMouseCursor();

	return cursor == ImGuiMouseCursor_ResizeEW
		|| cursor == ImGuiMouseCursor_ResizeNS
		|| cursor == ImGuiMouseCursor_ResizeNWSE
		|| cursor == ImGuiMouseCursor_ResizeNESW
		|| cursor == ImGuiMouseCursor_ResizeAll;
}

void AmberEditor::Panels::SceneView::HandleActorPicking()
{
	auto& inputManager = *EDITOR_CONTEXT(inputManager);

	if (inputManager.IsMouseButtonReleased(AmberWindowing::Inputs::EMouseButton::MOUSE_BUTTON_LEFT))
	{
		m_gizmoOperations.StopPicking();
	}

	if (IsHovered() && !IsResizing())
	{
		RenderSceneForActorPicking();

		auto[mouseX, mouseY] = inputManager.GetMousePosition();
		mouseX -= m_position.x;
		mouseY -= m_position.y;
		mouseY = GetSafeSize().second - mouseY + 25;

		m_actorPickingFramebuffer.Bind();

		uint8_t pixel[3];
		EDITOR_CONTEXT(renderer)->GetDriver().ReadPixels(static_cast<int>(mouseX), static_cast<int>(mouseY), 1, 1, AmberRendering::Settings::EPixelDataFormat::RGB, AmberRendering::Settings::EPixelDataType::UNSIGNED_BYTE, pixel);

		m_actorPickingFramebuffer.Unbind();

		uint32_t actorID = (0 << 24) | (pixel[2] << 16) | (pixel[1] << 8) | (pixel[0] << 0);

		auto actorUnderMouse = EDITOR_CONTEXT(sceneManager).GetCurrentScene()->FindActorByID(actorID);

		auto direction = m_gizmoOperations.IsPicking() ? m_gizmoOperations.GetDirection() : EDITOR_EXEC(IsAnyActorSelected()) && pixel[0] == 255 && pixel[1] == 255 && pixel[2] >= 252 && pixel[2] <= 254 ? static_cast<AmberEditor::Core::GizmoBehaviour::EDirection>(pixel[2] - 252) : std::optional<AmberEditor::Core::GizmoBehaviour::EDirection>{};

		m_highlightedActor = {};
		m_highlightedGizmoDirection = {};

		if (!m_cameraController.IsRightMousePressed())
		{
			if (direction.has_value())
			{
				m_highlightedGizmoDirection = direction;
			}
			else if (actorUnderMouse != nullptr)
			{
				m_highlightedActor = std::ref(*actorUnderMouse);
			}
		}

		if (inputManager.IsMouseButtonPressed(AmberWindowing::Inputs::EMouseButton::MOUSE_BUTTON_LEFT) && !m_cameraController.IsRightMousePressed())
		{
			if (direction.has_value())
			{
				if(EDITOR_EXEC(IsAnyActorSelected()) && !EDITOR_EXEC(IsManyActorsSelected()))
				{
					m_gizmoOperations.StartPicking(EDITOR_EXEC(GetSelectedActor()), m_cameraPosition, m_currentOperation, direction.value());
				}
				else
				{
					m_gizmoOperations.StartPicking(m_selectionTransform, m_cameraPosition, m_currentOperation, direction.value());
				}
			}
			else
			{
				if (actorUnderMouse)
				{
					if (EDITOR_CONTEXT(inputManager)->GetKey(AmberWindowing::Inputs::EKey::KEY_LEFT_CONTROL) == AmberWindowing::Inputs::EKeyState::KEY_DOWN)
					{
						EDITOR_EXEC(ToggleActorSelection(*actorUnderMouse));
					}
					else
					{
						EDITOR_EXEC(SelectActor(*actorUnderMouse));
					}
				}
				else
				{
					if (EDITOR_CONTEXT(inputManager)->GetKey(AmberWindowing::Inputs::EKey::KEY_LEFT_CONTROL) != AmberWindowing::Inputs::EKeyState::KEY_DOWN)
					{
						EDITOR_EXEC(UnselectActor());
					}
				}
			}
		}
	}
	else
	{
		m_highlightedActor = std::nullopt;
	}

	if (m_gizmoOperations.IsPicking())
	{
		auto mousePosition = EDITOR_CONTEXT(inputManager)->GetMousePosition();

		auto[winWidth, winHeight] = GetSafeSize();

		m_gizmoOperations.SetCurrentMouse({ static_cast<float>(mousePosition.first - m_position.x), static_cast<float>(mousePosition.second - m_position.y) });
		m_gizmoOperations.ApplyOperation(m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(), m_cameraPosition, { static_cast<float>(winWidth), static_cast<float>(winHeight) });
	}
}

void AmberEditor::Panels::SceneView::RenderScene(uint8_t p_defaultRenderState)
{
	auto& baseRenderer = *EDITOR_CONTEXT(renderer).get();
	auto& driver = *EDITOR_CONTEXT(driver).get();

	auto& currentScene = *m_sceneManager.GetCurrentScene();
	auto& gameView = EDITOR_PANEL(AmberEditor::Panels::GameView, "Game View");

	if (auto gameViewFrustum = gameView.GetActiveFrustum(); gameViewFrustum && gameView.GetCamera().HasFrustumLightCulling() && Settings::EditorSettings::ShowLightFrustumCullingInSceneView)
	{
		m_editorRenderer.UpdateLightsInFrustum(currentScene, *gameViewFrustum);
	}
	else
	{
		m_editorRenderer.UpdateLights(currentScene);
	}

	m_frameBuffer.Bind();

	driver.SetStencilMask(0xFF);
	baseRenderer.Clear(m_camera);
	driver.SetStencilMask(0x00);

	m_editorRenderer.RenderGrid(m_cameraPosition, m_gridColor, m_div, m_bias, m_lwidht, m_mwidth);
	m_editorRenderer.RenderCameras();

	if (auto gameViewFrustum = gameView.GetActiveFrustum(); gameViewFrustum && gameView.GetCamera().HasFrustumGeometryCulling() && Settings::EditorSettings::ShowGeometryFrustumCullingInSceneView)
	{
		m_camera.SetFrustumGeometryCulling(gameView.HasCamera() ? gameView.GetCamera().HasFrustumGeometryCulling() : false);
		m_editorRenderer.RenderScene(m_cameraPosition, m_camera, gameViewFrustum);
		m_camera.SetFrustumGeometryCulling(false);
	}
	else
	{
		m_editorRenderer.RenderScene(m_cameraPosition, m_camera);
	}

	m_editorRenderer.RenderLights();
	
	if (EDITOR_EXEC(IsAnyActorSelected()))
	{
		auto& selectedActors = EDITOR_EXEC(GetSelectedActors());

		const size_t selectedActorsCount = selectedActors.size();

		m_editorRenderer.RenderActorOutlinePass(selectedActors, true, true);
		baseRenderer.ApplyStateMask(p_defaultRenderState);
		m_editorRenderer.RenderActorOutlinePass(selectedActors, false, true);

		baseRenderer.ApplyStateMask(p_defaultRenderState);
		baseRenderer.Clear(false, true, false);

		int highlightedAxis = -1;

		if (m_highlightedGizmoDirection.has_value())
		{
			highlightedAxis = static_cast<int>(m_highlightedGizmoDirection.value());
		}

		if (selectedActorsCount == 1)
		{
			m_editorRenderer.RenderGizmo(selectedActors.front()->transform.GetWorldPosition(), selectedActors.front()->transform.GetWorldRotation(), m_currentOperation, false, highlightedAxis);
		}
		else if (selectedActorsCount > 1)
		{
			m_editorRenderer.RenderGizmo(m_selectionTransform.GetWorldPosition(), m_selectionTransform.GetWorldRotation(), m_currentOperation, false, highlightedAxis);
		}
	}

	if (m_highlightedActor.has_value())
	{
		m_editorRenderer.RenderActorOutlinePass(m_highlightedActor.value().get(), true, false);
		baseRenderer.ApplyStateMask(p_defaultRenderState);
		m_editorRenderer.RenderActorOutlinePass(m_highlightedActor.value().get(), false, false);
	}

	if (m_isSelecting)
	{
		auto& shapeDrawer = *EDITOR_CONTEXT(shapeDrawer).get();

		auto[viewportWidth, viewportHeight] = GetSafeSize();

		glm::vec2 startPos = m_selectionStartPosition -glm::vec2(0.0f, Data::EditorConstants::EDITOR_FONT_SIZE_MEDIUM + ImGui::GetStyle().FramePadding.y * 2);
		glm::vec2 endPos   = m_selectionEndPosition - glm::vec2(0.0f, Data::EditorConstants::EDITOR_FONT_SIZE_MEDIUM + ImGui::GetStyle().FramePadding.y * 2);

		glm::mat4 orthoProjection = glm::ortho(0.0f, static_cast<float>(viewportWidth), static_cast<float>(viewportHeight), 0.0f, -1.0f, 1.0f);
		shapeDrawer.SetViewProjection(orthoProjection);
		
		driver.SetCapability(AmberRendering::Settings::ERenderingCapability::DEPTH_TEST, false);

		//TODO: Move this part in the EditorRenderer
		glm::vec3 color = glm::vec3(0.0f, 0.47f, 0.84f);

		glm::vec2 topLeft     = { std::min(startPos.x, endPos.x), std::min(startPos.y, endPos.y) };
		glm::vec2 topRight    = { std::max(startPos.x, endPos.x), std::min(startPos.y, endPos.y) };
		glm::vec2 bottomLeft  = { std::min(startPos.x, endPos.x), std::max(startPos.y, endPos.y) };
		glm::vec2 bottomRight = { std::max(startPos.x, endPos.x), std::max(startPos.y, endPos.y) };

		shapeDrawer.DrawLine(glm::vec3(topLeft, 0.0f), glm::vec3(topRight, 0.0f), color);
		shapeDrawer.DrawLine(glm::vec3(topRight, 0.0f), glm::vec3(bottomRight, 0.0f), color);
		shapeDrawer.DrawLine(glm::vec3(bottomRight, 0.0f), glm::vec3(bottomLeft, 0.0f), color);
		shapeDrawer.DrawLine(glm::vec3(bottomLeft, 0.0f), glm::vec3(topLeft, 0.0f), color);

		driver.SetCapability(AmberRendering::Settings::ERenderingCapability::DEPTH_TEST, true);

		shapeDrawer.SetViewProjection(m_camera.GetProjectionMatrix() * m_camera.GetViewMatrix());
	}

	m_frameBuffer.Unbind();
}

void AmberEditor::Panels::SceneView::RenderImplementation()
{
	PrepareCamera();

	auto& baseRenderer = *EDITOR_CONTEXT(renderer).get();

	uint8_t glState = baseRenderer.FetchGLState();
	baseRenderer.ApplyStateMask(glState);
	HandleActorPicking();
	baseRenderer.ApplyStateMask(glState);
	RenderScene(glState);
	baseRenderer.ApplyStateMask(glState);
}

void AmberEditor::Panels::SceneView::HandleRectangleSelectInputs()
{
	PROFILER_SPY("HandleRectangleSelectInputs");

	if (m_gizmoOperations.IsPicking())
	{
		m_isSelecting = false;
		return;
	}

	if (EDITOR_CONTEXT(inputManager)->GetKey(AmberWindowing::Inputs::EKey::KEY_LEFT_CONTROL) == AmberWindowing::Inputs::EKeyState::KEY_DOWN && EDITOR_CONTEXT(inputManager)->IsMouseButtonPressed(AmberWindowing::Inputs::EMouseButton::MOUSE_BUTTON_LEFT) && !m_cameraController.IsRightMousePressed() && !m_highlightedGizmoDirection.has_value() && IsHovered())
	{
		auto[mouseX, mouseY] = EDITOR_CONTEXT(inputManager)->GetMousePosition();
		mouseX -= m_position.x;
		mouseY -= m_position.y;

		m_selectionStartPosition = glm::vec2(mouseX, mouseY);
		m_selectionEndPosition = m_selectionStartPosition;

		m_isSelecting = true;
	}

	if (EDITOR_CONTEXT(inputManager)->GetKey(AmberWindowing::Inputs::EKey::KEY_LEFT_CONTROL) == AmberWindowing::Inputs::EKeyState::KEY_DOWN && EDITOR_CONTEXT(inputManager)->GetMouseButton(AmberWindowing::Inputs::EMouseButton::MOUSE_BUTTON_LEFT) == AmberWindowing::Inputs::EMouseButtonState::MOUSE_DOWN && m_isSelecting)
	{
		auto[mouseX, mouseY] = EDITOR_CONTEXT(inputManager)->GetMousePosition();
		mouseX -= m_position.x;
		mouseY -= m_position.y;

		m_selectionEndPosition = glm::vec2(mouseX, mouseY);

		if (m_selectionEndPosition != m_selectionStartPosition)
			HandleRectangleSelect();
	}

	if (EDITOR_CONTEXT(inputManager)->IsMouseButtonReleased(AmberWindowing::Inputs::EMouseButton::MOUSE_BUTTON_LEFT) && m_isSelecting)
	{
		m_isSelecting = false;
	}
}

void AmberEditor::Panels::SceneView::HandleRectangleSelect()
{
	PROFILER_SPY("HandleRectangleSelect");

	auto& actors = m_sceneManager.GetCurrentScene()->GetActors();

	float minX = std::min(m_selectionStartPosition.x, m_selectionEndPosition.x);
	float maxX = std::max(m_selectionStartPosition.x, m_selectionEndPosition.x);
	float minY = std::min(m_selectionStartPosition.y, m_selectionEndPosition.y);
	float maxY = std::max(m_selectionStartPosition.y, m_selectionEndPosition.y);

	glm::vec2 windowSize = glm::vec2(GetSafeSize().first, GetSafeSize().second);

	std::vector<AmberCore::ECS::Actor*> selectedActors;

	for (auto actor : actors)
	{
		glm::vec4 clipSpacePosition = m_camera.GetProjectionMatrix() * m_camera.GetViewMatrix() * glm::vec4(actor->transform.GetWorldPosition(), 1.0f);

		if (clipSpacePosition.w <= 0.0f)
			continue;

		glm::vec3 ndcSpacePosition = glm::vec3(clipSpacePosition) / clipSpacePosition.w;

		glm::vec2 screenPosition;
		screenPosition.x = ((ndcSpacePosition.x + 1.0f) / 2.0f) * windowSize.x;
		screenPosition.y = ((1.0f - ndcSpacePosition.y) / 2.0f) * windowSize.y;

		if (screenPosition.x >= minX && screenPosition.x <= maxX && screenPosition.y >= minY && screenPosition.y <= maxY)
		{
			selectedActors.push_back(actor);
		}
	}

	if (!selectedActors.empty())
	{
		EDITOR_EXEC(SelectActors(selectedActors));
	}
	else
	{
		EDITOR_EXEC(UnselectActors());
	}
}

void AmberEditor::Panels::SceneView::UpdateSelectedActorsTransform()
{
	if (!m_gizmoOperations.IsPicking() || m_selectedActorsData.size() == 1)
		return;

	const glm::vec3& selectionActorPosition = m_selectionTransform.GetWorldPosition();
	const glm::vec3& selectionActorScale = m_selectionTransform.GetWorldScale();
	const glm::quat& selectionActorRotation = m_selectionTransform.GetWorldRotation();

	for (auto& data : m_selectedActorsData)
	{
		data.Actor->transform.SetWorldPosition(selectionActorPosition + selectionActorRotation * (data.OffsetPosition * selectionActorScale));
		data.Actor->transform.SetWorldScale(selectionActorScale * data.OffsetScale);
		data.Actor->transform.SetWorldRotation(selectionActorRotation * data.OffsetRotation);
	}
}

AmberEditor::Core::CameraController& AmberEditor::Panels::SceneView::GetCameraController()
{
	return m_cameraController;
}
