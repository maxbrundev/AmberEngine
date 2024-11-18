#pragma once

#include "AmberEngine/Windowing/Dialogs/FileDialog.h"

namespace AmberEngine::Windowing::Dialogs
{
	class SaveFileDialog final : public FileDialog
	{
	public:
		SaveFileDialog(const std::string& p_dialogTitle);
		virtual ~SaveFileDialog() override = default;

		virtual void Show(EExplorerFlags p_flags = EExplorerFlags::DONTADDTORECENT | EExplorerFlags::FILEMUSTEXIST | EExplorerFlags::HIDEREADONLY | EExplorerFlags::NOCHANGEDIR) override;

		void DefineExtension(const std::string& p_label, const std::string& p_extension);

	private:
		void AddExtensionToFilePathAndName();

	private:
		std::string m_extension;
	};
}