#include "Amberpch.h"

#include "AmberEngine/UI/GUIDrawer.h"

#include "AmberEngine/Managers/MaterialManager.h"
#include "AmberEngine/Managers/ModelManager.h"
#include "AmberEngine/Managers/ShaderManager.h"
#include "AmberEngine/Managers/TextureManager.h"

#include "AmberEngine/Tools/Global/ServiceLocator.h"
#include "AmberEngine/Tools/Utils/PathParser.h"

#include "AmberEngine/UI/Widgets/Button.h"
#include "AmberEngine/UI/Widgets/ButtonSmall.h"
#include "AmberEngine/UI/Widgets/CheckBox.h"
#include "AmberEngine/UI/Widgets/ColorEdit.h"
#include "AmberEngine/UI/Widgets/DataWidget.h"
#include "AmberEngine/UI/Widgets/DragMultipleScalars.h"
#include "AmberEngine/UI/Widgets/Group.h"
#include "AmberEngine/UI/Widgets/InputText.h"
#include "AmberEngine/UI/Widgets/TextColored.h"

const AmberEngine::Data::Color AmberEngine::UI::GUIDrawer::TITLE_COLOR        = { 0.85f, 0.65f, 0.0f };
const AmberEngine::Data::Color AmberEngine::UI::GUIDrawer::CLEAR_BUTTON_COLOR = { 0.5f, 0.0f, 0.0f };

const float AmberEngine::UI::GUIDrawer::MIN_FLOAT = -999999999.0f;
const float AmberEngine::UI::GUIDrawer::MAX_FLOAT = +999999999.0f;

AmberEngine::Resources::Texture* AmberEngine::UI::GUIDrawer::S_EMPTY_TEXTURE = nullptr;

void AmberEngine::UI::GUIDrawer::ProvideEmptyTexture(Resources::Texture& p_emptyTexture)
{
	S_EMPTY_TEXTURE = &p_emptyTexture;
}

void AmberEngine::UI::GUIDrawer::CreateTitle(WidgetContainer& p_root, const std::string& p_title)
{
	p_root.CreateWidget<Widgets::TextColored>(p_title, TITLE_COLOR);
}

void AmberEngine::UI::GUIDrawer::DrawBoolean(WidgetContainer& p_root, const std::string& p_name, bool& p_data)
{
	CreateTitle(p_root, p_name);

	auto& checkBox = p_root.CreateWidget<Widgets::CheckBox>();

	checkBox.RegisterReference(p_data);
}

void AmberEngine::UI::GUIDrawer::DrawBoolean(WidgetContainer& p_root, const std::string& p_name, const std::function<bool()>& p_gatherer, const std::function<void(bool)>& p_provider)
{
	CreateTitle(p_root, p_name);

	auto& checkBox = p_root.CreateWidget<Widgets::CheckBox>();

	checkBox.SetGathererAndProvider(p_gatherer, p_provider);
}

void AmberEngine::UI::GUIDrawer::DrawVec2(WidgetContainer& p_root, const std::string& p_name, glm::vec2& p_data, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);

	auto& dragMultipleScalars = p_root.CreateWidget<Widgets::DragMultipleScalars<float, 2>>(GetDataType<float>(), p_min, p_max, 0.0f, p_step, "", GetFormat<float>());

	dragMultipleScalars.ValueChangedEvent += [&p_data](std::array<float, 2>& array)
	{
		auto& arrayData = reinterpret_cast<std::array<float, 2>&>(p_data);

		arrayData = array;
	};
}

void AmberEngine::UI::GUIDrawer::DrawColor(WidgetContainer& p_root, const std::string& p_title, Data::Color& p_color, bool p_hasAlpha)
{
	CreateTitle(p_root, p_title);

	auto& colorEdit = p_root.CreateWidget<Widgets::ColorEdit>(p_hasAlpha, p_color);

	colorEdit.SetGathererAndProvider(nullptr, [&](Data::Color color) { p_color = color; });
}

void AmberEngine::UI::GUIDrawer::DrawString(WidgetContainer& p_root, const std::string& p_title, const std::function<std::string(void)>& p_gatherer, const std::function<void(std::string)>& p_provider)
{
	CreateTitle(p_root, p_title);

	auto& inputText = p_root.CreateWidget<Widgets::InputText>("");
	inputText.SetGathererAndProvider(p_gatherer, p_provider);
}

