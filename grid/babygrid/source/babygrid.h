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

//---------------------------------------------------------------------------
// Data
//---------------------------------------------------------------------------

/****************************************************************************/
// Public errors

#define SG_ERROR -1

//---------------------------------------------------------------------------
//! \brief An column object
//! \var SGCOLUMN	
//! \brief Pointer to a column
//! \var LPSGCOLUMN
//! \brief This is the data associated with a grid column
//! \struct tagSGCOLUMN
typedef struct tagSGCOLUMN {
	DWORD dwType;           ///< Column type
	LPTSTR lpszHeader;      ///< Column header text
	LPVOID pOptional;       ///< Optional data (ex: combobox choices)
} SGCOLUMN, *LPSGCOLUMN;

/// @var SGITEM
/// @brief A grid item structure

/// @var LPSGITEM
/// @brief Pointer to a grid item structure

/// @struct tagSGITEM
/// @brief This is the address data associated with a grid cell
typedef struct tagSGITEM{
    int col;                 ///< Column number
    int row;                 ///< Row number
    LPARAM lpCurValue;       ///< Item (cell) value
} SGITEM, *LPSGITEM;

//MAXCOLUMNS is set to limit column header text to 4 Hexavigesimal digits
// if grid is configured to display numbered columns.  If the desired max
// is increased then the call to Alphabetize() in the DisplayColumn() method
// needs to be adjusted accordingly.
#define MAXCOLUMNS 475255           ///< Constant

#define GCT_ROWHEADER -1            ///< Constant

#define CHECKED _T("T")             ///< GCT_CHECK checked 
#define UNCHECKED _T("F")           ///< GCT_CHECK unchecked

//---------------------------------------------------------------------------
//! \name Simple Grid messages.
#define SG_ADDCOLUMN WM_USER + 0x01             ///<SimpleGrid_AddColumn()
#define SG_ADDROW WM_USER + 0x02                ///<SimpleGrid_AddRow()
#define SG_SETCOLAUTOWIDTH WM_USER + 0x19       ///<SimpleGrid_SetColAutoWidth()
#define SG_SETHEADERROWHEIGHT WM_USER + 0x21    ///<SimpleGrid_SetHeaderRowHeight()
#define SG_SETITEMDATA WM_USER + 0x26           ///<SimpleGrid_SetItemData()
#define SG_SETROWHEADERWIDTH WM_USER + 0x2B     ///<SimpleGrid_SetRowHeaderWidth()
#define SG_SETROWHEIGHT WM_USER + 0x2C          ///<SimpleGrid_SetRowHeight()
#define SG_SETSELECTIONMODE WM_USER + 0x2E      ///<SimpleGrid_SetSelectionMode()

//---------------------------------------------------------------------------
//! \name Grid Column types.
#define GCT_EDIT        1   ///< Grid column type: Edit
#define GCT_COMBO       2   ///< Grid column type: Combobox
#define GCT_BUTTON      3   ///< Grid column type: Button
#define GCT_CHECK       4   ///< Grid column type: Checkbox
#define GCT_LINK        5   ///< Grid column type: Hyperlink
#define GCT_IMAGE       6   ///< Grid column type: Image

/****************************************************************************/
//! \name Grid Text alignments.
static uint32_t GSA_LEFT        = 1;   ///< Item Text alignment: Left justified
static uint32_t GSA_GENERAL     = 2;   ///< Item Text alignment: Auto selected
static uint32_t GSA_RIGHT       = 3;   ///< Item Text alignment: Right justified

/****************************************************************************/
/// @name Grid Selection mode options.
/// @{

#define GSO_ROWHEADER   1   ///< Grid selection option: Row Header click
#define GSO_CELL        2   ///< Grid selection option: Single cell
#define GSO_FULLROW     3   ///< Grid selection option: Full row select

/****************************************************************************/
/// @name Macroes
/// @{

