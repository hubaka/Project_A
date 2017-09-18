//!
//! $URL: $
//!
//!
//! \file       mainwindow.h
//! \author     Anand.Kathiresan
//! \date       September,2017
//!
//! \brief      @ Header file for main window class
//!
//! @(#)$Id:    $
//!

//---------------------------------------------------------------------------
// Header Guard
//---------------------------------------------------------------------------
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "ibasewindow.h"

namespace window {
	//---------------------------------------------------------------------------
	//! \class      MainWindow
	//!
	//! \brief		Concrete class of BaseWindow
	//!
	class MainWindow : public BaseWindow<MainWindow> {
	public:
		//---------------------------------------------------------------------------------------------------
		//! \brief		constructor
		//!
		//! \param[in]	nothing
		//!
		//! \return		nothing
		//!
		MainWindow(
			void
		);

		//---------------------------------------------------------------------------------------------------
		//! \brief		destructor
		//!
		//! \param[in]	nothing
		//!
		//! \return		nothing
		//!
		~MainWindow(
			void
		);

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
		) const;

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
		);

	private:
	};
} //namespace window

//---------------------------------------------------------------------------
#endif // MAIN_WINDOW_H
//---------------------------------------------------------------------------
// End of File
//---------------------------------------------------------------------------
