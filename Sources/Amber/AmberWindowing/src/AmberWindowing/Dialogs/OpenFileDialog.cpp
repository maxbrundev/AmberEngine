#include "Amberpch.h"

#include "AmberWindowing/Dialogs/OpenFileDialog.h"

#include <Windows.h>

AmberWindowing::Dialogs::OpenFileDialog::OpenFileDialog(const std::string& p_dialogTitle) :
FileDialog(GetOpenFileNameA, p_dialogTitle)
{
}

void AmberWindowing::Dialogs::OpenFileDialog::AddFileType(const std::string& p_label, const std::string& p_filter)
{
	m_filter += p_label + '\0' + p_filter + '\0';
}
