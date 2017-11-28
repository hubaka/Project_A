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
#include "babygrid.h"

namespace grid
{

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
	
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

} //namespace mainwind