#pragma once

#include "AmberUI/Widgets/AButton.h"

#include "AmberRendering/Data/Color.h"

namespace AmberUI::Widgets
{
	class Button : public AButton
	{
	public:
		Button(const std::string& p_label = "", const glm::vec2& p_size = glm::vec2(0.f, 0.f), bool p_disabled = false);
		virtual ~Button() override = default;

	protected:
		virtual void DrawImplementation() override;

	public:
		std::string Label;
		glm::vec2 Size;
		bool Disabled = false;

		AmberRendering::Data::Color IdleBackgroundColor;
		AmberRendering::Data::Color HoveredBackgroundColor;
		AmberRendering::Data::Color ClickedBackgroundColor;
		AmberRendering::Data::Color TextColor;
	};
}
