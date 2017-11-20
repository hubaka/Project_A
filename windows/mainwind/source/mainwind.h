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

namespace mainwind
{
	class MainWind {
	public:
		MainWind(HINSTANCE& hInstance, const char* p_className, int nCmdShow);
		~MainWind(void);
		void regClass(void);
		void createWind(void);
		void showWind(void);

	private:
		WNDCLASSEX	m_wndClassEx;
		HWND		m_hwnd;
		HINSTANCE&	m_hInstance;
		const char*	m_pClassName;
		int			m_nCmdShow;

		static LRESULT	CALLBACK StaticWndProc(HWND, UINT, WPARAM, LPARAM);
		LRESULT CALLBACK mainWndProc(UINT	uMsg, WPARAM	wParam, LPARAM	lParam);
		static void ErrorExit(LPTSTR lpszFunction);
		void getFileName(void);

	};

} //namespace mainwind

#endif //main_window_h