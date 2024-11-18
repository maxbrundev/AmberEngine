#include "Amberpch.h"

#include "AmberEngine/UI/Panels/AssetBrowser.h"

#include "AmberEngine/Core/EditorAction.h"
#include "AmberEngine/Debug/Logger.h"
#include "AmberEngine/Tools/Global/ServiceLocator.h"
#include "AmberEngine/Tools/Utils/PathParser.h"
#include "AmberEngine/Tools/Utils/String.h"
#include "AmberEngine/Tools/Utils/SystemCalls.h"
#include "AmberEngine/UI/Widgets/Image.h"
#include "AmberEngine/UI/Widgets/InputText.h"
#include "AmberEngine/UI/Widgets/Separator.h"
#include "AmberEngine/UI/Widgets/TextClickable.h"
#include "AmberEngine/UI/Core/IPlugin.h"
#include "AmberEngine/UI/Widgets/TextColored.h"
#include "AmberEngine/Windowing/Dialogs/MessageBox.h"
#include "AmberEngine/Resources/Loaders/ShaderLoader.h"
#include "AmberEngine/UI/Panels/AssetProperties.h"
#include "AmberEngine/UI/Panels/MaterialEditor.h"
#include "AmberEngine/UI/Widgets/ContextualMenuItem.h"
#include "AmberEngine/UI/Panels/Views/AssetView.h"


#define FILENAMES_CHARS AmberEngine::UI::Panels::AssetBrowser::__FILENAMES_CHARS

const std::string FILENAMES_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-_=+ 0123456789()[]";

std::string GetAssociatedMetaFile(const std::string& p_assetPath)
{
	return p_assetPath + ".meta";
}


void RenameAsset(const std::string& p_prev, const std::string& p_new)
{
	std::filesystem::rename(p_prev, p_new);

	if (const std::string previousMetaPath = GetAssociatedMetaFile(p_prev); std::filesystem::exists(previousMetaPath))
	{
		if (const std::string newMetaPath = GetAssociatedMetaFile(p_new); !std::filesystem::exists(newMetaPath))
		{
			std::filesystem::rename(previousMetaPath, newMetaPath);
		}
	}
}


void RemoveAsset(const std::string& p_toDelete)
{
	std::filesystem::remove(p_toDelete);

	if (const std::string metaPath = GetAssociatedMetaFile(p_toDelete); std::filesystem::exists(metaPath))
	{
		std::filesystem::remove(metaPath);
	}
}


class TexturePreview : public IPlugin
{
public:
	TexturePreview() : image(0, { 80, 80 })
	{

	}

	void SetPath(const std::string& p_path)
	{
		texture = AmberEngine::Tools::Global::ServiceLocator::Get<AmberEngine::ResourceManagement::TextureManager>()[p_path];
	}

	virtual void Execute() override
	{
		if (ImGui::IsItemHovered())
		{
			if (texture)
				image.textureID.id = texture->ID;

			ImGui::BeginTooltip();
			image.Draw();
			ImGui::EndTooltip();
		}
	}

	AmberEngine::Resources::Texture* texture = nullptr;
	AmberEngine::UI::Widgets::Image image;
};


class BrowserItemContextualMenu : public AmberEngine::UI::Widgets::ContextualMenuItem
{
public:
	BrowserItemContextualMenu(const std::string p_filePath, bool p_protected = false) : m_protected(p_protected), filePath(p_filePath) {}

	virtual void CreateList()
	{
		if (!m_protected)
		{
			auto& deleteAction = CreateWidget<AmberEngine::UI::Widgets::MenuItem>("Delete");
			deleteAction.ClickedEvent += [this] { DeleteItem(); };

			auto& renameMenu = CreateWidget<AmberEngine::UI::Widgets::MenuList>("Rename to...");

			auto& nameEditor = renameMenu.CreateWidget<AmberEngine::UI::Widgets::InputText>("");
			nameEditor.selectAllOnClick = true;

			renameMenu.ClickedEvent += [this, &nameEditor]
			{
				nameEditor.content = AmberEngine::Tools::Utils::PathParser::GetElementName(filePath);

				if (!std::filesystem::is_directory(filePath))
					if (size_t pos = nameEditor.content.rfind('.'); pos != std::string::npos)
						nameEditor.content = nameEditor.content.substr(0, pos);
			};

			nameEditor.EnterPressedEvent += [this](std::string p_newName)
			{
				if (!std::filesystem::is_directory(filePath))
					p_newName += '.' + AmberEngine::Tools::Utils::PathParser::GetExtension(filePath);

				/* Clean the name (Remove special chars) */
				p_newName.erase(std::remove_if(p_newName.begin(), p_newName.end(), [](auto& c)
				{
					return std::find(FILENAMES_CHARS.begin(), FILENAMES_CHARS.end(), c) == FILENAMES_CHARS.end();
				}), p_newName.end());

				std::string containingFolderPath = AmberEngine::Tools::Utils::PathParser::GetContainingFolder(filePath);
				std::string newPath = containingFolderPath + p_newName;
				std::string oldPath = filePath;

				if (filePath != newPath && !std::filesystem::exists(newPath))
					filePath = newPath;

				if (std::filesystem::is_directory(oldPath))
					filePath += '\\';

				RenamedEvent.Invoke(oldPath, newPath);
			};
		}
	}

	virtual void DeleteItem() = 0;

public:
	bool m_protected;
	std::string filePath;
	AmberEngine::Eventing::Event<std::string> DestroyedEvent;
	AmberEngine::Eventing::Event<std::string, std::string> RenamedEvent;
	void* userData = nullptr;
};






class FolderContextualMenu : public BrowserItemContextualMenu
{
public:
	FolderContextualMenu(const std::string& p_filePath, bool p_protected = false) : BrowserItemContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& showInExplorer = CreateWidget<AmberEngine::UI::Widgets::MenuItem>("Show in explorer");
		showInExplorer.ClickedEvent += [this]
		{
			AmberEngine::Tools::Utils::SystemCalls::ShowInExplorer(filePath);
		};

