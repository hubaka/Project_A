//!
//! $URL: $
//!
//!
//! \file       
//! \author     Anand.Kathiresan
//! \date       
//!
//! \brief      @ Header file for 
//!
//! @(#)$Id:    $
//!

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// This program is the confidential and proprietary product of			  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//---------------------------------------------------------------------------
#include "sys.h"

namespace grid {
	class IGrid;
}

namespace bar {
	class ToolBar;
}

namespace dbms {
	class Dbms;
}

namespace encryptmain {
	class EncryptDBFile;
}

namespace mainwind
{
	class MainWind {
	public:
		MainWind(HINSTANCE& hParentInstance, int nCmdShow, dbms::Dbms* pDbms, encryptmain::EncryptDBFile* pEncrypter);
		~MainWind(void);
		void createWind(void);
		void attachGrid(grid::IGrid *p_grid);
		void attachBar(bar::ToolBar *p_bar);

	private:
		WNDCLASSEX		m_wndClassEx;
		HWND			m_hWnd;
		HINSTANCE&		m_hParentInstance;
		int				m_nCmdShow;
		HWND*			m_hWndToolbar;
		dbms::Dbms*		m_pDbms;
		char			m_exePath[MAX_PATH];

		static LRESULT	CALLBACK StaticWndProc(HWND, UINT, WPARAM, LPARAM);
		LRESULT CALLBACK mainWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

		void getFileName(void);
		void getFolderName(void);
		void getFolderContent(TCHAR* pDirPath);
		static HWND* createToolBar(HWND hWnd);
		static void createWindowsMenu(HWND hWnd);
		static void setWindowsIcon(HWND hWnd);
		void stripFileName(LPTSTR fullPath, char* pFileName, char* pFilePath);
		void addNewEntryToGrid(char* pFileName, char* pFilePath);
		void addExistingEntryToGrid(char* pFileName, char* pFilePath, bool encrypt, bool decrypt);
		bool findFile(char* pDirPath, char* pFileName);

	};

} //namespace mainwind

#endif //MAINWINDOW_H