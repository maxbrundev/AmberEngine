#pragma once


#include "AmberRendering/Resources/Model.h"
#include "AmberRendering/Resources/Parsers/AssimpParser.h"

namespace AmberRendering::Resources::Loaders
{
	class ModelLoader
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