#pragma once

#include "AmberEngine/UI/Widgets/AButton.h"

#include "AmberEngine/Data/Color.h"

namespace AmberEngine::UI::Widgets
{
	class Button : public AButton
	{
	public:
		Button(const std::string& p_label = "", const glm::vec2& p_size = glm::vec2(0.f, 0.f), bool p_disabled = false);
		virtual ~Button() override = default;

	protected:
		virtual void DrawImplementation() override;

	public:
		std::string label;
		glm::vec2 size;
		bool disabled = false;

		Data::Color idleBackgroundColor;
		Data::Color hoveredBackgroundColor;
		Data::Color clickedBackgroundColor;
		Data::Color textColor;
	};
}
