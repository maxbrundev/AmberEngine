#pragma once

namespace AmberTools::Utils
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
		static std::string ToReadableNumber(const uint64_t number);
	};
}
