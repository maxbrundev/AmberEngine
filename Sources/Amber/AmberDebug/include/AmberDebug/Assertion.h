#pragma once

#define AB_ASSERT(condition, message) AmberDebug::Assertion::Assert(condition, message)

namespace AmberDebug
{
	class Assertion
	{
	public:

		Assertion() = delete;

		static void Assert(bool p_condition, const std::string& p_message = "");
	};
}