void AmberEngine::UI::GUIDrawer::DrawVec3(WidgetContainer& p_root, const std::string& p_title, const std::function<glm::vec3()>& p_gatherer, const std::function<void(glm::vec3)>& p_provider, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_title);

	auto& dragMultipleScalars = p_root.CreateWidget<Widgets::DragMultipleScalars<float, 3>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());

	auto RegisterGatherer([p_gatherer]()
	{
		const glm::vec3 value = p_gatherer();
		return reinterpret_cast<const std::array<float, 3>&>(value);
	});

	auto RegisterProvider([p_provider](std::array<float, 3> p_value)
	{
		p_provider(reinterpret_cast<glm::vec3&>(p_value));
	});

	dragMultipleScalars.SetGathererAndProvider(RegisterGatherer, RegisterProvider);
}

void AmberEngine::UI::GUIDrawer::DrawColor(WidgetContainer& p_root, const std::string& p_name,
	std::function<Data::Color()> p_gatherer, std::function<void(Data::Color)> p_provider, bool p_hasAlpha)
{

	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<Widgets::ColorEdit>(p_hasAlpha);
	widget.SetGathererAndProvider(p_gatherer, p_provider);
}

AmberEngine::UI::Widgets::Text& AmberEngine::UI::GUIDrawer::DrawMesh(WidgetContainer& p_root, const std::string& p_title, Resources::Model*& p_data, Eventing::Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_title);

	std::string displayedText = (p_data ? p_data->Path : std::string("Empty"));
	auto& group = p_root.CreateWidget<Widgets::Group>();

	auto& pathText = group.CreateWidget<Widgets::Text>(displayedText);
	pathText.AddPlugin<DDTarget<std::pair<std::string, Widgets::Group*>>>("File").DataReceivedEvent += [&pathText, &p_data, p_updateNotifier](auto p_receivedData)
	{
		if (Tools::Utils::PathParser::GetFileType(p_receivedData.first) == Tools::Utils::PathParser::EFileType::MODEL)
		{
			if (auto* resource = Tools::Global::ServiceLocator::Get<ResourceManagement::ModelManager>().GetResource(p_receivedData.first); resource)
			{
				p_data = resource;

				pathText.content = p_receivedData.first;

				if (p_updateNotifier)
					p_updateNotifier->Invoke();
			}
		}
	};

	pathText.lineBreak = false;

	auto& clearSmallButton = group.CreateWidget<Widgets::ButtonSmall>("Clear");
	clearSmallButton.idleBackgroundColor = CLEAR_BUTTON_COLOR;
	clearSmallButton.ClickedEvent += [&pathText, &p_data, p_updateNotifier]
	{
		p_data = nullptr;

		pathText.content = "Empty";

		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	return pathText;
}

AmberEngine::UI::Widgets::Image& AmberEngine::UI::GUIDrawer::DrawTexture(WidgetContainer& p_root, const std::string& p_name, Resources::Texture*& p_data, Eventing::Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_name);

	std::string displayedText = (p_data ? p_data->Path : std::string("Empty"));
	auto& group = p_root.CreateWidget<Widgets::Group>();

	auto& image = group.CreateWidget<Widgets::Image>(p_data ? p_data->ID : (S_EMPTY_TEXTURE ? S_EMPTY_TEXTURE->ID : 0), glm::vec2{ 75, 75 });

	image.AddPlugin<DDTarget<std::pair<std::string, Widgets::Group*>>>("File").DataReceivedEvent += [&image, &p_data, p_updateNotifier](auto p_receivedData)
	{
		if (Tools::Utils::PathParser::GetFileType(p_receivedData.first) == Tools::Utils::PathParser::EFileType::TEXTURE)
		{
			if (auto resource = Tools::Global::ServiceLocator::Get<ResourceManagement::TextureManager>().GetResource(p_receivedData.first); resource)
			{
				p_data = resource;

				image.textureID.id = resource->ID;

				if (p_updateNotifier)
					p_updateNotifier->Invoke();
			}
		}
	};

	image.lineBreak = false;

	auto& resetButton = group.CreateWidget<Widgets::Button>("Clear");
	resetButton.idleBackgroundColor = CLEAR_BUTTON_COLOR;
	resetButton.ClickedEvent += [&image, &p_data, p_updateNotifier]
	{
		p_data = nullptr;

		image.textureID.id = (S_EMPTY_TEXTURE ? S_EMPTY_TEXTURE->ID : 0);

		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	return image;
}

