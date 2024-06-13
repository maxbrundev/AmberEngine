#pragma once

#include "AmberEngine/Data/Color.h"

#include "AmberEngine/Resources/Model.h"
#include "AmberEngine/Resources/Texture.h"

#include "AmberEngine/UI/WidgetContainer.h"
#include "AmberEngine/UI/Widgets/DragingScalar.h"
#include "AmberEngine/UI/Widgets/Text.h"
#include "AmberEngine/UI/Widgets/TextDropTarget.h"

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

		template <typename T>
		static void DrawScalar(WidgetContainer& p_root, const std::string& p_title, T& p_data, float p_step = 1.f, T p_min = std::numeric_limits<T>::min(), T p_max = std::numeric_limits<T>::max());
		static void DrawColor(WidgetContainer& p_root, const std::string& p_title, Data::Color& p_color, bool p_hasAlpha = false);
		static void DrawString(WidgetContainer& p_root, const std::string& p_title, std::function<std::string(void)> p_gatherer, std::function<void(std::string)> p_provider);
		static void DrawVec3(WidgetContainer& p_root, const std::string& p_title, std::function<glm::vec3(void)> p_gatherer, std::function<void(glm::vec3)> p_provider, float p_step = 1.0f, float p_min = MIN_FLOAT, float p_max = MAX_FLOAT);
		
		static Widgets::TextDropTarget& DrawMesh(WidgetContainer& p_root, const std::string& p_title, Resources::Model*& p_data, Eventing::Event<>* p_updateNotifier = nullptr);

		template <typename T>
		static ImGuiDataType_ GetDataType();
		template <typename T>
		static std::string GetFormat();

	private:
		static Resources::Texture* S_EMPTY_TEXTURE;
	};
}

#include "AmberEngine/UI/GUIDrawer.inl"