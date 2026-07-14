#pragma once

#include "AmberUI/Widgets/DataWidget.h"

#include "AmberTools/Eventing/Event.h"

namespace AmberUI::Widgets
{
	class InputText : public DataWidget<std::string>
	{
	public:
		InputText(const std::string& p_content = "", const std::string& p_label = "");
		virtual ~InputText() override = default;

	protected:
		void DrawImplementation() override;

	public:
		std::string Content;
		std::string Label;

		bool SelectAllOnClick = false;

		AmberTools::Eventing::Event<std::string> ContentChangedEvent;
		AmberTools::Eventing::Event<std::string> EnterPressedEvent;
	};
}
