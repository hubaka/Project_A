//!
//! $URL: $
//!
//!
//! \file       ibasewindow.cpp
//! \author     Anand.Kathiresan
//! \date       September,2017
//!
//! \brief      @ source file for base window interface file
//!
//! @(#)$Id:    $
//!

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "ibasewindow.h"

namespace window {
	//---------------------------------------------------------------------------------------------------
	//! \brief		constructor
	//!
	//! \param[in]	nothing
	//!
	//! \return		nothing
	//!
	template <class DERIVED_TYPE>
	BaseWindow<DERIVED_TYPE>::BaseWindow(
		void
	) : m_hwnd(0) {
		;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		destructor
	//!
	//! \param[in]	nothing
	//!
	//! \return		nothing
	//!
	template <class DERIVED_TYPE>
	BaseWindow<DERIVED_TYPE>::~BaseWindow(
		void
	) {
		m_hwnd = 0;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	nothing
	//!
	//! \return		nothing
	//!
	template <class DERIVED_TYPE>
	static LRESULT CALLBACK 
	BaseWindow<DERIVED_TYPE>::WindowProc(
		HWND hwnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam
	) {
		DERIVED_TYPE *pThis = NULL;
		if (uMsg == WNCREATE) {
			CREATESTRUCT *pCreateWindow = (CREATESTRUCT)*lParam;
			pThis = (DERIVED_TYPE*)lParam->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
			pThis->m_hwnd = hwnd;
		}
		else {
			pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}
		if (pThis) {
			pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else {
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	nothing
	//!
	//! \return		nothing
	//!
	template <class DERIVED_TYPE>
	BOOL
	BaseWindow<DERIVED_TYPE>::Create(
			PCWSTR lpWindowName,
			DWORD dwStyle,
			DWORD dwExStyle = 0,
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int nWidth = CW_USEDEFAULT,
			int nHeight = CW_USEDEFAULT,
			HWND hWndParent = 0,
			HMENU hMenu = 0
	) {

		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = ClassName();

		RegisterClass(&wc);

		m_hwnd = CreateWindowEx(
			dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
			nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
		);

		return (m_hwnd ? TRUE : FALSE);
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	nothing
	//!
	//! \return		nothing
	//!
	template <class DERIVED_TYPE>
	HWND 
	BaseWindow<DERIVED_TYPE>::Window(
		void
	) const { 
		return m_hwnd; 
	}

} // namespace window
