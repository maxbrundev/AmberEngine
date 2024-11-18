#pragma once

#include "AmberEngine/Tools/Utils/CallbackQueue.h"

#include "AmberEngine/UI/ContextualMenuContainer.h"
#include "AmberEngine/UI/WidgetContainer.h"

namespace AmberEngine::UI::Panels
{
	class APanel : public WidgetContainer, public ContextualMenuContainer
	{
	public:
		APanel();
		virtual ~APanel() override = default;

		virtual void Draw();

	protected:
		virtual void DrawImplementation() = 0;

	public:
		bool Enabled = true;

	protected:
		Tools::Utils::CallbackQueue m_callbackQueue;
	};
}
