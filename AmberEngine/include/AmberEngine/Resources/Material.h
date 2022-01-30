#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Resources/Texture.h"
#include "AmberEngine/Resources/Shader.h"

namespace AmberEngine::Resources
{
	class API_AMBERENGINE Material
	{
	public:
		Material() = default;
		~Material();

		void Bind(const Texture* p_emptyTexture);
		void Unbind() const;

		bool HasShader() const;

		void FillTextures(const std::vector<std::shared_ptr<Texture>>& p_textures);

		void SetShader(Shader* p_shader);
		void SetTexture(Texture* p_texture);
		void SetBlendable(bool p_blendable);
		void SetBackfaceCulling(bool p_backfaceCulling);
		void SetFrontfaceCulling(bool p_frontfaceCulling);
		void SetDepthTest(bool p_depthTest);
		void SetDepthWriting(bool p_depthWriting);
		void SetColorWriting(bool p_colorWriting);
		void SetGPUInstances(uint64_t p_instances);

		Shader* GetShader();
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