		if (!m_protected)
		{
			auto& importAssetHere = CreateWidget<AmberEngine::UI::Widgets::MenuItem>("Import Here...");
			importAssetHere.ClickedEvent += [this]
			{
				if (EDITOR_EXEC(ImportAssetAtLocation(filePath)))
				{
					AmberEngine::UI::Widgets::TreeNode* pluginOwner = reinterpret_cast<AmberEngine::UI::Widgets::TreeNode*>(userData);
					pluginOwner->Close();
					EDITOR_EXEC(DelayAction(std::bind(&AmberEngine::UI::Widgets::TreeNode::Open, pluginOwner)));
				}
			};

			auto& createMenu = CreateWidget<AmberEngine::UI::Widgets::MenuList>("Create..");

			auto& createFolderMenu   = createMenu.CreateWidget<AmberEngine::UI::Widgets::MenuList>("Folder");
			auto& createSceneMenu    = createMenu.CreateWidget<AmberEngine::UI::Widgets::MenuList>("Scene");
			auto& createShaderMenu   = createMenu.CreateWidget<AmberEngine::UI::Widgets::MenuList>("Shader");
			auto& createMaterialMenu = createMenu.CreateWidget<AmberEngine::UI::Widgets::MenuList>("Material");

			auto& createStandardShaderMenu    = createShaderMenu.CreateWidget<AmberEngine::UI::Widgets::MenuList>("Standard template");
			auto& createStandardPBRShaderMenu = createShaderMenu.CreateWidget<AmberEngine::UI::Widgets::MenuList>("Standard PBR template");
			auto& createUnlitShaderMenu       = createShaderMenu.CreateWidget<AmberEngine::UI::Widgets::MenuList>("Unlit template");
			auto& createLambertShaderMenu     = createShaderMenu.CreateWidget<AmberEngine::UI::Widgets::MenuList>("Lambert template");

			auto& createEmptyMaterialMenu       = createMaterialMenu.CreateWidget<AmberEngine::UI::Widgets::MenuList>("Empty");
			auto& createStandardMaterialMenu    = createMaterialMenu.CreateWidget<AmberEngine::UI::Widgets::MenuList>("Standard");
			auto& createStandardPBRMaterialMenu = createMaterialMenu.CreateWidget<AmberEngine::UI::Widgets::MenuList>("Standard PBR");
			auto& createUnlitMaterialMenu       = createMaterialMenu.CreateWidget<AmberEngine::UI::Widgets::MenuList>("Unlit");
			auto& createLambertMaterialMenu     = createMaterialMenu.CreateWidget<AmberEngine::UI::Widgets::MenuList>("Lambert");

			auto& createFolder = createFolderMenu.CreateWidget<AmberEngine::UI::Widgets::InputText>("");
			auto& createScene  =  createSceneMenu.CreateWidget<AmberEngine::UI::Widgets::InputText>("");

			auto& createEmptyMaterial       =    createEmptyMaterialMenu.CreateWidget<AmberEngine::UI::Widgets::InputText>("");
			auto& createStandardMaterial    = createStandardMaterialMenu.CreateWidget<AmberEngine::UI::Widgets::InputText>("");
			auto& createStandardPBRMaterial = createStandardPBRMaterialMenu.CreateWidget<AmberEngine::UI::Widgets::InputText>("");
			auto& createUnlitMaterial       =   createUnlitMaterialMenu.CreateWidget<AmberEngine::UI::Widgets::InputText>("");
			auto& createLambertMaterial     = createLambertMaterialMenu.CreateWidget<AmberEngine::UI::Widgets::InputText>("");

			auto& createStandardShader    =    createStandardShaderMenu.CreateWidget<AmberEngine::UI::Widgets::InputText>("");
			auto& createStandardPBRShader = createStandardPBRShaderMenu.CreateWidget<AmberEngine::UI::Widgets::InputText>("");
			auto& createUnlitShader       =       createUnlitShaderMenu.CreateWidget<AmberEngine::UI::Widgets::InputText>("");
			auto& createLambertShader     =     createLambertShaderMenu.CreateWidget<AmberEngine::UI::Widgets::InputText>("");

			createFolderMenu.ClickedEvent += [&createFolder] { createFolder.content = ""; };
			createSceneMenu.ClickedEvent += [&createScene] { createScene.content = ""; };
			createStandardShaderMenu.ClickedEvent += [&createStandardShader] { createStandardShader.content = ""; };
			createStandardPBRShaderMenu.ClickedEvent += [&createStandardPBRShader] { createStandardPBRShader.content = ""; };
			createUnlitShaderMenu.ClickedEvent += [&createUnlitShader] { createUnlitShader.content = ""; };
			createLambertShaderMenu.ClickedEvent += [&createLambertShader] { createLambertShader.content = ""; };
			createEmptyMaterialMenu.ClickedEvent += [&createEmptyMaterial] { createEmptyMaterial.content = ""; };
			createStandardMaterialMenu.ClickedEvent += [&createStandardMaterial] { createStandardMaterial.content = ""; };
			createStandardPBRMaterialMenu.ClickedEvent += [&createStandardPBRMaterial] { createStandardPBRMaterial.content = ""; };
			createUnlitMaterialMenu.ClickedEvent += [&createUnlitMaterial] { createUnlitMaterial.content = ""; };
			createLambertMaterialMenu.ClickedEvent += [&createLambertMaterial] { createLambertMaterial.content = ""; };

			createFolder.EnterPressedEvent += [this](std::string newFolderName)
			{
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + (!fails ? newFolderName : newFolderName + " (" + std::to_string(fails) + ')');

					++fails;
				} while (std::filesystem::exists(finalPath));

				std::filesystem::create_directory(finalPath);

				ItemAddedEvent.Invoke(finalPath);
				Close();
			};

			createScene.EnterPressedEvent += [this](std::string newSceneName)
			{
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + (!fails ? newSceneName : newSceneName + " (" + std::to_string(fails) + ')') + ".ovscene";

					++fails;
				} while (std::filesystem::exists(finalPath));

