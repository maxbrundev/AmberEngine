#pragma once

#include "AmberUI/Widgets/Text.h"

#include "AmberRendering/Data/Color.h"

namespace AmberUI::Widgets
{
	class TextColored : public Text
	{
	public:
		TextColored(const std::string& p_content = "", const AmberRendering::Data::Color& p_color = AmberRendering::Data::Color(1.0f, 1.0f, 1.0f, 1.0f));
		virtual ~TextColored() override = default;

	public:
		AmberRendering::Data::Color Color;

	protected:
		virtual void DrawImplementation() override;
	};
}
