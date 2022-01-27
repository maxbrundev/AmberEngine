#pragma once

#include "AmberEngine/API/Export.h"

#include <any>
#include <map>

#include "AmberEngine/Resources/Texture.h"
#include "AmberEngine/Resources/Shader.h"

namespace AmberEngine::Resources
{
	class Mesh;
	class API_AMBERENGINE Material
	{
	public:
		Material(const std::vector<std::shared_ptr<Texture>>& p_textures);
		~Material();

		void Bind(const Texture* p_emptyTexture);
		void Unbind() const;

		void SetShader(Shader* p_shader);
		void SetTexture(Texture* p_texture);
		void SetBlendable(bool p_blendable);
		void SetBackfaceCulling(bool p_backfaceCulling);
		void SetFrontfaceCulling(bool p_frontfaceCulling);
		void SetDepthTest(bool p_depthTest);
		void SetDepthWriting(bool p_depthWriting);
		void SetColorWriting(bool p_colorWriting);
		void SetGPUInstances(uint64_t p_instances);

		Shader*& GetShader();
		bool HasShader() const;
		std::vector<std::shared_ptr<Texture>>& GetTextures();
		std::vector<std::string>& GetMaterialNames();
		bool IsBlendable() const;
		bool HasBackfaceCulling() const;
		bool HasFrontfaceCulling() const;
		bool HasDepthTest() const;
		bool HasDepthWriting() const;
		bool HasColorWriting() const;

		uint64_t GetGPUInstances() const;

	private:
		Shader* m_shader = nullptr;
		std::map<std::string, std::any> m_uniformsData;
		// TODO: 
		std::vector<std::shared_ptr<Texture>> m_textures;
		std::vector<std::string> m_materialNames;
		bool m_blendable = false;
		bool m_backfaceCulling = true;
		bool m_frontfaceCulling = false;
		bool m_depthTest = true;
		bool m_depthWriting = true;
		bool m_colorWriting = true;
		uint64_t m_gpuInstances = 1;
	};
}
