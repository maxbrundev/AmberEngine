#pragma once

#include <string>

#include "AmberDebug/ILogHandler.h"

namespace AmberDebug
{
	class ConsoleHandler final : public ILogHandler
	{
		friend class Logger;

	private:
		void Log(const LogData& p_logData) override;

		static std::string GetLogHeader(ELogLevel p_logLevel);
	};
}