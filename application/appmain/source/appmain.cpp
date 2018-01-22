//!
//! $URL: $
//!
//!	Project		
//!
//! \file       appmain.cpp
//! \author     Anand.Kathiresan
//! \date       2017
//!
//! \brief      @ Source file of the Main application
//!
//! @(#)$Id:    $
//!

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// This program is the confidential and proprietary product of			  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

#include <windows.h>	// Windows-specific header file
#include <Commctrl.h>	// reqd. for InitCommonControlsEx() fxn
#include "resource.h"	// reqd. for resource like icon, dialog features
//#include "errhandle.h"	// reqd. for error handling
#include "mainwind.h"	// reqd. for Main Window class
#include "babygrid.h"	// reqd. for grid class
#include "toolbar.h"	// reqd. for toolbar class
#include "dbms.h"		// reqd. for database class
#include "cryptomain.h"	// reqd. for crypt class
#include "appmain.h"	// reqd. for database class

#pragma comment(lib,"COMCTL32.LIB")	// reqd. for InitCommonControlsEx() fxn

extern HWND g_hDialogWind;	// window handle of grid

//---------------------------------------------------------------------------------------------------
//! \brief		main funcion of this application. It creates and initializes algorithms
//!
//! \param[in]	hInstance		hInstance is something called a "handle to an instance" or 
//!								"handle to a module." The operating system uses this value to 
//!								identify the executable (EXE) when it is loaded in memory. 
//!								The instance handle is needed for certain Windows functions
//! \param[in]	hPrevInstance	hPrevInstance has no meaning. It was used in 16-bit Windows, 
//!								but is now always zero.
//! \param[in]	lpCmdLine		pCmdLine contains the command-line arguments as a Unicode string.
//! \param[in]	nCmdShow		nCmdShow is a flag that says whether the main application window 
//!								will be minimized, maximized, or shown normally.
//!
//! \return						The return value is not used by the operating system, but you can 
//!								use the return value to convey a status code to some other program 
//!								that you write.
//!
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    MSG Msg;

	//errhandle::ErrHandle errh;

	// Initialize common controls. Also needed for MANIFEST's
	InitializeMyCommonControls();
	
	// creating database instance
	dbms::Dbms dbmsObj;
	// creating grid instance
	grid::BabyGrid	babygrid(hInstance);
	// creating Toolbar instance
	bar::ToolBar toolbar;
	// creating cypter instance
	encryptmain::EncryptDBFile encrypter;
	//creating mainwindow instance
	mainwind::MainWind mainWindow(hInstance, nCmdShow, &dbmsObj, &encrypter);
	mainWindow.attachGrid(&babygrid);
	mainWindow.attachBar(&toolbar);
	mainWindow.createWind();
	// getting the instance of toolbar
	HWND ptoolbar = toolbar.getToolBarInstance();

	//IMPORTANT: 
	//GetMessage() will return -1 if it encounters an error. 
	//Make sure you remember this, or it will catch you out at some point... 
	//even though GetMessage() is defined as returning a BOOL, it can return 
	//values other than TRUE or FALSE, since BOOL is defined as UINT (unsigned int). 
	//The following are examples of code that may seem to work, but will not process certian conditions correctly:
    //while(GetMessage(&Msg, NULL, 0, 0))
    //while(GetMessage(&Msg, NULL, 0, 0) != 0)
    //while(GetMessage(&Msg, NULL, 0, 0) == TRUE)
	//The above are all wrong! It may be of note that I used to use the first of 
	//these throughout the tutorial, since as I just mentioned, it works fine as long as GetMessage() 
	//never fails, which when your code is correct it won't. However I failed to take into 
	//consideration that if you're reading this, your code probably won't be correct a lot of the time, 
	//and GetMessage() will fail at some point :) I've gone through and corrected this, but forgive me if I've missed a few spots.
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
		if(!IsDialogMessage(g_hDialogWind, &Msg))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
    }
    return Msg.wParam;
}

//---------------------------------------------------------------------------------------------------
//! \brief		 This function registers and initializes certain common control window classes
//!
//! \param[in]	nothing
//!
//! \return		nothing
//!
//! 
void InitializeMyCommonControls(void)
{
    //LPINITCOMMONCONTROLSEX lpInitCtrls;
	INITCOMMONCONTROLSEX lpInitCtrls;
 
    //lpInitCtrls->dwSize = (LPINITCOMMONCONTROLSEX);
	lpInitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
    lpInitCtrls.dwICC = ICC_BAR_CLASSES; //	to Load toolbar, status bar, trackbar, and tooltip control classes.
 
    InitCommonControlsEx(&lpInitCtrls);
}