//!
//! $URL: $
//!
//!
//! \file       
//! \author     Anand.Kathiresan
//! \date       
//!
//! \brief      @ Source File for cryotmain.cpp
//!
//! @(#)$Id:    $
//!

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// This program is the confidential and proprietary product of			  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <WinCred.h>
#include "resource.h"
#include "errhandle.h"
#include "cryptomain.h"

#pragma comment(lib,"Credui.lib")

namespace encryptmain
{

	//---------------------------------------------------------------------------
	/// @def SimpleGrid_SetProtectColor(hGrid, clrProtect)
	///
	/// @brief Sets the background color of protected cells.
	///
	/// @param hGrid The handle of the grid.
	/// @param clrProtect A COLORREF value. 
	///
	/// @returns The return value is not meaningful. 
	#define MainWindow_Close(hWnd) (BOOL)SNDMSG((hWnd),WM_CLOSE, 0L,0L)

	static errhandle::ErrHandle g_errHandle;
	static bool	newUserCreation = FALSE;
	static HWND gParentWindow;

	static INT_PTR CALLBACK PasswordProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK NewUserProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	EncryptDBFile::EncryptDBFile(
		void
	) {
		;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	EncryptDBFile::~EncryptDBFile(
		void
	) {
		;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void
	EncryptDBFile::getUserCredentials(
		HINSTANCE	hParentInstance,
		HWND hParentWindow
	) {

		bool retVal = FALSE;
		gParentWindow = hParentWindow;
		retVal = DialogBox(hParentInstance,                   // application instance
			MAKEINTRESOURCE(IDD_USERCREDENTIALS), // dialog box resource
			hParentWindow,                          // owner window
			PasswordProc                    // dialog box window procedure
		);
		//g_errHandle.getErrorInfo((LPTSTR)L"dialog Registration Failed!");

		while ((newUserCreation == TRUE) && (retVal == FALSE)) {
			DialogBox(hParentInstance, MAKEINTRESOURCE(IDD_NEWUSERCREDENTIALS), hParentWindow, NewUserProc);
			DialogBox(hParentInstance, MAKEINTRESOURCE(IDD_USERCREDENTIALS), hParentWindow, PasswordProc);
		}
	}


	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	INT_PTR CALLBACK PasswordProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		TCHAR lpszPassword[16];
		TCHAR lpszUsername[16];
		WORD passWord;
		WORD userName;
		bool retVal = FALSE;

		switch (message)
		{
			case WM_INITDIALOG:
			{
				// Set password character to a plus sign (*) 
				SendDlgItemMessage(hDlg, IDE_PASSWORDEDIT, EM_SETPASSWORDCHAR, (WPARAM) '*', (LPARAM)0);
				retVal = TRUE;
				break;
			}
			case WM_COMMAND:
			{
				switch (wParam)
				{
					case IDCANCEL:
					{
						EndDialog(hDlg, TRUE);
						retVal = TRUE;
						MainWindow_Close(gParentWindow);
						break;
					}
					case IDE_OK:
					{
						// Get number of characters. 
						userName = (WORD)SendDlgItemMessage(hDlg, IDE_PASSWORDEDIT, EM_LINELENGTH, (WPARAM)0, (LPARAM)0);
						passWord = (WORD)SendDlgItemMessage(hDlg, IDE_PASSWORDEDIT, EM_LINELENGTH, (WPARAM)0, (LPARAM)0);
						if (passWord >= 16) 
						{
							MessageBox(hDlg, L"Password can't be more than 16 characters", L"Error", MB_OK);
							retVal = FALSE;
						}
						else if (userName >= 16)
						{
							MessageBox(hDlg, L"Username can't be more than 16 characters", L"Error", MB_OK);
							retVal = FALSE;
						}
						else if (passWord == 0)
						{
							MessageBox(hDlg, L"Password can't be empty", L"Error", MB_OK);
							retVal = FALSE;
						}
						else if (userName == 0)
						{
							MessageBox(hDlg, L"Username can't be empyty", L"Error", MB_OK);
							retVal = FALSE;
						}
						else
						{

							// Put the number of characters into first word of buffer. 
							*((LPWORD)lpszPassword) = passWord;
							*((LPWORD)lpszUsername) = userName;

							// Get the characters. 
							SendDlgItemMessage(hDlg, IDE_USERNAMEEDIT, EM_GETLINE, (WPARAM)0, (LPARAM)lpszUsername);
							SendDlgItemMessage(hDlg, IDE_PASSWORDEDIT, EM_GETLINE, (WPARAM)0, (LPARAM)lpszPassword);
							// Null-terminate the string. 
							lpszUsername[userName] = 0;
							lpszPassword[passWord] = 0;

							MessageBox(hDlg, lpszUsername, L"Did it work?", MB_OK);
							MessageBox(hDlg, lpszPassword, L"Did it work?", MB_OK);

							// Call a local password-parsing function. 
							//ParsePassword(lpszPassword);

							EndDialog(hDlg, TRUE);
							retVal = TRUE;
						}
						break;
					}
					case IDE_NEWUSER:
					{
						newUserCreation = TRUE;
						EndDialog(hDlg, FALSE);
						retVal = TRUE;
						break;
					}
					default:
					{
						retVal = FALSE;
						break;
					}
				}
			}
		}
		return retVal;

		UNREFERENCED_PARAMETER(lParam);
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	INT_PTR CALLBACK NewUserProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		TCHAR lpszPassword[16];
		TCHAR lpszUsername[16];
		TCHAR lpsznameCredentials[MAX_PATH];
		WORD passWord;
		WORD userName;
		WORD nameCredentials;
		bool retVal = FALSE;

		switch (message)
		{
			case WM_INITDIALOG:
			{
				// Set password character to a plus sign (*) 
				SendDlgItemMessage(hDlg, IDE_PASSWORDEDIT, EM_SETPASSWORDCHAR, (WPARAM) '*', (LPARAM)0);
				SendDlgItemMessage(hDlg, IDE_RETYPEPASSWORDEDIT, EM_SETPASSWORDCHAR, (WPARAM) '*', (LPARAM)0);
				retVal = TRUE;
				break;
			}
			case WM_COMMAND:
			{
				switch (wParam)
				{
					case IDE_OK:
					{
						// Get number of characters. 
						userName = (WORD)SendDlgItemMessage(hDlg, IDE_PASSWORDEDIT, EM_LINELENGTH, (WPARAM)0, (LPARAM)0);
						passWord = (WORD)SendDlgItemMessage(hDlg, IDE_PASSWORDEDIT, EM_LINELENGTH, (WPARAM)0, (LPARAM)0);
						if (passWord >= 16)
						{
							MessageBox(hDlg, L"Password can't be more than 16 characters", L"Error", MB_OK);
							retVal = FALSE;
						}
						else if (userName >= 16)
						{
							MessageBox(hDlg, L"Username can't be more than 16 characters", L"Error", MB_OK);
							retVal =  FALSE;
						}
						else if (passWord == 0)
						{
							MessageBox(hDlg, L"Password can't be empty", L"Error", MB_OK);
							retVal =  FALSE;
						}
						else if (userName == 0)
						{
							MessageBox(hDlg, L"Username can't be empyty", L"Error", MB_OK);
							retVal=  FALSE;
						}
						else
						{
							// Put the number of characters into first word of buffer. 
							*((LPWORD)lpszPassword) = passWord;
							*((LPWORD)lpszUsername) = userName;

							// Get the characters. 
							SendDlgItemMessage(hDlg, IDE_USERNAMEEDIT, EM_GETLINE, (WPARAM)0, (LPARAM)lpszUsername);
							SendDlgItemMessage(hDlg, IDE_PASSWORDEDIT, EM_GETLINE, (WPARAM)0, (LPARAM)lpszPassword);
							// Null-terminate the string. 
							lpszUsername[userName] = 0;
							lpszPassword[passWord] = 0;

							MessageBox(hDlg, lpszUsername, L"Did it work?", MB_OK);
							MessageBox(hDlg, lpszPassword, L"Did it work?", MB_OK);

							// Call a local password-parsing function. 
							//ParsePassword(lpszPassword);

							EndDialog(hDlg, TRUE);
							retVal = TRUE;
						}
						newUserCreation = FALSE;
						break;
					}
					case IDE_CANCEL:
					{
						newUserCreation = FALSE;
						EndDialog(hDlg, TRUE);
						retVal = FALSE;
						break;
					}
					default:
					{
						retVal = FALSE;
						break;
					}
				}
			}
		}
		return retVal;

		UNREFERENCED_PARAMETER(lParam);
	}

}