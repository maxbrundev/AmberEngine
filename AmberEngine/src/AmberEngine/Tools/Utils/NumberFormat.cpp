#include "Amberpch.h"

#include "AmberEngine/Tools/Utils/NumberFormat.h"

std::string AmberEngine::Tools::Utils::Format::ToReadableNumber(const uint64_t number)
{
	std::stringstream ss;
	ss.imbue(std::locale(std::cout.getloc(), new DefaultNumberFormat));
	ss << number;

	return ss.str();
}
