#include "Amberpch.h"

#include "AmberEditor/Core/GizmoBehaviour.h"
#include "AmberEditor/Core/EditorAction.h"
#include "AmberEditor/Settings/EditorSettings.h"

float SnapValue(float p_value, float p_step)
{
	return p_value - std::fmod(p_value, p_step);
}

glm::vec3 SnapValue(const glm::vec3& p_value, float p_step)
{
	glm::vec3 result;
	result.x = std::round(p_value.x / p_step) * p_step;
	result.y = std::round(p_value.y / p_step) * p_step;
	result.z = std::round(p_value.z / p_step) * p_step;
	return result;
}

bool AmberEditor::Core::GizmoBehaviour::IsSnappedBehaviourEnabled() const
{
	const auto& inputManager = EDITOR_CONTEXT(inputManager);
	return inputManager->GetKey(AmberWindowing::Inputs::EKey::KEY_LEFT_CONTROL) == AmberWindowing::Inputs::EKeyState::KEY_DOWN || inputManager->GetKey(AmberWindowing::Inputs::EKey::KEY_RIGHT_CONTROL) == AmberWindowing::Inputs::EKeyState::KEY_DOWN;
}

void AmberEditor::Core::GizmoBehaviour::StartPicking(AmberCore::ECS::Actor& p_target, const glm::vec3& p_cameraPosition, EGizmoOperation p_operation, EDirection p_direction)
{
	m_target = &p_target;
	m_firstMouse = true;
	m_firstPick = true;
	m_originalTransform = p_target.transform.GetTransform();
	m_distanceToActor = glm::distance(p_cameraPosition, m_target->transform.GetWorldPosition());
	m_currentOperation = p_operation;
	m_direction = p_direction;
}

void AmberEditor::Core::GizmoBehaviour::StartPicking(AmberMaths::Transform& p_target, const glm::vec3& p_cameraPosition, EGizmoOperation p_operation, EDirection p_direction)
{
	m_targetTransform = &p_target;
	m_firstMouse = true;
	m_firstPick = true;
	m_originalTransform = *m_targetTransform;
	m_distanceToActor = glm::distance(p_cameraPosition, m_targetTransform->GetWorldPosition());
	m_currentOperation = p_operation;
	m_direction = p_direction;
}

void AmberEditor::Core::GizmoBehaviour::StopPicking()
{
	m_target = nullptr;
	m_targetTransform = nullptr;
}

glm::vec3 AmberEditor::Core::GizmoBehaviour::GetFakeDirection() const
{
	auto result = glm::vec3();

	switch (m_direction)
	{
	case EDirection::X:
		result = glm::vec3(1.0f, 0.0f, 0.0f);
		break;
	case EDirection::Y:
		result = glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	case EDirection::Z:
		result = glm::vec3(0.0f, 0.0f, 1.0f);
		break;
	}

	return result;
}

glm::vec3 AmberEditor::Core::GizmoBehaviour::GetRealDirection(bool p_relative) const
{
	auto result = glm::vec3();

	switch (m_direction)
	{
	case EDirection::X:
		result = p_relative ? m_originalTransform.GetWorldRight() : m_originalTransform.GetLocalRight();
		break;
	case EDirection::Y:
		result = p_relative ? m_originalTransform.GetWorldUp() : m_originalTransform.GetLocalUp();
		break;
	case EDirection::Z:
		result = p_relative ? m_originalTransform.GetWorldForward() : m_originalTransform.GetLocalForward();
		break;
	}

	return result;
}

glm::vec2 AmberEditor::Core::GizmoBehaviour::GetScreenDirection(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize) const
{
	auto start = m_originalTransform.GetWorldPosition();
	auto end = m_originalTransform.GetWorldPosition() + GetRealDirection(true) * 0.01f;

	auto start2D = glm::vec2();
	{
		auto clipSpacePos = p_projectionMatrix * (p_viewMatrix * glm::vec4{ start.x, start.y, start.z, 1.0f });
		auto ndcSpacePos = glm::vec3{ clipSpacePos.x, clipSpacePos.y, clipSpacePos.z } / clipSpacePos.w;
		auto windowSpacePos = ((glm::vec2{ ndcSpacePos.x, ndcSpacePos.y } + 1.0f) / 2.0f);
		windowSpacePos.x *= p_viewSize.x;
		windowSpacePos.y *= p_viewSize.y;
		start2D = windowSpacePos;
	}

	auto end2D = glm::vec2();
	{
		auto clipSpacePos = p_projectionMatrix * (p_viewMatrix * glm::vec4{ end.x, end.y, end.z, 1.0f });
		auto ndcSpacePos = glm::vec3{ clipSpacePos.x, clipSpacePos.y, clipSpacePos.z } / clipSpacePos.w;
		auto windowSpacePos = ((glm::vec2{ ndcSpacePos.x, ndcSpacePos.y } + 1.0f) / 2.0f);
		windowSpacePos.x *= p_viewSize.x;
		windowSpacePos.y *= p_viewSize.y;
		end2D = windowSpacePos;
	}

	auto result = end2D - start2D;

	result.y *= -1; // Screen coordinates are reversed, so we inverse the Y

	return glm::normalize(result);
}

