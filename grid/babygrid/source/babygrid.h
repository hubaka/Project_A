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

namespace grid
{
	class BabyGrid {
	public:
		BabyGrid(HINSTANCE&	hParentInstance, const char* p_className);
		~BabyGrid(void);
		void regClass(void);

	private:
		HINSTANCE&	m_hParentInstance;
		const char* m_pClassName;
		WNDCLASSEX	m_gridClassEx;

		static LRESULT CALLBACK Grid_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};

} //namespace grid

#endif //baby_grid_h