#pragma once

namespace AmberEngine::Tools::Utils
{
	struct DefaultNumberFormat : std::numpunct<char>
	{
		char do_thousands_sep() const { return ','; }
		std::string do_grouping() const { return "\03"; }
	};

	class Format
	{
		Format() = delete;

	public:
		/**
		* @brief Transfrom a number into a more readable version
		* by grouping every 3 digits occurences together seperated by commas.
		* @param number Number to make more readable
		* @return A std::string representation of the number, more readable.
		*/
		static std::string ToReadableNumber(const uint64_t number);
	};
}
