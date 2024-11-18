#pragma once

#include <string>

#include "AmberEngine/Debug/ILogHandler.h"

namespace AmberEngine::Debug
{
	class ConsoleHandler final : public ILogHandler
	{
		friend class Logger;

	private:
		void Log(const LogData& p_logData) override;

		static std::string GetLogHeader(ELogLevel p_logLevel);
	};
}