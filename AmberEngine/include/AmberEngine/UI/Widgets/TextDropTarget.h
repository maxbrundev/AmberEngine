#pragma once

#include "AmberEngine/Eventing/Event.h"
#include "AmberEngine/UI/Widgets/DataWidget.h"

namespace AmberEngine::UI::Widgets
{
	class TextDropTarget : public DataWidget<std::string>
	{
	public:
		TextDropTarget(const std::string& p_content = "", Eventing::Event<>* p_updateNotifier = nullptr);
		virtual ~TextDropTarget() override = default;
		void Update();
	protected:
		virtual void DrawImplementation() override;

	public:
		std::string content;

		Eventing::Event<>* UpdateNotif;
	};
}
