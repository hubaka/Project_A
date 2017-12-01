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
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "resource.h"
#include "errhandle.h"
#include "babygrid.h"

extern "C" IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

namespace grid
{

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

	/// @def SimpleGrid_AddRow(hGrid,lpszHeader)
	///
	/// @brief Add a row to the grid.
	///
	/// @param hGrid The handle of the grid.
	/// @param lpszHeader Row header text string. 
	///
	/// @returns The index of the added row if successful, otherwise SG_ERROR  
	#define SimpleGrid_AddRow(hGrid,lpszHeader) ((int)(DWORD)SNDMSG((hGrid),SG_ADDROW,0,(LPARAM)(lpszHeader)))

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

	/// @def SimpleGrid_SetItemData(hGrid, pItem)
	///
	/// @brief Set the content of an individual cell.
	///
	/// @param hGrid The handle of the grid.
	/// @param pItem A pointer to a SGIETEM struct
	///
	/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds
	#define SimpleGrid_SetItemData(hGrid, pItem) (int)SNDMSG((hGrid),SG_SETITEMDATA, 0, (LPARAM)(pItem))

	//---------------------------------------------------------------------------
	// Data
	//---------------------------------------------------------------------------
	#define GCT_ROWHEADER -1            ///< Constant
	static const uint32_t VECTOR_INITIAL_CAPACITY	= 16;	///< Constant
	static const uint32_t VECTOR_CAPACITY_DELTA		= 16;	///< Constant
	static const uint32_t INITIAL_COL_WIDTH			= 50;	///< Constant

	//---------------------------------------------------------------------------
	//!	\brief A flexable array of objects 
	//!	\var VECTOR
	//!	\brief Pointer to a VECTOR object
	//!	\var LPVECTOR
	//!	\brief A data structure to hold a flexable array of objects
	//!	\struct tagVECTOR
	typedef struct tagVECTOR {
		int _current_capacity;  ///< Current size of storage allocation
		PVOID *_data;           ///< Allocated space for item storage
		int _size;              ///< Number of items in storage
	} VECTOR , *LPVECTOR;

	//---------------------------------------------------------------------------
	//!	\brief An column object
	//!	\var GRIDCOLUMN
	//!	\brief Pointer to a column
	//!	\var LPGRIDCOLUMN
	//!	\brief This is the data associated with a grid column
	//!	\struct tagGRIDCOLUMN
	typedef struct tagGRIDCOLUMN {
		DWORD dwType;		///< Column type
		uint32_t iWidth;	///< Column width
		LPVOID pOptional;	///< Optional data (ex: combobox choices)
		LPVECTOR items;		///< The row cells associated with this column
	} GRIDCOLUMN   , *LPGRIDCOLUMN;

	//---------------------------------------------------------------------------
	//!	\brief An item object
	//!	\var GRIDITEM
	//!	\brief Pointer to an item
	//!	\var LPGRIDITEM
	//!	\brief This is the data associated with a grid item
	//!	\struct tagGRIDITEM
	typedef struct tagGRIDITEM {
		LPTSTR lpszCurValue;    ///< Item (cell) value
		LPTSTR lpszMisc;        ///< Item (cell) specific data string
		DWORD dwAllignment;     ///< Item (cell) text allignment
		BOOL fProtected;        ///< Item (cell) protection status
	} GRIDITEM   , *LPGRIDITEM;

