#pragma once

#include <AmberEngine/UI/WidgetContainer.h>

namespace AmberEngine::Core::API
{
	class IInspectorItem
	{
	public:
		virtual void OnInspector(UI::WidgetContainer& p_root) = 0;
	};
}