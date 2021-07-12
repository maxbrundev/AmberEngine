#include "Amberpch.h"

#include "AmberEngine/Tools/Utils/String.h"

std::string Utils::String::ExtractDirectoryFromPath(const std::string& p_path)
{
	size_t characterCount = p_path.find_last_of('/') + 1;

	return p_path.substr(0, characterCount);
}

std::string Utils::String::ExtractFileNameFromPath(const std::string& p_path)
{
	size_t characterCount = p_path.find_last_of('/') + 1;

	return p_path.substr(characterCount);
}

std::string Utils::String::RemoveExtensionFromFileName(const std::string& p_file)
{
	size_t characterCount = p_file.find_last_of('.');

	if(characterCount > 0 && characterCount != std::string::npos)
	{
		return p_file.substr(0, characterCount);
	}

	return p_file;
}

int* Utils::String::StringIntToArray(const std::string& p_file)
{
	const int asciiDecimalIndex = 48;

	const size_t str_lenght = p_file.length();
	int* array = new int[str_lenght] {0};

	int currentIndex = 0;
	
	for (int i = 0; i < p_file[i] != '\0'; i++)
	{
		if (p_file[i] == ' ')
		{
			continue;
		}

		if (p_file[i] == ',')
		{
			currentIndex++;
		}
		else
		{
			array[currentIndex] = array[currentIndex] * 10 + (p_file[i] - asciiDecimalIndex);
		}

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
		p_outVector.push_back(token);
	}

	//for (size_t i = 0; i < p_inputString.size(); i++)
	//{
	//	if (p_inputString[i] != ' ')
	//	{
	//		tempInput += p_inputString[i];
	//	}
	//	else
	//	{
	//		p_outVector.push_back(tempInput);
	//		tempInput = "";
	//	}
	//}
}
