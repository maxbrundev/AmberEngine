#pragma once

#include "AmberEngine/UI/Widgets/AButton.h"

#include "AmberEngine/Data/Color.h"

namespace AmberEngine::UI::Widgets
{
	class ButtonSmall : public AButton
	{
	public:
		ButtonSmall(const std::string& p_label = "");
		virtual ~ButtonSmall() override = default;

	protected:
		virtual void DrawImplementation() override;

	public:
		std::string label;

		Data::Color idleBackgroundColor;
		Data::Color hoveredBackgroundColor;
		Data::Color clickedBackgroundColor;
		Data::Color textColor;
	};
}
