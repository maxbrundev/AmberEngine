#include "Amberpch.h"

#include "AmberUI/Widgets/APlot.h"

AmberUI::Widgets::APlot::APlot(const std::vector<float>& p_data, float p_minScale, float p_maxScale,
	const glm::vec2& p_size, const std::string& p_overlay, const std::string& p_label, int p_forceHover) :
	DataWidget(Data), Data(p_data), MinScale(p_minScale), MaxScale(p_maxScale), Size(p_size), Overlay(p_overlay), Label(p_label), ForceHover(p_forceHover)
{

}
