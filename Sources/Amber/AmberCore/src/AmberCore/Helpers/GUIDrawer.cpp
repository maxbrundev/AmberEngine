#include "Amberpch.h"

#include "AmberCore/Helpers/GUIDrawer.h"

#include "AmberAudio/Resources/Sound.h"

#include "AmberCore/ResourceManagement/MaterialManager.h"
#include "AmberCore/ResourceManagement/SoundManager.h"
#include "AmberCore/ResourceManagement/ModelManager.h"
#include "AmberCore/ResourceManagement/ShaderManager.h"
#include "AmberCore/ResourceManagement/TextureManager.h"

#include "AmberTools/Global/ServiceLocator.h"
#include "AmberTools/Utils/PathParser.h"

#include "AmberUI/Widgets/Button.h"
#include "AmberUI/Widgets/ButtonSmall.h"
#include "AmberUI/Widgets/CheckBox.h"
#include "AmberUI/Widgets/ColorEdit.h"
#include "AmberUI/Widgets/DataWidget.h"
#include "AmberUI/Widgets/DragMultipleScalars.h"
#include "AmberUI/Widgets/Group.h"
#include "AmberUI/Widgets/InputText.h"
#include "AmberUI/Widgets/TextColored.h"

const AmberRendering::Data::Color AmberCore::Helpers::GUIDrawer::TITLE_COLOR        = { 0.85f, 0.65f, 0.0f };
const AmberRendering::Data::Color AmberCore::Helpers::GUIDrawer::CLEAR_BUTTON_COLOR = { 0.5f, 0.0f, 0.0f };

const float AmberCore::Helpers::GUIDrawer::MIN_FLOAT = -999999999.0f;
const float AmberCore::Helpers::GUIDrawer::MAX_FLOAT = +999999999.0f;

AmberRendering::Resources::Texture* AmberCore::Helpers::GUIDrawer::S_EMPTY_TEXTURE = nullptr;

void AmberCore::Helpers::GUIDrawer::ProvideEmptyTexture(AmberRendering::Resources::Texture& p_emptyTexture)
{
	S_EMPTY_TEXTURE = &p_emptyTexture;
}

void AmberCore::Helpers::GUIDrawer::CreateTitle(AmberUI::WidgetContainer& p_root, const std::string& p_title)
{
	p_root.CreateWidget<AmberUI::Widgets::TextColored>(p_title, TITLE_COLOR);
}

void AmberCore::Helpers::GUIDrawer::DrawBoolean(AmberUI::WidgetContainer& p_root, const std::string& p_name, bool& p_data)
{
	CreateTitle(p_root, p_name);

	auto& checkBox = p_root.CreateWidget<AmberUI::Widgets::CheckBox>();

	checkBox.RegisterReference(p_data);
}

void AmberCore::Helpers::GUIDrawer::DrawBoolean(AmberUI::WidgetContainer& p_root, const std::string& p_name, const std::function<bool()>& p_gatherer, const std::function<void(bool)>& p_provider)
{
	CreateTitle(p_root, p_name);

	auto& checkBox = p_root.CreateWidget<AmberUI::Widgets::CheckBox>();

	checkBox.SetGathererAndProvider(p_gatherer, p_provider);
}

void AmberCore::Helpers::GUIDrawer::DrawVec2(AmberUI::WidgetContainer& p_root, const std::string& p_name, glm::vec2& p_data, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);

	auto& dragMultipleScalars = p_root.CreateWidget<AmberUI::Widgets::DragMultipleScalars<float, 2>>(GetDataType<float>(), p_min, p_max, 0.0f, p_step, "", GetFormat<float>());

	dragMultipleScalars.ValueChangedEvent += [&p_data](std::array<float, 2>& array)
	{
		auto& arrayData = reinterpret_cast<std::array<float, 2>&>(p_data);

		arrayData = array;
	};
}

void AmberCore::Helpers::GUIDrawer::DrawColor(AmberUI::WidgetContainer& p_root, const std::string& p_title, AmberRendering::Data::Color& p_color, bool p_hasAlpha)
{
	CreateTitle(p_root, p_title);

	auto& colorEdit = p_root.CreateWidget<AmberUI::Widgets::ColorEdit>(p_hasAlpha, p_color);

	colorEdit.SetGathererAndProvider(nullptr, [&](AmberRendering::Data::Color color) { p_color = color; });
}

void AmberCore::Helpers::GUIDrawer::DrawString(AmberUI::WidgetContainer& p_root, const std::string& p_title, const std::function<std::string(void)>& p_gatherer, const std::function<void(std::string)>& p_provider)
{
	CreateTitle(p_root, p_title);

	auto& inputText = p_root.CreateWidget<AmberUI::Widgets::InputText>("");
	inputText.SetGathererAndProvider(p_gatherer, p_provider);
}

void AmberCore::Helpers::GUIDrawer::DrawVec3(AmberUI::WidgetContainer& p_root, const std::string& p_title, const std::function<glm::vec3()>& p_gatherer, const std::function<void(glm::vec3)>& p_provider, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_title);

	auto& dragMultipleScalars = p_root.CreateWidget<AmberUI::Widgets::DragMultipleScalars<float, 3>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());

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

