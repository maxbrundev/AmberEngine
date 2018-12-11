#pragma once

#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Resources/AssimpMesh.h"

namespace RenderEngine
{
	namespace Resources
	{
		unsigned int TextureFromFile(const std::string& p_path, const std::string& directory);

		class AssimpModel
		{
		public:
			std::vector<TextureData> textures_loaded;
			std::vector<AssimpMesh> meshes;
			std::string directory;
			bool gammaCorrection;

		public:
			AssimpModel(const std::string& path);
			void Draw(Shader& shader);
			~AssimpModel() = default;

		private:
			void loadModel(const std::string& path);
			void processNode(aiNode* node, const aiScene* scene);
			AssimpMesh processMesh(aiMesh* mesh, const aiScene* scene);
			std::vector<TextureData> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
		};
	}
}