	//---------------------------------------------------------------------------
	//! \brief Data for this instance of the control
	//!	\var INSTANCEDATA
	//!	\brief Pointer instance data
	//!	\var LPINSTANCEDATA
	//!	\brief This is the data associated with an instance of the grid
	//!	\struct tagINSTANCEDATA
	typedef struct tagINSTANCEDATA {
		HINSTANCE hInstance;            ///< Handle to this instance
		HWND hWndParent;                ///< Handle of grid's parent
		//HWND hwndControl;               ///< Handle to the current cell control
		//HMENU gridmenu;                 ///< The child-window identifier of this grid
		LPVECTOR data;                  ///< A collection of columns of cells
		LPTSTR title;                   ///< The grid title string
		INT gridwidth;                  ///< Grid width
		INT gridheight;                 ///< Grid height
		INT homerow;                    ///< The index of the home row
		INT homecol;                    ///< The index of the home col
		INT rowheight;                  ///< The height of the rows
		INT leftvisiblecol;             ///< The index of the leftmost visible col
		INT rightvisiblecol;            ///< The index of the rightmost visible col
		INT topvisiblerow;              ///< The index of the topmost visible row
		INT bottomvisiblerow;           ///< The index of the bottommost visible row
		INT headerrowheight;            ///< The height of the header row
		INT cursorrow;                  ///< The row index of the cursor
		INT cursorcol;                  ///< The col index of the cursor
		INT visiblecolumns;             ///< The number of visible columns
		INT titleheight;                ///< The height of the grid title
		INT fontascentheight;           ///< The ascent height of the current font
		INT cWheelRemainder;            ///< Mouse wheel scroll units
		COLORREF clrProtect;            ///< Protected cell background color value
		COLORREF clrHighlight;          ///< Highlight cell background color value
		COLORREF clrGridline;           ///< Grid lines color value
		COLORREF clrHighlightText;      ///< Highlight cell text color value
		COLORREF clrBackground;         ///< The normal background color
		COLORREF clrLink;               ///< The normal link color
		DWORD selectionMode;            ///< The Selection mode (full row, cell, or row header click)
		BOOL HIGHLIGHTFULLROW;          ///< Draw highlight full row flag
		BOOL GRIDHASFOCUS;              ///< Grid has focus flag
		RECT activecellrect;            ///< The boundaries of the active cell
		HFONT hfont;                    ///< The handle to the cell text font
		HFONT hcolumnheadingfont;       ///< The handle of the font for column headers
		HFONT htitlefont;               ///< The handle of the font for the grid title
		BOOL ROWSNUMBERED;              ///< Numbered row option flag
		BOOL COLUMNSNUMBERED;           ///< Numbered col option flag
		BOOL EDITABLE;                  ///< The grid editable flag 
		BOOL EXTENDLASTCOLUMN;          ///< Extend last column flag
		BOOL HSCROLL;                   ///< Horizontal scrolling flag
		BOOL VSCROLL;                   ///< Vertical scrolling flag
		BOOL SHOWINTEGRALROWS;          ///< Show integral rows flag
		BOOL REMEMBERINTEGRALROWS;      ///< Remember the integral rows flag during resizing
		BOOL SIZING;                    ///< Sizing flag
		BOOL ELLIPSIS;                  ///< Draw ellipsis flag
		BOOL COLAUTOWIDTH;              ///< Column auto width flag
		BOOL COLUMNSIZING;              ///< Column resizing flag
		BOOL ALLOWCOLUMNRESIZING;       ///< Allow column resizing flag
		BOOL FORCELEFTJUSTIFY;          ///< Numeric types Left justified
		BOOL DOUBLEBUFFER;              ///< Use double buffering while painting
		BOOL LAUNCLINK;                 ///< Link launch flag
		INT columntoresize;             ///< Field to hold the index of the column to resize
		INT columntoresizeinitsize;     ///< Field to hold the initial size of a column to resize
		INT columntoresizeinitx;        ///< Field to hold the x coord of the mouse button during resize 
		INT cursortype;                 ///< Text, pointer, or column resize cursor
	} INSTANCEDATA , *LPINSTANCEDATA;

	//---------------------------------------------------------------------------
	// Defines and Macros
	//---------------------------------------------------------------------------
	static errhandle::ErrHandle g_errHandle;
	static LPINSTANCEDATA g_lpInst;     ///< instance data (this) pointer

