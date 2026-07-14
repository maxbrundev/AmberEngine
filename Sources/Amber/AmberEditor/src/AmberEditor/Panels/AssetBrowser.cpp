#include "Amberpch.h"

#include "AmberEditor/Panels/AssetBrowser.h"

#include "AmberEditor/Core/EditorAction.h"
#include "AmberDebug/Logger.h"
#include "AmberTools/Global/ServiceLocator.h"
#include "AmberTools/Utils/PathParser.h"
#include "AmberTools/Utils/String.h"
#include "AmberTools/Utils/SystemCalls.h"
#include "AmberUI/Widgets/Image.h"
#include "AmberUI/Widgets/InputText.h"
#include "AmberUI/Widgets/Separator.h"
#include "AmberUI/Widgets/TextClickable.h"
#include "AmberUI/Widgets/TextColored.h"
#include "AmberWindowing/Dialogs/MessageBox.h"
#include "AmberRendering/Resources/Loaders/ShaderLoader.h"
#include "AmberEditor/Panels/AssetProperties.h"
#include "AmberEditor/Panels/MaterialEditor.h"
#include "AmberUI/Widgets/ContextualMenuItem.h"
#include "AmberEditor/Panels/AssetView.h"


#define FILENAMES_CHARS AmberEditor::Panels::AssetBrowser::__FILENAMES_CHARS

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


class TexturePreview
{
public:
	void SetPath(const std::string& p_path)
	{
		texture = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::TextureManager>()[p_path];
	}

	void Draw() const
	{
		if (ImGui::IsItemHovered())
		{
			ImTextureID textureID = 0;

			if (texture)
				textureID = reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(texture->ID));

			ImGui::BeginTooltip();
			ImGui::Image(textureID, ImVec2(80.f, 80.f), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
			ImGui::EndTooltip();
		}
	}

	AmberRendering::Resources::Texture* texture = nullptr;
};


class BrowserItemContextualMenu : public AmberUI::Widgets::ContextualMenuItem
{
public:
	BrowserItemContextualMenu(const std::string p_filePath, bool p_protected = false) : m_protected(p_protected), FilePath(p_filePath) {}

	virtual void CreateList()
	{
		if (!m_protected)
		{
			auto& deleteAction = CreateWidget<AmberUI::Widgets::MenuItem>("Delete");
			deleteAction.ClickedEvent += [this] { DeleteItem(); };

			auto& renameMenu = CreateWidget<AmberUI::Widgets::MenuList>("Rename to...");

			auto& nameEditor = renameMenu.CreateWidget<AmberUI::Widgets::InputText>("");
			nameEditor.SelectAllOnClick = true;

			renameMenu.ClickedEvent += [this, &nameEditor]
			{
				nameEditor.Content = AmberTools::Utils::PathParser::GetElementName(FilePath);

				if (!std::filesystem::is_directory(FilePath))
					if (size_t pos = nameEditor.Content.rfind('.'); pos != std::string::npos)
						nameEditor.Content = nameEditor.Content.substr(0, pos);
			};

			nameEditor.EnterPressedEvent += [this](std::string p_newName)
			{
				if (!std::filesystem::is_directory(FilePath))
					p_newName += '.' + AmberTools::Utils::PathParser::GetExtension(FilePath);

				/* Clean the name (Remove special chars) */
				p_newName.erase(std::remove_if(p_newName.begin(), p_newName.end(), [](auto& c)
				{
					return std::find(FILENAMES_CHARS.begin(), FILENAMES_CHARS.end(), c) == FILENAMES_CHARS.end();
				}), p_newName.end());

				std::string containingFolderPath = AmberTools::Utils::PathParser::GetContainingFolder(FilePath);
				std::string newPath = containingFolderPath + p_newName;
				std::string oldPath = FilePath;

				if (FilePath != newPath && !std::filesystem::exists(newPath))
					FilePath = newPath;

				if (std::filesystem::is_directory(oldPath))
					FilePath += '\\';

				RenamedEvent.Invoke(oldPath, newPath);
			};
		}
	}

	virtual void DeleteItem() = 0;

public:
	bool m_protected;
	std::string FilePath;
	AmberTools::Eventing::Event<std::string> DestroyedEvent;
	AmberTools::Eventing::Event<std::string, std::string> RenamedEvent;
	void* UserData = nullptr;
};






class FolderContextualMenu : public BrowserItemContextualMenu
{
public:
	FolderContextualMenu(const std::string& p_filePath, bool p_protected = false) : BrowserItemContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& showInExplorer = CreateWidget<AmberUI::Widgets::MenuItem>("Show in explorer");
		showInExplorer.ClickedEvent += [this]
		{
			AmberTools::Utils::SystemCalls::ShowInExplorer(FilePath);
		};

