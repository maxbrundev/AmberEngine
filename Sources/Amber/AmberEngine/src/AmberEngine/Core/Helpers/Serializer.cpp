#include "Amberpch.h"

#include "AmberEngine/Core/Helpers/Serializer.h"

#include "AmberEngine/Managers/TextureManager.h"
#include "AmberEngine/Managers/ModelManager.h"
#include "AmberEngine/Managers/ShaderManager.h"
#include "AmberEngine/Managers/MaterialManager.h"

#include "AmberEngine/Tools/Global/ServiceLocator.h"

void AmberEngine::Core::Helpers::Serializer::SerializeBoolean(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, bool p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void AmberEngine::Core::Helpers::Serializer::SerializeString(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, const std::string & p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value.c_str());
	p_node->InsertEndChild(element);
}

void AmberEngine::Core::Helpers::Serializer::SerializeFloat(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, float p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void AmberEngine::Core::Helpers::Serializer::SerializeDouble(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, double p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void AmberEngine::Core::Helpers::Serializer::SerializeInt(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, int p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void AmberEngine::Core::Helpers::Serializer::SerializeUint(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, unsigned p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void AmberEngine::Core::Helpers::Serializer::SerializeInt64(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, int64_t p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void AmberEngine::Core::Helpers::Serializer::SerializeVec2(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, const glm::vec2 & p_value)
{
	tinyxml2::XMLNode* element = p_doc.NewElement(p_name.c_str());
	p_node->InsertEndChild(element);

	tinyxml2::XMLElement* x = p_doc.NewElement("x");
	x->SetText(p_value.x);
	element->InsertEndChild(x);

	tinyxml2::XMLElement* y = p_doc.NewElement("y");
	y->SetText(p_value.y);
	element->InsertEndChild(y);
}

void AmberEngine::Core::Helpers::Serializer::SerializeVec3(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, const glm::vec3 & p_value)
{
	tinyxml2::XMLNode* element = p_doc.NewElement(p_name.c_str());
	p_node->InsertEndChild(element);

	tinyxml2::XMLElement* x = p_doc.NewElement("x");
	x->SetText(p_value.x);
	element->InsertEndChild(x);

	tinyxml2::XMLElement* y = p_doc.NewElement("y");
	y->SetText(p_value.y);
	element->InsertEndChild(y);

	tinyxml2::XMLElement* z = p_doc.NewElement("z");
	z->SetText(p_value.z);
	element->InsertEndChild(z);
}

void AmberEngine::Core::Helpers::Serializer::SerializeVec4(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, const glm::vec4 & p_value)
{
	tinyxml2::XMLNode* element = p_doc.NewElement(p_name.c_str());
	p_node->InsertEndChild(element);

	tinyxml2::XMLElement* x = p_doc.NewElement("x");
	x->SetText(p_value.x);
	element->InsertEndChild(x);

	tinyxml2::XMLElement* y = p_doc.NewElement("y");
	y->SetText(p_value.y);
	element->InsertEndChild(y);

	tinyxml2::XMLElement* z = p_doc.NewElement("z");
	z->SetText(p_value.z);
	element->InsertEndChild(z);

	tinyxml2::XMLElement* w = p_doc.NewElement("w");
	w->SetText(p_value.w);
	element->InsertEndChild(w);
}

void AmberEngine::Core::Helpers::Serializer::SerializeQuat(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, const glm::quat & p_value)
{
	tinyxml2::XMLNode* element = p_doc.NewElement(p_name.c_str());
	p_node->InsertEndChild(element);

	tinyxml2::XMLElement* x = p_doc.NewElement("x");
	x->SetText(p_value.x);
	element->InsertEndChild(x);

	tinyxml2::XMLElement* y = p_doc.NewElement("y");
	y->SetText(p_value.y);
	element->InsertEndChild(y);

	tinyxml2::XMLElement* z = p_doc.NewElement("z");
	z->SetText(p_value.z);
	element->InsertEndChild(z);

	tinyxml2::XMLElement* w = p_doc.NewElement("w");
	w->SetText(p_value.w);
	element->InsertEndChild(w);
}

void AmberEngine::Core::Helpers::Serializer::SerializeColor(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, const AmberEngine::Data::Color & p_value)
{
	tinyxml2::XMLNode* element = p_doc.NewElement(p_name.c_str());
	p_node->InsertEndChild(element);

	tinyxml2::XMLElement* r = p_doc.NewElement("r");
	r->SetText(p_value.r);
	element->InsertEndChild(r);

	tinyxml2::XMLElement* g = p_doc.NewElement("g");
	g->SetText(p_value.g);
	element->InsertEndChild(g);

	tinyxml2::XMLElement* b = p_doc.NewElement("b");
	b->SetText(p_value.b);
	element->InsertEndChild(b);

	tinyxml2::XMLElement* a = p_doc.NewElement("a");
	a->SetText(p_value.a);
	element->InsertEndChild(a);
}

void AmberEngine::Core::Helpers::Serializer::SerializeModel(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, AmberEngine::Resources::Model * p_value)
{
	SerializeString(p_doc, p_node, p_name.c_str(), p_value ? p_value->Path : "?");
}

void AmberEngine::Core::Helpers::Serializer::SerializeTexture(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, AmberEngine::Resources::Texture * p_value)
{
	SerializeString(p_doc, p_node, p_name.c_str(), p_value ? p_value->Path : "?");
}

void AmberEngine::Core::Helpers::Serializer::SerializeShader(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, AmberEngine::Resources::Shader * p_value)
{
	SerializeString(p_doc, p_node, p_name.c_str(), p_value ? p_value->Path : "?");
}

void AmberEngine::Core::Helpers::Serializer::DeserializeBoolean(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, bool & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryBoolText(&p_out);
}

void AmberEngine::Core::Helpers::Serializer::SerializeMaterial(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, AmberEngine::Resources::Material * p_value)
{
	SerializeString(p_doc, p_node, p_name.c_str(), p_value ? p_value->path : "?");
}

void AmberEngine::Core::Helpers::Serializer::DeserializeString(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, std::string & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
	{
		const char* result = element->GetText();
		p_out = result ? result : "";
	}
}

void AmberEngine::Core::Helpers::Serializer::DeserializeFloat(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, float & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryFloatText(&p_out);
}

void AmberEngine::Core::Helpers::Serializer::DeserializeDouble(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, double & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryDoubleText(&p_out);
}

void AmberEngine::Core::Helpers::Serializer::DeserializeInt(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, int & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryIntText(&p_out);
}

void AmberEngine::Core::Helpers::Serializer::DeserializeUint(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, unsigned & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryUnsignedText(&p_out);
}

void AmberEngine::Core::Helpers::Serializer::DeserializeInt64(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, int64_t & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryInt64Text(&p_out);
}

void AmberEngine::Core::Helpers::Serializer::DeserializeVec2(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, glm::vec2 & p_out)
{
	if (auto node = p_node->FirstChildElement(p_name.c_str()); node)
	{
		if (auto element = node->FirstChildElement("x"); element)
			element->QueryFloatText(&p_out.x);

		if (auto element = node->FirstChildElement("y"); element)
			element->QueryFloatText(&p_out.y);
	}
}

void AmberEngine::Core::Helpers::Serializer::DeserializeVec3(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, glm::vec3 & p_out)
{
	if (auto node = p_node->FirstChildElement(p_name.c_str()); node)
	{
		if (auto element = node->FirstChildElement("x"); element)
			element->QueryFloatText(&p_out.x);

		if (auto element = node->FirstChildElement("y"); element)
			element->QueryFloatText(&p_out.y);

		if (auto element = node->FirstChildElement("z"); element)
			element->QueryFloatText(&p_out.z);
	}
}

void AmberEngine::Core::Helpers::Serializer::DeserializeVec4(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, glm::vec4 & p_out)
{
	if (auto node = p_node->FirstChildElement(p_name.c_str()); node)
	{
		if (auto element = node->FirstChildElement("x"); element)
			element->QueryFloatText(&p_out.x);

		if (auto element = node->FirstChildElement("y"); element)
			element->QueryFloatText(&p_out.y);

		if (auto element = node->FirstChildElement("z"); element)
			element->QueryFloatText(&p_out.z);

		if (auto element = node->FirstChildElement("w"); element)
			element->QueryFloatText(&p_out.w);
	}
}

void AmberEngine::Core::Helpers::Serializer::DeserializeQuat(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, glm::quat & p_out)
{
	if (auto node = p_node->FirstChildElement(p_name.c_str()); node)
	{
		if (auto element = node->FirstChildElement("x"); element)
			element->QueryFloatText(&p_out.x);

		if (auto element = node->FirstChildElement("y"); element)
			element->QueryFloatText(&p_out.y);

		if (auto element = node->FirstChildElement("z"); element)
			element->QueryFloatText(&p_out.z);

		if (auto element = node->FirstChildElement("w"); element)
			element->QueryFloatText(&p_out.w);
	}
}

void AmberEngine::Core::Helpers::Serializer::DeserializeColor(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, AmberEngine::Data::Color & p_out)
{
	if (auto node = p_node->FirstChildElement(p_name.c_str()); node)
	{
		if (auto element = node->FirstChildElement("r"); element)
			element->QueryFloatText(&p_out.r);

		if (auto element = node->FirstChildElement("g"); element)
			element->QueryFloatText(&p_out.g);

		if (auto element = node->FirstChildElement("b"); element)
			element->QueryFloatText(&p_out.b);

		if (auto element = node->FirstChildElement("q"); element)
			element->QueryFloatText(&p_out.a);
	}
}

void AmberEngine::Core::Helpers::Serializer::DeserializeModel(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, AmberEngine::Resources::Model *& p_out)
{
	if (std::string path = DeserializeString(p_doc, p_node, p_name.c_str()); path != "?" && path != "")
		p_out = Tools::Global::ServiceLocator::Get<AmberEngine::ResourceManagement::ModelManager>().GetResource(path);
	else
		p_out = nullptr;
}

void AmberEngine::Core::Helpers::Serializer::DeserializeTexture(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, AmberEngine::Resources::Texture *& p_out)
{
	if (std::string path = DeserializeString(p_doc, p_node, p_name.c_str()); path != "?" && path != "")
		p_out = Tools::Global::ServiceLocator::Get<AmberEngine::ResourceManagement::TextureManager>().GetResource(path);
	else
		p_out = nullptr;
}

void AmberEngine::Core::Helpers::Serializer::DeserializeShader(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, AmberEngine::Resources::Shader *& p_out)
{
	if (std::string path = DeserializeString(p_doc, p_node, p_name.c_str()); path != "?" && path != "")
		p_out = Tools::Global::ServiceLocator::Get<AmberEngine::ResourceManagement::ShaderManager>().GetResource(path);
	else
		p_out = nullptr;
}

void AmberEngine::Core::Helpers::Serializer::DeserializeMaterial(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, AmberEngine::Resources::Material *& p_out)
{
	if (std::string path = DeserializeString(p_doc, p_node, p_name.c_str()); path != "?" && path != "")
		p_out = Tools::Global::ServiceLocator::Get<AmberEngine::ResourceManagement::MaterialManager>().GetResource(path);
	else
		p_out = nullptr;
}

bool AmberEngine::Core::Helpers::Serializer::DeserializeBoolean(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	bool result;
	DeserializeBoolean(p_doc, p_node, p_name, result);
	return result;
}

std::string AmberEngine::Core::Helpers::Serializer::DeserializeString(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	std::string result;
	DeserializeString(p_doc, p_node, p_name, result);
	return result;
}

float AmberEngine::Core::Helpers::Serializer::DeserializeFloat(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	float result;
	DeserializeFloat(p_doc, p_node, p_name, result);
	return result;
}

double AmberEngine::Core::Helpers::Serializer::DeserializeDouble(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	double result;
	DeserializeDouble(p_doc, p_node, p_name, result);
	return result;
}

int AmberEngine::Core::Helpers::Serializer::DeserializeInt(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	int result;
	DeserializeInt(p_doc, p_node, p_name, result);
	return result;
}

unsigned AmberEngine::Core::Helpers::Serializer::DeserializeUint(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	unsigned result;
	DeserializeUint(p_doc, p_node, p_name, result);
	return result;
}

int64_t AmberEngine::Core::Helpers::Serializer::DeserializeInt64(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	int64_t result;
	DeserializeInt64(p_doc, p_node, p_name, result);
	return result;
}

glm::vec2 AmberEngine::Core::Helpers::Serializer::DeserializeVec2(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	glm::vec2 result;
	DeserializeVec2(p_doc, p_node, p_name, result);
	return result;
}

glm::vec3 AmberEngine::Core::Helpers::Serializer::DeserializeVec3(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	glm::vec3 result;
	DeserializeVec3(p_doc, p_node, p_name, result);
	return result;
}

glm::vec4 AmberEngine::Core::Helpers::Serializer::DeserializeVec4(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	glm::vec4 result;
	DeserializeVec4(p_doc, p_node, p_name, result);
	return result;
}

glm::quat AmberEngine::Core::Helpers::Serializer::DeserializeQuat(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	glm::quat result;
	DeserializeQuat(p_doc, p_node, p_name, result);
	return result;
}

AmberEngine::Data::Color AmberEngine::Core::Helpers::Serializer::DeserializeColor(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	AmberEngine::Data::Color result;
	DeserializeColor(p_doc, p_node, p_name, result);
	return result;
}

AmberEngine::Resources::Model* AmberEngine::Core::Helpers::Serializer::DeserializeModel(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	AmberEngine::Resources::Model* result;
	DeserializeModel(p_doc, p_node, p_name, result);
	return result;
}

AmberEngine::Resources::Texture* AmberEngine::Core::Helpers::Serializer::DeserializeTexture(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	AmberEngine::Resources::Texture* result;
	DeserializeTexture(p_doc, p_node, p_name, result);
	return result;
}

AmberEngine::Resources::Shader* AmberEngine::Core::Helpers::Serializer::DeserializeShader(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	AmberEngine::Resources::Shader* result;
	DeserializeShader(p_doc, p_node, p_name, result);
	return result;
}

AmberEngine::Resources::Material * AmberEngine::Core::Helpers::Serializer::DeserializeMaterial(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	AmberEngine::Resources::Material* result;
	DeserializeMaterial(p_doc, p_node, p_name, result);
	return result;
}