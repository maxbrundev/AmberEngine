#include "Amberpch.h"

#include "AmberEngine/Resources/Loaders/MaterialLoader.h"

AmberEngine::Resources::Material* AmberEngine::Resources::Loaders::MaterialLoader::Create(const std::string& p_path)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(p_path.c_str());
	if (!doc.Error())
	{
		tinyxml2::XMLNode* root = doc.FirstChild();

		Material* material = new Material();

		material->OnDeserialize(doc, root);

		return material;
	}
	else
	{
		return nullptr;
	}
}

void AmberEngine::Resources::Loaders::MaterialLoader::Reload(Material& p_material, const std::string& p_path)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(p_path.c_str());
	if (!doc.Error())
	{
		tinyxml2::XMLNode* root = doc.FirstChild();
		p_material.OnDeserialize(doc, root);
	}
}

void AmberEngine::Resources::Loaders::MaterialLoader::Save(Material& p_material, const std::string& p_path)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLNode* node = doc.NewElement("root");
	doc.InsertFirstChild(node);

	p_material.OnSerialize(doc, node);

	doc.SaveFile(p_path.c_str());
}

bool AmberEngine::Resources::Loaders::MaterialLoader::Destroy(Material*& p_material)
{
	if (p_material)
	{
		delete p_material;
		p_material = nullptr;

		return true;
	}

	return false;
}
