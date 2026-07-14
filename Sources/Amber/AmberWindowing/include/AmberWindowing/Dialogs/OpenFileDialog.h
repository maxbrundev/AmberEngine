#pragma once

#include "AmberWindowing/Dialogs/FileDialog.h"

namespace AmberWindowing::Dialogs
{
	class OpenFileDialog final : public FileDialog
	{
	public:
		OpenFileDialog(const std::string& p_dialogTitle);
		virtual ~OpenFileDialog() override = default;

		void AddFileType(const std::string& p_label, const std::string& p_filter);
	};
}