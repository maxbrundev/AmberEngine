#pragma once
#include "Context/Device.h"
#include "Context/Driver.h"

namespace RenderEngine
{
	namespace Managers
	{
		class WindowManager
		{
		private:
			std::unique_ptr<Core::Device> m_device;
			std::unique_ptr<Core::Driver> m_driver;

		public:
			WindowManager();
			~WindowManager();

			void Setup();
			void Update();

			bool IsOpen() const;

			Core::Device& GetDevice() const;
			Core::Driver& GetDriver() const;
		};
	}
}