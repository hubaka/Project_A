//!
//! $URL: $
//!
//!
//! \file       mainwindow.cpp
//! \author     Anand.Kathiresan
//! \date       September,2017
//!
//! \brief      @ Source file for main window class
//!
//! @(#)$Id:    $
//!

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "mainwindow.h"

namespace window {
	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	nothing
	//!
	//! \return		nothing
	//!
	MainWindow::MainWindow(
		void
	) {
		;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	nothing
	//!
	//! \return		nothing
	//!
	MainWindow::~MainWindow(
		void
	) {
		;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	nothing
	//!
	//! \return		nothing
	//!
	LRESULT
	MainWindow::HandleMessage(
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam
	) {

		switch (uMsg) {
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(m_hwnd, &ps);
			FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
			EndPaint(m_hwnd, &ps);
			return 0;
		}
		default:
			return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
		}
		return true;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	nothing
	//!
	//! \return		nothing
	//!
	PCWSTR 
	MainWindow::ClassName(
		void
	) const {

		return L"Sample Window Class";

	}

} //namespace window