		if (!m_protected)
		{
			auto& importAssetHere = CreateWidget<AmberUI::Widgets::MenuItem>("Import Here...");
			importAssetHere.ClickedEvent += [this]
			{
				if (EDITOR_EXEC(ImportAssetAtLocation(FilePath)))
				{
					AmberUI::Widgets::TreeNode* pluginOwner = reinterpret_cast<AmberUI::Widgets::TreeNode*>(UserData);
					pluginOwner->Close();
					EDITOR_EXEC(DelayAction(std::bind(&AmberUI::Widgets::TreeNode::Open, pluginOwner)));
				}
			};

			auto& createMenu = CreateWidget<AmberUI::Widgets::MenuList>("Create..");

			auto& createFolderMenu   = createMenu.CreateWidget<AmberUI::Widgets::MenuList>("Folder");
			auto& createSceneMenu    = createMenu.CreateWidget<AmberUI::Widgets::MenuList>("Scene");
			auto& createShaderMenu   = createMenu.CreateWidget<AmberUI::Widgets::MenuList>("Shader");
			auto& createMaterialMenu = createMenu.CreateWidget<AmberUI::Widgets::MenuList>("Material");

			auto& createStandardShaderMenu    = createShaderMenu.CreateWidget<AmberUI::Widgets::MenuList>("Standard template");
			auto& createStandardPBRShaderMenu = createShaderMenu.CreateWidget<AmberUI::Widgets::MenuList>("Standard PBR template");
			auto& createUnlitShaderMenu       = createShaderMenu.CreateWidget<AmberUI::Widgets::MenuList>("Unlit template");
			auto& createLambertShaderMenu     = createShaderMenu.CreateWidget<AmberUI::Widgets::MenuList>("Lambert template");

			auto& createEmptyMaterialMenu       = createMaterialMenu.CreateWidget<AmberUI::Widgets::MenuList>("Empty");
			auto& createStandardMaterialMenu    = createMaterialMenu.CreateWidget<AmberUI::Widgets::MenuList>("Standard");
			auto& createStandardPBRMaterialMenu = createMaterialMenu.CreateWidget<AmberUI::Widgets::MenuList>("Standard PBR");
			auto& createUnlitMaterialMenu       = createMaterialMenu.CreateWidget<AmberUI::Widgets::MenuList>("Unlit");
			auto& createLambertMaterialMenu     = createMaterialMenu.CreateWidget<AmberUI::Widgets::MenuList>("Lambert");

			auto& createFolder = createFolderMenu.CreateWidget<AmberUI::Widgets::InputText>("");
			auto& createScene  =  createSceneMenu.CreateWidget<AmberUI::Widgets::InputText>("");

			auto& createEmptyMaterial       =    createEmptyMaterialMenu.CreateWidget<AmberUI::Widgets::InputText>("");
			auto& createStandardMaterial    = createStandardMaterialMenu.CreateWidget<AmberUI::Widgets::InputText>("");
			auto& createStandardPBRMaterial = createStandardPBRMaterialMenu.CreateWidget<AmberUI::Widgets::InputText>("");
			auto& createUnlitMaterial       =   createUnlitMaterialMenu.CreateWidget<AmberUI::Widgets::InputText>("");
			auto& createLambertMaterial     = createLambertMaterialMenu.CreateWidget<AmberUI::Widgets::InputText>("");

			auto& createStandardShader    =    createStandardShaderMenu.CreateWidget<AmberUI::Widgets::InputText>("");
			auto& createStandardPBRShader = createStandardPBRShaderMenu.CreateWidget<AmberUI::Widgets::InputText>("");
			auto& createUnlitShader       =       createUnlitShaderMenu.CreateWidget<AmberUI::Widgets::InputText>("");
			auto& createLambertShader     =     createLambertShaderMenu.CreateWidget<AmberUI::Widgets::InputText>("");

			createFolderMenu.ClickedEvent += [&createFolder] { createFolder.Content = ""; };
			createSceneMenu.ClickedEvent += [&createScene] { createScene.Content = ""; };
			createStandardShaderMenu.ClickedEvent += [&createStandardShader] { createStandardShader.Content = ""; };
			createStandardPBRShaderMenu.ClickedEvent += [&createStandardPBRShader] { createStandardPBRShader.Content = ""; };
			createUnlitShaderMenu.ClickedEvent += [&createUnlitShader] { createUnlitShader.Content = ""; };
			createLambertShaderMenu.ClickedEvent += [&createLambertShader] { createLambertShader.Content = ""; };
			createEmptyMaterialMenu.ClickedEvent += [&createEmptyMaterial] { createEmptyMaterial.Content = ""; };
			createStandardMaterialMenu.ClickedEvent += [&createStandardMaterial] { createStandardMaterial.Content = ""; };
			createStandardPBRMaterialMenu.ClickedEvent += [&createStandardPBRMaterial] { createStandardPBRMaterial.Content = ""; };
			createUnlitMaterialMenu.ClickedEvent += [&createUnlitMaterial] { createUnlitMaterial.Content = ""; };
			createLambertMaterialMenu.ClickedEvent += [&createLambertMaterial] { createLambertMaterial.Content = ""; };

			createFolder.EnterPressedEvent += [this](std::string newFolderName)
			{
				size_t fails = 0;
				std::string finalPath;

				do
				{
					finalPath = FilePath + (!fails ? newFolderName : newFolderName + " (" + std::to_string(fails) + ')');

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
					finalPath = FilePath + (!fails ? newSceneName : newSceneName + " (" + std::to_string(fails) + ')') + ".abscene";

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
					finalPath = FilePath + '\\' + (!fails ? newShaderName : newShaderName + " (" + std::to_string(fails) + ')') + ".glsl";

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
					finalPath = FilePath + '\\' + (!fails ? newShaderName : newShaderName + " (" + std::to_string(fails) + ')') + ".glsl";

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
					finalPath = FilePath + '\\' + (!fails ? newShaderName : newShaderName + " (" + std::to_string(fails) + ')') + ".glsl";

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
					finalPath = FilePath + '\\' + (!fails ? newShaderName : newShaderName + " (" + std::to_string(fails) + ')') + ".glsl";

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
					finalPath = FilePath + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".abmat";

					++fails;
				} while (std::filesystem::exists(finalPath));

				{
					std::ofstream outfile(finalPath);
					outfile << "<root><shader>?</shader></root>" << std::endl; // Empty material content
				}

				ItemAddedEvent.Invoke(finalPath);

				if (auto instance = EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))])
				{
					auto& materialEditor = EDITOR_PANEL(AmberEditor::Panels::MaterialEditor, "Material Editor");
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
					finalPath = FilePath + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".abmat";

					++fails;
				} while (std::filesystem::exists(finalPath));

				{
					std::ofstream outfile(finalPath);
					outfile << "<root><shader>:Shaders\\Standard.glsl</shader></root>" << std::endl; // Empty standard material content
				}

				ItemAddedEvent.Invoke(finalPath);

				if (auto instance = EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))])
				{
					auto& materialEditor = EDITOR_PANEL(AmberEditor::Panels::MaterialEditor, "Material Editor");
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
					finalPath = FilePath + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".abmat";

					++fails;
				} while (std::filesystem::exists(finalPath));

				{
					std::ofstream outfile(finalPath);
					outfile << "<root><shader>:Shaders\\StandardPBR.glsl</shader></root>" << std::endl; // Empty standard material content
				}

				ItemAddedEvent.Invoke(finalPath);

				if (auto instance = EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))])
				{
					
					auto& materialEditor = EDITOR_PANEL(AmberEditor::Panels::MaterialEditor, "Material Editor");
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
					finalPath = FilePath + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".abmat";

					++fails;
				} while (std::filesystem::exists(finalPath));

				{
					std::ofstream outfile(finalPath);
					outfile << "<root><shader>:Shaders\\Unlit.glsl</shader></root>" << std::endl; // Empty unlit material content
				}

				ItemAddedEvent.Invoke(finalPath);

				if (auto instance = EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))])
				{
			
					auto& materialEditor = EDITOR_PANEL(AmberEditor::Panels::MaterialEditor, "Material Editor");
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
					finalPath = FilePath + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".abmat";

					++fails;
				} while (std::filesystem::exists(finalPath));

				{
					std::ofstream outfile(finalPath);
					outfile << "<root><shader>:Shaders\\Lambert.glsl</shader></root>" << std::endl; // Empty unlit material content
				}

				ItemAddedEvent.Invoke(finalPath);

				if (auto instance = EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))])
				{
					auto& materialEditor = EDITOR_PANEL(AmberEditor::Panels::MaterialEditor, "Material Editor");
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
		AmberWindowing::Dialogs::MessageBox message("Delete folder", "Deleting a folder (and all its content) is irreversible, are you sure that you want to delete \"" + FilePath + "\"?", AmberWindowing::Dialogs::MessageBox::EMessageType::WARNING, AmberWindowing::Dialogs::MessageBox::EButtonLayout::YES_NO);

		if (message.GetUserAction() == AmberWindowing::Dialogs::MessageBox::EUserAction::YES)
		{
			if (std::filesystem::exists(FilePath) == true)
			{
				EDITOR_EXEC(PropagateFolderDestruction(FilePath));
				std::filesystem::remove_all(FilePath);
				DestroyedEvent.Invoke(FilePath);
			}
		}
	}

