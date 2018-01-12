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

namespace mainwind
{
	class MainWind {
	public:
		MainWind(HINSTANCE& hParentInstance, int nCmdShow, dbms::Dbms* pDbms);
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
		char 			m_filePath[MAX_PATH];
		char 			m_fileName[MAX_PATH];
		dbms::Dbms*		m_pDbms;
		uint32_t		m_rowCnt;

		static LRESULT	CALLBACK StaticWndProc(HWND, UINT, WPARAM, LPARAM);
		LRESULT CALLBACK mainWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

		void getFileName(void);
		static HWND* createToolBar(HWND hWnd);
		static void createWindowsMenu(HWND hWnd);
		static void setWindowsIcon(HWND hWnd);
		void stripFileName(LPTSTR filePath);
		void updateGrid(void);

	};

} //namespace mainwind

#endif //MAINWINDOW_H