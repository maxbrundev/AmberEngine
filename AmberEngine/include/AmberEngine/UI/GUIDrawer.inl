#pragma once

#include "AmberEngine/UI/GUIDrawer.h"

namespace AmberEngine::UI
{
	template<typename T>
	inline ImGuiDataType_ GUIDrawer::GetDataType()
	{
		if constexpr (std::is_same_v<T, float>)         return ImGuiDataType_Float;
		else if constexpr (std::is_same_v<T, double>)   return ImGuiDataType_Double;
		else if constexpr (std::is_same_v<T, uint8_t>)  return ImGuiDataType_U8;
		else if constexpr (std::is_same_v<T, uint16_t>) return ImGuiDataType_U16;
		else if constexpr (std::is_same_v<T, uint32_t>) return ImGuiDataType_U32;
		else if constexpr (std::is_same_v<T, uint64_t>) return ImGuiDataType_U64;
		else if constexpr (std::is_same_v<T, int8_t>)   return ImGuiDataType_S8;
		else if constexpr (std::is_same_v<T, int16_t>)  return ImGuiDataType_S16;
		else if constexpr (std::is_same_v<T, int32_t>)  return ImGuiDataType_S32;
		else if constexpr (std::is_same_v<T, int64_t>)  return ImGuiDataType_S64;
	}

	template <typename T>
	std::string GUIDrawer::GetFormat()
	{
		if constexpr (std::is_same_v<T, double>) return "%.5f";
		else if constexpr (std::is_same_v<T, float>) return "%.3f";
		else return "%d";
	}

	template <typename T>
	void GUIDrawer::DrawScalar(WidgetContainer& p_root, const std::string& p_title, T& p_data, float p_step, T p_min, T p_max)
	{
		static_assert(std::is_scalar_v<T>, "T must be a scalar");

		CreateTitle(p_root, p_title);
		auto& widget = p_root.CreateWidget<Widgets::DragSingleScalar<T>>(GetDataType<T>(), p_min, p_max, p_data, p_step, "", GetFormat<T>());
		widget.SetGathererAndProvider([&]() { return p_data; }, [&](T scalar) { p_data = scalar; });
	}
}
