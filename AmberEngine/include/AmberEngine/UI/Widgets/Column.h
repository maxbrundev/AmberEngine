#pragma once

#include "AmberEngine/UI/Widgets/AWidget.h"
#include "AmberEngine/UI/WidgetContainer.h"

namespace AmberEngine::UI::Widgets
{
	template <size_t _Size>
	class Columns : public AWidget, public WidgetContainer
	{
	public:
		Columns()
		{
			widths.fill(-1.f);
		}

		virtual ~Columns() override = default;

	protected:
		virtual void DrawImplementation() override
		{
			ImGui::Columns(static_cast<int>(_Size), ("##" + m_widgetID).c_str(), false);

			int counter = 0;

			CollectGarbages();

			for (auto it = m_widgets.begin(); it != m_widgets.end();)
			{
				it->get()->Draw();

				++it;

				if (it != m_widgets.end())
				{
					if (widths[counter] != -1.f)
						ImGui::SetColumnWidth(counter, widths[counter]);

					ImGui::NextColumn();
				}

				++counter;

				if (counter == _Size)
					counter = 0;
			}

			ImGui::Columns(1); // Necessary to not break the layout for following widget
		}

	public:
		std::array<float, _Size> widths;
	};
}