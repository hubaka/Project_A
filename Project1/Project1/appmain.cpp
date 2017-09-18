//!
//! $URL: $
//!
//!
//! \file       appmain.cpp
//! \author     Anand.Kathiresan
//! \date       September,2017
//!
//! \brief      @ This file contains the main function and the initialization 
//!				of major windows
//!
//! @(#)$Id:    $
//!

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
//#include "ibasewindow.h"
#include "mainwindow.h"
#include "appmain.h"

namespace appmain {
	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	nothing
	//!
	//! \return		nothing
	//!
	int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {

		window::MainWindow mainWin;

		if (!mainWin.Create(L"Learn to Program Windows", WS_OVERLAPPEDWINDOW))
		{
			return 0;
		}

		ShowWindow(mainWin.Window(), nCmdShow);

		// Run the message loop.

		MSG msg = {};
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return 0;

	}
} // namespace appmain


