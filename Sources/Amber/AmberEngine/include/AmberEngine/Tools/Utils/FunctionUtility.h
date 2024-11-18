#pragma once

namespace AmberEngine::Tools::Utils
{
	inline std::function<void()> Combine(std::function<void()> p_function, const std::optional<std::function<void()>>& p_optionalFunction)
	{
		if (p_optionalFunction.has_value())
		{
			return [=]()
			{
				p_function();
				p_optionalFunction.value()();
			};
		}

		return p_function;
	}
}
