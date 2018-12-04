#pragma once

#include "Context/Device.h"
#include "LowRenderer/Camera.h"

namespace  RenderEngine
{
	namespace Managers
	{
		class UIManager
		{
		private:
			Core::Device& m_device;
			float m_clearColor[4];

		public:
			UIManager(Core::Device& p_context);
			~UIManager() = default;

			void PreUpdate();
			void Update();
			void PostUpdate();
			void DisplayMenuBar();
			void Close();
		};
	}
}