				std::ofstream outfile(finalPath);
				outfile << "<root><scene><actors><actor><name>Directional Light</name><tag></tag><active>true</active><id>1</id><parent>0</parent><components><component><type>class OvCore::ECS::Components::CDirectionalLight</type><data><diffuse><x>1</x><y>1</y><z>1</z></diffuse><specular><x>1</x><y>1</y><z>1</z></specular><intensity>0.75</intensity></data></component><component><type>class OvCore::ECS::Components::CTransform</type><data><position><x>0</x><y>10</y><z>0</z></position><rotation><x>0.81379771</x><y>-0.17101006</y><z>0.29619816</z><w>0.46984628</w></rotation><scale><x>1</x><y>1</y><z>1</z></scale></data></component></components><behaviours/></actor><actor><name>Ambient Light</name><tag></tag><active>true</active><id>2</id><parent>0</parent><components><component><type>class OvCore::ECS::Components::CAmbientSphereLight</type><data><ambient><x>1</x><y>1</y><z>1</z></ambient><intensity>0.1</intensity><radius>10000</radius></data></component><component><type>class OvCore::ECS::Components::CTransform</type><data><position><x>0</x><y>0</y><z>0</z></position><rotation><x>0</x><y>0</y><z>0</z><w>1</w></rotation><scale><x>1</x><y>1</y><z>1</z></scale></data></component></components><behaviours/></actor><actor><name>Main Camera</name><tag></tag><active>true</active><id>3</id><parent>0</parent><components><component><type>class OvCore::ECS::Components::CCamera</type><data><fov>45</fov><near>0.1</near><far>1000</far><clear_color><x>0.1921569</x><y>0.3019608</y><z>0.47450981</z></clear_color></data></component><component><type>class OvCore::ECS::Components::CTransform</type><data><position><x>0</x><y>3</y><z>8</z></position><rotation><x>-7.5904039e-09</x><y>0.98480773</y><z>-0.17364819</z><w>-4.3047311e-08</w></rotation><scale><x>1</x><y>1</y><z>1</z></scale></data></component></components><behaviours/></actor></actors></scene></root>" << std::endl; // Empty scene content

				ItemAddedEvent.Invoke(finalPath);
				Close();
			};

			createStandardShader.EnterPressedEvent += [this](std::string newShaderName)
			{
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + '\\' + (!fails ? newShaderName : newShaderName + " (" + std::to_string(fails) + ')') + ".glsl";

					++fails;
				} while (std::filesystem::exists(finalPath));

				std::filesystem::copy_file(EDITOR_CONTEXT(engineAssetsPath) + "Shaders\\Standard.glsl", finalPath);
				ItemAddedEvent.Invoke(finalPath);
				Close();
			};

			createStandardPBRShader.EnterPressedEvent += [this](std::string newShaderName)
			{
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + '\\' + (!fails ? newShaderName : newShaderName + " (" + std::to_string(fails) + ')') + ".glsl";

					++fails;
				} while (std::filesystem::exists(finalPath));

				std::filesystem::copy_file(EDITOR_CONTEXT(engineAssetsPath) + "Shaders\\StandardPBR.glsl", finalPath);
				ItemAddedEvent.Invoke(finalPath);
				Close();
			};

			createUnlitShader.EnterPressedEvent += [this](std::string newShaderName)
			{
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + '\\' + (!fails ? newShaderName : newShaderName + " (" + std::to_string(fails) + ')') + ".glsl";

					++fails;
				} while (std::filesystem::exists(finalPath));

				std::filesystem::copy_file(EDITOR_CONTEXT(engineAssetsPath) + "Shaders\\Unlit.glsl", finalPath);
				ItemAddedEvent.Invoke(finalPath);
				Close();
			};

			createLambertShader.EnterPressedEvent += [this](std::string newShaderName)
			{
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + '\\' + (!fails ? newShaderName : newShaderName + " (" + std::to_string(fails) + ')') + ".glsl";

					++fails;
				} while (std::filesystem::exists(finalPath));

				std::filesystem::copy_file(EDITOR_CONTEXT(engineAssetsPath) + "Shaders\\Lambert.glsl", finalPath);
				ItemAddedEvent.Invoke(finalPath);
				Close();
			};

			createEmptyMaterial.EnterPressedEvent += [this](std::string materialName)
			{
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".abmat";

					++fails;
				} while (std::filesystem::exists(finalPath));

				{
					std::ofstream outfile(finalPath);
					outfile << "<root><shader>?</shader></root>" << std::endl; // Empty material content
				}

				ItemAddedEvent.Invoke(finalPath);

				if (auto instance = EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))])
				{
					auto& materialEditor = EDITOR_PANEL(AmberEngine::UI::Panels::MaterialEditor, "Material Editor");
					materialEditor.SetTarget(*instance);
					materialEditor.Open();
					materialEditor.Focus();
					materialEditor.Preview();
				}
				Close();
			};

			createStandardMaterial.EnterPressedEvent += [this](std::string materialName)
			{
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".abmat";

					++fails;
				} while (std::filesystem::exists(finalPath));

				{
					std::ofstream outfile(finalPath);
					outfile << "<root><shader>:Shaders\\Standard.glsl</shader></root>" << std::endl; // Empty standard material content
				}

				ItemAddedEvent.Invoke(finalPath);

				if (auto instance = EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))])
				{
					auto& materialEditor = EDITOR_PANEL(AmberEngine::UI::Panels::MaterialEditor, "Material Editor");
					materialEditor.SetTarget(*instance);
					materialEditor.Open();
					materialEditor.Focus();
					materialEditor.Preview();
				}
				Close();
			};

			createStandardPBRMaterial.EnterPressedEvent += [this](std::string materialName)
			{
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".abmat";

					++fails;
				} while (std::filesystem::exists(finalPath));

				{
					std::ofstream outfile(finalPath);
					outfile << "<root><shader>:Shaders\\StandardPBR.glsl</shader></root>" << std::endl; // Empty standard material content
				}

				ItemAddedEvent.Invoke(finalPath);

				if (auto instance = EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))])
				{
					
					auto& materialEditor = EDITOR_PANEL(AmberEngine::UI::Panels::MaterialEditor, "Material Editor");
					materialEditor.SetTarget(*instance);
					materialEditor.Open();
					materialEditor.Focus();
					materialEditor.Preview();
				}
				Close();
			};

			createUnlitMaterial.EnterPressedEvent += [this](std::string materialName)
			{
				std::string newSceneName = "Material";
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".abmat";

					++fails;
				} while (std::filesystem::exists(finalPath));

				{
					std::ofstream outfile(finalPath);
					outfile << "<root><shader>:Shaders\\Unlit.glsl</shader></root>" << std::endl; // Empty unlit material content
				}

				ItemAddedEvent.Invoke(finalPath);

				if (auto instance = EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))])
				{
			
					auto& materialEditor = EDITOR_PANEL(AmberEngine::UI::Panels::MaterialEditor, "Material Editor");
					materialEditor.SetTarget(*instance);
					materialEditor.Open();
					materialEditor.Focus();
					materialEditor.Preview();
				}
				Close();
			};

			createLambertMaterial.EnterPressedEvent += [this](std::string materialName)
			{
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = filePath + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".abmat";

					++fails;
				} while (std::filesystem::exists(finalPath));

				{
					std::ofstream outfile(finalPath);
					outfile << "<root><shader>:Shaders\\Lambert.glsl</shader></root>" << std::endl; // Empty unlit material content
				}

				ItemAddedEvent.Invoke(finalPath);

				if (auto instance = EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))])
				{
					auto& materialEditor = EDITOR_PANEL(AmberEngine::UI::Panels::MaterialEditor, "Material Editor");
					materialEditor.SetTarget(*instance);
					materialEditor.Open();
					materialEditor.Focus();
					materialEditor.Preview();
				}
				Close();
			};

			BrowserItemContextualMenu::CreateList();
		}
	}

	virtual void DeleteItem() override
	{
		AmberEngine::Windowing::Dialogs::MessageBox message("Delete folder", "Deleting a folder (and all its content) is irreversible, are you sure that you want to delete \"" + filePath + "\"?", AmberEngine::Windowing::Dialogs::MessageBox::EMessageType::WARNING, AmberEngine::Windowing::Dialogs::MessageBox::EButtonLayout::YES_NO);

		if (message.GetUserAction() == AmberEngine::Windowing::Dialogs::MessageBox::EUserAction::YES)
		{
			if (std::filesystem::exists(filePath) == true)
			{
				EDITOR_EXEC(PropagateFolderDestruction(filePath));
				std::filesystem::remove_all(filePath);
				DestroyedEvent.Invoke(filePath);
			}
		}
	}

