#pragma once
#include "AWidget.h"


namespace AmberUI::Widgets
{
	class Spacing : public AWidget
	{
	public:
		Spacing(uint16_t p_spaces = 1);

	protected:
		void DrawImplementation() override;

	public:
		uint16_t Spaces = 1;
	};
}
