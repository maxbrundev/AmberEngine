#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Resources/Model.h"
#include "AmberEngine/Resources/Parsers/AssimpParser.h"

namespace AmberEngine::Resources::Loaders
{
	class API_AMBERENGINE ModelLoader
	{
	public:
		ModelLoader() = delete;

		static Model* Create(const std::string& p_filePath, Parsers::EModelParserFlags p_parserFlags = Parsers::EModelParserFlags::NONE);
		static void Reload(Model& p_model, const std::string& p_filePath, Parsers::EModelParserFlags p_parserFlags = Parsers::EModelParserFlags::NONE);
		static bool Destroy(Model*& p_modelInstance);

	private:
		static Parsers::AssimpParser __ASSIMP;
		static std::string __FILE_TRACE;
	};
}