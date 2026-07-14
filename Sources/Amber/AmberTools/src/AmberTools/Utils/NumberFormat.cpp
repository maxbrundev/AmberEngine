#include "Amberpch.h"

#include "AmberTools/Utils/NumberFormat.h"

std::string AmberTools::Utils::Format::ToReadableNumber(const uint64_t number)
{
	std::stringstream ss;
	ss.imbue(std::locale(std::cout.getloc(), new DefaultNumberFormat));
	ss << number;

	return ss.str();
}
