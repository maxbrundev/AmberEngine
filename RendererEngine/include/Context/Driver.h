#pragma once

#include "Context/Icontext.h"

namespace  RenderEngine
{
	namespace Core
	{
		class Driver : public IContext
		{
		private:
			bool m_isActive;

		public:
			Driver();
			~Driver() = default;

			void Setup() override;
			void Close() override {};
			bool IsActive() override;

			void DisplayDriverInfo();
		};
	}
}