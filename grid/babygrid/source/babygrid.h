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
#include "sys.h"
#include "igrid.h"

//---------------------------------------------------------------------------
// Public structures
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//!	\brief An column object
//!	\var SGCOLUMN
//!	\brief Pointer to a column
//!	\var LPSGCOLUMN
//!	\brief This is the data associated with a grid column
//!	\struct tagSGCOLUMN
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

namespace grid
{

	//---------------------------------------------------------------------------
	// Defines and Macros
	//---------------------------------------------------------------------------
	//!	\name Grid Text alignments.
	static const uint32_t GSA_LEFT		= 1;   ///< Item Text alignment: Left justified
	static const uint32_t GSA_GENERAL	= 2;   ///< Item Text alignment: Auto selected
	static const uint32_t GSA_RIGHT		= 3;   ///< Item Text alignment: Right justified

	//---------------------------------------------------------------------------
	//! \name Grid Column types.
	static const uint32_t GCT_EDIT        = 1;   ///< Grid column type: Edit
	static const uint32_t GCT_COMBO       = 2;   ///< Grid column type: Combobox
	static const uint32_t GCT_BUTTON      = 3;   ///< Grid column type: Button
	static const uint32_t GCT_CHECK       = 4;   ///< Grid column type: Checkbox
	static const uint32_t GCT_LINK        = 5;   ///< Grid column type: Hyperlink
	static const uint32_t GCT_IMAGE       = 6;   ///< Grid column type: Image

	/****************************************************************************/
	/// @name Grid Selection mode options.
	/// @{

	static const uint32_t GSO_ROWHEADER   = 1;   ///< Grid selection option: Row Header click
	static const uint32_t GSO_CELL        = 2;	 ///< Grid selection option: Single cell
	static const uint32_t GSO_FULLROW     = 3;   ///< Grid selection option: Full row select


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
			createBabyGrid(
			HWND hWnd
			);

	private:
		HINSTANCE&	m_hParentInstance;
		const char* m_pClassName;
		WNDCLASSEX	m_gridClassEx;
		HWND		m_gridhWnd;

	};

} //namespace grid

#endif //baby_grid_h