void AmberCore::Helpers::GUIDrawer::DrawColor(AmberUI::WidgetContainer& p_root, const std::string& p_name,
	std::function<AmberRendering::Data::Color()> p_gatherer, std::function<void(AmberRendering::Data::Color)> p_provider, bool p_hasAlpha)
{

	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<AmberUI::Widgets::ColorEdit>(p_hasAlpha);
	widget.SetGathererAndProvider(p_gatherer, p_provider);
}

AmberUI::Widgets::Text& AmberCore::Helpers::GUIDrawer::DrawMesh(AmberUI::WidgetContainer& p_root, const std::string& p_title, AmberRendering::Resources::Model*& p_data, AmberTools::Eventing::Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_title);

	std::string displayedText = (p_data ? p_data->Path : std::string("Empty"));
	auto& group = p_root.CreateWidget<AmberUI::Widgets::Group>();

	auto& pathText = group.CreateWidget<AmberUI::Widgets::Text>(displayedText);
	pathText.AddDragDropTarget("File").DataReceivedEvent += [&pathText, &p_data, p_updateNotifier](auto p_receivedData)
	{
		if (AmberTools::Utils::PathParser::GetFileType(p_receivedData.first) == AmberTools::Utils::PathParser::EFileType::MODEL)
		{
			if (auto* resource = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::ModelManager>().GetResource(p_receivedData.first); resource)
			{
				p_data = resource;

				pathText.Content = p_receivedData.first;

				if (p_updateNotifier)
					p_updateNotifier->Invoke();
			}
		}
	};

	pathText.LineBreak = false;

	auto& clearSmallButton = group.CreateWidget<AmberUI::Widgets::ButtonSmall>("Clear");
	clearSmallButton.IdleBackgroundColor = CLEAR_BUTTON_COLOR;
	clearSmallButton.ClickedEvent += [&pathText, &p_data, p_updateNotifier]
	{
		p_data = nullptr;

		pathText.Content = "Empty";

		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	return pathText;
}

AmberUI::Widgets::Image& AmberCore::Helpers::GUIDrawer::DrawTexture(AmberUI::WidgetContainer& p_root, const std::string& p_name, AmberRendering::Resources::Texture*& p_data, AmberTools::Eventing::Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_name);

	std::string displayedText = (p_data ? p_data->Path : std::string("Empty"));
	auto& group = p_root.CreateWidget<AmberUI::Widgets::Group>();

	auto& image = group.CreateWidget<AmberUI::Widgets::Image>(p_data ? p_data->ID : (S_EMPTY_TEXTURE ? S_EMPTY_TEXTURE->ID : 0), glm::vec2{ 75, 75 });

	image.AddDragDropTarget("File").DataReceivedEvent += [&image, &p_data, p_updateNotifier](auto p_receivedData)
	{
		if (AmberTools::Utils::PathParser::GetFileType(p_receivedData.first) == AmberTools::Utils::PathParser::EFileType::TEXTURE)
		{
			if (auto resource = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::TextureManager>().GetResource(p_receivedData.first); resource)
			{
				p_data = resource;

				image.TextureID.id = resource->ID;

				if (p_updateNotifier)
					p_updateNotifier->Invoke();
			}
		}
	};

	image.LineBreak = false;

	auto& resetButton = group.CreateWidget<AmberUI::Widgets::Button>("Clear");
	resetButton.IdleBackgroundColor = CLEAR_BUTTON_COLOR;
	resetButton.ClickedEvent += [&image, &p_data, p_updateNotifier]
	{
		p_data = nullptr;

		image.TextureID.id = (S_EMPTY_TEXTURE ? S_EMPTY_TEXTURE->ID : 0);

		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	return image;
}

AmberUI::Widgets::Text& AmberCore::Helpers::GUIDrawer::DrawShader(AmberUI::WidgetContainer& p_root, const std::string& p_name, AmberRendering::Resources::Shader*& p_data, AmberTools::Eventing::Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_name);

	std::string displayedText = (p_data ? p_data->Path : std::string("Empty"));
	auto& group = p_root.CreateWidget<AmberUI::Widgets::Group>();

	auto& pathText = group.CreateWidget<AmberUI::Widgets::Text>(displayedText);

	pathText.AddDragDropTarget("File").DataReceivedEvent += [&pathText, &p_data, p_updateNotifier](auto p_receivedData)
	{
		if (AmberTools::Utils::PathParser::GetFileType(p_receivedData.first) == AmberTools::Utils::PathParser::EFileType::SHADER)
		{
			if (auto resource = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::ShaderManager>().GetResource(p_receivedData.first); resource)
			{
				p_data = resource;

				pathText.Content = p_receivedData.first;

				if (p_updateNotifier)
					p_updateNotifier->Invoke();
			}
		}
	};

	pathText.LineBreak = false;

	auto& resetButton = group.CreateWidget<AmberUI::Widgets::ButtonSmall>("Clear");
	resetButton.IdleBackgroundColor = CLEAR_BUTTON_COLOR;
	resetButton.ClickedEvent += [&pathText, &p_data, p_updateNotifier]
	{
		p_data = nullptr;

		pathText.Content = "Empty";

		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	return pathText;
}

