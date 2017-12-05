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

//---------------------------------------------------------------------------
#ifndef BABYGRID_H
#define BABYGRID_H
//---------------------------------------------------------------------------
#include "sys.h"
#include "igrid.h"

//---------------------------------------------------------------------------
// Public structures
//---------------------------------------------------------------------------

namespace grid
{
	class BabyGrid : public IGrid {
	public:
		BabyGrid(
			HINSTANCE&	hParentInstance
		);

		~BabyGrid(
			void
		);

		virtual void 
			createBabyGrid(
			HWND hWnd
			);

	private:
		HINSTANCE&	m_hParentInstance;
		WNDCLASSEX	m_gridClassEx;
		HWND		m_gridhWnd;

	};

} //namespace grid

#endif //BABYGRID_H