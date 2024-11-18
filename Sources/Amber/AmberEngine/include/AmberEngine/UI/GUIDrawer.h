#pragma once

#include "AmberEngine/Data/Color.h"

#include "AmberEngine/Resources/Model.h"
#include "AmberEngine/Resources/Texture.h"

#include "AmberEngine/UI/Widgets/DragSingleScalar.h"
#include "AmberEngine/UI/Widgets/Image.h"
#include "AmberEngine/UI/Widgets/Text.h"

namespace AmberEngine::UI
{
	class GUIDrawer
	{
	public:
		static const Data::Color TITLE_COLOR;
		static const Data::Color CLEAR_BUTTON_COLOR;

		static const float MIN_FLOAT;
		static const float MAX_FLOAT;

		static void ProvideEmptyTexture(Resources::Texture& p_emptyTexture);

		static void CreateTitle(WidgetContainer& p_root, const std::string& p_title);

		static void DrawBoolean(WidgetContainer& p_root, const std::string& p_name, bool& p_data);
		static void DrawVec2(WidgetContainer& p_root, const std::string& p_name, glm::vec2& p_data, float p_step = 1.f, float p_min = MIN_FLOAT, float p_max = MAX_FLOAT);
		static void DrawBoolean(WidgetContainer& p_root, const std::string& p_name, const std::function<bool(void)>& p_gatherer, const std::function<void(bool)>& p_provider);

		template <typename T>
		static void DrawScalar(WidgetContainer& p_root, const std::string& p_title, T& p_data, float p_step = 1.f, T p_min = std::numeric_limits<T>::min(), T p_max = std::numeric_limits<T>::max());
		template <typename T>
		static void DrawScalar(WidgetContainer& p_root, const std::string& p_name, std::function<T(void)> p_gatherer, std::function<void(T)> p_provider, float p_step = 1.f, T p_min = std::numeric_limits<T>::min(), T p_max = std::numeric_limits<T>::max());
		static void DrawColor(WidgetContainer& p_root, const std::string& p_title, Data::Color& p_color, bool p_hasAlpha = false);
		static void DrawString(WidgetContainer& p_root, const std::string& p_title, const std::function<std::string(void)>& p_gatherer, const std::function<void(std::string)>& p_provider);
		static void DrawVec3(WidgetContainer& p_root, const std::string& p_title, const std::function<glm::vec3(void)>& p_gatherer, const std::function<void(glm::vec3)>& p_provider, float p_step = 1.0f, float p_min = MIN_FLOAT, float p_max = MAX_FLOAT);
		static void DrawColor(WidgetContainer& p_root, const std::string& p_name, std::function<Data::Color(void)> p_gatherer, std::function<void(Data::Color)> p_provider, bool p_hasAlpha = false);
		static Widgets::Text& DrawMesh(WidgetContainer& p_root, const std::string& p_title, Resources::Model*& p_data, Eventing::Event<>* p_updateNotifier = nullptr);
		static Widgets::Image& DrawTexture(WidgetContainer& p_root, const std::string& p_name, Resources::Texture*& p_data,  Eventing::Event<>* p_updateNotifier = nullptr);
		static Widgets::Text& DrawShader(WidgetContainer& p_root, const std::string& p_name, Resources::Shader*& p_data,   Eventing::Event<>* p_updateNotifier = nullptr);
		static Widgets::Text& DrawMaterial(WidgetContainer& p_root, const std::string& p_name, Resources::Material*& p_data, Eventing::Event<>* p_updateNotifier = nullptr);
		static Widgets::Text& DrawAsset(WidgetContainer& p_root, const std::string& p_name, std::string& p_data, Eventing::Event<>* p_updateNotifier = nullptr);
		static void DrawDDString(WidgetContainer& p_root, const std::string& p_name, std::function<std::string(void)> p_gatherer, std::function<void(std::string)> p_provider, const std::string& p_identifier);
		template <typename T>
		static ImGuiDataType_ GetDataType();
		template <typename T>
		static std::string GetFormat();

	private:
		static Resources::Texture* S_EMPTY_TEXTURE;
	};
}

#include "AmberEngine/UI/GUIDrawer.inl"