public:
	AmberEngine::Eventing::Event<std::string> ItemAddedEvent;
};


class FileContextualMenu : public BrowserItemContextualMenu
{
public:
	FileContextualMenu(const std::string& p_filePath, bool p_protected = false) : BrowserItemContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& editAction = CreateWidget<AmberEngine::UI::Widgets::MenuItem>("Open");

		editAction.ClickedEvent += [this]
		{
			AmberEngine::Tools::Utils::SystemCalls::OpenFile(filePath);
		};

		if (!m_protected)
		{
			auto& duplicateAction = CreateWidget<AmberEngine::UI::Widgets::MenuItem>("Duplicate");

			duplicateAction.ClickedEvent += [this]
			{
				std::string filePathWithoutExtension = filePath;

				if (size_t pos = filePathWithoutExtension.rfind('.'); pos != std::string::npos)
					filePathWithoutExtension = filePathWithoutExtension.substr(0, pos);

				std::string extension = "." + AmberEngine::Tools::Utils::PathParser::GetExtension(filePath);

				auto filenameAvailable = [&extension](const std::string& target)
				{
					return !std::filesystem::exists(target + extension);
				};

				const auto newNameWithoutExtension = AmberEngine::Tools::Utils::String::GenerateUnique(filePathWithoutExtension, filenameAvailable);

				std::string finalPath = newNameWithoutExtension + extension;
				std::filesystem::copy(filePath, finalPath);

				DuplicateEvent.Invoke(finalPath);
			};
		}

		BrowserItemContextualMenu::CreateList();


		auto& editMetadata = CreateWidget<AmberEngine::UI::Widgets::MenuItem>("Properties");

		editMetadata.ClickedEvent += [this]
		{
			auto& panel = EDITOR_PANEL(AmberEngine::UI::Panels::AssetProperties, "Asset Properties");
			std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
			panel.SetTarget(resourcePath);
			panel.Open();
			panel.Focus();
		};
	}

	virtual void DeleteItem() override
	{
		AmberEngine::Windowing::Dialogs::MessageBox message("Delete file", "Deleting a file is irreversible, are you sure that you want to delete \"" + filePath + "\"?", AmberEngine::Windowing::Dialogs::MessageBox::EMessageType::WARNING, AmberEngine::Windowing::Dialogs::MessageBox::EButtonLayout::YES_NO);

		if (message.GetUserAction() == AmberEngine::Windowing::Dialogs::MessageBox::EUserAction::YES)
		{
			RemoveAsset(filePath);
			DestroyedEvent.Invoke(filePath);
			EDITOR_EXEC(PropagateFileRename(filePath, "?"));
		}
	}

public:
	AmberEngine::Eventing::Event<std::string> DuplicateEvent;
};




template<typename Resource, typename ResourceLoader>
class PreviewableContextualMenu : public FileContextualMenu
{
public:
	PreviewableContextualMenu(const std::string& p_filePath, bool p_protected = false) : FileContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& previewAction = CreateWidget< AmberEngine::UI::Widgets::MenuItem>("Preview");

		previewAction.ClickedEvent += [this]
		{
			Resource* resource = AmberEngine::Tools::Global::ServiceLocator::Get<ResourceLoader>()[EDITOR_EXEC(GetResourcePath(filePath, m_protected))];
			auto& assetView = EDITOR_PANEL(AmberEngine::UI::Panels::AssetView, "Asset View");
			assetView.SetResource(resource);
			assetView.Open();
			assetView.Focus();
		};

		FileContextualMenu::CreateList();
	}
};

class ShaderContextualMenu : public FileContextualMenu
{
public:
	ShaderContextualMenu(const std::string& p_filePath, bool p_protected = false) : FileContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		FileContextualMenu::CreateList();

