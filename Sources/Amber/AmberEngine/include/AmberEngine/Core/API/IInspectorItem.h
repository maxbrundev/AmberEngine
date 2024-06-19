#pragma once

#include <AmberEngine/Core/API/ISerializable.h>
#include <AmberEngine/UI/WidgetContainer.h>

namespace AmberEngine::Core::API
{
	class IInspectorItem : public ISerializable
	{
	public:
		virtual void OnInspector(UI::WidgetContainer& p_root) = 0;
	};
}