public:
	AmberTools::Eventing::Event<std::string> ItemAddedEvent;
};

class ScriptFolderContextualMenu : public FolderContextualMenu
{
public:
	ScriptFolderContextualMenu(const std::string& p_filePath, bool p_protected = false) : FolderContextualMenu(p_filePath, p_protected) {}

	void CreateScript(const std::string& p_name, const std::string& p_path)
	{
		std::string fileContent = "local " + p_name + " =\n{\n}\n\nfunction " + p_name + ":OnStart()\nend\n\nfunction " + p_name + ":OnUpdate(deltaTime)\nend\n\nreturn " + p_name;

		std::ofstream outfile(p_path);
		outfile << fileContent << std::endl;

		ItemAddedEvent.Invoke(p_path);
		Close();
	}

	virtual void CreateList() override
	{
		FolderContextualMenu::CreateList();

		auto& newScriptMenu = CreateWidget<AmberUI::Widgets::MenuList>("New script...");
		auto& nameEditor = newScriptMenu.CreateWidget<AmberUI::Widgets::InputText>("");

		newScriptMenu.ClickedEvent += [&nameEditor]
		{
			nameEditor.Content = "";
		};

		nameEditor.EnterPressedEvent += [this](std::string p_newName)
		{
			p_newName.erase(std::remove_if(p_newName.begin(), p_newName.end(), [](char p_char)
			{
				return std::find(FILENAMES_CHARS.begin(), FILENAMES_CHARS.end(), p_char) == FILENAMES_CHARS.end();
			}), p_newName.end());

			const std::string newPath = FilePath + p_newName + ".lua";

			if (!std::filesystem::exists(newPath))
			{
				CreateScript(p_newName, newPath);
			}
		};
	}
};

