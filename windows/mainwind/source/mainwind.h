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
#ifndef main_window_h
#define main_window_h
//---------------------------------------------------------------------------

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
		MainWind(HINSTANCE& hParentInstance, const char* p_className, int nCmdShow, dbms::Dbms* pDbms);
		~MainWind(void);
		void createWind(void);
		void attachGrid(grid::IGrid *p_grid);
		void attachBar(bar::ToolBar *p_bar);

	private:
		WNDCLASSEX		m_wndClassEx;
		HWND			m_hWnd;
		HINSTANCE&		m_hParentInstance;
		const char*		m_pClassName;
		int				m_nCmdShow;
		HWND*			m_hWndToolbar;
		char 			m_filePath[MAX_PATH];
		char 			m_fileName[MAX_PATH];
		dbms::Dbms*		m_pDbms;

		static LRESULT	CALLBACK StaticWndProc(HWND, UINT, WPARAM, LPARAM);
		LRESULT CALLBACK mainWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

		void getFileName(void);
		static HWND* createToolBar(HWND hWnd);
		static void createWindowsMenu(HWND hWnd);
		static void setWindowsIcon(HWND hWnd);
		void stripFileName(LPTSTR filePath);

	};

} //namespace mainwind

#endif //main_window_h