		auto& compileAction = CreateWidget<AmberEngine::UI::Widgets::MenuItem>("Compile");

		compileAction.ClickedEvent += [this]
		{
			auto& shaderManager = AmberEngine::Tools::Global::ServiceLocator::Get <AmberEngine::ResourceManagement::ShaderManager>();
			std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
			if (shaderManager.IsResourceRegistered(resourcePath))
			{
				/* Trying to recompile */
				AmberEngine::Resources::Loaders::ShaderLoader::Recompile(*shaderManager[resourcePath], filePath);
			}
			else
			{
				/* Trying to compile */
				AmberEngine::Resources::Shader* shader = shaderManager[resourcePath];\

					if (shader)
						AB_LOG_INFO("[COMPILE] \"" + filePath + "\": Success!");
			}
		};
	}
};




class ModelContextualMenu : public PreviewableContextualMenu<AmberEngine::Resources::Model, AmberEngine::ResourceManagement::ModelManager>
{
public:
	ModelContextualMenu(const std::string& p_filePath, bool p_protected = false) : PreviewableContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& reloadAction = CreateWidget<AmberEngine::UI::Widgets::MenuItem>("Reload");

		reloadAction.ClickedEvent += [this]
		{
			auto& modelManager = AmberEngine::Tools::Global::ServiceLocator::Get <AmberEngine::ResourceManagement::ModelManager>();
			std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
			if (modelManager.IsResourceRegistered(resourcePath))
			{
				modelManager.AResourceManager::ReloadResource(resourcePath);
			}
		};

		if (!m_protected)
		{
			auto& generateMaterialsMenu = CreateWidget<AmberEngine::UI::Widgets::MenuList>("Generate materials...");

			generateMaterialsMenu.CreateWidget<AmberEngine::UI::Widgets::MenuItem>("Standard").ClickedEvent += [this]
			{
				auto& modelManager = AmberEngine::Tools::Global::ServiceLocator::Get <AmberEngine::ResourceManagement::ModelManager>();
				std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
				if (auto model = modelManager.GetResource(resourcePath))
				{
					for (const std::string& materialName : model->GetMaterialNames())
					{
						size_t fails = 0;
						std::string finalPath;

						do
						{
							finalPath = AmberEngine::Tools::Utils::PathParser::GetContainingFolder(filePath) + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".abmat";

							++fails;
						} while (std::filesystem::exists(finalPath));

						{
							std::ofstream outfile(finalPath);
							outfile << "<root><shader>:Shaders\\Standard.glsl</shader></root>" << std::endl; // Empty standard material content
						}

						DuplicateEvent.Invoke(finalPath);
					}
				}
			};

			generateMaterialsMenu.CreateWidget<AmberEngine::UI::Widgets::MenuItem>("StandardPBR").ClickedEvent += [this]
			{
				auto& modelManager = AmberEngine::Tools::Global::ServiceLocator::Get <AmberEngine::ResourceManagement::ModelManager>();
				std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
				if (auto model = modelManager.GetResource(resourcePath))
				{
					for (const std::string& materialName : model->GetMaterialNames())
					{
						size_t fails = 0;
						std::string finalPath;

						do
						{
							finalPath = AmberEngine::Tools::Utils::PathParser::GetContainingFolder(filePath) + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".abmat";

							++fails;
						} while (std::filesystem::exists(finalPath));

						{
							std::ofstream outfile(finalPath);
							outfile << "<root><shader>:Shaders\\StandardPBR.glsl</shader></root>" << std::endl; // Empty standard material content
						}

						DuplicateEvent.Invoke(finalPath);
					}
				}
			};

			generateMaterialsMenu.CreateWidget<AmberEngine::UI::Widgets::MenuItem>("Unlit").ClickedEvent += [this]
			{
				auto& modelManager = AmberEngine::Tools::Global::ServiceLocator::Get <AmberEngine::ResourceManagement::ModelManager>();
				std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
				if (auto model = modelManager.GetResource(resourcePath))
				{
					for (const std::string& materialName : model->GetMaterialNames())
					{
						size_t fails = 0;
						std::string finalPath;

						do
						{
							finalPath = AmberEngine::Tools::Utils::PathParser::GetContainingFolder(filePath) + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".abmat";

							++fails;
						} while (std::filesystem::exists(finalPath));

						{
							std::ofstream outfile(finalPath);
							outfile << "<root><shader>:Shaders\\Unlit.glsl</shader></root>" << std::endl; // Empty standard material content
						}

						DuplicateEvent.Invoke(finalPath);
					}
				}
			};

			generateMaterialsMenu.CreateWidget<AmberEngine::UI::Widgets::MenuItem>("Lambert").ClickedEvent += [this]
			{
				auto& modelManager = AmberEngine::Tools::Global::ServiceLocator::Get <AmberEngine::ResourceManagement::ModelManager>();
				std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
				if (auto model = modelManager.GetResource(resourcePath))
				{
					for (const std::string& materialName : model->GetMaterialNames())
					{
						size_t fails = 0;
						std::string finalPath;

						do
						{
							finalPath = AmberEngine::Tools::Utils::PathParser::GetContainingFolder(filePath) + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".abmat";

							++fails;
						} while (std::filesystem::exists(finalPath));

						{
							std::ofstream outfile(finalPath);
							outfile << "<root><shader>:Shaders\\Lambert.glsl</shader></root>" << std::endl; // Empty standard material content
						}

						DuplicateEvent.Invoke(finalPath);
					}
				}
			};
		}

		PreviewableContextualMenu::CreateList();
	}
};


class TextureContextualMenu : public PreviewableContextualMenu<AmberEngine::Resources::Texture, AmberEngine::ResourceManagement::TextureManager>
{
public:
	TextureContextualMenu(const std::string& p_filePath, bool p_protected = false) : PreviewableContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& reloadAction = CreateWidget<AmberEngine::UI::Widgets::MenuItem>("Reload");

