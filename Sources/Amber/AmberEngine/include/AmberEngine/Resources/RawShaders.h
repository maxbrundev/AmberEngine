#pragma once

namespace AmberEngine::Resources
{
	class RawShader
	{
	public:
		static std::pair<std::string, std::string> GetGrid();
		static std::pair<std::string, std::string> GetGizmo();
		static std::pair<std::string, std::string> GetNormalVisualizer();
	};
}
