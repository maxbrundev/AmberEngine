#pragma once

#include "AmberEngine/UI/Widgets/Text.h"

#include "AmberEngine/Data/Color.h"

namespace AmberEngine::UI::Widgets
{
	class TextColored : public Text
	{
	public:
		TextColored(const std::string& p_content = "", const AmberEngine::Data::Color& p_color = AmberEngine::Data::Color(1.0f, 1.0f, 1.0f, 1.0f));
		virtual ~TextColored() override = default;

	public:
		Data::Color color;

	protected:
		virtual void DrawImplementation() override;
	};
}
