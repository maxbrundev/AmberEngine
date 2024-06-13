#include "Amberpch.h"

#include "AmberEngine/Tools/Utils/String.h"

#include <ctype.h>

std::string AmberEngine::Tools::Utils::String::ExtractDirectoryFromPath(const std::string& p_path)
{
	size_t end_pos = p_path.find_last_of('/') + 1;

	end_pos = end_pos > 0 ? end_pos :  p_path.find_last_of('\\') + 1;

	return p_path.substr(0, end_pos);
}

std::string AmberEngine::Tools::Utils::String::ExtractFileNameFromPath(const std::string& p_path)
{
	size_t characterCount = p_path.find_last_of('/') + 1;

	characterCount = characterCount > 0 ? characterCount : p_path.find_last_of('\\') + 1;

	return p_path.substr(characterCount);
}

std::string AmberEngine::Tools::Utils::String::RemoveExtensionFromFileName(const std::string& p_file)
{
	const size_t end_pos = p_file.find_last_of('.');

	if(end_pos > 0 && end_pos != std::string::npos)
	{
		return p_file.substr(0, end_pos);
	}

	return p_file;
}

bool AmberEngine::Tools::Utils::String::Replace(std::string& p_target, const std::string& p_from, const std::string& p_to)
{
	const size_t start_pos = p_target.find(p_from);

	if (start_pos != std::string::npos)
	{
		p_target.replace(start_pos, p_from.length(), p_to);
		return true;
	}

	return false;
}

void AmberEngine::Tools::Utils::String::ReplaceAll(std::string& p_target, const std::string& p_from, const std::string& p_to)
{
	if (p_from.empty()) return;

	size_t start_pos = 0;
	while ((start_pos = p_target.find(p_from, start_pos)) != std::string::npos)
	{
		p_target.replace(start_pos, p_from.length(), p_to);
		start_pos += p_to.length();
	}
}

std::string AmberEngine::Tools::Utils::String::GenerateUnique(const std::string& p_source,
	std::function<bool(std::string)> p_isAvailable)
{
	auto suffixlessSource = p_source;

	auto suffixOpeningParenthesisPos = std::string::npos;
	auto suffixClosingParenthesisPos = std::string::npos;

	// Keep track of the current character position when iterating onto `p_source`
	auto currentPos = decltype(std::string::npos){p_source.length() - 1};

	// Here we search for `(` and `)` positions. (Needed to extract the number between those parenthesis)
	for (auto it = p_source.rbegin(); it < p_source.rend(); ++it, --currentPos)
	{
		const auto c = *it;

		if (suffixClosingParenthesisPos == std::string::npos && c == ')') suffixClosingParenthesisPos = currentPos;
		if (suffixClosingParenthesisPos != std::string::npos && c == '(') suffixOpeningParenthesisPos = currentPos;
	}

	// We need to declare our `counter` here to store the number between found parenthesis OR 1 (In the case no parenthesis, AKA, suffix, has been found)
	auto counter = uint32_t{ 1 };

	// If the two parenthis have been found AND the closing parenthesis is the last character AND there is a space before the opening parenthesis
	if (suffixOpeningParenthesisPos != std::string::npos && suffixClosingParenthesisPos == p_source.length() - 1 && suffixOpeningParenthesisPos > 0 && p_source[suffixOpeningParenthesisPos - 1] == ' ')
	{
		// Extract the string between those parenthesis
		const auto between = p_source.substr(suffixOpeningParenthesisPos + 1, suffixClosingParenthesisPos - suffixOpeningParenthesisPos - 1);

		// If the `between` string is composed of digits (AKA, `between` is a number)
		if (!between.empty() && std::find_if(between.begin(), between.end(), [](unsigned char c) { return !isdigit(c); }) == between.end())
		{
			counter = static_cast<uint32_t>(std::atoi(between.c_str()));
			suffixlessSource = p_source.substr(0, suffixOpeningParenthesisPos - 1);
		}
	}

	auto result = suffixlessSource;

	// While `result` isn't available, we keep generating new strings
	while (!p_isAvailable(result))
	{
		// New strings are composed of the `suffixlessSource` (Ex: "Foo (1)" without suffix is "Foo")
		result = suffixlessSource + " (" + std::to_string(counter++) + ")";
	}

	return result;
}
