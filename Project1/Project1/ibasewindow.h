//!
//! $URL: $
//!
//!
//! \file       ibasewindow.h
//! \author     Anand.Kathiresan
//! \date       September,2017
//!
//! \brief      @ Header file for base window interface file
//!
//! @(#)$Id:    $
//!

//---------------------------------------------------------------------------
// Header Guard
//---------------------------------------------------------------------------
#ifndef IBASE_WINDOW_H
#define IBASE_WINDOW_H

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "commonheader.h"

namespace window {
	//---------------------------------------------------------------------------
	//! \class      BaseWindow
	//!
	//! \brief		Abstract class of BaseWindow
	//!
	template <class DERIVED_TYPE>
	class BaseWindow {
	public:
		//---------------------------------------------------------------------------------------------------
		//! \brief		constructor
		//!
		//! \param[in]	nothing
		//!
		//! \return		nothing
		//!
		BaseWindow(
			void
		);

		//---------------------------------------------------------------------------------------------------
		//! \brief		destructor
		//!
		//! \param[in]	nothing
		//!
		//! \return		nothing
		//!
		~BaseWindow(
			void
		);

		//---------------------------------------------------------------------------------------------------
		//! \brief		
		//!
		//! \param[in]	nothing
		//!
		//! \return		nothing
		//!
		static LRESULT CALLBACK 
			WindowProc(
			HWND hwnd,
			UINT uMsg,
			WPARAM wParam,
			LPARAM lParam
		);

		//---------------------------------------------------------------------------------------------------
		//! \brief		
		//!
		//! \param[in]	nothing
		//!
		//! \return		nothing
		//!
		BOOL 
		Create(
			PCWSTR lpWindowName,
			DWORD dwStyle,
			DWORD dwExStyle = 0,
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int nWidth = CW_USEDEFAULT,
			int nHeight = CW_USEDEFAULT,
			HWND hWndParent = 0,
			HMENU hMenu = 0
		);

		//---------------------------------------------------------------------------------------------------
		//! \brief		
		//!
		//! \param[in]	nothing
		//!
		//! \return		nothing
		//!
		HWND Window(
			void
		) const;

	protected:
		//---------------------------------------------------------------------------------------------------
		//! \brief		
		//!
		//! \param[in]	nothing
		//!
		//! \return		nothing
		//!
		virtual PCWSTR ClassName(
			void
		) const = 0;

		//---------------------------------------------------------------------------------------------------
		//! \brief		
		//!
		//! \param[in]	nothing
		//!
		//! \return		nothing
		//!
		virtual LRESULT HandleMessage(
			UINT uMsg,
			WPARAM wParam,
			LPARAM lParam
		) = 0;

		HWND m_hwnd;
	};

} // namespace window
//---------------------------------------------------------------------------
#endif //IBASE_WINDOW_H
//---------------------------------------------------------------------------
// End of File
//---------------------------------------------------------------------------