AmberUI::Widgets::Text& AmberCore::Helpers::GUIDrawer::DrawMaterial(AmberUI::WidgetContainer& p_root, const std::string& p_name, AmberCore::Resources::Material*& p_data, AmberTools::Eventing::Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_name);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto& group = p_root.CreateWidget<AmberUI::Widgets::Group>();

	auto& pathText = group.CreateWidget<AmberUI::Widgets::Text>(displayedText);

	pathText.AddDragDropTarget("File").DataReceivedEvent += [&pathText, &p_data, p_updateNotifier](auto p_receivedData)
	{
		if (AmberTools::Utils::PathParser::GetFileType(p_receivedData.first) == AmberTools::Utils::PathParser::EFileType::MATERIAL)
		{
			if (auto resource = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::MaterialManager>().GetResource(p_receivedData.first); resource)
			{
				p_data = resource;

				pathText.Content = p_receivedData.first;

				if (p_updateNotifier)
					p_updateNotifier->Invoke();
			}
		}
	};

	pathText.LineBreak = false;

	auto& resetButton = group.CreateWidget<AmberUI::Widgets::ButtonSmall>("Clear");
	resetButton.IdleBackgroundColor = CLEAR_BUTTON_COLOR;
	resetButton.ClickedEvent += [&pathText, &p_data, p_updateNotifier]
	{
		p_data = nullptr;

		pathText.Content = "Empty";

		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	return pathText;
}

AmberUI::Widgets::Text& AmberCore::Helpers::GUIDrawer::DrawAsset(AmberUI::WidgetContainer& p_root,
	const std::string& p_name, std::string& p_data, AmberTools::Eventing::Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_name);

	const std::string displayedText = (p_data.empty() ? std::string("Empty") : p_data);
	auto& rightSide = p_root.CreateWidget<AmberUI::Widgets::Group>();

	auto& widget = rightSide.CreateWidget<AmberUI::Widgets::Text>(displayedText);

	widget.AddDragDropTarget("File").DataReceivedEvent += [&widget, &p_data, p_updateNotifier](auto p_receivedData)
	{
		p_data = p_receivedData.first;
		widget.Content = p_receivedData.first;
		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	widget.LineBreak = false;

	auto& resetButton = rightSide.CreateWidget<AmberUI::Widgets::ButtonSmall>("Clear");
	resetButton.IdleBackgroundColor = CLEAR_BUTTON_COLOR;
	resetButton.ClickedEvent += [&widget, &p_data, p_updateNotifier]
	{
		p_data = "";
		widget.Content = "Empty";
		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	return widget;
}

void AmberCore::Helpers::GUIDrawer::DrawDDString(AmberUI::WidgetContainer& p_root, const std::string& p_name,
	std::function<std::string()> p_gatherer, std::function<void(std::string)> p_provider,
	const std::string& p_identifier)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<AmberUI::Widgets::InputText>("");

	widget.SetGathererAndProvider(p_gatherer, p_provider);

	auto& ddTarget = widget.AddDragDropTarget(p_identifier);
	ddTarget.DataReceivedEvent += [&widget](std::pair<std::string, AmberUI::Widgets::Group*> p_data)
	{
		widget.Content = p_data.first;
		widget.NotifyChange();
	};
}

AmberUI::Widgets::Text& AmberCore::Helpers::GUIDrawer::DrawSound(AmberUI::WidgetContainer& p_root, const std::string& p_name, AmberAudio::Resources::Sound*& p_data, AmberTools::Eventing::Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_name);

	std::string displayedText = (p_data ? p_data->Path : std::string("Empty"));
	auto& group = p_root.CreateWidget<AmberUI::Widgets::Group>();

	auto& pathText = group.CreateWidget<AmberUI::Widgets::Text>(displayedText);

	pathText.AddDragDropTarget("File").DataReceivedEvent += [&pathText, &p_data, p_updateNotifier](auto p_receivedData)
	{
		if (AmberTools::Utils::PathParser::GetFileType(p_receivedData.first) == AmberTools::Utils::PathParser::EFileType::SOUND)
		{
			if (auto resource = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::SoundManager>().GetResource(p_receivedData.first); resource)
			{
				p_data = resource;

				pathText.Content = p_receivedData.first;

				if (p_updateNotifier)
					p_updateNotifier->Invoke();
			}
		}
	};

	pathText.LineBreak = false;

	auto& resetButton = group.CreateWidget<AmberUI::Widgets::ButtonSmall>("Clear");
	resetButton.IdleBackgroundColor = CLEAR_BUTTON_COLOR;
	resetButton.ClickedEvent += [&pathText, &p_data, p_updateNotifier]
	{
		p_data = nullptr;

		pathText.Content = "Empty";

		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	return pathText;
}
