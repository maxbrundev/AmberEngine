#include "Amberpch.h"

#include "AmberEngine/Tools/Utils/String.h"

constexpr char Utils::String::__BACKSLASH    = '\\';
constexpr char Utils::String::__FORWARDSLASH = '/';
constexpr char Utils::String::__DOT          = '.';

std::string Utils::String::ExtractDirectoryFromPath(const std::string& p_path)
{
	size_t characterCount = p_path.find_last_of(__FORWARDSLASH) + 1;

	characterCount = characterCount > 0 ? characterCount :  p_path.find_last_of(__BACKSLASH) + 1;

	return p_path.substr(0, characterCount);
}

std::string Utils::String::ExtractFileNameFromPath(const std::string& p_path)
{
	size_t characterCount = p_path.find_last_of(__FORWARDSLASH) + 1;

	characterCount = characterCount > 0 ? characterCount : p_path.find_last_of(__BACKSLASH) + 1;

	return p_path.substr(characterCount);
}

std::string Utils::String::RemoveExtensionFromFileName(const std::string& p_file)
{
	const size_t characterCount = p_file.find_last_of(__DOT);

	if(characterCount > 0 && characterCount != std::string::npos)
	{
		return p_file.substr(0, characterCount);
	}

	return p_file;
}

std::string Utils::String::IntToString(int p_number)
{
	std::string result;

	unsigned int devider = 1;

	if (p_number < 0)
	{
		result += "-";

		p_number = -p_number;
	}

	while (p_number / devider > 9)
	{
		devider *= 10;
	}

	while (devider > 0)
	{
		result += '0' + (p_number / devider) % 10;

		devider /= 10;
	}

	return result;
}

int Utils::String::StringToInt(const std::string& p_string)
{
	const uint8_t asciiDecimalIndex = 48;

	int result = 0;
	int multiplier = 10;

	bool isNegative = false;

	for (uint32_t i = 0; i < p_string.size(); i++)
	{
		if (p_string[i] == '-')
		{
			multiplier = -multiplier;
			isNegative = true;

			continue;
		}

		if (!isNegative)
		{
			result = result * multiplier + (p_string[i] - asciiDecimalIndex);
		}
		else
		{
			result = result * multiplier - (p_string[i] - asciiDecimalIndex);
		}

		multiplier = 10;
	}

	return result;
}

int* Utils::String::StringToIntArray(const std::string& p_string)
{
	const uint8_t asciiDecimalIndex = 48;

	const size_t size = p_string.size();

	int* array = new int[size] {0};

	int multiplier = 10;

	int currentIndex = 0;

	bool isNegative = false;

	for (uint32_t i = 0; i < p_string[i] != '\0'; i++)
	{
		if (p_string[i] == ' ')
		{
			continue;
		}

		if (p_string[i] == '-')
		{
			multiplier = -multiplier;
			isNegative = true;
			continue;
		}

		if (p_string[i] == ',')
		{
			currentIndex++;
			isNegative = false;
		}
		else
		{
			if (!isNegative)
			{
				array[currentIndex] = array[currentIndex] * multiplier + (p_string[i] - asciiDecimalIndex);
			}
			else
			{
				array[currentIndex] = array[currentIndex] * multiplier - (p_string[i] - asciiDecimalIndex);
			}
		}

		multiplier = 10;
	}

	return array;
}

void Utils::String::ParseInputIntoVector(const std::string& p_inputString, std::vector<std::string>& p_outVector)
{
	std::stringstream ss(p_inputString);

	while (ss.good())
	{
		std::string token;

		ss >> token;
		p_outVector.push_back(std::move(token));
	}

	/*std::string result;

	for (uint32_t i = 0; i < p_inputString.size(); i++)
	{
		if (p_inputString[i] != ' ')
		{
			result += p_inputString[i];
		}
		else
		{
			p_outVector.push_back(result);
			result = "";
		}
	}*/
}

std::string Utils::String::RemoveAllOcurrences(const std::string& p_target, const char p_character)
{
	std::string result;

	for (uint32_t i = 0; i < p_target.size(); i++)
	{
		if (p_target[i] != p_character)
		{
			result += p_target[i];
		}
	}

	return result;
}

std::string Utils::String::ToUpper(const std::string& p_target)
{
	std::string result;
	const char offset = 'a' - 'A';

	for (uint32_t i = 0; i < p_target.size(); i++)
	{
		if (p_target[i] >= 'a' && p_target[i] <= 'z')
		{
			result += p_target[i] - offset;
		}
		else
		{
			result += p_target[i];
		}

	}

	return result;
}

std::string Utils::String::ToMinor(const std::string& p_target)
{
	std::string result;
	const char offset = 'a' - 'A';

	for (uint32_t i = 0; i < p_target.size(); i++)
	{
		if (p_target[i] >= 'a' && p_target[i] <= 'z')
		{
			result += p_target[i];
		}
		else
		{
			result += p_target[i] + offset;
		}

	}

	return result;
}