void AmberEditor::Core::GizmoBehaviour::ApplyTranslation(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec3& p_cameraPosition, const glm::vec2& p_viewSize)
{
	auto ray = GetMouseRay(m_currentMouse, p_viewMatrix, p_projectionMatrix, p_viewSize);

	const glm::vec3 planeTangent = glm::cross(GetRealDirection(true), (m_target ? m_target->transform.GetWorldPosition() : m_targetTransform->GetWorldPosition()) - p_cameraPosition);
	const glm::vec3 planeNormal = glm::cross(GetRealDirection(true), planeTangent);

	glm::vec3 direction = GetRealDirection(true);

	glm::vec3 planePoint = m_originalTransform.GetWorldPosition();

	const float denom = glm::dot(ray, planeNormal);

	if (std::abs(denom) <= 0.001f)
		return;

	const float t = glm::dot(planePoint - p_cameraPosition, planeNormal) / denom;

	if (t <= 0.001f)
		return;

	glm::vec3 point = p_cameraPosition + ray * t;

	if (m_firstPick)
	{
		m_initialOffset = m_originalTransform.GetWorldPosition() - point;
		m_firstPick = false;
	}

	auto translationVector = point - planePoint + m_initialOffset;

	if (IsSnappedBehaviourEnabled())
	{
		translationVector = SnapValue(translationVector, Settings::EditorSettings::TranslationSnapUnit);
	}

	glm::vec3 projectedPoint = planePoint + direction * glm::dot(translationVector, direction);

	if (m_target)
		m_target->transform.SetWorldPosition(projectedPoint);
	else
		m_targetTransform->SetWorldPosition(projectedPoint);
}

void AmberEditor::Core::GizmoBehaviour::ApplyRotation(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize) const
{
	auto unitsPerPixel = 0.2f;
	auto originRotation = m_originalTransform.GetWorldRotation();

	auto screenDirection = GetScreenDirection(p_viewMatrix, p_projectionMatrix, p_viewSize);
	screenDirection = glm::vec2(-screenDirection.y, screenDirection.x);

	auto totalDisplacement = m_currentMouse - m_originMouse;
	auto rotationCoefficient = glm::dot(totalDisplacement, screenDirection) * unitsPerPixel;

	if (IsSnappedBehaviourEnabled())
	{
		rotationCoefficient = SnapValue(rotationCoefficient, Settings::EditorSettings::RotationSnapUnit);
	}

	auto rotationToApply = glm::quat(glm::vec3(GetFakeDirection() * rotationCoefficient));

	if (m_target)
		m_target->transform.SetWorldRotation(originRotation * rotationToApply);
	else
		m_targetTransform->SetWorldRotation(originRotation * rotationToApply);
}

void AmberEditor::Core::GizmoBehaviour::ApplyScale(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize) const
{
	auto unitsPerPixel = 0.01f;
	auto originScale = m_originalTransform.GetWorldScale();

	auto screenDirection = GetScreenDirection(p_viewMatrix, p_projectionMatrix, p_viewSize);

	auto totalDisplacement = m_currentMouse - m_originMouse;
	auto scaleCoefficient = glm::dot(totalDisplacement, screenDirection) * unitsPerPixel;

	if (IsSnappedBehaviourEnabled())
	{
		scaleCoefficient = SnapValue(scaleCoefficient, Settings::EditorSettings::ScalingSnapUnit);
	}

	auto newScale = originScale + GetFakeDirection() * scaleCoefficient;

	/* Prevent scale from being negative*/
	newScale.x = std::max(newScale.x, 0.0001f);
	newScale.y = std::max(newScale.y, 0.0001f);
	newScale.z = std::max(newScale.z, 0.0001f);
	if (m_target)
		m_target->transform.SetWorldScale(newScale);
	else
		m_targetTransform->SetWorldScale(newScale);
}

void AmberEditor::Core::GizmoBehaviour::ApplyOperation(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec3& p_cameraPosition, const glm::vec2& p_viewSize)
{
	switch (m_currentOperation)
	{
	case EGizmoOperation::TRANSLATE:
		ApplyTranslation(p_viewMatrix, p_projectionMatrix, p_cameraPosition, p_viewSize);
		break;

	case EGizmoOperation::ROTATE:
		ApplyRotation(p_viewMatrix, p_projectionMatrix, p_viewSize);
		break;

	case EGizmoOperation::SCALE:
		ApplyScale(p_viewMatrix, p_projectionMatrix, p_viewSize);
		break;
	}
}

void AmberEditor::Core::GizmoBehaviour::SetCurrentMouse(const glm::vec2& p_mousePosition)
{
	if (m_firstMouse)
	{
		m_currentMouse = m_originMouse = p_mousePosition;
		m_firstMouse = false;
	}
	else
	{
		m_currentMouse = p_mousePosition;
	}
}

bool AmberEditor::Core::GizmoBehaviour::IsPicking() const
{
	return m_target || m_targetTransform;
}

AmberEditor::Core::GizmoBehaviour::EDirection AmberEditor::Core::GizmoBehaviour::GetDirection() const
{
	return m_direction;
}

glm::vec3 AmberEditor::Core::GizmoBehaviour::GetMouseRay(const glm::vec2& p_mousePos, const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize)
{
	float x = 2.0f * (p_mousePos.x / p_viewSize.x) - 1.0f;
	float y = 1.0f - 2.0f * (p_mousePos.y / p_viewSize.y);

	glm::mat4 inverseView = glm::inverse(p_viewMatrix);
	glm::mat4 inverseProjection = glm::inverse(p_projectionMatrix);

	glm::mat4 inverseViewProjection = inverseView * inverseProjection;

	glm::vec4 nearestPoint = inverseViewProjection * glm::vec4(x, y, -1.0f, 1.0f);
	glm::vec4 farthestPoint = inverseViewProjection * glm::vec4(x, y, 1.0f, 1.0f);

	return glm::vec3(farthestPoint.x, farthestPoint.y, farthestPoint.z) * nearestPoint.w - glm::vec3(nearestPoint.x, nearestPoint.y, nearestPoint.z) * farthestPoint.w;
}
