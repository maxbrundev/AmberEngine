#pragma once

#include <glm/glm.hpp>

#include "AmberUI/Panels/APanel.h"
#include "AmberUI/Settings/EAlignment.h"

namespace AmberUI::Panels
{
	class APanelTransformable : public APanel
	{
	public:
		APanelTransformable
		(
			const glm::vec2& p_defaultPosition = glm::vec2(-1.f, -1.f),
			const glm::vec2& p_defaultSize = glm::vec2(-1.f, -1.f),
			EHorizontalAlignment p_defaultHorizontalAlignment = EHorizontalAlignment::LEFT,
			EVerticalAlignment p_defaultVerticalAlignment = EVerticalAlignment::TOP
		);

		void SetPosition(const glm::vec2& p_position);
		void SetSize(const glm::vec2& p_size);
		void SetAlignment(EHorizontalAlignment p_horizontalAlignment, EVerticalAlignment p_verticalAlignment);

		const glm::vec2& GetPosition() const;
		const glm::vec2& GetSize() const;
		EHorizontalAlignment GetHorizontalAlignment() const;
		EVerticalAlignment GetVerticalAlignment() const;

	protected:
		void Update();

	private:
		glm::vec2 CalculatePositionAlignmentOffset(bool p_default);

		void UpdatePosition();
		void UpdateSize();
		void CopyImGuiPosition();
		void CopyImGuiSize();

	public:
		bool AutoSize = true;

	protected:
		std::string m_panelID;

		glm::vec2 m_defaultPosition;
		glm::vec2 m_defaultSize;
		EHorizontalAlignment m_defaultHorizontalAlignment;
		EVerticalAlignment m_defaultVerticalAlignment;

		glm::vec2 m_position = glm::vec2(0.0f, 0.0f);
		glm::vec2 m_size = glm::vec2(0.0f, 0.0f);

		EHorizontalAlignment m_horizontalAlignment = EHorizontalAlignment::LEFT;
		EVerticalAlignment m_verticalAlignment = EVerticalAlignment::TOP;

	private:
		static uint64_t PANEL_ID_INCREMENT;

		bool m_positionChanged = false;
		bool m_sizeChanged = false;
		bool m_alignmentChanged = false;
		bool m_firstFrame = true;
	};
}
