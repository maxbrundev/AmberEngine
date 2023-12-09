#pragma once

#include <queue>

namespace AmberEngine::Tools::Utils
{
	class CallbackQueue : public std::queue<std::function<void()>>
	{
	public:
		void Process()
		{
			while (!empty())
			{
				auto& callback = front();
				callback();
				pop();
			}
		}
	};
}