AmberEngine::UI::Widgets::Text& AmberEngine::UI::GUIDrawer::DrawShader(WidgetContainer& p_root, const std::string& p_name, Resources::Shader*& p_data, Eventing::Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_name);

	std::string displayedText = (p_data ? p_data->Path : std::string("Empty"));
	auto& group = p_root.CreateWidget<Widgets::Group>();

	auto& pathText = group.CreateWidget<Widgets::Text>(displayedText);

	pathText.AddPlugin<DDTarget<std::pair<std::string, Widgets::Group*>>>("File").DataReceivedEvent += [&pathText, &p_data, p_updateNotifier](auto p_receivedData)
	{
		if (Tools::Utils::PathParser::GetFileType(p_receivedData.first) == Tools::Utils::PathParser::EFileType::SHADER)
		{
			if (auto resource = Tools::Global::ServiceLocator::Get<ResourceManagement::ShaderManager>().GetResource(p_receivedData.first); resource)
			{
				p_data = resource;

				pathText.content = p_receivedData.first;

				if (p_updateNotifier)
					p_updateNotifier->Invoke();
			}
		}
	};

	pathText.lineBreak = false;

	auto& resetButton = group.CreateWidget<Widgets::ButtonSmall>("Clear");
	resetButton.idleBackgroundColor = CLEAR_BUTTON_COLOR;
	resetButton.ClickedEvent += [&pathText, &p_data, p_updateNotifier]
	{
		p_data = nullptr;

		pathText.content = "Empty";

		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	return pathText;
}

AmberEngine::UI::Widgets::Text& AmberEngine::UI::GUIDrawer::DrawMaterial(WidgetContainer& p_root, const std::string& p_name, Resources::Material*& p_data, Eventing::Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_name);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto& group = p_root.CreateWidget<Widgets::Group>();

	auto& pathText = group.CreateWidget<Widgets::Text>(displayedText);

	pathText.AddPlugin<DDTarget<std::pair<std::string, Widgets::Group*>>>("File").DataReceivedEvent += [&pathText, &p_data, p_updateNotifier](auto p_receivedData)
	{
		if (Tools::Utils::PathParser::GetFileType(p_receivedData.first) == Tools::Utils::PathParser::EFileType::MATERIAL)
		{
			if (auto resource = Tools::Global::ServiceLocator::Get<ResourceManagement::MaterialManager>().GetResource(p_receivedData.first); resource)
			{
				p_data = resource;

				pathText.content = p_receivedData.first;

				if (p_updateNotifier)
					p_updateNotifier->Invoke();
			}
		}
	};

	pathText.lineBreak = false;

	auto& resetButton = group.CreateWidget<Widgets::ButtonSmall>("Clear");
	resetButton.idleBackgroundColor = CLEAR_BUTTON_COLOR;
	resetButton.ClickedEvent += [&pathText, &p_data, p_updateNotifier]
	{
		p_data = nullptr;

		pathText.content = "Empty";

		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	return pathText;
}

AmberEngine::UI::Widgets::Text& AmberEngine::UI::GUIDrawer::DrawAsset(WidgetContainer& p_root,
	const std::string& p_name, std::string& p_data, Eventing::Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_name);

	const std::string displayedText = (p_data.empty() ? std::string("Empty") : p_data);
	auto& rightSide = p_root.CreateWidget<Widgets::Group>();

	auto& widget = rightSide.CreateWidget<Widgets::Text>(displayedText);

	widget.AddPlugin<DDTarget<std::pair<std::string, Widgets::Group*>>>("File").DataReceivedEvent += [&widget, &p_data, p_updateNotifier](auto p_receivedData)
	{
		p_data = p_receivedData.first;
		widget.content = p_receivedData.first;
		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	widget.lineBreak = false;

	auto& resetButton = rightSide.CreateWidget<Widgets::ButtonSmall>("Clear");
	resetButton.idleBackgroundColor = CLEAR_BUTTON_COLOR;
	resetButton.ClickedEvent += [&widget, &p_data, p_updateNotifier]
	{
		p_data = "";
		widget.content = "Empty";
		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	return widget;
}

void AmberEngine::UI::GUIDrawer::DrawDDString(WidgetContainer& p_root, const std::string& p_name,
	std::function<std::string()> p_gatherer, std::function<void(std::string)> p_provider,
	const std::string& p_identifier)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<Widgets::InputText>("");

	widget.SetGathererAndProvider(p_gatherer, p_provider);

	auto& ddTarget = widget.AddPlugin<DDTarget<std::pair<std::string, Widgets::Group*>>>(p_identifier);
	ddTarget.DataReceivedEvent += [&widget](std::pair<std::string, Widgets::Group*> p_data)
	{
		widget.content = p_data.first;
		widget.NotifyChange();
	};
}
