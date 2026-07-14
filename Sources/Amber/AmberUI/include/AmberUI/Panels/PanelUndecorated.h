#pragma once

#include "AmberUI/Panels/APanelTransformable.h"

namespace AmberUI::Panels
{
	class PanelUndecorated : public APanelTransformable
	{
	protected:
		virtual void DrawImplementation() override;

	private:
		int CollectFlags();
	};
}
