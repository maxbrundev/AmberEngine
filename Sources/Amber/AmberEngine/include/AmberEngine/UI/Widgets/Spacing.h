#pragma once
#include "AWidget.h"


namespace AmberEngine::UI::Widgets
{
	class Spacing : public AWidget
	{
	public:
		Spacing(uint16_t p_spaces = 1);

	protected:
		void DrawImplementation() override;

	public:
		uint16_t spaces = 1;
	};
}