		reloadAction.ClickedEvent += [this]
		{
			auto& textureManager = AmberEngine::Tools::Global::ServiceLocator::Get <AmberEngine::ResourceManagement::TextureManager>();
			std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
			if (textureManager.IsResourceRegistered(resourcePath))
			{
				/* Trying to recompile */
				textureManager.AResourceManager::ReloadResource(resourcePath);
				EDITOR_PANEL(AmberEngine::UI::Panels::MaterialEditor, "Material Editor").Refresh();
			}
		};

		PreviewableContextualMenu::CreateList();
	}
};

class SceneContextualMenu : public FileContextualMenu
{
public:
	SceneContextualMenu(const std::string& p_filePath, bool p_protected = false) : FileContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& editAction = CreateWidget<AmberEngine::UI::Widgets::MenuItem>("Edit");

		editAction.ClickedEvent += [this]
		{
			EDITOR_EXEC(LoadSceneFromDisk(EDITOR_EXEC(GetResourcePath(filePath))));
		};

		FileContextualMenu::CreateList();
	}
};

class MaterialContextualMenu : public PreviewableContextualMenu<AmberEngine::Resources::Material, AmberEngine::ResourceManagement::MaterialManager>
{
public:
	MaterialContextualMenu(const std::string& p_filePath, bool p_protected = false) : PreviewableContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& editAction = CreateWidget<AmberEngine::UI::Widgets::MenuItem>("Edit");

		editAction.ClickedEvent += [this]
		{
			AmberEngine::Resources::Material* material = AmberEngine::Tools::Global::ServiceLocator::Get <AmberEngine::ResourceManagement::MaterialManager>()[EDITOR_EXEC(GetResourcePath(filePath, m_protected))];
			if (material)
			{
				
				auto& materialEditor = EDITOR_PANEL(AmberEngine::UI::Panels::MaterialEditor, "Material Editor");
				materialEditor.SetTarget(*material);
				materialEditor.Open();
				materialEditor.Focus();

				AmberEngine::Resources::Material* resource = AmberEngine::Tools::Global::ServiceLocator::Get<AmberEngine::ResourceManagement::MaterialManager>()[EDITOR_EXEC(GetResourcePath(filePath, m_protected))];
				auto& assetView = EDITOR_PANEL(AmberEngine::UI::Panels::AssetView, "Asset View");
				assetView.SetResource(resource);
				assetView.Open();
				assetView.Focus();
			}
		};

		auto& reload = CreateWidget<AmberEngine::UI::Widgets::MenuItem>("Reload");
		reload.ClickedEvent += [this]
		{
			auto materialManager = AmberEngine::Tools::Global::ServiceLocator::Get <AmberEngine::ResourceManagement::MaterialManager>();
			auto resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
			AmberEngine::Resources::Material* material = materialManager[resourcePath];
			if (material)
			{
				materialManager.AResourceManager::ReloadResource(resourcePath);
				EDITOR_PANEL(AmberEngine::UI::Panels::MaterialEditor, "Material Editor").Refresh();
			}
		};

		PreviewableContextualMenu::CreateList();
	}
};

///////////////////////////////////////////////////////////////

AmberEngine::UI::Panels::AssetBrowser::AssetBrowser(const std::string& p_title, bool p_opened,
                                                    const PanelSettings& p_windowSettings, const std::string& p_engineAssetFolder,
                                                    const std::string& p_projectAssetFolder, const std::string& p_projectScriptFolder) :
	APanelWindow(p_title, p_opened, p_windowSettings),
	m_engineAssetFolder(p_engineAssetFolder),
	m_projectAssetFolder(p_projectAssetFolder),
	m_projectScriptFolder(p_projectScriptFolder)
{
	auto& refreshButton = CreateWidget<Widgets::Button>("Rescan assets");
	refreshButton.ClickedEvent += std::bind(&AssetBrowser::Refresh, this);
	refreshButton.lineBreak = false;
	refreshButton.idleBackgroundColor = { 0.f, 0.5f, 0.0f };

	auto& importButton = CreateWidget<Widgets::Button>("Import asset");
	importButton.ClickedEvent += EDITOR_BIND(ImportAsset, m_projectAssetFolder);
	importButton.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };

	m_assetList = &CreateWidget<Widgets::Group>();

	Fill();
}

void AmberEngine::UI::Panels::AssetBrowser::Fill()
{
	m_assetList->CreateWidget<Widgets::Separator>();
	ConsiderItem(nullptr, std::filesystem::directory_entry(m_engineAssetFolder), true);
	m_assetList->CreateWidget<Widgets::Separator>();
	ConsiderItem(nullptr, std::filesystem::directory_entry(m_projectAssetFolder), false);
}

void AmberEngine::UI::Panels::AssetBrowser::Clear()
{
	m_assetList->RemoveAllWidgets();
}

void AmberEngine::UI::Panels::AssetBrowser::Refresh()
{
	Clear();
	Fill();
}

void AmberEngine::UI::Panels::AssetBrowser::ParseFolder(Widgets::TreeNode& p_root,
                                                        const std::filesystem::directory_entry& p_directory, bool p_isEngineItem, bool p_scriptFolder)
{
	/* Iterates another time to display list files */
	for (auto& item : std::filesystem::directory_iterator(p_directory))
		if (item.is_directory())
			ConsiderItem(&p_root, item, p_isEngineItem, false, p_scriptFolder);

	/* Iterates another time to display list files */
	for (auto& item : std::filesystem::directory_iterator(p_directory))
		if (!item.is_directory())
			ConsiderItem(&p_root, item, p_isEngineItem, false, p_scriptFolder);
}

