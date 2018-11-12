#pragma once

namespace RenderEngine
{
	class IContext
	{
	public:
		virtual ~IContext() = default;

		virtual void Setup() = 0;
		virtual void Close() = 0;
		virtual bool IsActive() = 0;

		//virtual bool IsActive() = 0;
	};
}