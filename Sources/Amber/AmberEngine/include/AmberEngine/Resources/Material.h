#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Resources/Texture.h"
#include "AmberEngine/Resources/Shader.h"

#include "AmberEngine/Core/API/ISerializable.h"

namespace AmberEngine::Resources
{
	class API_AMBERENGINE Material : public Core::API::ISerializable
	{
	public:
		Material();
		~Material();

		void Bind(const Texture* p_emptyTexture) const;
		void Unbind() const;

		bool HasShader() const;
		void FillUniform();
	
		template<typename T>
		void SetUniform(const std::string p_key, const T& p_value)
		{
			if (m_uniformsData.find(p_key) != m_uniformsData.end())
				m_uniformsData[p_key] = std::any(p_value);
		}

		template<typename T>
		const T& GetUniform(const std::string p_key)
		{
			if(m_uniformsData.find(p_key) != m_uniformsData.end())
			{
				return T();
			}

			return std::any_cast<T>(m_uniformsData.at(p_key));
		}

		void SetShader(Shader* p_shader);
		void SetBlendable(bool p_blendable);
		void SetBackFaceCulling(bool p_backFaceCulling);
		void SetFrontFaceCulling(bool p_frontFaceCulling);
		void SetDepthTest(bool p_depthTest);
		void SetDepthWriting(bool p_depthWriting);
		void SetColorWriting(bool p_colorWriting);
		void SetGPUInstances(uint64_t p_instances);
		void SetName(std::string p_name);

		Shader* GetShader() const;
		const std::string& GetName();
		std::map<std::string, std::any>& GetUniformsData();
		bool IsBlendable() const;
		bool HasBackFaceCulling() const;
		bool HasFrontFaceCulling() const;
		bool HasDepthTest() const;
		bool HasDepthWriting() const;
		bool HasColorWriting() const;

		int GetGPUInstances() const;

		uint8_t GenerateStateMask() const;

		void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		const std::string path;
		int m_gpuInstances = 1;
	private:
		Shader* m_shader         = nullptr;

		std::map<std::string, std::any> m_uniformsData;

		bool m_blendable        = false;
		bool m_backFaceCulling  = true;
		bool m_frontFaceCulling = false;
		bool m_depthTest        = true;
		bool m_depthWriting     = true;
		bool m_colorWriting     = true;
		bool hasSpecularMap     = true;

		

		std::string m_name;
	};
}