#pragma once

#include "AmberEngine/UI/Widgets/DataWidget.h"

namespace AmberEngine::UI::Widgets
{
	class Text : public DataWidget<std::string>
	{
	public:
		Text(const std::string& p_content = "");
		virtual ~Text() override = default;

	protected:
		virtual void DrawImplementation() override;

	public:
		std::string content;
	};
}
