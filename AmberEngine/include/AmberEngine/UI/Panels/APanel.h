#pragma once

#include "AmberEngine/UI/WidgetContainer.h"

#include "AmberEngine/Tools/Utils/CallbackQueue.h"

namespace AmberEngine::UI::Panels
{
	class APanel : public WidgetContainer
	{
	public:
		APanel();
		virtual ~APanel() override = default;

		void Draw();

	protected:
		virtual void DrawImplementation() = 0;

	public:
		bool enabled = true;

	protected:
		Tools::Utils::CallbackQueue m_callbackQueue;
	};
}
