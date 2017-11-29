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
#include "resource.h"
#include "errhandle.h"
#include "babygrid.h"

extern "C" IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

namespace grid
{
//---------------------------------------------------------------------------
// Defines and Macros
//---------------------------------------------------------------------------
	static errhandle::ErrHandle *errHandle;

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	BabyGrid::BabyGrid(
		HINSTANCE&	hParentInstance,
		const char*	p_className
		) : m_hParentInstance(hParentInstance),
			m_pClassName(p_className) {
			;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	BabyGrid::~BabyGrid(
		void
		) {
		m_pClassName = 0U;
	}

		//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void
	BabyGrid::regClass(
		void
		) {
		    //Step 1: Registering the Window Class
			m_gridClassEx.cbSize        = sizeof(WNDCLASSEX);
			m_gridClassEx.style         = 0;
			m_gridClassEx.cbClsExtra    = 0;
			m_gridClassEx.cbWndExtra    = 0;
			m_gridClassEx.hIcon         = NULL;
			m_gridClassEx.hIconSm       = NULL;
			m_gridClassEx.hCursor       = NULL;
			m_gridClassEx.hbrBackground = NULL;
			m_gridClassEx.lpszMenuName  = NULL;
			m_gridClassEx.lpszClassName = (LPCWSTR)m_pClassName;
			m_gridClassEx.hInstance     = m_hParentInstance;
			m_gridClassEx.lpfnWndProc   = (WNDPROC)Grid_Proc;

			if(!RegisterClassEx(&m_gridClassEx))
			{
				MessageBox(NULL, (LPCWSTR)L"Grid Class Registration Failed!", (LPCWSTR)L"Error!",
					MB_ICONEXCLAMATION | MB_OK);
			}
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	LRESULT	CALLBACK 
		BabyGrid::Grid_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
	
		switch (uMsg) {
			case WM_INITDIALOG: 
				{
					//return SetDlgMsgResult((m_hWnd),(WM_INITDIALOG),HANDLE_WM_INITDIALOG((m_hWnd),(wParam),(lParam), mainWindInitDialog));  /* added 05-01-29 */
					break;
				}
			case SG_SETCOLAUTOWIDTH: 
				{
					break;
				}
			default: 
				{
					return DefWindowProc(hWnd, uMsg, wParam, lParam);
					break;
				}
		}
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void 
	BabyGrid::createBabyGrid(
		HWND hWnd
	)
	{
		//m_gridhWnd = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_BABY_GRID), hWnd, (DLGPROC)Grid_Proc);
		//ShowWindow(m_gridhWnd, SW_SHOW);
		//DialogBox(HINST_THISCOMPONENT, MAKEINTRESOURCE(ID_BABY_GRID), hWnd, (DLGPROC)Grid_Proc);
		static HWND hControl;
		HINSTANCE hinst;

		//Get hinstance if this code is compiled into and called from a dll 
		// as well as if it were compiled into the executable.  (XP and later)
		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		 (LPCTSTR)L"New_SimpleGrid", &hinst);

		//Only need to register the property grid once
		/*if (!aControl)
			aControl = InitSimpleGrid(hinst);*/

		hControl = CreateWindowEx(0, (LPCWSTR)m_pClassName, NULL, WS_CHILD | 
		  WS_TABSTOP, 0, 0, 0, 0, hWnd, (HMENU)ID_BABY_GRID, GetModuleHandle(NULL), NULL);

		if (hControl == NULL) {
			errHandle->getErrorInfo((LPTSTR)L"createBabyGrid");
		}
	}

} //namespace mainwind