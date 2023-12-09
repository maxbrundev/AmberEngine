#include "Amberpch.h"

#include "AmberEngine/UI/GUIDrawer.h"

#include "AmberEngine/UI/Widgets/ColorEdit.h"
#include "AmberEngine/UI/Widgets/InputText.h"
#include "AmberEngine/UI/Widgets/DataWidget.h"
#include "AmberEngine/UI/Widgets/DragMultipleScalars.h"
#include "AmberEngine/UI/Widgets/TextColored.h"

const AmberEngine::Data::Color AmberEngine::UI::GUIDrawer::TITLE_COLOR        = { 0.85f, 0.65f, 0.0f };
const AmberEngine::Data::Color AmberEngine::UI::GUIDrawer::CLEAR_BUTTON_COLOR = { 0.5f, 0.0f, 0.0f };

const float AmberEngine::UI::GUIDrawer::MIN_FLOAT = -999999999.0f;
const float AmberEngine::UI::GUIDrawer::MAX_FLOAT = +999999999.0f;

void AmberEngine::UI::GUIDrawer::CreateTitle(WidgetContainer& p_root, const std::string& p_title)
{
	p_root.CreateWidget<Widgets::TextColored>(p_title, TITLE_COLOR);
}

void AmberEngine::UI::GUIDrawer::DrawColor(WidgetContainer& p_root, const std::string& p_title, Data::Color& p_color, bool p_hasAlpha)
{
	CreateTitle(p_root, p_title);
	auto& widget = p_root.CreateWidget<Widgets::ColorEdit>(p_hasAlpha, p_color);
	widget.SetGathererAndProvider(nullptr, [&](Data::Color color) { p_color = color; });
}

void AmberEngine::UI::GUIDrawer::DrawString(WidgetContainer& p_root, const std::string& p_title, std::function<std::string(void)> p_gatherer, std::function<void(std::string)> p_provider)
{
	CreateTitle(p_root, p_title);
	auto& inputText = p_root.CreateWidget<Widgets::InputText>("");
	inputText.SetGathererAndProvider(std::move(p_gatherer), std::move(p_provider));
}

void AmberEngine::UI::GUIDrawer::DrawVec3(WidgetContainer& p_root, const std::string& p_title, std::function<glm::vec3()> p_gatherer, std::function<void(glm::vec3)> p_provider, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_title);
	auto& widget = p_root.CreateWidget<Widgets::DragMultipleScalars<float, 3>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());

	auto RegisterGatherer([p_gatherer]()
	{
		const glm::vec3 value = p_gatherer();
		return reinterpret_cast<const std::array<float, 3>&>(value);
	});

	auto RegisterProvider([p_provider](std::array<float, 3> p_value)
	{
		p_provider(reinterpret_cast<glm::vec3&>(p_value));
	});

	widget.SetGathererAndProvider(RegisterGatherer, RegisterProvider);
}
