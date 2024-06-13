#pragma once

#include "AmberEngine/UI/Widgets/DataWidget.h"

#include "AmberEngine/Data/Color.h"

#include "AmberEngine/Eventing/Event.h"

namespace AmberEngine::UI::Widgets
{
	class ColorEdit : public DataWidget<Data::Color>
	{
	public:
		ColorEdit(bool p_enableAlpha = false, const Data::Color& p_defaultColor = {});
		virtual ~ColorEdit() override = default;

	protected:
		void DrawImplementation() override;

	public:
		bool enableAlpha;
		Data::Color color;
		Eventing::Event<Data::Color&> ColorChangedEvent;
	};
}
