#pragma once


#include "AmberEngine/Data/Color.h"

#include "AmberEngine/UI/WidgetContainer.h"
#include "AmberEngine/UI/Widgets/DragingScalar.h"

namespace AmberEngine::UI
{
	class GUIDrawer
	{
	public:
		static const Data::Color TITLE_COLOR;
		static const Data::Color CLEAR_BUTTON_COLOR;

		static const float MIN_FLOAT;
		static const float MAX_FLOAT;

		template <typename T>
		static ImGuiDataType_ GetDataType();
		template <typename T>
		static std::string GetFormat();

		static void CreateTitle(WidgetContainer& p_root, const std::string& p_title);
		static void DrawColor(WidgetContainer& p_root, const std::string& p_title, Data::Color& p_color, bool p_hasAlpha = false);
		static void DrawString(WidgetContainer& p_root, const std::string& p_title, std::function<std::string(void)> p_gatherer, std::function<void(std::string)> p_provider);
		static void DrawVec3(WidgetContainer& p_root, const std::string& p_title, std::function<glm::vec3(void)> p_gatherer, std::function<void(glm::vec3)> p_provider, float p_step = 1.0f, float p_min = MIN_FLOAT, float p_max = MAX_FLOAT);
		template <typename T>
		static void DrawScalar(WidgetContainer& p_root, const std::string& p_title, T& p_data, float p_step = 1.f, T p_min = std::numeric_limits<T>::min(), T p_max = std::numeric_limits<T>::max());
	};

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