class FileContextualMenu : public BrowserItemContextualMenu
{
public:
	FileContextualMenu(const std::string& p_filePath, bool p_protected = false) : BrowserItemContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& editAction = CreateWidget<AmberUI::Widgets::MenuItem>("Open");

		editAction.ClickedEvent += [this]
		{
			AmberTools::Utils::SystemCalls::OpenFile(FilePath);
		};

		if (!m_protected)
		{
			auto& duplicateAction = CreateWidget<AmberUI::Widgets::MenuItem>("Duplicate");

			duplicateAction.ClickedEvent += [this]
			{
				std::string filePathWithoutExtension = FilePath;

				if (size_t pos = filePathWithoutExtension.rfind('.'); pos != std::string::npos)
					filePathWithoutExtension = filePathWithoutExtension.substr(0, pos);

				std::string extension = "." + AmberTools::Utils::PathParser::GetExtension(FilePath);

				auto filenameAvailable = [&extension](const std::string& target)
				{
					return !std::filesystem::exists(target + extension);
				};

				const auto newNameWithoutExtension = AmberTools::Utils::String::GenerateUnique(filePathWithoutExtension, filenameAvailable);

				std::string finalPath = newNameWithoutExtension + extension;
				std::filesystem::copy(FilePath, finalPath);

				DuplicateEvent.Invoke(finalPath);
			};
		}

		BrowserItemContextualMenu::CreateList();


		auto& editMetadata = CreateWidget<AmberUI::Widgets::MenuItem>("Properties");

		editMetadata.ClickedEvent += [this]
		{
			auto& panel = EDITOR_PANEL(AmberEditor::Panels::AssetProperties, "Asset Properties");
			std::string resourcePath = EDITOR_EXEC(GetResourcePath(FilePath, m_protected));
			panel.SetTarget(resourcePath);
			panel.Open();
			panel.Focus();
		};
	}

	virtual void DeleteItem() override
	{
		AmberWindowing::Dialogs::MessageBox message("Delete file", "Deleting a file is irreversible, are you sure that you want to delete \"" + FilePath + "\"?", AmberWindowing::Dialogs::MessageBox::EMessageType::WARNING, AmberWindowing::Dialogs::MessageBox::EButtonLayout::YES_NO);

		if (message.GetUserAction() == AmberWindowing::Dialogs::MessageBox::EUserAction::YES)
		{
			RemoveAsset(FilePath);
			DestroyedEvent.Invoke(FilePath);
			EDITOR_EXEC(PropagateFileRename(FilePath, "?"));
		}
	}

public:
	AmberTools::Eventing::Event<std::string> DuplicateEvent;
};




template<typename Resource, typename ResourceLoader>
class PreviewableContextualMenu : public FileContextualMenu
{
public:
	PreviewableContextualMenu(const std::string& p_filePath, bool p_protected = false) : FileContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& previewAction = CreateWidget< AmberUI::Widgets::MenuItem>("Preview");

		previewAction.ClickedEvent += [this]
		{
			Resource* resource = AmberTools::Global::ServiceLocator::Get<ResourceLoader>()[EDITOR_EXEC(GetResourcePath(FilePath, m_protected))];
			auto& assetView = EDITOR_PANEL(AmberEditor::Panels::AssetView, "Asset View");
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

		auto& compileAction = CreateWidget<AmberUI::Widgets::MenuItem>("Compile");

		compileAction.ClickedEvent += [this]
		{
			auto& shaderManager = AmberTools::Global::ServiceLocator::Get <AmberCore::ResourceManagement::ShaderManager>();
			std::string resourcePath = EDITOR_EXEC(GetResourcePath(FilePath, m_protected));
			if (shaderManager.IsResourceRegistered(resourcePath))
			{
				/* Trying to recompile */
				AmberRendering::Resources::Loaders::ShaderLoader::Recompile(*shaderManager[resourcePath], FilePath);
			}
			else
			{
				/* Trying to compile */
				AmberRendering::Resources::Shader* shader = shaderManager[resourcePath];\

					if (shader)
						AB_LOG_INFO("[COMPILE] \"" + FilePath + "\": Success!");
			}
		};
	}
};




