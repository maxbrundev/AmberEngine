#pragma once

#include "AmberRendering/Data/Color.h"

#include "AmberRendering/Resources/Model.h"
#include "AmberRendering/Resources/Texture.h"

#include "AmberUI/WidgetContainer.h"
#include "AmberUI/Widgets/DragSingleScalar.h"
#include "AmberUI/Widgets/Image.h"
#include "AmberUI/Widgets/Text.h"

namespace AmberRendering::Resources
{
	class Shader;
}

namespace AmberCore::Resources
{
	class Material;
}

namespace AmberAudio::Resources
{
	class Sound;
}

namespace AmberCore::Helpers
{
	class GUIDrawer
	{
	public:
		static const AmberRendering::Data::Color TITLE_COLOR;
		static const AmberRendering::Data::Color CLEAR_BUTTON_COLOR;

		static const float MIN_FLOAT;
		static const float MAX_FLOAT;

		static void ProvideEmptyTexture(AmberRendering::Resources::Texture& p_emptyTexture);

		static void CreateTitle(AmberUI::WidgetContainer& p_root, const std::string& p_title);
		static void CreateSectionTitle(AmberUI::WidgetContainer& p_root, const std::string& p_title);

		static void DrawBoolean(AmberUI::WidgetContainer& p_root, const std::string& p_name, bool& p_data);
		static void DrawVec2(AmberUI::WidgetContainer& p_root, const std::string& p_name, glm::vec2& p_data, float p_step = 1.f, float p_min = MIN_FLOAT, float p_max = MAX_FLOAT);
		static void DrawBoolean(AmberUI::WidgetContainer& p_root, const std::string& p_name, const std::function<bool(void)>& p_gatherer, const std::function<void(bool)>& p_provider);

		template <typename T>
		static void DrawScalar(AmberUI::WidgetContainer& p_root, const std::string& p_title, T& p_data, float p_step = 1.f, T p_min = std::numeric_limits<T>::min(), T p_max = std::numeric_limits<T>::max());
		template <typename T>
		static void DrawScalar(AmberUI::WidgetContainer& p_root, const std::string& p_name, std::function<T(void)> p_gatherer, std::function<void(T)> p_provider, float p_step = 1.f, T p_min = std::numeric_limits<T>::min(), T p_max = std::numeric_limits<T>::max());
		static void DrawColor(AmberUI::WidgetContainer& p_root, const std::string& p_title, AmberRendering::Data::Color& p_color, bool p_hasAlpha = false);
		static void DrawString(AmberUI::WidgetContainer& p_root, const std::string& p_title, const std::function<std::string(void)>& p_gatherer, const std::function<void(std::string)>& p_provider);
		static void DrawVec3(AmberUI::WidgetContainer& p_root, const std::string& p_title, const std::function<glm::vec3(void)>& p_gatherer, const std::function<void(glm::vec3)>& p_provider, float p_step = 1.0f, float p_min = MIN_FLOAT, float p_max = MAX_FLOAT);
		static void DrawColor(AmberUI::WidgetContainer& p_root, const std::string& p_name, std::function<AmberRendering::Data::Color(void)> p_gatherer, std::function<void(AmberRendering::Data::Color)> p_provider, bool p_hasAlpha = false);
		static AmberUI::Widgets::Text& DrawMesh(AmberUI::WidgetContainer& p_root, const std::string& p_title, AmberRendering::Resources::Model*& p_data, AmberTools::Eventing::Event<>* p_updateNotifier = nullptr);
		static AmberUI::Widgets::Image& DrawTexture(AmberUI::WidgetContainer& p_root, const std::string& p_name, AmberRendering::Resources::Texture*& p_data,  AmberTools::Eventing::Event<>* p_updateNotifier = nullptr);
		static AmberUI::Widgets::Text& DrawShader(AmberUI::WidgetContainer& p_root, const std::string& p_name, AmberRendering::Resources::Shader*& p_data,   AmberTools::Eventing::Event<>* p_updateNotifier = nullptr);
		static AmberUI::Widgets::Text& DrawMaterial(AmberUI::WidgetContainer& p_root, const std::string& p_name, AmberCore::Resources::Material*& p_data, AmberTools::Eventing::Event<>* p_updateNotifier = nullptr);
		static AmberUI::Widgets::Text& DrawSound(AmberUI::WidgetContainer& p_root, const std::string& p_name, AmberAudio::Resources::Sound*& p_data, AmberTools::Eventing::Event<>* p_updateNotifier = nullptr);
		static AmberUI::Widgets::Text& DrawAsset(AmberUI::WidgetContainer& p_root, const std::string& p_name, std::string& p_data, AmberTools::Eventing::Event<>* p_updateNotifier = nullptr);
		static void DrawDDString(AmberUI::WidgetContainer& p_root, const std::string& p_name, std::function<std::string(void)> p_gatherer, std::function<void(std::string)> p_provider, const std::string& p_identifier);
		template <typename T>
		static ImGuiDataType_ GetDataType();
		template <typename T>
		static std::string GetFormat();

	private:
		static AmberRendering::Resources::Texture* S_EMPTY_TEXTURE;
	};
}

#include "AmberCore/Helpers/GUIDrawer.inl"