#include "Amberpch.h"

#include "AmberEngine/UI/Panels/Views/SceneView.h"

#include "AmberEngine/Analytics/Profiling/Profiler.h"
#include "AmberEngine/Analytics/Profiling/ProfilerSpy.h"
#include "AmberEngine/Core/Editor.h"
#include "AmberEngine/Core/SceneSystem/SceneManager.h"
#include "AmberEngine/Core/SceneSystem/Scene.h"

#include "AmberEngine/Tools/Global/ServiceLocator.h"

AmberEngine::UI::Panels::SceneView::SceneView(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings) : 
AView(p_title, p_opened, p_panelSettings),
m_sceneManager(Tools::Global::ServiceLocator::Get<AmberEngine::Core::SceneSystem::SceneManager>()),
m_cameraController(m_camera, m_cameraPosition, m_cameraRotation)
{
	m_camera.SetClearColor({ 0.098f, 0.098f, 0.098f });
	m_camera.SetFar(5000.0f);
}

void AmberEngine::UI::Panels::SceneView::Update(float p_deltaTime)
{
	AView::Update(p_deltaTime);

	if(IsHovered())
	{
		m_cameraController.Update(p_deltaTime);
	}

	HandleRectangleSelectInputs();
	UpdateSelectedActorsTransform();
}

