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
#include "sys.h"
#include "resource.h"
#include "errhandle.h"
#include "babygrid.h"

extern "C" IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

namespace grid
{

	//---------------------------------------------------------------------------
	// Data
	//---------------------------------------------------------------------------
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
	//! \brief An column object
	//! \var GRIDCOLUMN
	//! \brief Pointer to a column
	//! \var LPGRIDCOLUMN
	//! \brief This is the data associated with a grid column
	//! \struct tagGRIDCOLUMN
	typedef struct tagGRIDCOLUMN {
		DWORD dwType;           ///< Column type
		INT iWidth;             ///< Column width
		LPVOID pOptional;       ///< Optional data (ex: combobox choices)
		LPVECTOR items;         ///< The row cells associated with this column
	} GRIDCOLUMN   , *LPGRIDCOLUMN;

	//---------------------------------------------------------------------------
	//! \brief Data for this instance of the control
	//! \var INSTANCEDATA
	//! \brief Pointer instance data
	//! \var LPINSTANCEDATA
	//! \brief This is the data associated with an instance of the grid
	//! \struct tagINSTANCEDATA
	typedef struct tagINSTANCEDATA {
		HINSTANCE hInstance;            ///< Handle to this instance
		HWND hWndParent;                ///< Handle of grid's parent
		HWND hwndControl;               ///< Handle to the current cell control
		HMENU gridmenu;                 ///< The child-window identifier of this grid
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

	static LPINSTANCEDATA g_lpInst;     ///< instance data (this) pointer

	//---------------------------------------------------------------------------
	// Defines and Macros
	//---------------------------------------------------------------------------
	static uint32_t VECTOR_INITIAL_CAPACITY		= 16;  ///< Constant
	static uint32_t VECTOR_CAPACITY_DELTA		= 16;  ///< Constant
	static uint32_t INITIAL_COL_WIDTH			= 50;  ///< Constant
	//static uint32_t HEIGHT_DESC					= 80;  ///< Constant

	#define GCT_ROWHEADER -1            ///< Constant

	static errhandle::ErrHandle g_errHandle;

	//---------------------------------------------------------------------------
	// FUNCTION DECLARATIONS
	//---------------------------------------------------------------------------
	static LRESULT CALLBACK Grid_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static BOOL createGrid(HWND hWnd, LPCREATESTRUCT lpCreateStruct); //Grid_OnCreate
	static LPVECTOR createVector(void); // New_Vector
	static BOOL addVector(const LPVECTOR pVector, PVOID object); //Vector_Add
	static LPGRIDITEM createNewItem(LPTSTR szCurValue); //New_Item
	static BOOL addWindowPropList(HWND hControl, LPINSTANCEDATA pInstanceData); //Control_CreateInstanceData
	static LPGRIDCOLUMN addNewColumn(LPSGCOLUMN lpColumn, uint32_t iWidth, LPVECTOR lpVector); //New_Column
	static LPTSTR createNewString(LPTSTR str); //NewString
	
	static int32_t Vector_Size(const LPVECTOR pVector);
	static LRESULT Grid_OnSetColWidth(HWND hWnd, WPARAM wParam, LPARAM lParam);
	static int ColCount(VOID);
	static PVOID Vector_Get(const LPVECTOR pVector, const int index);
	static void SetVisibleColumns(HWND hWnd);
	static int GetColWidth(INT col);
	static void Grid_OnSetHeaderRowHeight(HWND hWnd, WPARAM wParam, LPARAM lParam);
	static LRESULT Grid_OnAddColumn(HWND hWnd, WPARAM wParam, LPARAM lParam);
	static LRESULT Grid_OnAddRow(HWND hWnd, WPARAM wParam, LPARAM lParam);
	static LRESULT Grid_OnSetItemData(HWND hWnd, WPARAM wParam, LPARAM lParam);
	static LPGRIDITEM GetCellData(int col, int row);
	static DWORD GetColType(INT col);
	static VOID RefreshGrid(HWND hWnd);
	static int RowCount(VOID);
	static BOOL OutOfRange(LPSGITEM cell);
	static LPTSTR NewStringArray(LPTSTR szzStr);
	static VOID GetCellRect(HWND hwnd, int col, int row, PRECT prc);
	static int GetAdjacentCol(int startcol, BOOL fNextCol);
	static VOID AdjustParentColWidth(HWND hwnd, INT col, INT row, LPTSTR lpszValue);
	static int FindLongestLine(HDC hdc, LPTSTR text, PSIZE size);
	static BOOL Control_GetInstanceData(HWND hControl, LPINSTANCEDATA *ppInstanceData);

	/// @def StringArray_Replace(lpszTarget, lpszReplace)
	///
	/// @brief Replace an allocated string array.
	///
	/// @param lpszTarget The existing allocated string array.
	/// @param lpszReplace The new string array.
	#define StringArray_Replace(lpszTarget, lpszReplace) \
		(free((lpszTarget)), (lpszTarget) = NewStringArray(lpszReplace))

	/// @def String_Replace(lpszTarget, lpszReplace)
	///
	/// @brief Replace an allocated string.
	///
	/// @param lpszTarget The existing allocated string.
	/// @param lpszReplace The new string.
	#define String_Replace(lpszTarget, lpszReplace) \
		(free((lpszTarget)), (lpszTarget) = createNewString(lpszReplace))

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	BabyGrid::BabyGrid(
		HINSTANCE&	hParentInstance
		) : m_hParentInstance(hParentInstance) {
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

		;

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
		m_gridClassEx.lpszClassName = _T("BabyGridClass");
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

		hControl = CreateWindowEx(0, _T("BabyGridClass"), NULL, WS_CHILD | 
		  WS_TABSTOP, 0, 0, 0, 0, hWnd, (HMENU)IDC_SIMPLEGRID1, hinst, NULL);

		if (hControl == NULL) {
			g_errHandle.getErrorInfo((LPTSTR)L"createBabyGrid");
		}
		//initGridDialog(hWnd);
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

		RECT rect = {0,0,0,0};

		Control_GetInstanceData(hWnd, &g_lpInst);   //Update the instance pointer
		//update the grid width and height variable
		if (NULL != g_lpInst)
		{
			GetClientRect(hWnd, &rect);
			g_lpInst->gridwidth = rect.right - rect.left;
			g_lpInst->gridheight = rect.bottom - rect.top;
		}
		switch (uMsg) {
			HANDLE_MSG(hWnd, WM_CREATE, createGrid);
			case SG_ADDCOLUMN:
				{
				return Grid_OnAddColumn(hWnd, wParam, lParam) - 1; // don't include row header column
				}
			case SG_SETCOLAUTOWIDTH:
				{
					g_lpInst->COLAUTOWIDTH = (BOOL)wParam;
					break;
				}
			case SG_SETROWHEADERWIDTH:
				{
					DWORD dwRtn = 0;
					dwRtn = Grid_OnSetColWidth(hWnd, 0, lParam);
					return dwRtn;
				}
			case SG_SETHEADERROWHEIGHT:
				{
					Grid_OnSetHeaderRowHeight(hWnd, wParam, lParam);
					break;
				}
			case SG_SETSELECTIONMODE:
				{
					g_lpInst->selectionMode = (DWORD) wParam;
					g_lpInst->HIGHLIGHTFULLROW = (GSO_FULLROW == g_lpInst->selectionMode);
					RefreshGrid(hWnd);
					break;
				}
			case SG_ADDROW:
				{
					return Grid_OnAddRow(hWnd, wParam, lParam) - 1; //don't include the column header row
				}
			case SG_SETITEMDATA:
				{
					DWORD dwRtn = 0;
					((LPSGITEM)lParam)->col++; // don't include row header column
					((LPSGITEM)lParam)->row++; // don't include column header row
					dwRtn = Grid_OnSetItemData(hWnd, wParam, lParam);
					((LPSGITEM)lParam)->col--; // restore initial index
					((LPSGITEM)lParam)->row--; // restore initial index
					return dwRtn;
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
		BOOL retVal = true;

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
	createVector(
		void
	) {
		LPVECTOR pvector;
		pvector = (LPVECTOR)SMALLOC(sizeof(VECTOR));
		pvector->_current_capacity = VECTOR_INITIAL_CAPACITY;
		pvector->_data = (PVOID*) SMALLOC(sizeof(PVOID) * pvector->_current_capacity);//dwm 1.9 added cast
		pvector->_size = 0;
		return pvector;
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
	addVector(
		const LPVECTOR pVector, PVOID object
	) {
		bool retVal = true;
		int old_size = Vector_Size(pVector);
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
	Vector_Size(
		const LPVECTOR pVector
	) {
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
	createNewItem(
		LPTSTR szCurValue
	) {
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
	createNewString(
		LPTSTR str
	) {
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
		addNewColumn(LPSGCOLUMN lpColumn, uint32_t iWidth, LPVECTOR lpVector
	) {
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
	addWindowPropList(
		HWND hControl, LPINSTANCEDATA pInstanceData
	) {
		LPINSTANCEDATA pInst = (LPINSTANCEDATA)SMALLOC(sizeof(INSTANCEDATA));
		memmove(pInst, pInstanceData, sizeof(INSTANCEDATA));
		return SetProp(hControl, (LPCTSTR)_T("lpInsData"), pInst);
	}

	/// @brief Get the Instance data associated with this instance.
	///
	/// @param hControl Handle to Current instance.
	/// @param ppInstanceData - Pointer to the address of an INSTANCEDATA struct. 
	///
	/// @returns TRUE if successful
	static BOOL 
	Control_GetInstanceData(HWND hControl, LPINSTANCEDATA *ppInstanceData)
	{
		*ppInstanceData = (LPINSTANCEDATA)GetProp(hControl, (LPCTSTR)_T("lpInsData"));
		if (NULL != *ppInstanceData)
		{
			return TRUE;
		}
		else {
			return FALSE;
		}
	}

	/// @brief Handles SG_SETCOLWIDTH message.
	///
	/// @param hWnd The handle of the grid
	/// @param wParam The index of the column
	/// @param lParam The desired width (in pixels) of the column
	///
	/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds
	static LRESULT 
	Grid_OnSetColWidth(
		HWND hWnd, WPARAM wParam, LPARAM lParam
	) {
		INT iCol = wParam;
		if (iCol > ColCount())
		{
			SetLastError(ERROR_INVALID_INDEX);
			return SG_ERROR;
		}
		RECT rect = {0,0,0,0};
		((LPGRIDCOLUMN)Vector_Get(g_lpInst->data, iCol))->iWidth = (INT)lParam;
		GetClientRect(hWnd, &rect);
		InvalidateRect(hWnd, &rect, FALSE);
		SetVisibleColumns(hWnd);
		return ERROR_SUCCESS;
	}

	/// @brief wrapper to access column count (including the row header column 0)
	///
	/// @returns The number of columns in the underlying data table
	static int 
	ColCount(
		VOID
	) {
		return Vector_Size(g_lpInst->data);
	}

	/// @brief Access an item stored in the VECTOR
	///
	/// @param pVector a pointer to a VECTOR instance
	/// @param index the array index associated with the desired item
	///
	/// @returns The desired item if successful, otherwise NULL
	static PVOID 
	Vector_Get(
		const LPVECTOR pVector, const int index
	) {
		if (index < Vector_Size(pVector))
		{
			return pVector->_data[index];
		}
		else
		{
			return NULL;
		}
	}

	/// @brief Sets the visible columns field to the number of visible columns.
	///
	/// @param hWnd Handle of the grid
	///
	/// @returns VOID
	static void 
	SetVisibleColumns(
		HWND hWnd
	) {
		int j;
		int cols = ColCount();
		int value;
		value = 0;
		for (j = 1; j < cols; j++)
		{
			if (0 < GetColWidth(j))
			{
				value++;
			}
		}
		g_lpInst->visiblecolumns = value;
		SetScrollRange(hWnd, SB_HORZ, 1, value, TRUE);
	}

	/// @brief wrapper to access column width
	///
	/// @param col The column number
	///
	/// @returns The width of the column
	static int 
	GetColWidth(
		INT col
	) {
		LPGRIDCOLUMN lpgc = (LPGRIDCOLUMN)Vector_Get(g_lpInst->data, col);
		if(NULL == lpgc)
			return 0;

		return lpgc->iWidth;
	}

	/// @brief Handles SG_SETHEADERROWHEIGHT message.
	///
	/// @param hWnd The handle of the grid
	/// @param wParam The desired height (in pixels) of the header row
	/// @param lParam This parameter is not used
	///
	/// @returns VOID
	static void 
	Grid_OnSetHeaderRowHeight(
		HWND hWnd, WPARAM wParam, LPARAM lParam
	) {
		if ((int)wParam >= 0)
		{
			RECT rect = {0,0,0,0};

			g_lpInst->headerrowheight = wParam;
			GetClientRect(hWnd, &rect);
			InvalidateRect(hWnd, &rect, FALSE);
		}
	}

	/// @brief Handles SG_ADDCOLUMN message.
	///
	/// @param hWnd The handle of the grid
	/// @param wParam  The type of column to add
	/// @param lParam The Column header text string
	///
	/// @returns The index of the added column if successful, otherwise SG_ERROR 
	static LRESULT 
	Grid_OnAddColumn(
		HWND hWnd, WPARAM wParam, LPARAM lParam
	) {
		BOOL fSuccess = TRUE;
		LPSGCOLUMN lpColumn;
		LPVECTOR temp;

		if(MAXCOLUMNS <= ColCount())
			fSuccess = FALSE;

		if (fSuccess)
		{
			//Create column
			lpColumn = (LPSGCOLUMN)lParam;
			temp = createVector();

			fSuccess = addVector(temp, createNewItem(lpColumn->lpszHeader));
			if (fSuccess)
			{
				//Add rows (if any)
				for (int i = RowCount() - 1; 0 < i; --i)
				{   //Don't include col headers
					fSuccess = addVector(temp, createNewItem(_T("")));
					if (!fSuccess)
						break;
				}
			}
			fSuccess = addVector(g_lpInst->data, addNewColumn(lpColumn, INITIAL_COL_WIDTH, temp));
		}
		if(fSuccess)
		{
			return ColCount() - 1;
		}
		else
		{
			SetLastError(ERROR_OUTOFMEMORY);
			return SG_ERROR;
		}
	}

	/// @brief Handles SG_ADDROW message.
	///
	/// @param hWnd The handle of the grid
	/// @param wParam  This parameter is not used
	/// @param lParam The Column header text string
	///
	/// @returns The index of the added column if successful, otherwise SG_ERROR 
	static LRESULT 
	Grid_OnAddRow(
		HWND hWnd, WPARAM wParam, LPARAM lParam
	) {
		BOOL fSuccess = FALSE;
		LPTSTR tempStr;
		LPVECTOR temp;
		LPGRIDITEM lpgi;
		int nCols = ColCount();

		if (0 < nCols)
		{
			// Add Row header
			temp = ((LPGRIDCOLUMN)Vector_Get(g_lpInst->data, 0))->items;
			tempStr = (LPTSTR)lParam;
			lpgi = createNewItem(tempStr);
			fSuccess = addVector(temp, lpgi);
			if (fSuccess)   //Add an empty row
			{
				for (int i = 1; i < nCols; ++i)
				{
					temp = ((LPGRIDCOLUMN)Vector_Get(g_lpInst->data, i))->items;
					lpgi = createNewItem(_T(""));
					fSuccess = addVector(temp, lpgi);
					if (!fSuccess)
					{
						break;
					}
				}
			}
		}
		if(fSuccess)
		{
			return RowCount() - 1;
		}
		else
		{
			SetLastError(ERROR_OUTOFMEMORY);
			return SG_ERROR;
		}
	}

	/// @brief Handles SG_SETITEMDATA message.
	///
	/// @param hWnd The handle of the grid
	/// @param wParam This parameter is not used
	/// @param lParam A pointer to a SGITEM struct with column and row indexes.
	///
	/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds
	static LRESULT 
	Grid_OnSetItemData(
		HWND hWnd, WPARAM wParam, LPARAM lParam
	) {
		LPTSTR lpszValue = TEXT("");
		TCHAR buf[5]; //Allow for indexes up to 9999 (more images than an image list likely can hold)
		LPGRIDITEM lpgi;
		LPSGITEM lpSGitem = (LPSGITEM)lParam;
		DWORD dwColType;

		if (OutOfRange(lpSGitem))
		{
			SetLastError(ERROR_INVALID_INDEX);
			return SG_ERROR;
		}

		int col = lpSGitem->col;
		int row = lpSGitem->row;

		lpgi = GetCellData(col, row);
		if(NULL == lpgi)
		{
			SetLastError(ERROR_INVALID_DATA);
			return SG_ERROR;
		}

		dwColType = GetColType(col);
		switch (dwColType)
		{
			case GCT_BUTTON: //Make sure that the button text is the same as the cell if updated
				if(NULL != g_lpInst->hwndControl)
					SetWindowText(g_lpInst->hwndControl, (LPTSTR) lpSGitem->lpCurValue);
				//Fall through
			case GCT_EDIT:
			case GCT_COMBO:
			case GCT_LINK:
				lpszValue = (LPTSTR) lpSGitem->lpCurValue;
				break;
			case GCT_CHECK:
				lpszValue = ((BOOL) lpSGitem->lpCurValue) ? CHECKED : UNCHECKED;
				break;
			case GCT_IMAGE:
				_sntprintf(buf, NELEMS(buf), _T("%d"), lpSGitem->lpCurValue);//DWM 1.9: was _stprintf
				lpszValue = buf;
				//lpszValue = _ltot(lpSGitem->lpCurValue, buf, 10);
				break;
		}

		if(GCT_LINK == dwColType)
		{
			// The Link display text and hyperlink are allocated in a double null terminated string buf
			// and so are adjacent.  The pointer lpgi->lpszCurValue points to the first element
			// and the beginning of the array.
			StringArray_Replace(lpgi->lpszCurValue, lpszValue);

			//Increment to next string to get pointer to the link
			lpgi->lpszMisc = lpgi->lpszCurValue + _tcslen(lpgi->lpszCurValue) + 1;
		}
		else
		{
			String_Replace(lpgi->lpszCurValue, lpszValue);
		}

		RECT rect = {0,0,0,0};

		GetCellRect(hWnd, col, row, &rect);
		InvalidateRect(hWnd, &rect, FALSE);

		//adjust the column width if COLAUTOWIDTH==TRUE
		if ((g_lpInst->COLAUTOWIDTH) || (lpSGitem->row == 0))
		{
			AdjustParentColWidth(hWnd, lpSGitem->col, lpSGitem->row, lpszValue);
		}
		return ERROR_SUCCESS;
	}

	/// @brief Gets the contents of a cell.
	///
	/// @param col The column that the cell belongs to
	/// @param row The row index of the cell in the column
	///
	/// @returns The item and data associated with the cell
	static LPGRIDITEM 
	GetCellData(
		int col, int row
	) {
		if(-1 < col && -1 < row)
		{
			LPGRIDCOLUMN lpgc = (LPGRIDCOLUMN)Vector_Get(g_lpInst->data, col);
	        
			if(NULL != lpgc)
			{
				return (LPGRIDITEM)Vector_Get(lpgc->items, row);
			}
		}
		// If we get here we failed
		return NULL;
	}

	/// @brief wrapper to access column type
	///
	/// @param col The column number
	///
	/// @returns The type of the column
	static DWORD 
	GetColType(
		INT col
	) {
		LPGRIDCOLUMN lpgc = (LPGRIDCOLUMN)Vector_Get(g_lpInst->data, col);
		if(NULL == lpgc)
			return 0;

		return lpgc->dwType;
	}

	/// @brief Cause the grid to be repainted.
	///
	/// @param hWnd Handle of the grid
	///
	/// @returns VOID
	static VOID 
	RefreshGrid(
		HWND hWnd
	) {
		RECT rect = {0,0,0,0};
		GetClientRect(hWnd, &rect);
		InvalidateRect(hWnd, &rect, FALSE);
	}

	/// @brief wrapper to access row count (including the column header row 0)
	///
	/// @returns The number of rows in the underlying data table
	static int 
	RowCount(
		VOID
	) {
		LPGRIDCOLUMN lpgc = (LPGRIDCOLUMN)Vector_Get(g_lpInst->data, 0);
		if(NULL == lpgc)
			return 0;

		LPVECTOR temp = lpgc->items;
		return NULL == temp ? 0 : Vector_Size(temp);
	}

	/// @brief Determines if a cells coordinates are out of range.
	///
	/// @param cell A cell struct pointer (x and y coordinate)
	///
	/// @returns TRUE if the cell is out of range (doesn't exist), otherwise FALSE
	static BOOL 
	OutOfRange(
		LPSGITEM cell
	) {
		if ((cell->row > RowCount()) || (cell->col > ColCount()))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	/// @brief Allocate and store a string array (double-null-terminated string).
	///
	/// @param szzStr The double-null-terminated string to store.
	///
	/// @returns a Pointer to the allocated string array.
	static LPTSTR NewStringArray(LPTSTR szzStr)
	{
		if(NULL == szzStr || _T('\0') == *szzStr) szzStr = _T("");

		//Determine total buffer length
		INT iLen = 0;
		//Walk the buffer to find the terminating empty string.
		for (LPTSTR p = szzStr; *p; (p += _tcslen(p) + 1, iLen = p - szzStr)) ;

		//Allocate for array
		LPTSTR tmp = (LPTSTR)calloc(iLen + 1, sizeof(TCHAR));

		if(NULL == tmp)
		{
			return (LPTSTR)calloc(1, sizeof(TCHAR)); 
		}
		return _tmemmove(tmp, szzStr, iLen);
	}

	/// @brief Calculates the rectangle of a cell location.
	///
	/// @param hwnd Handle of the grid
	/// @param col The column that the cell belongs to
	/// @param row The row index of the cell in the column
	/// @param prc A pointer to a RECT to recieve coordinates
	///
	/// @returns VOID
	static VOID 
	GetCellRect(
		HWND hwnd, int col, int row, PRECT prc
	) {
		int offset;
		int j;
		//col and row must be greater than zero

		//get column offset
		//first get col 0 width
		offset = GetColWidth(0);
		for (j = g_lpInst->homecol; j < col; j++)
		{
			offset += GetColWidth(j);
		}
		prc->left = offset;
		prc->right = offset + GetColWidth(col);

		if (g_lpInst->EXTENDLASTCOLUMN)
		{
			//see if this is the last column
			if (!GetAdjacentCol(col, TRUE))
			{
				//extend this column
				RECT trect = {0,0,0,0};
				int temp;
				GetClientRect(hwnd, &trect);
				temp = (offset + (trect.right - prc->left)) - prc->left;
				if (temp > GetColWidth(col))
				{
					prc->right = offset + (trect.right - prc->left);
				}
			}
		}

		//now get the top and bottom of the rect
		offset = g_lpInst->headerrowheight + g_lpInst->titleheight;
		for (j = g_lpInst->homerow; j < row; j++)
		{
			offset += g_lpInst->rowheight;
		}
		prc->top = offset;
		prc->bottom = offset + g_lpInst->rowheight;
	}

	/// @brief Get the next adjacent column or previous adjacent column.
	///
	/// @param startcol The number of the column to begin search.
	/// @param fNextCol TRUE to get the next column
	///                  FALSE to get the previous column.
	///
	/// @returns The number of the desired adjacent column or
	///           0 if no more cols in that direction.
	static int GetAdjacentCol(int startcol, BOOL fNextCol)
	{
		//calls with direction == 1 for right, direction == -1 for left
		//returns 0 if no more cols in that direction, else column number
		int j = startcol;
		int ReturnValue;
		int colCount = ColCount();
		if (fNextCol)
		{
			j++;
		}
		else
		{
			j--;
		}

		while ((j < colCount) && (j > 0) && (0 == GetColWidth(j)))
		{
			if (fNextCol)
			{
				j++;
			}
			else
			{
				j--;
			}
		}
		if ((j < colCount) && (0 < GetColWidth(j)))
		{
			ReturnValue = j;
		}
		else
		{
			ReturnValue = 0;
		}
		return ReturnValue;
	}

	/// @brief Calculate and increase the parent column width based on the length of
	///         the item text.
	///
	/// @param hwnd The handle of the grid
	/// @param col The column of the item
	/// @param row The row of the item
	/// @param lpszValue The item text
	///
	/// @returns VOID
	static VOID AdjustParentColWidth(HWND hwnd, INT col, INT row, LPTSTR lpszValue)
	{
		HDC hdc;
		SIZE size = {0,0};
		int required_width;
		int current_width;
		int required_height;
		int current_height;
		int longestline;
		HFONT holdfont;
		hdc = GetDC(hwnd);
		if (0 == row)
		{
			holdfont = (HFONT)SelectObject(hdc, g_lpInst->hcolumnheadingfont);
		}
		else
		{
			holdfont = (HFONT)SelectObject(hdc, g_lpInst->hfont);
		}
		//if there are \n codes in the string, find the longest line
		longestline = FindLongestLine(hdc, lpszValue, &size);

		required_width = longestline + 5;
		required_height = size.cy;
		//count lines
		{
			int count = 1;
			LPTSTR tbuffer = (LPTSTR) _alloca(sizeof(TCHAR) * (_tcslen(lpszValue) + 1));
			_tcscpy(tbuffer, lpszValue);
			for (int j = 0; j < NELEMS(tbuffer); j++)
			{
				if (tbuffer[j] == _T('\n'))
				{
					count++;
				}
			}
			if ((!g_lpInst->ELLIPSIS) || (row == 0))
			{
				required_height *= count;
			}
			required_height += 5;
		}
		SelectObject(hdc, holdfont);
		ReleaseDC(hwnd, hdc);

		current_width = GetColWidth(col);
		if (0 == row)
		{
			current_height = g_lpInst->headerrowheight;
			if (required_height > current_height)
			{
				SimpleGrid_SetHeaderRowHeight(hwnd, required_height);
			}
		}
		else
		{
			current_height = g_lpInst->rowheight;
			if (required_height > current_height)
			{
				SimpleGrid_SetRowHeight(hwnd, required_height);
			}

		}
		if (required_width > current_width)
		{
			Grid_OnSetColWidth(hwnd, col, required_width);
		}
	}

	/// @brief If there are \n codes in the string, find the longest line
	///
	/// @param hdc Handle of the device context
	/// @param text The text to analyze
	/// @param size Pointer to a SIZE struct to receive text height and width
	///
	/// @returns The length of the longest line of multiline text
	static int FindLongestLine(HDC hdc, LPTSTR text, PSIZE size)
	{
		int longest, lines, j, len;
		len = _tcslen(text);
		LPTSTR temptext = (LPTSTR) _alloca(sizeof(TCHAR) * (len + 1));
		LPTSTR p;
		longest = 0;
		lines = 1;
		for (j = 0; j < len; j++)
		{
			if (text[j] == _T('\n'))
			{
				lines++;
			}
		}
		_tcscpy(temptext, text);
		p = _tcstok(temptext, _T("\n"));
		while (p)
		{
			GetTextExtentPoint32(hdc, p, _tcslen(p), size);
			if (size->cx > longest)
			{
				longest = size->cx;
			}
			p = _tcstok(_T('\0'), _T("\n"));
		}
		return longest;
	}
} //namespace mainwind