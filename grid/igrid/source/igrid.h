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

/****************************************************************************/
/// @name Simple Grid messages.
/// @{

#define SG_ADDCOLUMN WM_USER + 0x01             ///<SimpleGrid_AddColumn()
#define SG_ADDROW WM_USER + 0x02                ///<SimpleGrid_AddRow()
#define SG_SETCOLAUTOWIDTH WM_USER + 0x19       ///<SimpleGrid_SetColAutoWidth()
#define SG_SETHEADERROWHEIGHT WM_USER + 0x21    ///<SimpleGrid_SetHeaderRowHeight()
#define SG_SETITEMDATA WM_USER + 0x26           ///<SimpleGrid_SetItemData()
#define SG_SETROWHEADERWIDTH WM_USER + 0x2B     ///<SimpleGrid_SetRowHeaderWidth()
#define SG_SETSELECTIONMODE WM_USER + 0x2E      ///<SimpleGrid_SetSelectionMode()

namespace grid
{

	class IGrid {
	public:
		virtual 
			~IGrid(
			void
			);

		virtual void 
			createBabyGrid(
			HWND hWnd
			) = 0;
	};

} //namespace grid

#endif //i_grid_h