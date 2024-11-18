#include "Amberpch.h"

#include <assert.h>

#include "AmberEngine/Debug/Assertion.h"

void AmberEngine::Debug::Assertion::Assert(bool p_condition, const std::string& p_message)
{
	assert(p_condition && p_message.c_str());
}