void AmberEngine::UI::Panels::AssetBrowser::ConsiderItem(Widgets::TreeNode* p_root,
	const std::filesystem::directory_entry& p_entry, bool p_isEngineItem, bool p_autoOpen, bool p_scriptFolder)
{
	bool isDirectory = p_entry.is_directory();
	std::string itemname = Tools::Utils::PathParser::GetElementName(p_entry.path().string());
	std::string path = p_entry.path().string();
	if (isDirectory && path.back() != '\\') // Add '\\' if is directory and backslash is missing
		path += '\\';
	std::string resourceFormatPath = EDITOR_EXEC(GetResourcePath(path, p_isEngineItem));
	bool protectedItem = !p_root || p_isEngineItem;

	Tools::Utils::PathParser::EFileType fileType = Tools::Utils::PathParser::GetFileType(itemname);

	// Unknown file, so we skip it
	if (fileType == Tools::Utils::PathParser::EFileType::UNKNOWN && !isDirectory)
	{
		return;
	}

	/* If there is a given treenode (p_root) we attach the new widget to it */
	auto& itemGroup = p_root ? p_root->CreateWidget<Widgets::Group>() : m_assetList->CreateWidget<Widgets::Group>();

	/* Find the icon to apply to the item */
	uint32_t iconTextureID = isDirectory ? EDITOR_CONTEXT(editorResources)->GetTexture("Icon_Folder")->ID : EDITOR_CONTEXT(editorResources)->GetFileIcon(itemname)->ID;

	itemGroup.CreateWidget<Widgets::Image>(iconTextureID, glm::vec2{ 16, 16 }).lineBreak = false;

	/* If the entry is a directory, the content must be a tree node, otherwise (= is a file), a text will suffice */
	if (isDirectory)
	{
		auto& treeNode = itemGroup.CreateWidget<Widgets::TreeNode>(itemname);

		if (p_autoOpen)
			treeNode.Open();

		auto& ddSource = treeNode.AddPlugin<DDSource<std::pair<std::string, Widgets::Group*>>>("Folder", resourceFormatPath, std::make_pair(resourceFormatPath, &itemGroup));

		if (!p_root || p_scriptFolder)
			treeNode.RemoveAllPlugins();

		auto& contextMenu =  treeNode.CreateContextualMenu<FolderContextualMenu>(path, protectedItem && resourceFormatPath != "");
		contextMenu.userData = static_cast<void*>(&treeNode);

		contextMenu.ItemAddedEvent += [this, &treeNode, path, p_isEngineItem, p_scriptFolder](std::string p_string)
		{
			treeNode.Open();
			treeNode.RemoveAllWidgets();
			ParseFolder(treeNode, std::filesystem::directory_entry(Tools::Utils::PathParser::GetContainingFolder(p_string)), p_isEngineItem, p_scriptFolder);
		};

		if (!p_scriptFolder)
		{
			if (!p_isEngineItem) /* Prevent engine item from being DDTarget (Can't Drag and drop to engine folder) */
			{
				treeNode.AddPlugin<DDTarget<std::pair<std::string, Widgets::Group*>>>("Folder").DataReceivedEvent += [this, &treeNode, path, p_isEngineItem](std::pair<std::string, Widgets::Group*> p_data)
				{
					if (!p_data.first.empty())
					{
						std::string folderReceivedPath = EDITOR_EXEC(GetRealPath(p_data.first));

						std::string folderName = Tools::Utils::PathParser::GetElementName(folderReceivedPath) + '\\';
						std::string prevPath = folderReceivedPath;
						std::string correctPath = m_pathUpdate.find(&treeNode) != m_pathUpdate.end() ? m_pathUpdate.at(&treeNode) : path;
						std::string newPath = correctPath + folderName;

						if (!(newPath.find(prevPath) != std::string::npos) || prevPath == newPath)
						{
							if (!std::filesystem::exists(newPath))
							{
								bool isEngineFolder = p_data.first.at(0) == ':';

								if (isEngineFolder) /* Copy dd folder from Engine resources */
									std::filesystem::copy(prevPath, newPath, std::filesystem::copy_options::recursive);
								else
								{
									RenameAsset(prevPath, newPath);
									EDITOR_EXEC(PropagateFolderRename(prevPath, newPath));
								}

								treeNode.Open();
								treeNode.RemoveAllWidgets();
								ParseFolder(treeNode, std::filesystem::directory_entry(correctPath), p_isEngineItem);

								if (!isEngineFolder)
									p_data.second->Destroy();
							}
							else if (prevPath == newPath)
							{
								// Ignore
							}
							else
							{
								Windowing::Dialogs::MessageBox errorMessage("Folder already exists", "You can't move this folder to this location because the name is already taken", Windowing::Dialogs::MessageBox::EMessageType::ERROR, Windowing::Dialogs::MessageBox::EButtonLayout::OK);
							}
						}
						else
						{
							

							Windowing::Dialogs::MessageBox errorMessage("Wow!", "Crazy boy!", Windowing::Dialogs::MessageBox::EMessageType::ERROR, Windowing::Dialogs::MessageBox::EButtonLayout::OK);
						}
					}
				};

				treeNode.AddPlugin<DDTarget<std::pair<std::string, Widgets::Group*>>>("File").DataReceivedEvent += [this, &treeNode, path, p_isEngineItem](std::pair<std::string, Widgets::Group*> p_data)
				{
					if (!p_data.first.empty())
					{
						std::string fileReceivedPath = EDITOR_EXEC(GetRealPath(p_data.first));

						std::string fileName = Tools::Utils::PathParser::GetElementName(fileReceivedPath);
						std::string prevPath = fileReceivedPath;
						std::string correctPath = m_pathUpdate.find(&treeNode) != m_pathUpdate.end() ? m_pathUpdate.at(&treeNode) : path;
						std::string newPath = correctPath + fileName;

						if (!std::filesystem::exists(newPath))
						{
							bool isEngineFile = p_data.first.at(0) == ':';

							if (isEngineFile) /* Copy dd file from Engine resources */
								std::filesystem::copy_file(prevPath, newPath);
							else
							{
								RenameAsset(prevPath, newPath);
								EDITOR_EXEC(PropagateFileRename(prevPath, newPath));
							}

							treeNode.Open();
							treeNode.RemoveAllWidgets();
							ParseFolder(treeNode, std::filesystem::directory_entry(correctPath), p_isEngineItem);

							if (!isEngineFile)
								p_data.second->Destroy();
						}
						else if (prevPath == newPath)
						{
							// Ignore
						}
						else
						{
							Windowing::Dialogs::MessageBox errorMessage("File already exists", "You can't move this file to this location because the name is already taken", Windowing::Dialogs::MessageBox::EMessageType::ERROR, Windowing::Dialogs::MessageBox::EButtonLayout::OK);
						}
					}
				};
			}

			contextMenu.DestroyedEvent += [&itemGroup](std::string p_deletedPath) { itemGroup.Destroy(); };

			contextMenu.RenamedEvent += [this, &treeNode, path, &ddSource, p_isEngineItem](std::string p_prev, std::string p_newPath)
			{
				p_newPath += '\\';

				if (!std::filesystem::exists(p_newPath)) // Do not rename a folder if it already exists
				{
					RenameAsset(p_prev, p_newPath);
					EDITOR_EXEC(PropagateFolderRename(p_prev, p_newPath));
					std::string elementName = Tools::Utils::PathParser::GetElementName(p_newPath);
					std::string data = Tools::Utils::PathParser::GetContainingFolder(ddSource.data.first) + elementName + "\\";
					ddSource.data.first = data;
					ddSource.tooltip = data;
					treeNode.name = elementName;
					treeNode.Open();
					treeNode.RemoveAllWidgets();
					ParseFolder(treeNode, std::filesystem::directory_entry(p_newPath), p_isEngineItem);
					m_pathUpdate[&treeNode] = p_newPath;
				}
				else
				{
					Windowing::Dialogs::MessageBox errorMessage("Folder already exists", "You can't rename this folder because the given name is already taken", Windowing::Dialogs::MessageBox::EMessageType::ERROR, Windowing::Dialogs::MessageBox::EButtonLayout::OK);
				}
			};

			contextMenu.ItemAddedEvent += [this, &treeNode, p_isEngineItem](std::string p_path)
			{
				treeNode.RemoveAllWidgets();
				ParseFolder(treeNode, std::filesystem::directory_entry(Tools::Utils::PathParser::GetContainingFolder(p_path)), p_isEngineItem);
			};
		}

		contextMenu.CreateList();

		treeNode.OpenedEvent += [this, &treeNode, path, p_isEngineItem, p_scriptFolder]
		{
			treeNode.RemoveAllWidgets();
			std::string updatedPath = Tools::Utils::PathParser::GetContainingFolder(path) + treeNode.name;
			ParseFolder(treeNode, std::filesystem::directory_entry(updatedPath), p_isEngineItem, p_scriptFolder);
		};

		treeNode.ClosedEvent += [this, &treeNode]
		{
			treeNode.RemoveAllWidgets();
		};
	}
	else
	{
		auto& clickableText = itemGroup.CreateWidget<Widgets::TextClickable>(itemname);
	
		FileContextualMenu* contextMenu = nullptr;
	
		switch (fileType)
		{
		case Tools::Utils::PathParser::EFileType::MODEL:		contextMenu = &clickableText.CreateContextualMenu<ModelContextualMenu>(path, protectedItem);		break;
		case Tools::Utils::PathParser::EFileType::TEXTURE:	contextMenu = &clickableText.CreateContextualMenu<TextureContextualMenu>(path, protectedItem); 	break;
		case Tools::Utils::PathParser::EFileType::SHADER:		contextMenu = &clickableText.CreateContextualMenu<ShaderContextualMenu>(path, protectedItem);		break;
		case Tools::Utils::PathParser::EFileType::MATERIAL:	contextMenu = &clickableText.CreateContextualMenu<MaterialContextualMenu>(path, protectedItem);	break;
		case Tools::Utils::PathParser::EFileType::SCENE:		contextMenu = &clickableText.CreateContextualMenu<SceneContextualMenu>(path, protectedItem);		break;
		default: contextMenu = &clickableText.CreateContextualMenu<FileContextualMenu>(path, protectedItem); break;
		}
	
		contextMenu->CreateList();
	
		contextMenu->DestroyedEvent += [&itemGroup](std::string p_deletedPath)
		{
			itemGroup.Destroy();
	
			
			if (EDITOR_CONTEXT(sceneManager).GetCurrentSceneSourcePath() == p_deletedPath) // Modify current scene source path if the renamed file is the current scene
				EDITOR_CONTEXT(sceneManager).ForgetCurrentSceneSourcePath();
		};
	
		auto& ddSource = clickableText.AddPlugin<DDSource<std::pair<std::string, Widgets::Group*>>>
			(
				"File",
				resourceFormatPath,
				std::make_pair(resourceFormatPath, &itemGroup)
				);
	
	
		contextMenu->RenamedEvent += [&ddSource, &clickableText, p_scriptFolder](std::string p_prev, std::string p_newPath)
		{
			if (p_newPath != p_prev)
			{
				if (!std::filesystem::exists(p_newPath))
				{
					RenameAsset(p_prev, p_newPath);
					std::string elementName = Tools::Utils::PathParser::GetElementName(p_newPath);
					ddSource.data.first = Tools::Utils::PathParser::GetContainingFolder(ddSource.data.first) + elementName;
	
					if (!p_scriptFolder)
					{
						EDITOR_EXEC(PropagateFileRename(p_prev, p_newPath));
						if (EDITOR_CONTEXT(sceneManager).GetCurrentSceneSourcePath() == p_prev) // Modify current scene source path if the renamed file is the current scene
							EDITOR_CONTEXT(sceneManager).StoreCurrentSceneSourcePath(p_newPath);
					}
	
					clickableText.content = elementName;
				}
				else
				{
					Windowing::Dialogs::MessageBox errorMessage("File already exists", "You can't rename this file because the given name is already taken", Windowing::Dialogs::MessageBox::EMessageType::ERROR, Windowing::Dialogs::MessageBox::EButtonLayout::OK);
				}
			}
		};
	
		contextMenu->DuplicateEvent += [this, &clickableText, p_root, path, p_isEngineItem](std::string newItem)
		{
			EDITOR_EXEC(DelayAction(std::bind(&AssetBrowser::ConsiderItem, this, p_root, std::filesystem::directory_entry{ newItem }, p_isEngineItem, false, false), 0));
		};
	
		if (fileType == Tools::Utils::PathParser::EFileType::TEXTURE)
		{
			auto& texturePreview = clickableText.AddPlugin<TexturePreview>();
			texturePreview.SetPath(resourceFormatPath);
		}
	}
}
