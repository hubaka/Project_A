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

namespace grid {
	class IGrid;
}

namespace mainwind
{
	class MainWind {
	public:
		MainWind(HINSTANCE& hParentInstance, int nCmdShow);
		~MainWind(void);
		void createWind(void);
		void attachGrid(grid::IGrid *p_grid);

	private:
		WNDCLASSEX		m_wndClassEx;
		HWND			m_hWnd;
		HINSTANCE&		m_hParentInstance;
		int				m_nCmdShow;
		HWND*			m_hWndToolbar;

		static LRESULT	CALLBACK StaticWndProc(HWND, UINT, WPARAM, LPARAM);
		LRESULT CALLBACK mainWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

		void getFileName(void);
		static HWND* createToolBar(HWND hWnd);
		static void createWindowsMenu(HWND hWnd);
		static void setWindowsIcon(HWND hWnd);

	};

} //namespace mainwind

#endif //MAINWINDOW_H