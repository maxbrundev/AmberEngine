#pragma once
#include "DataWidget.h"

namespace AmberEngine::UI::Widgets
{
	class CheckBox : public DataWidget<bool>
	{
	public:
		CheckBox(bool p_value = false, const std::string& p_label = "");

	protected:
		void DrawImplementation() override;

	public:
		bool value;
		std::string label;
		Eventing::Event<bool> ValueChangedEvent;
	};
}
