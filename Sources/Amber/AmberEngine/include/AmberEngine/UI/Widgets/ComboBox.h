#pragma once
#include "DataWidget.h"

namespace AmberEngine::UI::Widgets
{
	class ComboBox : public DataWidget<int>
	{
	public:
		ComboBox(int p_currentChoice = 0);

	protected:
		void DrawImplementation() override;

	public:
		std::map<int, std::string> choices;
		int currentChoice;

	public:
		Eventing::Event<int> ValueChangedEvent;
	};
}
