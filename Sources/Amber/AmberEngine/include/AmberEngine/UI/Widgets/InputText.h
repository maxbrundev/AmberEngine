#pragma once

#include "AmberEngine/UI/Widgets/DataWidget.h"

#include "AmberEngine/Eventing/Event.h"

namespace AmberEngine::UI::Widgets
{
	class InputText : public DataWidget<std::string>
	{
	public:
		InputText(const std::string& p_content = "", const std::string& p_label = "");
		virtual ~InputText() override = default;

	protected:
		void DrawImplementation() override;

	public:
		std::string content;
		std::string label;

		bool selectAllOnClick = false;

		Eventing::Event<std::string> ContentChangedEvent;
		Eventing::Event<std::string> EnterPressedEvent;
	};
}
