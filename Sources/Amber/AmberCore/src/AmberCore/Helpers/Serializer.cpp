#include "Amberpch.h"

#include "AmberCore/Helpers/Serializer.h"

#include "AmberCore/ResourceManagement/TextureManager.h"
#include "AmberCore/ResourceManagement/ModelManager.h"
#include "AmberCore/ResourceManagement/ShaderManager.h"
#include "AmberCore/ResourceManagement/MaterialManager.h"
#include "AmberCore/ResourceManagement/SoundManager.h"

#include "AmberTools/Global/ServiceLocator.h"

void AmberCore::Helpers::Serializer::SerializeBoolean(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, bool p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void AmberCore::Helpers::Serializer::SerializeString(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, const std::string & p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value.c_str());
	p_node->InsertEndChild(element);
}

void AmberCore::Helpers::Serializer::SerializeFloat(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, float p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void AmberCore::Helpers::Serializer::SerializeDouble(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, double p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void AmberCore::Helpers::Serializer::SerializeInt(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, int p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void AmberCore::Helpers::Serializer::SerializeUint(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, unsigned p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void AmberCore::Helpers::Serializer::SerializeInt64(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, int64_t p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void AmberCore::Helpers::Serializer::SerializeVec2(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, const glm::vec2 & p_value)
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

void AmberCore::Helpers::Serializer::SerializeVec3(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, const glm::vec3 & p_value)
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

void AmberCore::Helpers::Serializer::SerializeVec4(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, const glm::vec4 & p_value)
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

void AmberCore::Helpers::Serializer::SerializeQuat(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, const glm::quat & p_value)
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

void AmberCore::Helpers::Serializer::SerializeColor(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, const AmberRendering::Data::Color & p_value)
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

void AmberCore::Helpers::Serializer::SerializeModel(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, AmberRendering::Resources::Model * p_value)
{
	SerializeString(p_doc, p_node, p_name.c_str(), p_value ? p_value->Path : "?");
}

void AmberCore::Helpers::Serializer::SerializeTexture(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, AmberRendering::Resources::Texture * p_value)
{
	SerializeString(p_doc, p_node, p_name.c_str(), p_value ? p_value->Path : "?");
}

void AmberCore::Helpers::Serializer::SerializeShader(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, AmberRendering::Resources::Shader * p_value)
{
	SerializeString(p_doc, p_node, p_name.c_str(), p_value ? p_value->Path : "?");
}

void AmberCore::Helpers::Serializer::DeserializeBoolean(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, bool & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryBoolText(&p_out);
}

void AmberCore::Helpers::Serializer::SerializeMaterial(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, AmberCore::Resources::Material * p_value)
{
	SerializeString(p_doc, p_node, p_name.c_str(), p_value ? p_value->path : "?");
}

void AmberCore::Helpers::Serializer::DeserializeString(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, std::string & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
	{
		const char* result = element->GetText();
		p_out = result ? result : "";
	}
}

void AmberCore::Helpers::Serializer::DeserializeFloat(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, float & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryFloatText(&p_out);
}

void AmberCore::Helpers::Serializer::DeserializeDouble(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, double & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryDoubleText(&p_out);
}

void AmberCore::Helpers::Serializer::DeserializeInt(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, int & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryIntText(&p_out);
}

void AmberCore::Helpers::Serializer::DeserializeUint(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, unsigned & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryUnsignedText(&p_out);
}

void AmberCore::Helpers::Serializer::DeserializeInt64(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, int64_t & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryInt64Text(&p_out);
}

void AmberCore::Helpers::Serializer::DeserializeVec2(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, glm::vec2 & p_out)
{
	if (auto node = p_node->FirstChildElement(p_name.c_str()); node)
	{
		if (auto element = node->FirstChildElement("x"); element)
			element->QueryFloatText(&p_out.x);

		if (auto element = node->FirstChildElement("y"); element)
			element->QueryFloatText(&p_out.y);
	}
}

void AmberCore::Helpers::Serializer::DeserializeVec3(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, glm::vec3 & p_out)
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

void AmberCore::Helpers::Serializer::DeserializeVec4(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, glm::vec4 & p_out)
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

void AmberCore::Helpers::Serializer::DeserializeQuat(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, glm::quat & p_out)
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

void AmberCore::Helpers::Serializer::DeserializeColor(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, AmberRendering::Data::Color & p_out)
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

void AmberCore::Helpers::Serializer::DeserializeModel(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, AmberRendering::Resources::Model *& p_out)
{
	if (std::string path = DeserializeString(p_doc, p_node, p_name.c_str()); path != "?" && path != "")
		p_out = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::ModelManager>().GetResource(path);
	else
		p_out = nullptr;
}

void AmberCore::Helpers::Serializer::DeserializeTexture(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, AmberRendering::Resources::Texture *& p_out)
{
	if (std::string path = DeserializeString(p_doc, p_node, p_name.c_str()); path != "?" && path != "")
		p_out = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::TextureManager>().GetResource(path);
	else
		p_out = nullptr;
}

void AmberCore::Helpers::Serializer::DeserializeShader(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, AmberRendering::Resources::Shader *& p_out)
{
	if (std::string path = DeserializeString(p_doc, p_node, p_name.c_str()); path != "?" && path != "")
		p_out = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::ShaderManager>().GetResource(path);
	else
		p_out = nullptr;
}

void AmberCore::Helpers::Serializer::DeserializeMaterial(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, AmberCore::Resources::Material *& p_out)
{
	if (std::string path = DeserializeString(p_doc, p_node, p_name.c_str()); path != "?" && path != "")
		p_out = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::MaterialManager>().GetResource(path);
	else
		p_out = nullptr;
}

bool AmberCore::Helpers::Serializer::DeserializeBoolean(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	bool result;
	DeserializeBoolean(p_doc, p_node, p_name, result);
	return result;
}

std::string AmberCore::Helpers::Serializer::DeserializeString(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	std::string result;
	DeserializeString(p_doc, p_node, p_name, result);
	return result;
}

float AmberCore::Helpers::Serializer::DeserializeFloat(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	float result;
	DeserializeFloat(p_doc, p_node, p_name, result);
	return result;
}

double AmberCore::Helpers::Serializer::DeserializeDouble(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	double result;
	DeserializeDouble(p_doc, p_node, p_name, result);
	return result;
}

int AmberCore::Helpers::Serializer::DeserializeInt(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	int result;
	DeserializeInt(p_doc, p_node, p_name, result);
	return result;
}

unsigned AmberCore::Helpers::Serializer::DeserializeUint(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	unsigned result;
	DeserializeUint(p_doc, p_node, p_name, result);
	return result;
}

int64_t AmberCore::Helpers::Serializer::DeserializeInt64(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	int64_t result;
	DeserializeInt64(p_doc, p_node, p_name, result);
	return result;
}

glm::vec2 AmberCore::Helpers::Serializer::DeserializeVec2(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	glm::vec2 result;
	DeserializeVec2(p_doc, p_node, p_name, result);
	return result;
}

glm::vec3 AmberCore::Helpers::Serializer::DeserializeVec3(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	glm::vec3 result;
	DeserializeVec3(p_doc, p_node, p_name, result);
	return result;
}

glm::vec4 AmberCore::Helpers::Serializer::DeserializeVec4(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	glm::vec4 result;
	DeserializeVec4(p_doc, p_node, p_name, result);
	return result;
}

glm::quat AmberCore::Helpers::Serializer::DeserializeQuat(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	glm::quat result;
	DeserializeQuat(p_doc, p_node, p_name, result);
	return result;
}

AmberRendering::Data::Color AmberCore::Helpers::Serializer::DeserializeColor(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	AmberRendering::Data::Color result;
	DeserializeColor(p_doc, p_node, p_name, result);
	return result;
}

AmberRendering::Resources::Model* AmberCore::Helpers::Serializer::DeserializeModel(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	AmberRendering::Resources::Model* result;
	DeserializeModel(p_doc, p_node, p_name, result);
	return result;
}

AmberRendering::Resources::Texture* AmberCore::Helpers::Serializer::DeserializeTexture(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	AmberRendering::Resources::Texture* result;
	DeserializeTexture(p_doc, p_node, p_name, result);
	return result;
}

AmberRendering::Resources::Shader* AmberCore::Helpers::Serializer::DeserializeShader(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	AmberRendering::Resources::Shader* result;
	DeserializeShader(p_doc, p_node, p_name, result);
	return result;
}

AmberCore::Resources::Material * AmberCore::Helpers::Serializer::DeserializeMaterial(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	AmberCore::Resources::Material* result;
	DeserializeMaterial(p_doc, p_node, p_name, result);
	return result;
}
void AmberCore::Helpers::Serializer::SerializeSound(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, AmberAudio::Resources::Sound* p_value)
{
	SerializeString(p_doc, p_node, p_name.c_str(), p_value ? p_value->Path : "?");
}

void AmberCore::Helpers::Serializer::DeserializeSound(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, AmberAudio::Resources::Sound*& p_out)
{
	if (std::string path = DeserializeString(p_doc, p_node, p_name.c_str()); path != "?" && path != "")
		p_out = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::SoundManager>().GetResource(path);
	else
		p_out = nullptr;
}

AmberAudio::Resources::Sound* AmberCore::Helpers::Serializer::DeserializeSound(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name)
{
	AmberAudio::Resources::Sound* result;
	DeserializeSound(p_doc, p_node, p_name, result);
	return result;
}
