#pragma once

#include "AmberEngine/Windowing/Dialogs/FileDialog.h"

namespace AmberEngine::Windowing::Dialogs
{
	class OpenFileDialog final : public FileDialog
	{
	public:
		OpenFileDialog(const std::string& p_dialogTitle);
		virtual ~OpenFileDialog() override = default;

		void AddFileType(const std::string& p_label, const std::string& p_filter);
	};
}