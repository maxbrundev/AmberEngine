#pragma once

#include "AmberUI/Widgets/AButton.h"

#include "AmberRendering/Data/Color.h"

namespace AmberUI::Widgets
{
	class ButtonSmall : public AButton
	{
	public:
		ButtonSmall(const std::string& p_label = "");
		virtual ~ButtonSmall() override = default;

	protected:
		virtual void DrawImplementation() override;

	public:
		std::string Label;

		AmberRendering::Data::Color IdleBackgroundColor;
		AmberRendering::Data::Color HoveredBackgroundColor;
		AmberRendering::Data::Color ClickedBackgroundColor;
		AmberRendering::Data::Color TextColor;
	};
}