class ModelContextualMenu : public PreviewableContextualMenu<AmberRendering::Resources::Model, AmberCore::ResourceManagement::ModelManager>
{
public:
	ModelContextualMenu(const std::string& p_filePath, bool p_protected = false) : PreviewableContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& reloadAction = CreateWidget<AmberUI::Widgets::MenuItem>("Reload");

		reloadAction.ClickedEvent += [this]
		{
			auto& modelManager = AmberTools::Global::ServiceLocator::Get <AmberCore::ResourceManagement::ModelManager>();
			std::string resourcePath = EDITOR_EXEC(GetResourcePath(FilePath, m_protected));
			if (modelManager.IsResourceRegistered(resourcePath))
			{
				modelManager.AResourceManager::ReloadResource(resourcePath);
			}
		};

		if (!m_protected)
		{
			auto& generateMaterialsMenu = CreateWidget<AmberUI::Widgets::MenuList>("Generate materials...");

			generateMaterialsMenu.CreateWidget<AmberUI::Widgets::MenuItem>("Standard").ClickedEvent += [this]
			{
				auto& modelManager = AmberTools::Global::ServiceLocator::Get <AmberCore::ResourceManagement::ModelManager>();
				std::string resourcePath = EDITOR_EXEC(GetResourcePath(FilePath, m_protected));
				if (auto model = modelManager.GetResource(resourcePath))
				{
					for (const std::string& materialName : model->GetMaterialNames())
					{
						size_t fails = 0;
						std::string finalPath;

						do
						{
							finalPath = AmberTools::Utils::PathParser::GetContainingFolder(FilePath) + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".abmat";

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

			generateMaterialsMenu.CreateWidget<AmberUI::Widgets::MenuItem>("StandardPBR").ClickedEvent += [this]
			{
				auto& modelManager = AmberTools::Global::ServiceLocator::Get <AmberCore::ResourceManagement::ModelManager>();
				std::string resourcePath = EDITOR_EXEC(GetResourcePath(FilePath, m_protected));
				if (auto model = modelManager.GetResource(resourcePath))
				{
					for (const std::string& materialName : model->GetMaterialNames())
					{
						size_t fails = 0;
						std::string finalPath;

						do
						{
							finalPath = AmberTools::Utils::PathParser::GetContainingFolder(FilePath) + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".abmat";

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

			generateMaterialsMenu.CreateWidget<AmberUI::Widgets::MenuItem>("Unlit").ClickedEvent += [this]
			{
				auto& modelManager = AmberTools::Global::ServiceLocator::Get <AmberCore::ResourceManagement::ModelManager>();
				std::string resourcePath = EDITOR_EXEC(GetResourcePath(FilePath, m_protected));
				if (auto model = modelManager.GetResource(resourcePath))
				{
					for (const std::string& materialName : model->GetMaterialNames())
					{
						size_t fails = 0;
						std::string finalPath;

						do
						{
							finalPath = AmberTools::Utils::PathParser::GetContainingFolder(FilePath) + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".abmat";

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

			generateMaterialsMenu.CreateWidget<AmberUI::Widgets::MenuItem>("Lambert").ClickedEvent += [this]
			{
				auto& modelManager = AmberTools::Global::ServiceLocator::Get <AmberCore::ResourceManagement::ModelManager>();
				std::string resourcePath = EDITOR_EXEC(GetResourcePath(FilePath, m_protected));
				if (auto model = modelManager.GetResource(resourcePath))
				{
					for (const std::string& materialName : model->GetMaterialNames())
					{
						size_t fails = 0;
						std::string finalPath;

						do
						{
							finalPath = AmberTools::Utils::PathParser::GetContainingFolder(FilePath) + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".abmat";

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


class TextureContextualMenu : public PreviewableContextualMenu<AmberRendering::Resources::Texture, AmberCore::ResourceManagement::TextureManager>
{
public:
	TextureContextualMenu(const std::string& p_filePath, bool p_protected = false) : PreviewableContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& reloadAction = CreateWidget<AmberUI::Widgets::MenuItem>("Reload");

		reloadAction.ClickedEvent += [this]
		{
			auto& textureManager = AmberTools::Global::ServiceLocator::Get <AmberCore::ResourceManagement::TextureManager>();
			std::string resourcePath = EDITOR_EXEC(GetResourcePath(FilePath, m_protected));
			if (textureManager.IsResourceRegistered(resourcePath))
			{
				/* Trying to recompile */
				textureManager.AResourceManager::ReloadResource(resourcePath);
				EDITOR_PANEL(AmberEditor::Panels::MaterialEditor, "Material Editor").Refresh();
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
		auto& editAction = CreateWidget<AmberUI::Widgets::MenuItem>("Edit");

		editAction.ClickedEvent += [this]
		{
			EDITOR_EXEC(LoadSceneFromDisk(EDITOR_EXEC(GetResourcePath(FilePath))));
		};

		FileContextualMenu::CreateList();
	}
};

class MaterialContextualMenu : public PreviewableContextualMenu<AmberCore::Resources::Material, AmberCore::ResourceManagement::MaterialManager>
{
public:
	MaterialContextualMenu(const std::string& p_filePath, bool p_protected = false) : PreviewableContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& editAction = CreateWidget<AmberUI::Widgets::MenuItem>("Edit");

		editAction.ClickedEvent += [this]
		{
			AmberCore::Resources::Material* material = AmberTools::Global::ServiceLocator::Get <AmberCore::ResourceManagement::MaterialManager>()[EDITOR_EXEC(GetResourcePath(FilePath, m_protected))];
			if (material)
			{
				
				auto& materialEditor = EDITOR_PANEL(AmberEditor::Panels::MaterialEditor, "Material Editor");
				materialEditor.SetTarget(*material);
				materialEditor.Open();
				materialEditor.Focus();

				AmberCore::Resources::Material* resource = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::MaterialManager>()[EDITOR_EXEC(GetResourcePath(FilePath, m_protected))];
				auto& assetView = EDITOR_PANEL(AmberEditor::Panels::AssetView, "Asset View");
				assetView.SetResource(resource);
				assetView.Open();
				assetView.Focus();
			}
		};

		auto& reload = CreateWidget<AmberUI::Widgets::MenuItem>("Reload");
		reload.ClickedEvent += [this]
		{
			auto materialManager = AmberTools::Global::ServiceLocator::Get <AmberCore::ResourceManagement::MaterialManager>();
			auto resourcePath = EDITOR_EXEC(GetResourcePath(FilePath, m_protected));
			AmberCore::Resources::Material* material = materialManager[resourcePath];
			if (material)
			{
				materialManager.AResourceManager::ReloadResource(resourcePath);
				EDITOR_PANEL(AmberEditor::Panels::MaterialEditor, "Material Editor").Refresh();
			}
		};

		PreviewableContextualMenu::CreateList();
	}
};

///////////////////////////////////////////////////////////////

AmberEditor::Panels::AssetBrowser::AssetBrowser(const std::string& p_title, bool p_opened,
                                                    const AmberUI::Panels::PanelSettings& p_windowSettings, const std::string& p_engineAssetFolder,
                                                    const std::string& p_projectAssetFolder, const std::string& p_projectScriptFolder) :
	AmberUI::Panels::APanelWindow(p_title, p_opened, p_windowSettings),
	m_engineAssetFolder(p_engineAssetFolder),
	m_projectAssetFolder(p_projectAssetFolder),
	m_projectScriptFolder(p_projectScriptFolder)
{
	auto& refreshButton = CreateWidget<AmberUI::Widgets::Button>("Rescan assets");
	refreshButton.ClickedEvent += std::bind(&AssetBrowser::Refresh, this);
	refreshButton.LineBreak = false;
	refreshButton.IdleBackgroundColor = { 0.f, 0.5f, 0.0f };

	auto& importButton = CreateWidget<AmberUI::Widgets::Button>("Import asset");
	importButton.ClickedEvent += EDITOR_BIND(ImportAsset, m_projectAssetFolder);
	importButton.IdleBackgroundColor = { 0.7f, 0.5f, 0.0f };

	m_assetList = &CreateWidget<AmberUI::Widgets::Group>();

	if (!std::filesystem::exists(m_projectScriptFolder))
	{
		std::filesystem::create_directories(m_projectScriptFolder);
	}

	Fill();
}

void AmberEditor::Panels::AssetBrowser::Fill()
{
	m_assetList->CreateWidget<AmberUI::Widgets::Separator>();
	ConsiderItem(nullptr, std::filesystem::directory_entry(m_engineAssetFolder), true);
	m_assetList->CreateWidget<AmberUI::Widgets::Separator>();
	ConsiderItem(nullptr, std::filesystem::directory_entry(m_projectAssetFolder), false);
	m_assetList->CreateWidget<AmberUI::Widgets::Separator>();
	ConsiderItem(nullptr, std::filesystem::directory_entry(m_projectScriptFolder), false, false, true);
}

void AmberEditor::Panels::AssetBrowser::Clear()
{
	m_assetList->RemoveAllWidgets();
}

void AmberEditor::Panels::AssetBrowser::Refresh()
{
	Clear();
	Fill();
}

void AmberEditor::Panels::AssetBrowser::ParseFolder(AmberUI::Widgets::TreeNode& p_root,
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

void AmberEditor::Panels::AssetBrowser::ConsiderItem(AmberUI::Widgets::TreeNode* p_root,
	const std::filesystem::directory_entry& p_entry, bool p_isEngineItem, bool p_autoOpen, bool p_scriptFolder)
{
	bool isDirectory = p_entry.is_directory();
	std::string itemname = AmberTools::Utils::PathParser::GetElementName(p_entry.path().string());
	std::string path = p_entry.path().string();
	if (isDirectory && path.back() != '\\') // Add '\\' if is directory and backslash is missing
		path += '\\';
	std::string resourceFormatPath = EDITOR_EXEC(GetResourcePath(path, p_isEngineItem));
	bool protectedItem = !p_root || p_isEngineItem;

	AmberTools::Utils::PathParser::EFileType fileType = AmberTools::Utils::PathParser::GetFileType(itemname);

	// Unknown file, so we skip it
	if (fileType == AmberTools::Utils::PathParser::EFileType::UNKNOWN && !isDirectory)
	{
		return;
	}

	/* If there is a given treenode (p_root) we attach the new widget to it */
	auto& itemGroup = p_root ? p_root->CreateWidget<AmberUI::Widgets::Group>() : m_assetList->CreateWidget<AmberUI::Widgets::Group>();

	/* Find the icon to apply to the item */
	uint32_t iconTextureID = isDirectory ? EDITOR_CONTEXT(editorResources)->GetTexture("Icon_Folder")->ID : EDITOR_CONTEXT(editorResources)->GetFileIcon(itemname)->ID;

	itemGroup.CreateWidget<AmberUI::Widgets::Image>(iconTextureID, glm::vec2{ 16, 16 }).LineBreak = false;

	/* If the entry is a directory, the content must be a tree node, otherwise (= is a file), a text will suffice */
	if (isDirectory)
	{
		auto& treeNode = itemGroup.CreateWidget<AmberUI::Widgets::TreeNode>(itemname);

		if (p_autoOpen)
			treeNode.Open();

		auto& ddSource = treeNode.SetDragDropSource("Folder", resourceFormatPath, std::make_pair(resourceFormatPath, &itemGroup));

		if (!p_root || p_scriptFolder)
			treeNode.ClearDragDrop();

		FolderContextualMenu& contextMenu = !p_scriptFolder ? treeNode.CreateContextualMenu<FolderContextualMenu>(path, protectedItem && resourceFormatPath != "") : treeNode.CreateContextualMenu<ScriptFolderContextualMenu>(path, protectedItem && resourceFormatPath != "");
		contextMenu.UserData = static_cast<void*>(&treeNode);

		contextMenu.ItemAddedEvent += [this, &treeNode, path, p_isEngineItem, p_scriptFolder](std::string p_string)
		{
			treeNode.Open();
			treeNode.RemoveAllWidgets();
			ParseFolder(treeNode, std::filesystem::directory_entry(AmberTools::Utils::PathParser::GetContainingFolder(p_string)), p_isEngineItem, p_scriptFolder);
		};

		if (!p_scriptFolder)
		{
			if (!p_isEngineItem) /* Prevent engine item from being a drag and drop target (can not drop into engine folder) */
			{
				treeNode.AddDragDropTarget("Folder").DataReceivedEvent += [this, &treeNode, path, p_isEngineItem](std::pair<std::string, AmberUI::Widgets::Group*> p_data)
				{
					if (!p_data.first.empty())
					{
						std::string folderReceivedPath = EDITOR_EXEC(GetRealPath(p_data.first));

						std::string folderName = AmberTools::Utils::PathParser::GetElementName(folderReceivedPath) + '\\';
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
								AmberWindowing::Dialogs::MessageBox errorMessage("Folder already exists", "You can't move this folder to this location because the name is already taken", AmberWindowing::Dialogs::MessageBox::EMessageType::ERROR, AmberWindowing::Dialogs::MessageBox::EButtonLayout::OK);
							}
						}
						else
						{
							

							AmberWindowing::Dialogs::MessageBox errorMessage("Wow!", "Crazy boy!", AmberWindowing::Dialogs::MessageBox::EMessageType::ERROR, AmberWindowing::Dialogs::MessageBox::EButtonLayout::OK);
						}
					}
				};

				treeNode.AddDragDropTarget("File").DataReceivedEvent += [this, &treeNode, path, p_isEngineItem](std::pair<std::string, AmberUI::Widgets::Group*> p_data)
				{
					if (!p_data.first.empty())
					{
						std::string fileReceivedPath = EDITOR_EXEC(GetRealPath(p_data.first));

						std::string fileName = AmberTools::Utils::PathParser::GetElementName(fileReceivedPath);
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
							AmberWindowing::Dialogs::MessageBox errorMessage("File already exists", "You can't move this file to this location because the name is already taken", AmberWindowing::Dialogs::MessageBox::EMessageType::ERROR, AmberWindowing::Dialogs::MessageBox::EButtonLayout::OK);
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
					std::string elementName = AmberTools::Utils::PathParser::GetElementName(p_newPath);
					std::string data = AmberTools::Utils::PathParser::GetContainingFolder(ddSource.Data.first) + elementName + "\\";
					ddSource.Data.first = data;
					ddSource.Tooltip = data;
					treeNode.Name = elementName;
					treeNode.Open();
					treeNode.RemoveAllWidgets();
					ParseFolder(treeNode, std::filesystem::directory_entry(p_newPath), p_isEngineItem);
					m_pathUpdate[&treeNode] = p_newPath;
				}
				else
				{
					AmberWindowing::Dialogs::MessageBox errorMessage("Folder already exists", "You can't rename this folder because the given name is already taken", AmberWindowing::Dialogs::MessageBox::EMessageType::ERROR, AmberWindowing::Dialogs::MessageBox::EButtonLayout::OK);
				}
			};

			contextMenu.ItemAddedEvent += [this, &treeNode, p_isEngineItem](std::string p_path)
			{
				treeNode.RemoveAllWidgets();
				ParseFolder(treeNode, std::filesystem::directory_entry(AmberTools::Utils::PathParser::GetContainingFolder(p_path)), p_isEngineItem);
			};
		}

		contextMenu.CreateList();

		treeNode.OpenedEvent += [this, &treeNode, path, p_isEngineItem, p_scriptFolder]
		{
			treeNode.RemoveAllWidgets();
			std::string updatedPath = AmberTools::Utils::PathParser::GetContainingFolder(path) + treeNode.Name;
			ParseFolder(treeNode, std::filesystem::directory_entry(updatedPath), p_isEngineItem, p_scriptFolder);
		};

		treeNode.ClosedEvent += [this, &treeNode]
		{
			treeNode.RemoveAllWidgets();
		};
	}
	else
	{
		auto& clickableText = itemGroup.CreateWidget<AmberUI::Widgets::TextClickable>(itemname);
	
		FileContextualMenu* contextMenu = nullptr;
	
		switch (fileType)
		{
		case AmberTools::Utils::PathParser::EFileType::MODEL:		contextMenu = &clickableText.CreateContextualMenu<ModelContextualMenu>(path, protectedItem);		break;
		case AmberTools::Utils::PathParser::EFileType::TEXTURE:	contextMenu = &clickableText.CreateContextualMenu<TextureContextualMenu>(path, protectedItem); 	break;
		case AmberTools::Utils::PathParser::EFileType::SHADER:		contextMenu = &clickableText.CreateContextualMenu<ShaderContextualMenu>(path, protectedItem);		break;
		case AmberTools::Utils::PathParser::EFileType::MATERIAL:	contextMenu = &clickableText.CreateContextualMenu<MaterialContextualMenu>(path, protectedItem);	break;
		case AmberTools::Utils::PathParser::EFileType::SCENE:		contextMenu = &clickableText.CreateContextualMenu<SceneContextualMenu>(path, protectedItem);		break;
		default: contextMenu = &clickableText.CreateContextualMenu<FileContextualMenu>(path, protectedItem); break;
		}
	
		contextMenu->CreateList();
	
		contextMenu->DestroyedEvent += [&itemGroup](std::string p_deletedPath)
		{
			itemGroup.Destroy();
	
			
			if (EDITOR_CONTEXT(sceneManager).GetCurrentSceneSourcePath() == p_deletedPath) // Modify current scene source path if the renamed file is the current scene
				EDITOR_CONTEXT(sceneManager).ForgetCurrentSceneSourcePath();
		};
	
		auto& ddSource = clickableText.SetDragDropSource(
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
					std::string elementName = AmberTools::Utils::PathParser::GetElementName(p_newPath);
					ddSource.Data.first = AmberTools::Utils::PathParser::GetContainingFolder(ddSource.Data.first) + elementName;
	
					if (!p_scriptFolder)
					{
						EDITOR_EXEC(PropagateFileRename(p_prev, p_newPath));
						if (EDITOR_CONTEXT(sceneManager).GetCurrentSceneSourcePath() == p_prev) // Modify current scene source path if the renamed file is the current scene
							EDITOR_CONTEXT(sceneManager).StoreCurrentSceneSourcePath(p_newPath);
					}
					else
					{
						EDITOR_EXEC(PropagateScriptRename(p_prev, p_newPath));
					}
	
					clickableText.Content = elementName;
				}
				else
				{
					AmberWindowing::Dialogs::MessageBox errorMessage("File already exists", "You can't rename this file because the given name is already taken", AmberWindowing::Dialogs::MessageBox::EMessageType::ERROR, AmberWindowing::Dialogs::MessageBox::EButtonLayout::OK);
				}
			}
		};
	
		contextMenu->DuplicateEvent += [this, &clickableText, p_root, path, p_isEngineItem](std::string newItem)
		{
			EDITOR_EXEC(DelayAction(std::bind(&AssetBrowser::ConsiderItem, this, p_root, std::filesystem::directory_entry{ newItem }, p_isEngineItem, false, false), 0));
		};
	
		if (fileType == AmberTools::Utils::PathParser::EFileType::TEXTURE)
		{
			TexturePreview texturePreview;
			texturePreview.SetPath(resourceFormatPath);

			clickableText.DrawnEvent += [texturePreview]
			{
				texturePreview.Draw();
			};
		}
	}
}
