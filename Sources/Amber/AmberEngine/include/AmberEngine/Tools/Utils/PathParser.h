#pragma once

namespace AmberEngine::Tools::Utils
{
	class PathParser
	{
	public:
		enum class EFileType : uint8_t
		{
			UNKNOWN,
			MODEL,
			TEXTURE,
			SHADER,
			MATERIAL
		};

		PathParser() = delete;

		static std::string MakeWindowsStyle(const std::string& p_path);
		static std::string MakeNonWindowsStyle(const std::string& p_path);
		static std::string GetContainingFolder(const std::string& p_path);
		static std::string GetElementName(const std::string& p_path);
		static std::string GetExtension(const std::string& p_path);
		static std::string FileTypeToString(EFileType p_fileType);
		static EFileType GetFileType(const std::string& p_path);
	};
}