	//---------------------------------------------------------------------------
	// FUNCTION DECLARATIONS
	//---------------------------------------------------------------------------
	static LRESULT CALLBACK Grid_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static BOOL createGrid(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
	static LPVECTOR createVector(void);
	static BOOL addVector(const LPVECTOR pVector, PVOID object);
	static int32_t getVectorSize(const LPVECTOR pVector);
	static LPGRIDITEM createNewItem(LPTSTR szCurValue);
	static LPTSTR createNewString(LPTSTR str);
	static LPGRIDCOLUMN addNewColumn(LPSGCOLUMN lpColumn, uint32_t iWidth, LPVECTOR lpVector);
	static BOOL addWindowPropList(HWND hControl, LPINSTANCEDATA pInstanceData);
	static BOOL initGridDialog(HWND hwnd);
	static void LoadGrid1(HWND hGrid);

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	BabyGrid::BabyGrid(
		HINSTANCE&	hParentInstance,
		const char*	p_className
		) : m_hParentInstance(hParentInstance),
			m_pClassName(p_className) {
			;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	BabyGrid::~BabyGrid(
		void
		) {

		m_pClassName = 0U;

	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void 
	BabyGrid::createBabyGrid(
		HWND hWnd
	)
	{
		//m_gridhWnd = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_BABY_GRID), hWnd, (DLGPROC)Grid_Proc);
		//ShowWindow(m_gridhWnd, SW_SHOW);
		//DialogBox(HINST_THISCOMPONENT, MAKEINTRESOURCE(ID_BABY_GRID), hWnd, (DLGPROC)Grid_Proc);
		static HWND hControl;
		HINSTANCE hinst;

		//Step 1: Registering the Window Class
		m_gridClassEx.cbSize        = sizeof(WNDCLASSEX);
		m_gridClassEx.style         = 0;
		m_gridClassEx.cbClsExtra    = 0;
		m_gridClassEx.cbWndExtra    = 0;
		m_gridClassEx.hIcon         = NULL;
		m_gridClassEx.hIconSm       = NULL;
		m_gridClassEx.hCursor       = NULL;
		m_gridClassEx.hbrBackground = NULL;
		m_gridClassEx.lpszMenuName  = NULL;
		m_gridClassEx.lpszClassName = (LPCWSTR)m_pClassName;
		m_gridClassEx.hInstance     = m_hParentInstance;
		m_gridClassEx.lpfnWndProc   = (WNDPROC)Grid_Proc;

		if(!RegisterClassEx(&m_gridClassEx))
		{
			g_errHandle.getErrorInfo((LPTSTR)L"Grid Class Registration Failed!");
		}

		//Get hinstance if this code is compiled into and called from a dll 
		// as well as if it were compiled into the executable.  (XP and later)
		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		 (LPCTSTR)L"New_SimpleGrid", &hinst);

		//Only need to register the property grid once
		/*if (!aControl)
			aControl = InitSimpleGrid(hinst);*/

		hControl = CreateWindowEx(0, (LPCWSTR)m_pClassName, NULL, WS_CHILD | 
		  WS_TABSTOP, 0, 0, 0, 0, hWnd, (HMENU)ID_BABY_GRID, GetModuleHandle(NULL), NULL);

		if (hControl == NULL) {
			g_errHandle.getErrorInfo((LPTSTR)L"createBabyGrid");
		}
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	LRESULT	CALLBACK 
	Grid_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
	
		switch (uMsg) {
			HANDLE_MSG(hWnd, WM_CREATE, createGrid);
			case WM_INITDIALOG:
			{
				initGridDialog(hWnd);
				break;
			}
			default: 
				{
					return DefWindowProc(hWnd, uMsg, wParam, lParam);
					break;
				}
		}
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	BOOL 
	createGrid(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
	{
		BOOL retVal = false;

		INSTANCEDATA instance;
		memset(&instance, 0, sizeof(INSTANCEDATA));
		instance.hInstance = lpCreateStruct->hInstance;
		instance.hWndParent = lpCreateStruct->hwndParent;
		//Create the data table
		instance.data = createVector();
		//Add Column 0 (Row headers)
		LPVECTOR temp = createVector();
		//First element (the column name) Should be blank for row header column
		retVal = addVector(temp, createNewItem(_T("")));

		if (retVal)
		{
			SGCOLUMN sgc;
			sgc.dwType = GCT_ROWHEADER;
			sgc.lpszHeader = _T("");
			sgc.pOptional = NULL;
			retVal = addVector(instance.data, addNewColumn(&sgc, INITIAL_COL_WIDTH, temp));
		}

		if (retVal)
		{
			instance.homerow = 1;
			instance.homecol = 1; 
			instance.rowheight = 21;
			instance.headerrowheight = 21;
			instance.ROWSNUMBERED = TRUE;
			instance.COLUMNSNUMBERED = TRUE;
			instance.EDITABLE = TRUE;
			instance.cursorcol = 1;
			instance.cursorrow = 1;
			instance.HIGHLIGHTFULLROW = FALSE;
			instance.DOUBLEBUFFER = TRUE;
			instance.cWheelRemainder = 0;
			instance.clrProtect = GetSysColor(COLOR_WINDOW);
			instance.clrHighlight = GetSysColor(COLOR_HIGHLIGHT);
			instance.clrLink = GetSysColor(COLOR_HIGHLIGHT);
			instance.LAUNCLINK = TRUE;
			instance.clrGridline = GetSysColor(COLOR_BTNFACE);
			instance.clrHighlightText = GetSysColor(COLOR_HIGHLIGHTTEXT);
			instance.clrBackground = GetSysColor(COLOR_WINDOW);
			instance.title = createNewString(_T(""));
			instance.titleheight = 0;
			instance.EXTENDLASTCOLUMN = TRUE;
			instance.SHOWINTEGRALROWS = TRUE;
			instance.SIZING = FALSE;
			instance.ELLIPSIS = TRUE;
			instance.COLAUTOWIDTH = FALSE;
			instance.COLUMNSIZING = FALSE;
			instance.ALLOWCOLUMNRESIZING = FALSE;
			instance.FORCELEFTJUSTIFY = FALSE;
			instance.cursortype = 0;
			instance.hcolumnheadingfont = NULL;
			instance.htitlefont = NULL;

			retVal = addWindowPropList(hWnd, &instance);
		}
		initGridDialog(hWnd);
		return retVal;
	}


	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	static LPVECTOR 
	createVector(void)
	{
		LPVECTOR pVector;
		pVector = (LPVECTOR)SMALLOC(sizeof(VECTOR));
		pVector->_current_capacity = VECTOR_INITIAL_CAPACITY;
		pVector->_data = (PVOID*) SMALLOC(sizeof(PVOID) * pVector->_current_capacity);//DWM 1.9 Added cast
		pVector->_size = 0;
		return pVector;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		Add an item to a VECTOR
	//!
	//! \param[in]	 pVector a pointer to a VECTOR instance
	//! \param[in]	 object a pointer to an item to store in the vector
	//!
	//! \return		TRUE if successful
	//!
	static BOOL 
	addVector(const LPVECTOR pVector, PVOID object)
	{
		bool retVal = true;
		int old_size = getVectorSize(pVector);
		int new_capacity;
		PVOID *new_data;//DWM 1.9: Added more explicit pointer

		(pVector->_size)++;
		if (old_size == pVector->_current_capacity)
		{
			new_capacity = pVector->_current_capacity + VECTOR_CAPACITY_DELTA;
			new_data = (PVOID *) SMALLOC(sizeof(PVOID) * new_capacity);//DWM 1.9: Added cast

			memcpy(new_data, pVector->_data, sizeof(PVOID) * old_size);
			free(pVector->_data);
			(pVector->_data) = new_data;
			pVector->_current_capacity = new_capacity;
		}
		(pVector->_data)[old_size] = object;
		return retVal;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		Gets the number of objects allocated in the array
	//!
	//! \param[in]	 pVector a pointer to a VECTOR instance
	//!
	//! \return		The number of objects
	//!
	static int32_t 
	getVectorSize(const LPVECTOR pVector)
	{
		return (NULL == pVector) ? 0 : pVector->_size;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		Allocate and populate a grid item data structure.
	//!
	//! \param[in]	 szCurValue The item's current value.
	//!
	//! \return		 a Pointer to the allocated grid item.
	//!
	static LPGRIDITEM 
	createNewItem(LPTSTR szCurValue)
	{
		LPGRIDITEM lpItem = (LPGRIDITEM)calloc(1, sizeof(GRIDITEM));

		lpItem->lpszMisc = NULL; // <- used to store pointer to data allocated
								 // to lpItem->lpszCurValue (if it is an array)
		lpItem->dwAllignment = GSA_GENERAL;
		lpItem->lpszCurValue = createNewString(szCurValue);
		return lpItem;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		Allocate and store a string.
	//!
	//! \param[in]	 str The string to store.
	//!
	//! \return		 a Pointer to the allocated string.
	//!
	static LPTSTR 
	createNewString(LPTSTR str)
	{
		if (NULL == str || _T('\0') == *str)
			str = _T("");
		LPTSTR tmp = (LPTSTR)calloc(_tcslen(str) + 1, sizeof(TCHAR));

		if (NULL == tmp)
		{
			return (LPTSTR)calloc(1, sizeof(TCHAR));
		}
		return _tmemmove(tmp, str, _tcslen(str));
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		Allocate and populate a grid column data structure.
	//!
	//! \param[in]	 lpColumn The column data.
	//! \param[in]	 iWidth The column width.
	//! \param[in]	 lpVector The column vector.
	//!
	//! \return		 a Pointer to the allocated grid column.
	//!
	static LPGRIDCOLUMN 
		addNewColumn(LPSGCOLUMN lpColumn, uint32_t iWidth, LPVECTOR lpVector)
	{
		LPGRIDCOLUMN lpCol = (LPGRIDCOLUMN)calloc(1, sizeof(GRIDCOLUMN));

		lpCol->dwType = lpColumn->dwType;
		switch(lpCol->dwType)
		{
			case GCT_ROWHEADER:
				lpCol->pOptional = NULL;
				break;
			case GCT_EDIT:
				lpCol->pOptional = NULL;
				break;
			//case GCT_COMBO:
			//	lpCol->pOptional = NewStringArray((LPTSTR)lpColumn->pOptional);
			//	break;
			case GCT_BUTTON:
				lpCol->pOptional = NULL;
				break;
			//case GCT_CHECK:
			//	lpCol->pOptional = NULL;
			//	break;
			//case GCT_LINK:
			//	lpCol->pOptional = NULL;
			//	break;
			//case GCT_IMAGE:
			//	lpCol->pOptional = (HIMAGELIST)lpColumn->pOptional;
			//	break;
		}
		lpCol->iWidth = iWidth;
		lpCol->items = lpVector;
		return lpCol;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		Allocate the Instance data associated with this instance.
	//!
	//! \param[in]	 lpColumn The column data.hControl Handle to current instance.
	//! \param[in]	 pInstanceData Pointer to an INSTANCEDATA struct
	//!
	//! \return		 TRUE if successful
	//!
	static BOOL 
	addWindowPropList(HWND hControl, LPINSTANCEDATA pInstanceData)
	{
		LPINSTANCEDATA pInst = (LPINSTANCEDATA)SMALLOC(sizeof(INSTANCEDATA));
		memmove(pInst, pInstanceData, sizeof(INSTANCEDATA));
		return SetProp(hControl, (LPCTSTR)_T("lpInsData"), pInst);
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	 
	//!
	//! \return	
	//!
	static BOOL 
	initGridDialog(HWND hwnd)
	{
		//Get window handles
		HWND hgrid1 = GetDlgItem(hwnd, ID_BABY_GRID);
		//HWND hgrid1 = GetDlgItem(hwnd, IDC_SIMPLEGRID1);
		//hgrid2 = GetDlgItem(hwnd, IDC_SIMPLEGRID2);
		//hgrid3 = GetDlgItem(hwnd, IDC_SIMPLEGRID3);
		//hgrid4 = GetDlgItem(hwnd, IDC_SIMPLEGRID4);
		//hgrid5 = GetDlgItem(hwnd, IDC_SIMPLEGRID5);
		//htab =  GetDlgItem(hwnd, IDC_TAB);

		//
		//Configure tab
		//

		//TCITEM tie;
		//tie.mask = TCIF_TEXT;
		//tie.pszText = _T("Original Baby Grid Demo");
		//TabCtrl_InsertItem(htab, 0, &tie);
		//tie.pszText = _T("Big Data");
		//TabCtrl_InsertItem(htab, 1, &tie);
		//tie.pszText = _T("Column Types");
		//TabCtrl_InsertItem(htab, 2, &tie);
		//tie.pszText = _T("Insert/Delete Rows");
		//TabCtrl_InsertItem(htab, 3, &tie);

		//
		//Configure grids
		//

		//set grid1 (the properties grid) to automatically size columns 
		//based on the length of the text entered into the cells
		SimpleGrid_SetColAutoWidth(hgrid1, TRUE);
		//I don't want a row header, so make it 0 pixels wide
		SimpleGrid_SetRowHeaderWidth(hgrid1,0);
		//this grid won't use column headings, set header row height = 0
		SimpleGrid_SetHeaderRowHeight(hgrid1, 0);
		//on selection hilight full row
		SimpleGrid_SetSelectionMode(hgrid1, GSO_FULLROW);
		//populate grid1 with data
		LoadGrid1(hgrid1);

		////Set the heading font for Grid 2
		//HFONT hFont = CreateFont(20, 0, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE,
		//	ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		//	PROOF_QUALITY, VARIABLE_PITCH | FF_MODERN, _T("ARIEL"));

		//SimpleGrid_SetHeadingFont(hgrid2,hFont);

		////make grid2 header row to initial height of 21 pixels
		//SimpleGrid_SetHeaderRowHeight(hgrid2, 21);
		////on row header selection hilight full row, otherwise individual cell
		//SimpleGrid_SetSelectionMode(hgrid2, GSO_ROWHEADER);
		////do not allow in cell editing of all grid items initially
		//SimpleGrid_EnableEdit(hgrid2,FALSE);

		////populate grid2 with initial demo data
		//LoadGrid2(hgrid2);

		////make grid3 header row to initial height of 21 pixels
		//SimpleGrid_SetHeaderRowHeight(hgrid3, 21);
		////use column header text
		//SimpleGrid_SetColsNumbered(hgrid3,FALSE);
		////use row header text
		//SimpleGrid_SetRowsNumbered(hgrid3,FALSE);
		////last column standard width
		//SimpleGrid_ExtendLastColumn(hgrid3,FALSE);
		////vertical scroll set to non integral rows
		//SimpleGrid_ShowIntegralRows(hgrid3,FALSE);
		////on row header selection hilight full row, otherwise individual cell
		//SimpleGrid_SetSelectionMode(hgrid3, GSO_ROWHEADER);

		////Include a title for this grid
		//SimpleGrid_SetTitleFont(hgrid3,hFont);
		//SimpleGrid_SetTitleHeight(hgrid3, 21);
		//SimpleGrid_SetTitle(hgrid3,_T("Grid's window text displayed here."));

		////populate grid3 with big data
		//LoadGrid3(hgrid3);

		////make grid4 header row to initial height of 21 pixels
		//SimpleGrid_SetHeaderRowHeight(hgrid4, 21);
		////allow column resizing
		//SimpleGrid_SetAllowColResize(hgrid4, TRUE);
		////use column header text
		//SimpleGrid_SetColsNumbered(hgrid4,FALSE);
		////on row header selection hilight full row, otherwise individual cell
		//SimpleGrid_SetSelectionMode(hgrid4, GSO_CELL);

		////populate grid4 with different column types
		//LoadGrid4(hgrid4);

		//Force the display of the vertical scroll in grids (if necessary)
		/*RECT rect;
		GetClientRect(hwnd, &rect);
		MoveWindow(hgrid2, rect.right / 3, 0, 
			rect.right - rect.right / 3, rect.bottom + 1, FALSE);*/

		return TRUE;
	}

	static void 
	LoadGrid1(HWND hGrid)
	{
		HWND retHandle;
		// Add some columns

		// Column type
		// Column header text
		// Optional data (ex: combobox choices)
		SGCOLUMN lpColumns[] = {
			GCT_EDIT, _T(""),  NULL,
			GCT_CHECK, _T(""),  NULL
		};
		for(int i = 0; i < NELEMS(lpColumns); ++i)
			SimpleGrid_AddColumn(hGrid, &lpColumns[i]);

		// Add some rows
		for(int i = 0; i < 9; ++i) 
			SimpleGrid_AddRow(hGrid, _T(""));

		// Column number
		// Row number
		// Item (cell) value
		SGITEM lpItems[] = {
			0, 0, (LPARAM)_T("User Column Resizing"),
			1, 0, (LPARAM) FALSE,
			0, 1, (LPARAM)_T("User Editable"),
			1, 1, (LPARAM) FALSE,
			0, 2, (LPARAM)_T("Show Ellipsis"),
			1, 2, (LPARAM) TRUE,
			0, 3, (LPARAM)_T("Auto Column Size"),
			1, 3, (LPARAM) FALSE,
			0, 4, (LPARAM)_T("Extend Last Column"),
			1, 4, (LPARAM) TRUE,
			0, 5, (LPARAM)_T("Numbered Columns"),
			1, 5, (LPARAM) TRUE,
			0, 6, (LPARAM)_T("Numbered Rows"),
			1, 6, (LPARAM) TRUE,
			0, 7, (LPARAM)_T("Highlight Row"),
			1, 7, (LPARAM) TRUE,
			0, 8, (LPARAM)_T("Show Gridlines"),
			1, 8, (LPARAM) TRUE
		};

		for(int i = 0; i < NELEMS(lpItems); ++i)
		{
			SimpleGrid_SetItemData(hGrid, &lpItems[i]);
		}

		//make the properties grid have the focus when the application starts
		retHandle = SetFocus(hGrid);
		if (NULL == retHandle) {
			g_errHandle.getErrorInfo((LPTSTR)L"SetFocus Failed!");
		}
	}

} //namespace mainwind