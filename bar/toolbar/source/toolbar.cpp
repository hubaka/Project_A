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

#include <windows.h>
#include "errhandle.h"
#include "resource.h"
#include "toolbar.h"


namespace bar
{

	static BOOL CALLBACK ToolDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	ToolBar::ToolBar(
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
	ToolBar::~ToolBar(
		void
		) {
			;
	}

	HWND 
		ToolBar::createToolBar(
			HWND hWnd
		)
		{
			m_hToolbar = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_TOOLBAR),
				hWnd, ToolDlgProc);
			if(m_hToolbar != NULL)
			{
				ShowWindow(m_hToolbar, SW_SHOW);
			}
			else
			{
				MessageBox(hWnd, (LPCWSTR)L"CreateDialog returned NULL", (LPCWSTR)L"Warning!",  
						MB_OK | MB_ICONINFORMATION);
			}
			return m_hToolbar;
		}

	BOOL CALLBACK 
		ToolDlgProc(
		HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam
	) {
		switch(Message)
		{
			case WM_COMMAND:
				switch(LOWORD(wParam))
				{
					case IDC_PRESS:
						MessageBox(hwnd, (LPCWSTR)L"Hi!", (LPCWSTR)L"This is a message", 
							MB_OK | MB_ICONEXCLAMATION);
					break;
					case IDC_OTHER:
						MessageBox(hwnd, (LPCWSTR)L"Bye!", (LPCWSTR)L"This is also a message", 
							MB_OK | MB_ICONEXCLAMATION);
					break;
				}
			break;
			default:
				return FALSE;
		}
		return TRUE;
	}

	HWND 
		ToolBar::getToolBarInstance(
		void
		) {
			return (m_hToolbar);
	}

} //namespace bar