#include <windows.h>
#include <Commctrl.h>
#include "mainwind.h"
#include "babygrid.h"
#include "appmain.h"

#pragma comment(lib,"COMCTL32.LIB")

extern HWND g_hToolbar;

//---------------------------------------------------------------------------------------------------
//! \brief		
//!
//! \param[in]	
//!
//! \return		
//!
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    MSG Msg;
	const char winclassName[] = "MainWindowClass";
	const char babyGridName[] = "BabyGridClass";

	InitializeMyCommonControls();

	grid::BabyGrid	babygrid(hInstance, &babyGridName[0]);
	mainwind::MainWind mainWindow(hInstance, &winclassName[0], nCmdShow);
	mainWindow.regClass();
	mainWindow.createWind();
	mainWindow.showWind();
	babygrid.regClass();

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
		if(!IsDialogMessage(g_hToolbar, &Msg))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
    }
    return Msg.wParam;
}

// Initialization of Common Controls Procedure
void InitializeMyCommonControls()
{
    //LPINITCOMMONCONTROLSEX lpInitCtrls;
	INITCOMMONCONTROLSEX lpInitCtrls;
 
    //lpInitCtrls->dwSize = (LPINITCOMMONCONTROLSEX);
	lpInitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
    lpInitCtrls.dwICC = ICC_BAR_CLASSES; //	to Load toolbar, status bar, trackbar, and tooltip control classes.
 
    InitCommonControlsEx(&lpInitCtrls);
}