#pragma once

#include "AmberEngine/UI/Widgets/AWidget.h"

namespace AmberEngine::UI::Widgets
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