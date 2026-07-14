#pragma once
#include "DataWidget.h"

namespace AmberUI::Widgets
{
	class CheckBox : public DataWidget<bool>
	{
	public:
		CheckBox(bool p_value = false, const std::string& p_label = "");

	protected:
		void DrawImplementation() override;

	public:
		bool Value;
		std::string Label;
	};
}