/// @def SimpleGrid_AddColumn(hGrid,lpszHeader)
///
/// @brief Add a column to the grid.
///
/// @param hGrid The handle of the grid.
/// @param lpColumn Pointer to an SGCOLUMN object.
///
/// @returns The index of the added column if successful, otherwise SG_ERROR 
#define SimpleGrid_AddColumn(hGrid, lpColumn) ((int)(DWORD)SNDMSG((hGrid),SG_ADDCOLUMN, 0,(LPARAM)(LPSGCOLUMN)(lpColumn)))

/// @def SimpleGrid_SetColAutoWidth(hGrid,fSet)
///
/// @brief Configure grid columns to auto adjust to fit contents.
///
/// @note This should be set before adding data to the grid.
///
/// @param hGrid The handle of the grid.
/// @param fSet fSet TRUE to autosize columns to updated content, otherwise FALSE.
///
/// @returns The return value is not meaningful.  
#define SimpleGrid_SetColAutoWidth(hGrid,fSet) (BOOL)SNDMSG((hGrid),SG_SETCOLAUTOWIDTH,(BOOL)(fSet),0L)

/// @def SimpleGrid_SetRowHeaderWidth(hGrid,nWidth)
///
/// @brief Set the width (in pixels) of the row header column.
///
/// @param hGrid The handle of the grid.
/// @param nWidth The desired width (in pixels) of the row headers.
///
/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired column is out of bounds
#define SimpleGrid_SetRowHeaderWidth(hGrid,nWidth) (int)SNDMSG((hGrid),SG_SETROWHEADERWIDTH,0,(LPARAM)(nWidth))

/// @def SimpleGrid_SetHeaderRowHeight(hGrid,iHeight)
///
/// @brief Set the height (in pixels) of the header row.
///
/// @param hGrid The handle of the grid.
/// @param iHeight The desired height (in pixels) of the header row. 
///
/// @returns The return value is not meaningful. 
#define SimpleGrid_SetHeaderRowHeight(hGrid,iHeight) (BOOL)SNDMSG((hGrid),SG_SETHEADERROWHEIGHT,(WPARAM)(int)(iHeight),0L)

/// @def SimpleGrid_SetSelectionMode(hGrid,iMode)
///
/// @brief Set whether and how the selected row will be hilighted.
///
/// @param hGrid The handle of the grid.
/// @param iMode One of the following selection mode options: GSO_ROWHEADER, GSO_CELL, or GSO_FULLROW. 
///
/// @returns The return value is not meaningful.
#define SimpleGrid_SetSelectionMode(hGrid,iMode) (BOOL)SNDMSG((hGrid),SG_SETSELECTIONMODE,(WPARAM)(INT)(iMode),0L)

/// @def SimpleGrid_AddRow(hGrid,lpszHeader)
///
/// @brief Add a row to the grid.
///
/// @param hGrid The handle of the grid.
/// @param lpszHeader Row header text string. 
///
/// @returns The index of the added row if successful, otherwise SG_ERROR  
#define SimpleGrid_AddRow(hGrid,lpszHeader) ((int)(DWORD)SNDMSG((hGrid),SG_ADDROW,0,(LPARAM)(lpszHeader)))

/// @def SimpleGrid_SetItemData(hGrid, pItem)
///
/// @brief Set the content of an individual cell.
///
/// @param hGrid The handle of the grid.
/// @param pItem A pointer to a SGIETEM struct
///
/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds
#define SimpleGrid_SetItemData(hGrid, pItem) (int)SNDMSG((hGrid),SG_SETITEMDATA, 0, (LPARAM)(pItem))

/// @def SimpleGrid_SetRowHeight(hGrid,iHeight)
///
/// @brief Set the height (in pixels) of the rows.
///
/// @param hGrid The handle of the grid.
/// @param iHeight The desired height (in pixels) of the rows. 
///
/// @returns The return value is not meaningful. 
#define SimpleGrid_SetRowHeight(hGrid,iHeight) (BOOL)SNDMSG((hGrid),SG_SETROWHEIGHT,(WPARAM)(int)(iHeight),0L)

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
