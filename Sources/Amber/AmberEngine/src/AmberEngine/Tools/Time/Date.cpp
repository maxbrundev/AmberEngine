#include "Amberpch.h"

#include "AmberEngine/Tools/Time/Date.h"

std::string AmberEngine::Tools::Time::Date::GetDateAsString()
{
	std::string date;
	const auto now = time(nullptr);
	tm ltm;

	localtime_s(&ltm, &now);

	std::string dateData[6] =
	{
		std::to_string(1900 + ltm.tm_year),
		std::to_string(1 + ltm.tm_mon),
		std::to_string(ltm.tm_mday),
		std::to_string(ltm.tm_hour),
		std::to_string(ltm.tm_min),
		std::to_string(ltm.tm_sec)
	};

	for (uint8_t i = 0; i < 6; i++)
	{
		if (dateData[i].size() < 2)
		{
			dateData[i].insert(dateData[i].begin(), '0');
		}

		date += dateData[i];

		if (i == 2)
			date += '_';
		else if (i != 5)
			date += '-';
	}

	return date;
}