void AmberEngine::UI::Panels::SceneView::RenderSceneForActorPicking()
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
		auto& selectedActor = EDITOR_EXEC(GetSelectedActor());
		baseRenderer.Clear(false, true, false);
		m_editorRenderer.RenderGizmo(selectedActor.transform.GetWorldPosition(), selectedActor.transform.GetWorldRotation(), m_currentOperation, true);
	}
	else if (!m_selectedActorsData.empty())
	{
		baseRenderer.Clear(false, true, false);
		m_editorRenderer.RenderGizmo(m_selectionTransform.GetWorldPosition(), m_selectionTransform.GetWorldRotation(), m_currentOperation, true);
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

void AmberEngine::UI::Panels::SceneView::HandleActorPicking()
{
	auto& inputManager = *EDITOR_CONTEXT(inputManager);

	if (inputManager.IsMouseButtonReleased(Inputs::EMouseButton::MOUSE_BUTTON_LEFT))
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
		EDITOR_CONTEXT(renderer)->GetDriver().ReadPixels(static_cast<int>(mouseX), static_cast<int>(mouseY), 1, 1, Rendering::Settings::EPixelDataFormat::RGB, Rendering::Settings::EPixelDataType::UNSIGNED_BYTE, pixel);

		m_actorPickingFramebuffer.Unbind();

		uint32_t actorID = (0 << 24) | (pixel[2] << 16) | (pixel[1] << 8) | (pixel[0] << 0);

		auto actorUnderMouse = EDITOR_CONTEXT(sceneManager).GetCurrentScene()->FindActorByID(actorID);

		auto direction = m_gizmoOperations.IsPicking() ? m_gizmoOperations.GetDirection() : (EDITOR_EXEC(IsAnyActorSelected()) || !m_selectedActorsData.empty()) && pixel[0] == 255 && pixel[1] == 255 && pixel[2] >= 252 && pixel[2] <= 254 ? static_cast<AmberEngine::Core::GizmoBehaviour::EDirection>(pixel[2] - 252) : std::optional<AmberEngine::Core::GizmoBehaviour::EDirection>{};

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

		if (inputManager.IsMouseButtonPressed(Inputs::EMouseButton::MOUSE_BUTTON_LEFT) && !m_cameraController.IsRightMousePressed())
		{
			if (direction.has_value())
			{
				if(m_selectedActorsData.empty() || m_selectedActorsData.size() == 1)
					m_gizmoOperations.StartPicking(EDITOR_EXEC(GetSelectedActor()), m_cameraPosition, m_currentOperation, direction.value());
				else
					m_gizmoOperations.StartPicking(m_selectionTransform, m_cameraPosition, m_currentOperation, direction.value());
			}
			else
			{
				if (actorUnderMouse)
				{
					EDITOR_EXEC(SelectActor(*actorUnderMouse));
				}
				else
				{
					EDITOR_EXEC(UnselectActor());
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

	if (EDITOR_CONTEXT(inputManager)->IsMouseButtonPressed(Inputs::EMouseButton::MOUSE_BUTTON_LEFT) && !m_cameraController.IsRightMousePressed() && !m_gizmoOperations.IsPicking())
	{
		if (!m_gizmoOperations.IsPicking())
			m_selectedActorsData.clear();
	}
}

void AmberEngine::UI::Panels::SceneView::RenderScene(uint8_t p_defaultRenderState)
{
	auto& baseRenderer = *EDITOR_CONTEXT(renderer).get();
	auto& driver = *EDITOR_CONTEXT(driver).get();

	auto& currentScene = *m_sceneManager.GetCurrentScene();
	m_editorRenderer.UpdateLights(currentScene);

	m_frameBuffer.Bind();
	
	driver.SetStencilMask(0xFF);
	baseRenderer.Clear(m_camera);
	driver.SetStencilMask(0x00);

	m_editorRenderer.RenderGrid(m_cameraPosition, m_gridColor, m_div, m_bias, m_lwidht, m_mwidth);
	m_editorRenderer.RenderCameras();

	m_editorRenderer.RenderScene(m_cameraPosition, m_camera);

	if(!m_selectedActorsData.empty() || m_selectedActorsData.size() > 1)
	{
		//TODO: dirty test, need rework + remove this horrible vector.
		std::vector<AmberEngine::Core::ECS::Actor*> actors;
		for (auto& actorData : m_selectedActorsData)
		{
			actors.push_back(actorData.actor);
		}

		if(!actors.empty())
		{
			m_editorRenderer.RenderActorOutlinePass(actors, true, false);
			baseRenderer.ApplyStateMask(p_defaultRenderState);
			m_editorRenderer.RenderActorOutlinePass(actors, false, false);
		}
	}
	
	if (EDITOR_EXEC(IsAnyActorSelected()))
	{
		auto& selectedActor = EDITOR_EXEC(GetSelectedActor());

		if (selectedActor.IsActive())
		{
			m_editorRenderer.RenderActorOutlinePass(selectedActor, true, true);
			baseRenderer.ApplyStateMask(p_defaultRenderState);
			m_editorRenderer.RenderActorOutlinePass(selectedActor, false, true);
		}

		baseRenderer.ApplyStateMask(p_defaultRenderState);
		baseRenderer.Clear(false, true, false);

		int highlightedAxis = -1;

		if (m_highlightedGizmoDirection.has_value())
		{
			highlightedAxis = static_cast<int>(m_highlightedGizmoDirection.value());
		}

		m_editorRenderer.RenderGizmo(selectedActor.transform.GetWorldPosition(), selectedActor.transform.GetWorldRotation(), m_currentOperation, false, highlightedAxis);
	}
	else if (!m_selectedActorsData.empty() && m_selectedActorsData.size() > 1)
	{
		baseRenderer.ApplyStateMask(p_defaultRenderState);
		baseRenderer.Clear(false, true, false);

		int highlightedAxis = -1;

		if (m_highlightedGizmoDirection.has_value())
		{
			highlightedAxis = static_cast<int>(m_highlightedGizmoDirection.value());
		}

		m_editorRenderer.RenderGizmo(m_selectionTransform.GetWorldPosition(), m_selectionTransform.GetWorldRotation(), m_currentOperation, false, highlightedAxis);
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
		
		driver.SetCapability(Rendering::Settings::ERenderingCapability::DEPTH_TEST, false);

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

		driver.SetCapability(Rendering::Settings::ERenderingCapability::DEPTH_TEST, true);

		shapeDrawer.SetViewProjection(m_camera.GetProjectionMatrix() * m_camera.GetViewMatrix());
	}

	m_frameBuffer.Unbind();
}

void AmberEngine::UI::Panels::SceneView::RenderImplementation()
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

void AmberEngine::UI::Panels::SceneView::HandleRectangleSelectInputs()
{
	PROFILER_SPY("HandleRectangleSelectInputs"); 
	if (EDITOR_CONTEXT(inputManager)->GetKey(Inputs::EKey::KEY_LEFT_CONTROL) == Inputs::EKeyState::KEY_DOWN && EDITOR_CONTEXT(inputManager)->IsMouseButtonPressed(Inputs::EMouseButton::MOUSE_BUTTON_LEFT) && !m_cameraController.IsRightMousePressed() && !m_gizmoOperations.IsPicking() && IsHovered())
	{
		auto[mouseX, mouseY] = EDITOR_CONTEXT(inputManager)->GetMousePosition();
		mouseX -= m_position.x;
		mouseY -= m_position.y;

		m_selectionStartPosition = glm::vec2(mouseX, mouseY);
		m_selectionEndPosition = m_selectionStartPosition;

		m_isSelecting = true;
	}

	if (EDITOR_CONTEXT(inputManager)->GetKey(Inputs::EKey::KEY_LEFT_CONTROL) == Inputs::EKeyState::KEY_DOWN && EDITOR_CONTEXT(inputManager)->GetMouseButton(Inputs::EMouseButton::MOUSE_BUTTON_LEFT) == Inputs::EMouseButtonState::MOUSE_DOWN && m_isSelecting)
	{
		auto[mouseX, mouseY] = EDITOR_CONTEXT(inputManager)->GetMousePosition();
		mouseX -= m_position.x;
		mouseY -= m_position.y;

		m_selectionEndPosition = glm::vec2(mouseX, mouseY);

		HandleRectangleSelect();
	}

	if (EDITOR_CONTEXT(inputManager)->IsMouseButtonReleased(Inputs::EMouseButton::MOUSE_BUTTON_LEFT) && m_isSelecting)
	{
		m_isSelecting = false;
	}
}

void AmberEngine::UI::Panels::SceneView::HandleRectangleSelect()
{
	PROFILER_SPY("HandleRectangleSelect");

	auto& actors = m_sceneManager.GetCurrentScene()->GetActors();

	float minX = std::min(m_selectionStartPosition.x, m_selectionEndPosition.x);
	float maxX = std::max(m_selectionStartPosition.x, m_selectionEndPosition.x);
	float minY = std::min(m_selectionStartPosition.y, m_selectionEndPosition.y);
	float maxY = std::max(m_selectionStartPosition.y, m_selectionEndPosition.y);

	glm::vec2 windowSize = glm::vec2(GetSafeSize().first, GetSafeSize().second);

	m_selectedActorsData.clear();

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
			SelectedActorData data;
			data.actor = actor;
			m_selectedActorsData.push_back(data);
		}
	}

	if (m_selectedActorsData.size() == 1)
	{
		auto* selectedActor = &EDITOR_EXEC(GetSelectedActor());

		if(selectedActor == nullptr || selectedActor != m_selectedActorsData[0].actor)
			EDITOR_EXEC(SelectActor(*m_selectedActorsData[0].actor));
	}
	else if (m_selectedActorsData.size() > 1)
	{
		EDITOR_EXEC(UnselectActor());

		m_selectionTransform.SetWorldPosition({ 0,0,0 });
		glm::vec3 center(0.0f);
		for (const auto& data : m_selectedActorsData)
		{
			center += data.actor->transform.GetWorldPosition();
		}
		center /= static_cast<float>(m_selectedActorsData.size());

		m_selectionTransform.SetWorldPosition(center);

		for (auto& data : m_selectedActorsData)
		{
			data.initialLocalPosition = data.actor->transform.GetWorldPosition() - m_selectionTransform.GetWorldPosition();
		}
	}
	else
	{
		EDITOR_EXEC(UnselectActor());
	}
}

void AmberEngine::UI::Panels::SceneView::UpdateSelectedActorsTransform()
{
	if (m_selectedActorsData.empty() || m_selectedActorsData.size() == 1)
		return;

	const glm::vec3& selectionActorPosition = m_selectionTransform.GetWorldPosition();

	if (m_previousSelectionActorPosition != selectionActorPosition)
	{
		for (auto& data : m_selectedActorsData)
		{
			data.actor->transform.SetWorldPosition(data.initialLocalPosition + selectionActorPosition);
		}

		m_previousSelectionActorPosition = selectionActorPosition;
	}
}

AmberEngine::Core::CameraController& AmberEngine::UI::Panels::SceneView::GetCameraController()
{
	return m_cameraController;
}
