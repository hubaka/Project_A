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
#ifndef baby_grid_h
#define baby_grid_h
//---------------------------------------------------------------------------
#include "igrid.h"

namespace grid
{
	class BabyGrid : public IGrid {
	public:
		BabyGrid(
			HINSTANCE&	hParentInstance, 
			const char* p_className
		);

		~BabyGrid(
			void
		);

		virtual void 
			regClass(
			void
			);

		virtual void 
			createBabyGrid(
			HWND hWnd
			);

	private:
		HINSTANCE&	m_hParentInstance;
		const char* m_pClassName;
		WNDCLASSEX	m_gridClassEx;
		HWND		m_gridhWnd;

		static LRESULT CALLBACK Grid_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};

} //namespace grid

#endif //baby_grid_h