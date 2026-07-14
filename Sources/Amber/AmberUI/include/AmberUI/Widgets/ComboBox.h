#pragma once
#include "DataWidget.h"

namespace AmberUI::Widgets
{
	class ComboBox : public DataWidget<int>
	{
	public:
		ComboBox(int p_currentChoice = 0);

	protected:
		void DrawImplementation() override;

	public:
		std::map<int, std::string> Choices;
		int CurrentChoice;

	public:
	};
}
