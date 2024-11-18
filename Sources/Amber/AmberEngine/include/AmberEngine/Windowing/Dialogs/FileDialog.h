#pragma once

#include <functional>
#include <string>

struct tagOFNA;

namespace AmberEngine::Windowing::Dialogs
{
	enum class EExplorerFlags
	{
		READONLY             = 0x00000001,
		OVERWRITEPROMPT      = 0x00000002,
		HIDEREADONLY         = 0x00000004,
		NOCHANGEDIR          = 0x00000008,
		SHOWHELP             = 0x00000010,
		ENABLEHOOK           = 0x00000020,
		ENABLETEMPLATE       = 0x00000040,
		ENABLETEMPLATEHANDLE = 0x00000080,
		NOVALIDATE           = 0x00000100,
		ALLOWMULTISELECT     = 0x00000200,
		EXTENSIONDIFFERENT   = 0x00000400,
		PATHMUSTEXIST        = 0x00000800,
		FILEMUSTEXIST        = 0x00001000,
		CREATEPROMPT         = 0x00002000,
		SHAREAWARE           = 0x00004000,
		NOREADONLYRETURN     = 0x00008000,
		NOTESTFILECREATE     = 0x00010000,
		NONETWORKBUTTON      = 0x00020000,
		NOLONGNAMES          = 0x00040000,	// force no long names for 4.x modules
		EXPLORER             = 0x00080000,	// new look commdlg
		NODEREFERENCELINKS   = 0x00100000,	
		LONGNAMES            = 0x00200000,	// force long names for 3.x modules
		ENABLEINCLUDENOTIFY  = 0x00400000,	// send include message to callback
		ENABLESIZING         = 0x00800000,	
		DONTADDTORECENT      = 0x02000000,	
		FORCESHOWHIDDEN      = 0x10000000	// Show All files including System and hidden files
	};

	inline EExplorerFlags operator~ (EExplorerFlags a) { return (EExplorerFlags)~(int)a; }
	inline EExplorerFlags operator| (EExplorerFlags a, EExplorerFlags b) { return (EExplorerFlags)((int)a | (int)b); }
	inline EExplorerFlags operator& (EExplorerFlags a, EExplorerFlags b) { return (EExplorerFlags)((int)a & (int)b); }
	inline EExplorerFlags operator^ (EExplorerFlags a, EExplorerFlags b) { return (EExplorerFlags)((int)a ^ (int)b); }
	inline EExplorerFlags& operator|= (EExplorerFlags& a, EExplorerFlags b) { return (EExplorerFlags&)((int&)a |= (int)b); }
	inline EExplorerFlags& operator&= (EExplorerFlags& a, EExplorerFlags b) { return (EExplorerFlags&)((int&)a &= (int)b); }
	inline EExplorerFlags& operator^= (EExplorerFlags& a, EExplorerFlags b) { return (EExplorerFlags&)((int&)a ^= (int)b); }

	class FileDialog
	{
	public:
		FileDialog(std::function<int(tagOFNA*)> p_callback, const std::string& p_dialogTitle);
		virtual ~FileDialog() = default;

		void SetInitialDirectory(const std::string& p_initialDirectory);

		virtual void Show(EExplorerFlags p_flags = EExplorerFlags::DONTADDTORECENT | EExplorerFlags::FILEMUSTEXIST | EExplorerFlags::HIDEREADONLY | EExplorerFlags::NOCHANGEDIR);

		bool HasSucceeded() const;

		std::string GetSelectedFileName();

		std::string GetSelectedFilePath();

		std::string GetErrorInfo();

		bool IsFileExisting() const;

	private:
		void HandleError();

	protected:
		const std::string m_dialogTitle;
		std::string m_initialDirectory;
		std::string m_filter;
		std::string m_error;
		std::string m_filename;
		std::string m_filepath;

		std::function<int(tagOFNA*)> m_callback;

		bool m_succeeded;
	};
}