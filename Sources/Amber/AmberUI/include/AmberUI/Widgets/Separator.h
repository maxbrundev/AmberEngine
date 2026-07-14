#pragma once

#include "AmberUI/Widgets/AWidget.h"

namespace AmberUI::Widgets
{
	class Separator : public AWidget
	{
	public:
		Separator() = default;
		virtual ~Separator() override = default;

	protected:
		void DrawImplementation() override;
	};
}