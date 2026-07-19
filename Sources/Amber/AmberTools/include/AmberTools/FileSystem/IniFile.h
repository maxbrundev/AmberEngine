#pragma once

#include <string>
#include <unordered_map>


namespace AmberTools::FileSystem
{
	class IniFile final
	{
	public:
		using AttributePair = std::pair<std::string, std::string>;
		using AttributeMap = std::unordered_map<std::string, std::string>;

		IniFile(const std::string& p_filePath);

		void Reload();

		void Rewrite() const;

		template<typename T>
		T Get(const std::string& p_key);

		template<typename T>
		T GetOrDefault(const std::string& p_key, T p_default);

		template<typename T>
		bool Set(const std::string& p_key, const T& p_value);

		template<typename T>
		bool Add(const std::string& p_key, const T& p_value);

		bool Remove(const std::string& p_key);

		void RemoveAll();

		bool IsKeyExisting(const std::string& p_key) const;

		std::vector<std::string> GetFormattedContent() const;

	private:
		void RegisterPair(const std::string& p_key, const std::string& p_value);
		void RegisterPair(const AttributePair& p_pair);

		void Load();

		AttributePair	ExtractKeyAndValue(const std::string& p_attributeLine)	const;
		bool			IsValidLine(const std::string& p_attributeLine)	const;
		bool			StringToBoolean(const std::string& p_value)			const;

	private:
		std::string		m_filePath;
		AttributeMap	m_data;
	};
}

#include "AmberTools/FileSystem/IniFile.inl"