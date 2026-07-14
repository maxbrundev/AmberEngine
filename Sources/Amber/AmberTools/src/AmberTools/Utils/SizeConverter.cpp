#include "Amberpch.h"
#include "AmberTools/Utils/SizeConverter.h"

std::pair<float, AmberTools::Utils::SizeConverter::ESizeUnit> AmberTools::Utils::SizeConverter::ConvertToOptimalUnit(float p_value, AmberTools::Utils::SizeConverter::ESizeUnit p_unit)
{
    if (p_value == 0.0f) return { 0.0f, ESizeUnit::BYTE };
    const float bytes = Convert(p_value, p_unit, ESizeUnit::BYTE);
    const int digits = static_cast<int>(trunc(log10(bytes)));
    const ESizeUnit targetUnit = static_cast<AmberTools::Utils::SizeConverter::ESizeUnit>((int)fmin(3.0f * floor(digits / 3.0f), static_cast<float>(AmberTools::Utils::SizeConverter::ESizeUnit::TERA_BYTE)));

    return { Convert(bytes, AmberTools::Utils::SizeConverter::ESizeUnit::BYTE, targetUnit), targetUnit };
}

float AmberTools::Utils::SizeConverter::Convert(float p_value, ESizeUnit p_from, ESizeUnit p_to)
{
    const float fromValue = powf(1024.0f, static_cast<float>(p_from) / 3.0f);
    const float toValue = powf(1024.0f, static_cast<float>(p_to) / 3.0f);

    return p_value * (fromValue / toValue);
}

std::string AmberTools::Utils::SizeConverter::UnitToString(ESizeUnit p_unit)
{
    switch (p_unit)
    {
    case AmberTools::Utils::SizeConverter::ESizeUnit::BYTE: return "B";
    case AmberTools::Utils::SizeConverter::ESizeUnit::KILO_BYTE: return "KB";
    case AmberTools::Utils::SizeConverter::ESizeUnit::MEGA_BYTE: return "MB";
    case AmberTools::Utils::SizeConverter::ESizeUnit::GIGA_BYTE: return "GB";
    case AmberTools::Utils::SizeConverter::ESizeUnit::TERA_BYTE: return "TB";
    }

    return "?";
}
