#pragma once

#include "AmberEngine/Resources/Texture.h"
#include "AmberEngine/Resources/Shader.h"

namespace AmberEngine::Resources
{
	class Material
	{
	public:
		Material() = default;
		~Material() = default;

		void Bind(const Texture* p_emptyTexture) const;
		void Unbind() const;

		void SetShader(Shader* p_shader);
		void SetBlendable(bool p_blendable);
		void SetBackfaceCulling(bool p_backfaceCulling);
		void SetFrontfaceCulling(bool p_frontfaceCulling);
		void SetDepthTest(bool p_depthTest);
		void SetDepthWriting(bool p_depthWriting);
		void SetColorWriting(bool p_colorWriting);
		void SetGPUInstances(uint64_t p_instances);

		bool HasShader() const;
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
		std::vector<Texture*> m_texture;

		bool m_blendable = false;
		bool m_backfaceCulling = true;
		bool m_frontfaceCulling = false;
		bool m_depthTest = true;
		bool m_depthWriting = true;
		bool m_colorWriting = true;
		uint64_t m_gpuInstances = 1;
	};
}
