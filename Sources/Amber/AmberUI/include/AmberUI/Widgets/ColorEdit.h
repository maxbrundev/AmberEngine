#pragma once

#include "AmberUI/Widgets/DataWidget.h"

#include "AmberRendering/Data/Color.h"

#include "AmberTools/Eventing/Event.h"

namespace AmberUI::Widgets
{
	class ColorEdit : public DataWidget<AmberRendering::Data::Color>
	{
	public:
		ColorEdit(bool p_enableAlpha = false, const AmberRendering::Data::Color& p_defaultColor = {});
		virtual ~ColorEdit() override = default;

	protected:
		void DrawImplementation() override;

	public:
		bool EnableAlpha;
		AmberRendering::Data::Color Color;
		AmberTools::Eventing::Event<AmberRendering::Data::Color&> ColorChangedEvent;
	};
}
