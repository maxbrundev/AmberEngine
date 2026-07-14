#pragma once

#include <AmberCore/API/ISerializable.h>

#include <AmberUI/WidgetContainer.h>

namespace AmberCore::API
{
	class IInspectorItem : public ISerializable
	{
	public:
		virtual void OnInspector(AmberUI::WidgetContainer& p_root) = 0;
	};
}