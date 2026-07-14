#include "Amberpch.h"

#include "AmberUI/Widgets/PlotHistogram.h"

#include "AmberUI/Tools/Converter.h"

AmberUI::Widgets::PlotHistogram::PlotHistogram
(
	const std::vector<float>& p_data,
	float p_minScale,
	float p_maxScale,
	const glm::vec2& p_size,
	const std::string& p_overlay,
	const std::string& p_label,
	int p_forceHover
) : APlot(p_data, p_minScale, p_maxScale, p_size, p_overlay, p_label, p_forceHover)
{
}

void AmberUI::Widgets::PlotHistogram::DrawImplementation()
{
	ImGui::PlotHistogram((Label + m_widgetID).c_str(), Data.data(), static_cast<int>(Data.size()), 0, Overlay.c_str(), MinScale, MaxScale, AmberUI::Tools::Converter::ToImVec2(Size), sizeof(float));
}
