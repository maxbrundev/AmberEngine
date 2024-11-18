/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/PlotHistogram.h"

#include "AmberEngine/Tools/Utils/Converter.h"

AmberEngine::UI::Widgets::PlotHistogram::PlotHistogram
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

void AmberEngine::UI::Widgets::PlotHistogram::DrawImplementation()
{
	ImGui::PlotHistogram((label + m_widgetID).c_str(), data.data(), static_cast<int>(data.size()), 0, overlay.c_str(), minScale, maxScale, Utils::Converter::ToImVec2(size), sizeof(float));
}
