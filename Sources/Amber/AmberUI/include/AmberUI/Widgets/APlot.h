#pragma once

#include "DataWidget.h"


namespace AmberUI::Widgets
{
	class APlot : public DataWidget<std::vector<float>>
	{
	public:
		APlot
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
		virtual void DrawImplementation() override = 0;

	public:
		std::vector<float> Data;
		float MinScale;
		float MaxScale;
		glm::vec2 Size;
		std::string Overlay;
		std::string Label;
		int ForceHover;
	};
}
