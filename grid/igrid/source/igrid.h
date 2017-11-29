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
#ifndef i_grid_h
#define i_grid_h
//---------------------------------------------------------------------------

#define SG_SETCOLAUTOWIDTH WM_USER + 0x01       ///<SimpleGrid_SetColAutoWidth()

namespace grid
{

	class IGrid {
	public:
		virtual 
			~IGrid(
			void
			);

		virtual void 
			regClass(
			void
			) = 0;

		virtual void 
			createBabyGrid(
			HWND hWnd
			) = 0;
	};

} //namespace grid

#endif //i_grid_h