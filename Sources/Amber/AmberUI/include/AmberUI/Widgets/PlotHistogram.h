#pragma once

#include "APlot.h"

namespace AmberUI::Widgets
{
	class PlotHistogram : public APlot
	{
	public:
		PlotHistogram
		(
			const std::vector<float>& p_data = std::vector<float>(),
			float p_minScale = std::numeric_limits<float>::min(),
			float p_maxScale = std::numeric_limits<float>::max(),
			const glm::vec2& p_size = { 0.0f, 0.0f },
			const std::string& p_overlay = "",
			const std::string& p_label = "",
			int p_forceHover = -1
		);

	protected:
		void DrawImplementation() override;
	};
}