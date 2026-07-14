#include "Amberpch.h"

#include "AmberUI/Panels/APanelTransformable.h"

#include "AmberUI/Tools/Converter.h"

uint64_t AmberUI::Panels::APanelTransformable::PANEL_ID_INCREMENT = 0;

AmberUI::Panels::APanelTransformable::APanelTransformable
(
	const glm::vec2& p_defaultPosition,
	const glm::vec2& p_defaultSize,
	EHorizontalAlignment p_defaultHorizontalAlignment,
	EVerticalAlignment p_defaultVerticalAlignment
) :
	m_defaultPosition(p_defaultPosition),
	m_defaultSize(p_defaultSize),
	m_defaultHorizontalAlignment(p_defaultHorizontalAlignment),
	m_defaultVerticalAlignment(p_defaultVerticalAlignment)
{
	m_panelID = "##" + std::to_string(PANEL_ID_INCREMENT++);
}

void AmberUI::Panels::APanelTransformable::SetPosition(const glm::vec2& p_position)
{
	m_position = p_position;
	m_positionChanged = true;
}

void AmberUI::Panels::APanelTransformable::SetSize(const glm::vec2& p_size)
{
	m_size = p_size;
	m_sizeChanged = true;
}

void AmberUI::Panels::APanelTransformable::SetAlignment(EHorizontalAlignment p_horizontalAlignment, EVerticalAlignment p_verticalAlignment)
{
	m_horizontalAlignment = p_horizontalAlignment;
	m_verticalAlignment = p_verticalAlignment;
	m_alignmentChanged = true;
}

const glm::vec2& AmberUI::Panels::APanelTransformable::GetPosition() const
{
	return m_position;
}

const glm::vec2& AmberUI::Panels::APanelTransformable::GetSize() const
{
	return m_size;
}

AmberUI::EHorizontalAlignment AmberUI::Panels::APanelTransformable::GetHorizontalAlignment() const
{
	return m_horizontalAlignment;
}

AmberUI::EVerticalAlignment AmberUI::Panels::APanelTransformable::GetVerticalAlignment() const
{
	return m_verticalAlignment;
}

void AmberUI::Panels::APanelTransformable::Update()
{
	if (!m_firstFrame)
	{
		if (!AutoSize)
			UpdateSize();
		CopyImGuiSize();

		UpdatePosition();
		CopyImGuiPosition();
	}

	m_firstFrame = false;
}

glm::vec2 AmberUI::Panels::APanelTransformable::CalculatePositionAlignmentOffset(bool p_default)
{
	glm::vec2 result(0.0f, 0.0f);

	switch (p_default ? m_defaultHorizontalAlignment : m_horizontalAlignment)
	{
	case EHorizontalAlignment::CENTER:
		result.x -= m_size.x / 2.0f;
		break;
	case EHorizontalAlignment::RIGHT:
		result.x -= m_size.x;
		break;
	}

	switch (p_default ? m_defaultVerticalAlignment : m_verticalAlignment)
	{
	case EVerticalAlignment::MIDDLE:
		result.y -= m_size.y / 2.0f;
		break;
	case EVerticalAlignment::BOTTOM:
		result.y -= m_size.y;
		break;
	}

	return result;
}

void AmberUI::Panels::APanelTransformable::UpdatePosition()
{
	if (m_defaultPosition.x != -1.f && m_defaultPosition.y != -1.f)
	{
		glm::vec2 offsettedDefaultPosition = m_defaultPosition + CalculatePositionAlignmentOffset(true);
		ImGui::SetWindowPos(Tools::Converter::ToImVec2(offsettedDefaultPosition), ImGuiCond_FirstUseEver);
	}

	if (m_positionChanged || m_alignmentChanged)
	{
		glm::vec2 offset = CalculatePositionAlignmentOffset(false);
		glm::vec2 offsettedPosition(m_position.x + offset.x, m_position.y + offset.y);
		ImGui::SetWindowPos(Tools::Converter::ToImVec2(offsettedPosition), ImGuiCond_Always);
		m_positionChanged = false;
		m_alignmentChanged = false;
	}
}

void AmberUI::Panels::APanelTransformable::UpdateSize()
{
	if (m_sizeChanged)
	{
		ImGui::SetWindowSize(Tools::Converter::ToImVec2(m_size), ImGuiCond_Always);
		m_sizeChanged = false;
	}
}

void AmberUI::Panels::APanelTransformable::CopyImGuiPosition()
{
	m_position = Tools::Converter::ToFVector2(ImGui::GetWindowPos());
}

void AmberUI::Panels::APanelTransformable::CopyImGuiSize()
{
	m_size = Tools::Converter::ToFVector2(ImGui::GetWindowSize());
}
