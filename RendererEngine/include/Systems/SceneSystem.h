#pragma once

#include "LowRenderer/Camera.h"
#include "Resources/Shader.h"

namespace RenderEngine
{
	namespace Systems
	{
		class SceneSystem
		{
		private:
			Core::Context& m_SceneContext;

		public:
			SceneSystem(Core::Context& p_context);
			~SceneSystem() = default;
		};
	}
}
