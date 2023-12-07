#pragma once

#include "AmberEngine/UI/WidgetContainer.h"

namespace AmberEngine::UI
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
	};
}
