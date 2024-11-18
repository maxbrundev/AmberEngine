#pragma once

#define AB_ASSERT(condition, message) AmberEngine::Debug::Assertion::Assert(condition, message)

namespace AmberEngine::Debug
{
	class Assertion
	{
	public:

		Assertion() = delete;

		static void Assert(bool p_condition, const std::string& p_message = "");
	};
}