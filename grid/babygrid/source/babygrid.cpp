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
#include <commctrl.h>
#include "sys.h"
#include "resource.h"
#include "errhandle.h"
#include "babygrid.h"

extern "C" IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

namespace grid
{

	/// @def HEIGHT(rect)
	///
	/// @brief Given a RECT, Computes height.
	///
	/// @param rect A RECT struct.
	#define HEIGHT(rect) ((LONG)(rect.bottom - rect.top))

	/// @def WIDTH(rect)
	///
	/// @brief Given a RECT, Computes width.
	///
	/// @param rect A RECT struct.
	#define WIDTH(rect) ((LONG)(rect.right - rect.left))
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
	static void Grid_OnSetFont(HWND hwnd, HFONT hfont, BOOL fRedraw);
	static VOID Grid_OnPaint(HWND hwnd);
	static VOID CalcVisibleCellBoundaries(VOID);
	static VOID DisplayTitle(HWND hwnd, HFONT hfont, HDC hdc, RECT rc);
	static VOID DisplayColumn(HWND hwnd, int col, int offset, HFONT hfont, HFONT hcolumnheadingfont, HDC hdc);
	static VOID ShowVscroll(HWND hwnd);
	static VOID ShowHscroll(HWND hwnd);
	static BOOL Alphabetize(INT num, LPTSTR buf, INT iSize);
	static LPVOID GetColOptional(INT col);
	static HFONT Font_SetUnderline(HWND hwnd, BOOL fUnderline);
	static BOOL IsNumeric(LPTSTR data);

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
		//GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		// (LPCTSTR)L"New_SimpleGrid", &hinst);

		//hControl = CreateWindowEx(0, _T("BabyGridClass"), NULL, WS_CHILD | 
		//  WS_TABSTOP, 0, 0, 0, 0, hWnd, (HMENU)IDC_SIMPLEGRID1, hinst, NULL);

		//if (hControl == NULL) {
		//	g_errHandle.getErrorInfo((LPTSTR)L"createBabyGrid");
		//}
		
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
		switch (uMsg) 
		{
			HANDLE_MSG(hWnd, WM_CREATE, createGrid);
			HANDLE_MSG(hWnd, WM_SETFONT, Grid_OnSetFont);
			HANDLE_MSG(hWnd, WM_PAINT, Grid_OnPaint);
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
			case WM_GETDLGCODE: // If this control is in a dialog
				{
					return DLGC_WANTALLKEYS; // Send all key presses to this proc
				}
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
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

	/// @brief Handle WM_SETFONT message.
	///
	/// @param hwnd Handle of grid.
	/// @param hfont A handle to the font. If this parameter is NULL,
	///               the control uses the default system font to draw text.
	/// @param fRedraw If TRUE this control should be redrawn immediately. 
	///
	/// @returns VOID.
	static void 
	Grid_OnSetFont(
		HWND hwnd, HFONT hfont, BOOL fRedraw
	){
		g_lpInst->hfont = hfont;
		if (!g_lpInst->hcolumnheadingfont)
		{
			g_lpInst->hcolumnheadingfont = hfont;
		}
		if (!g_lpInst->htitlefont)
		{
			g_lpInst->htitlefont = hfont;
		}
		if (fRedraw)
			RefreshGrid(hwnd);
	}

	/// @brief Handles WM_PAINT message.
	///
	/// @param hwnd Handle of grid.
	///
	/// @returns VOID.
	static VOID Grid_OnPaint(HWND hwnd)
	{
		PAINTSTRUCT ps;
		HDC hdc, hdcOrig = NULL;
		HBITMAP hbmp = NULL;

		CalcVisibleCellBoundaries();

		hdc = BeginPaint(hwnd, &ps);

		if (g_lpInst->DOUBLEBUFFER)
		{
			hdcOrig = hdc;

			hdc = CreateCompatibleDC(hdcOrig);
			if (!hdc)
				return;

			hbmp = CreateCompatibleBitmap(hdcOrig, ps.rcPaint.right, ps.rcPaint.bottom);
			if (!hbmp)
			{
				DeleteDC(hdc);
				return;
			}
		}
		SelectObject(hdc, hbmp);

		//display title
		DisplayTitle(hwnd, g_lpInst->htitlefont, hdc, ps.rcPaint);

		//display Row headers (column 0);
		DisplayColumn(hwnd, 0, 0, g_lpInst->hfont, g_lpInst->hcolumnheadingfont, hdc);
		{
			int c, j, k, offset;
			offset = GetColWidth(0);
			j = g_lpInst->leftvisiblecol;
			k = g_lpInst->rightvisiblecol;
			for (c = j; c <= k; c++)
			{
				DisplayColumn(hwnd, c, offset, g_lpInst->hfont, g_lpInst->hcolumnheadingfont, hdc);
				offset += GetColWidth(c);
			}
		}
		if (hbmp)
		{
			BitBlt(hdcOrig, ps.rcPaint.left, ps.rcPaint.top, WIDTH(ps.rcPaint), 
				HEIGHT(ps.rcPaint), hdc, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);

			DeleteObject(hbmp);
			DeleteDC(hdc);
		}

		EndPaint(hwnd, &ps);
	}

	/// @brief Wrapper for setting cell coordinates.
	///
	/// @returns VOID
	static VOID CalcVisibleCellBoundaries(VOID)
	{
		int gridx, gridy;
		int j;
		int cols = ColCount() - 1;  //Exclude header
		int rows = RowCount() - 1;  //Exclude header
		gridx = g_lpInst->gridwidth;
		gridy = g_lpInst->gridheight;

		j = g_lpInst->homecol;
		g_lpInst->leftvisiblecol = g_lpInst->homecol;
		g_lpInst->topvisiblerow = g_lpInst->homerow;
		//calc columns visible
		//first subtract the width of col 0;
		gridx = gridx - GetColWidth(0);
		do
		{
			gridx = gridx - GetColWidth(j);
			j++;
		}
		while ((j < cols) && (gridx >= 0));

		if (j > cols)
		{
			j = cols;
		}
		g_lpInst->rightvisiblecol = j;

		//calc rows visible;
		gridy = gridy - g_lpInst->headerrowheight;
		j = g_lpInst->homerow;
		do
		{
			gridy = gridy - g_lpInst->rowheight;
			j++;
		}
		while ((gridy > 0) && (j < rows));

		if (j > rows)
		{
			j = rows;
		}
		g_lpInst->bottomvisiblerow = j;
	}

	/// @brief Draw the grid Title with the specified font.
	///
	/// @param hwnd Handle of the grid
	/// @param hfont Handle of the desired font
	/// @param hdc Handle of the current device context
	/// @param rc A rectangle of the bounds of the client area
	///
	/// @returns VOID
	static VOID DisplayTitle(HWND hwnd, HFONT hfont, HDC hdc, RECT rc)
	{
		HFONT hOldfont;

		SetBkMode(hdc, TRANSPARENT);
		hOldfont = (HFONT)SelectObject(hdc, hfont);
		rc.bottom = g_lpInst->titleheight;
		DrawEdge(hdc, &rc, EDGE_ETCHED, BF_MIDDLE | BF_RECT | BF_ADJUST);
		DrawTextEx(hdc, g_lpInst->title, -1, &rc, DT_END_ELLIPSIS | DT_CENTER | DT_WORDBREAK | DT_NOPREFIX, NULL);
		SelectObject(hdc, hOldfont);
	}

	/// @brief Draw a column of cells in the grid.
	///
	/// @param hwnd Handle of the grid
	/// @param col The index of the column to draw
	/// @param offset number of pixles from the left to begin drawing
	/// @param hfont The handle of the desired cell font
	/// @param hcolumnheadingfont The handle of the font used for the headers
	/// @param hdc Handle of the current device context
	///
	/// @returns VOID
	static VOID DisplayColumn(HWND hwnd, int col, int offset, HFONT hfont, HFONT hcolumnheadingfont, HDC hdc)
	{
		RECT rect = {0,0,0,0};
		RECT rectsave = {0,0,0,0};
		HFONT holdfont;
		HBRUSH hbrush, holdbrush;
		HPEN hpen, holdpen;
		LPGRIDITEM lpgi;
		BOOL isProtected;
		BOOL isNumeric = FALSE;
		int row, iColumnType;

		//max 1024 chars display in an excel worksheet cell so
		// I'll use that for a max buffer size
		static TCHAR buffer[1025]; 
		memset(&buffer, 0, sizeof(buffer));

		if (0 == GetColWidth(col))
		{
			return;
		}
		iColumnType = GetColType(col);

		SetBkMode(hdc, TRANSPARENT);
		ShowHscroll(hwnd);
		ShowVscroll(hwnd);

		rect.left = offset;
		rect.top = g_lpInst->titleheight;
		rect.right = GetColWidth(col) + offset;
		rect.bottom = g_lpInst->headerrowheight + g_lpInst->titleheight;

		if (g_lpInst->EXTENDLASTCOLUMN)
		{
			//see if this is the last column
			if (!GetAdjacentCol(col, TRUE))
			{
				//extend this column
				RECT trect = {0,0,0,0};

				GetClientRect(hwnd, &trect);

				rect.right = offset + (trect.right - rect.left);
			}
		}
		else //Not extended column
		{
			if (!GetAdjacentCol(col, TRUE))
			{
				//repaint right side of grid
				RECT trect = {0,0,0,0};
				GetClientRect(hwnd, &trect);
				trect.left = offset + (rect.right - rect.left);
				holdbrush = (HBRUSH)SelectObject(hdc, GetStockObject(GRAY_BRUSH));
				holdpen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
				Rectangle(hdc, trect.left, trect.top + g_lpInst->titleheight, trect.right + 1, trect.bottom + 1);
				//Don't attempt to delete stock objects
				SelectObject(hdc, holdbrush);
				SelectObject(hdc, holdpen);
			}
		}

		//
		//display column header cell (Row 0)
		//
		row = 0;

		if (col > 0)
		{
			if (g_lpInst->COLUMNSNUMBERED)  //Print hexavigesimal digits
			{
				Alphabetize(col, buffer, 4); //Max 4 place column number EX: ABCD
			}
			else //Print column header text (if any)
			{
				lpgi = GetCellData(col, row);
				_tcscpy(buffer, NULL == lpgi ? _T("") : lpgi->lpszCurValue);
			}
		}
		rectsave = rect;

		SetTextColor(hdc, IsWindowEnabled(hwnd) ? 
			GetSysColor(COLOR_WINDOWTEXT) : GetSysColor(COLOR_GRAYTEXT));

		DrawEdge(hdc, &rect, EDGE_ETCHED, BF_MIDDLE | BF_RECT | BF_ADJUST);

		holdfont = (HFONT)SelectObject(hdc, hcolumnheadingfont);
		DrawTextEx(hdc, buffer, -1, &rect, DT_END_ELLIPSIS | DT_CENTER | DT_WORDBREAK | DT_NOPREFIX, NULL);
		SelectObject(hdc, holdfont);//Don't delete 

		rect = rectsave;

		//
		// Display visible cells
		//
		row = g_lpInst->topvisiblerow;
		//set font for grid body
		SelectObject(hdc, hfont);

		while (row <= g_lpInst->bottomvisiblerow)
		{
			rect.top = rect.bottom;
			rect.bottom = rect.top + g_lpInst->rowheight;
			rectsave = rect;
			lpgi = GetCellData(col, row);
			isProtected = NULL == lpgi ? 0 : lpgi->fProtected;
			_tcscpy(buffer, NULL == lpgi ? _T("") : lpgi->lpszCurValue);

			// Draw Cell if not row header
			if(GCT_ROWHEADER != iColumnType)// no need to draw row header twice
			{
				//set cursor (or entire cursor row) to different display color
				if (((g_lpInst->HIGHLIGHTFULLROW && row == g_lpInst->cursorrow) ||
					(! g_lpInst->HIGHLIGHTFULLROW && row == g_lpInst->cursorrow && col == g_lpInst->cursorcol))
					&& g_lpInst->GRIDHASFOCUS)
				{
					switch(iColumnType)
					{
						case GCT_ROWHEADER:
						case GCT_BUTTON:
							SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
							break;
						default:
							SetTextColor(hdc, g_lpInst->clrHighlightText);
							break;
					}
					hbrush = CreateSolidBrush(g_lpInst->clrHighlight);
				}
				else //Non hilight colors
				{
					if (isProtected)
					{
						SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
						hbrush = CreateSolidBrush(g_lpInst->clrProtect);
					}
					else //Normal in cell text color
					{
						if (GCT_LINK == iColumnType)
							SetTextColor(hdc, g_lpInst->clrLink);
						else
							SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));

						hbrush = CreateSolidBrush(g_lpInst->clrBackground);
					}
				}
				hpen = CreatePen(PS_SOLID, 1, g_lpInst->clrGridline);
				holdbrush = (HBRUSH)SelectObject(hdc, hbrush);
				holdpen = (HPEN)SelectObject(hdc, hpen);
				Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
				DeleteObject(SelectObject(hdc, holdpen));
				DeleteObject(SelectObject(hdc, holdbrush));
			}

			// Draw row header as well as controls and text over other cells
			switch(iColumnType)
			{
				case GCT_ROWHEADER:
				{
					if (g_lpInst->ROWSNUMBERED)
					{
						isNumeric = TRUE;
						wsprintf(buffer, _T("%d"), row);
					}
					else
						isNumeric = FALSE;

					SetTextColor(hdc, IsWindowEnabled(hwnd) ? 
						GetSysColor(COLOR_WINDOWTEXT) : GetSysColor(COLOR_GRAYTEXT));

					DrawEdge(hdc, &rect, EDGE_ETCHED, BF_MIDDLE | BF_RECT | BF_ADJUST);

					UINT uFormat = DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX;
					uFormat |= isNumeric ? DT_RIGHT : DT_LEFT;

					DrawTextEx(hdc, buffer, -1, &rect, uFormat, NULL);
				}
					break;
				case GCT_BUTTON:
				{
					InflateRect(&rect, -2, -2);
					DrawFrameControl(hdc, &rect, DFC_BUTTON, DFCS_BUTTONPUSH);
					if (g_lpInst->ELLIPSIS)
					{
						DrawTextEx(hdc, buffer, -1, &rect, DT_END_ELLIPSIS | DT_LEFT | DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_NOPREFIX, NULL);
					}
					else
					{
						DrawTextEx(hdc, buffer, -1, &rect, DT_LEFT | DT_VCENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_NOPREFIX, NULL);
					}
				}
					break;
				case GCT_COMBO:
				{
					//Calculate and draw drop down button
					RECT rectButton = {0,0,0,0};
					CopyRect(&rectButton, &rect);
					InflateRect(&rectButton, -2, -2);
					rectButton.left = rectButton.right - GetSystemMetrics(SM_CXVSCROLL);
					rectButton.top += (HEIGHT(rectButton) - GetSystemMetrics(SM_CYVSCROLL)) / 2;
					rectButton.bottom = rectButton.top + GetSystemMetrics(SM_CYVSCROLL);
					DrawFrameControl(hdc, &rectButton, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);

					rect.right = rectButton.left - 1;
					rect.left += 5;

					if (g_lpInst->ELLIPSIS)
					{
						DrawTextEx(hdc, buffer, -1, &rect, DT_END_ELLIPSIS | DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX, NULL);
					}
					else
					{
						DrawTextEx(hdc, buffer, -1, &rect, DT_LEFT | DT_VCENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_NOPREFIX, NULL);
					}
				}
					break;
				case GCT_CHECK:
				{
					//Calculate and draw checkbox
					RECT rectCheck = {0,0,0,0};
					CopyRect(&rectCheck, &rect);

					rectCheck.left += (WIDTH(rect) - GetSystemMetrics(SM_CXVSCROLL)) /2;
					rectCheck.top += (HEIGHT(rect) - GetSystemMetrics(SM_CYVSCROLL)) /2;
					rectCheck.right = rectCheck.left + GetSystemMetrics(SM_CXVSCROLL);
					rectCheck.bottom = rectCheck.top + GetSystemMetrics(SM_CYVSCROLL);

					DrawFrameControl(hdc, &rectCheck, DFC_BUTTON, DFCS_BUTTONCHECK |
						(0 == _tcsicmp(buffer, CHECKED) ? DFCS_CHECKED : 0));
				}
					break;
				case GCT_IMAGE:
				{
					//Calculate and draw image
					RECT rectImage = {0,0,0,0};
					LONG index = 0;
					LPTSTR end;
					UINT style = ILD_NORMAL;;
					CopyRect(&rectImage, &rect);
					HIMAGELIST hImageList = (HIMAGELIST)GetColOptional(col);
					int imageWidth, imageHeight;
					ImageList_GetIconSize(hImageList, &imageWidth, &imageHeight);

					rectImage.left += (WIDTH(rect) - imageWidth) /2;
					rectImage.top += (HEIGHT(rect) - imageHeight) /2;
					rectImage.right = rectImage.left + imageWidth;
					rectImage.bottom = rectImage.top + imageHeight;

					index = _tcstol(buffer, &end, 10);
					if(0 < _tcslen(end)) // we failed but I don't know how this could have happened
					{
						index = 0;
					}

					if (g_lpInst->GRIDHASFOCUS)
					{
						if ((g_lpInst->HIGHLIGHTFULLROW && row == g_lpInst->cursorrow) || 
							(col == g_lpInst->cursorcol && row == g_lpInst->cursorrow))
						{
							style = ILD_SELECTED;
						}
					}
					ImageList_DrawEx(hImageList, index, hdc, 
						rectImage.left, rectImage.top, WIDTH(rectImage), HEIGHT(rectImage), 
						g_lpInst->clrBackground, CLR_DEFAULT, style | ILD_TRANSPARENT);
				}
					break;
				case GCT_LINK:
				{
					InflateRect(&rect, -2, -2);

					// Draw the link underlined and in the link color
					HFONT oldFont;
					oldFont = (HFONT)SelectObject (hdc, Font_SetUnderline(hwnd, TRUE));

					if (g_lpInst->ELLIPSIS)
						DrawTextEx(hdc, buffer, -1, &rect, DT_END_ELLIPSIS | DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX, NULL);
					else
						DrawTextEx(hdc, buffer, -1, &rect, DT_LEFT | DT_VCENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_NOPREFIX, NULL);

					DeleteObject(SelectObject(hdc, oldFont));
				}
					break;
				default: //Text
				{
					InflateRect(&rect, -2, -2);

					DWORD dwAllignment =  NULL == lpgi ? GSA_GENERAL : lpgi->dwAllignment;
					if (g_lpInst->ELLIPSIS)
					{
						UINT uFormat =  DT_NOPREFIX | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
						uFormat |= GSA_LEFT == dwAllignment ? DT_LEFT :
							GSA_RIGHT == dwAllignment ? DT_RIGHT : // must be GSA_GENERAL
							 IsNumeric(buffer) ? DT_RIGHT : DT_LEFT;
						DrawTextEx(hdc, buffer, -1, &rect, uFormat, NULL);
					}
					else
					{
						UINT uFormat = DT_NOPREFIX | DT_WORDBREAK | DT_EDITCONTROL;
						uFormat |= GSA_LEFT == dwAllignment ? DT_LEFT :
							GSA_RIGHT == dwAllignment ? DT_RIGHT : // must be GSA_GENERAL
							 IsNumeric(buffer) ? DT_RIGHT : DT_LEFT;
						DrawTextEx(hdc, buffer, -1, &rect, uFormat, NULL);
					}
				}
			}
			rect = rectsave;
			row++;
		}   //end while r<=bottomvisiblerow

		//repaint bottom of grid
		RECT trect = {0,0,0,0};
		GetClientRect(hwnd, &trect);
		trect.top = rect.bottom;
		trect.left = rect.left;
		trect.right = rect.right;

		holdbrush = (HBRUSH)SelectObject(hdc, GetStockObject(GRAY_BRUSH));
		holdpen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
		Rectangle(hdc, trect.left, trect.top, trect.right + 1, trect.bottom + 1);
		SelectObject(hdc, holdbrush);
		SelectObject(hdc, holdpen);
	}


	/// @brief Determine if it is necessary to show the VScroll and then
	///         show it if necessary.
	///
	/// @param hwnd Handle of the grid
	///
	/// @returns VOID
	static VOID ShowVscroll(HWND hwnd)
	{
		//if more rows than can be visible on grid, display vertical scrollbar
		//otherwise, hide it.
		RECT gridrect = {0,0,0,0};
		int totalpixels;
		int rowsvisibleonscreen;
		int rows = RowCount();
		GetClientRect(hwnd, &gridrect);
		totalpixels = gridrect.bottom;
		totalpixels -= g_lpInst->titleheight;
		totalpixels -= g_lpInst->headerrowheight;
		totalpixels -= (g_lpInst->rowheight * rows);
		rowsvisibleonscreen = (gridrect.bottom - (g_lpInst->headerrowheight + g_lpInst->titleheight)) / g_lpInst->rowheight;
		if (totalpixels < 0)
		{
			//show vscrollbar
			ShowScrollBar(hwnd, SB_VERT, TRUE);
			SetScrollRange(hwnd, SB_VERT, 1, (rows - rowsvisibleonscreen) + 1, TRUE);
			g_lpInst->VSCROLL = TRUE;
		}
		else
		{
			//hide vscrollbar
			ShowScrollBar(hwnd, SB_VERT, FALSE);
			g_lpInst->VSCROLL = FALSE;
		}

	}

	/// @brief Determine if it is necessary to show the HScroll and then
	///         show it if necessary.
	///
	/// @param hwnd Handle of the grid
	///
	/// @returns VOID
	static VOID ShowHscroll(HWND hwnd)
	{
		//if more rows than can be visible on grid, display vertical scrollbar
		//otherwise, hide it.
		RECT gridrect = {0,0,0,0};
		int totalpixels;
		int colswithwidth;
		int cols = ColCount();
		int j;
		GetClientRect(hwnd, &gridrect);
		totalpixels = gridrect.right;
		totalpixels -= GetColWidth(0);
		colswithwidth = 0;
		for (j = 1; j < cols; j++)
		{
			totalpixels -= GetColWidth(j);
			if (0 < GetColWidth(j))
			{
				colswithwidth++;
			}
		}
		if (totalpixels < 0)
		{
			//show hscrollbar
			ShowScrollBar(hwnd, SB_HORZ, TRUE);
			SetScrollRange(hwnd, SB_HORZ, 1, colswithwidth, TRUE);
			g_lpInst->HSCROLL = TRUE;
		}
		else
		{
			//hide hscrollbar
			ShowScrollBar(hwnd, SB_HORZ, FALSE);
			g_lpInst->HSCROLL = FALSE;
		}

	}

	/// @brief Convert a number to an alphabetic representation.
	///
	/// @par Example:
	///       The number 728 would be converted to "AAZ"
	///
	/// @note Numeric values less than 1 return FALSE without converting
	///        the numeric value.  Also if the buffer is not big enough 
	///        to contain the converted value, this function returns FALSE. 
	///
	/// @param num The number to convert
	/// @param buf A buffer to receive text
	/// @param iSize The size of the buffer
	///
	/// @returns TRUE if successful, otherwise false
	static BOOL Alphabetize(INT num, LPTSTR buf, INT iSize)
	{
		//fill buffer with spaces initially
		for(int i = 0; i < iSize; ++i) buf[i] = ' ';

		//overwrite spaces with characters
		for(int i = iSize - 1; 0 <= i && 0 < num ; --i)
		{
			--num;
			buf[i] = (char)('A' + num % 26);
			num /= 26;
		}
		return 0 >= num;
	}

	/// @brief Wrapper to access column optional parameter
	///
	/// @param col The column number
	///
	/// @returns The column optional parameter
	static LPVOID GetColOptional(INT col)
	{
		LPGRIDCOLUMN lpgc = (LPGRIDCOLUMN)Vector_Get(g_lpInst->data, col);
		if(NULL == lpgc)
			return NULL;

		return lpgc->pOptional;
	}

	/// @brief Draw the grid Title with the specified font.
	///
	/// @param hwnd Handle of the grid
	/// @param fUnderline True to set underlined font
	///
	/// @returns a new font with new font weight
	static HFONT Font_SetUnderline(HWND hwnd, BOOL fUnderline)
	{
		HFONT hFont;
		LOGFONT  lf = {0};

		// Get a handle to the control's font object
		hFont = (HFONT)SendMessage(hwnd, WM_GETFONT, 0, 0);

		// Pull the handle into a Logical Font UDT type
		GetObject(hFont , sizeof(LOGFONT), &lf);

		// Set underline
		lf.lfUnderline = fUnderline;

		// Create a new font based off the logical font UDT
		return CreateFontIndirect(&lf);
	}

	/// @brief Determine whether a string of data represents a number.
	///
	/// @param data A string to analyze.
	///
	/// @returns TRUE if is numeric (decimal or floating point) otherwise FALSE.
	static BOOL IsNumeric(LPTSTR data)
	{
		int j, k, numberofperiods;
		k = _tcslen(data);
		//TCHAR tbuffer[k + 1]; //Sorry no VLA
		LPTSTR tbuffer = (LPTSTR) _alloca(sizeof(TCHAR) * (k + 1)); //use _alloca instead
		BOOL DIGIT, ALPHA, WHITESPACE;
		_tcscpy(tbuffer, data);
		k = _tcslen(tbuffer);
		_tcsupr(tbuffer);
		DIGIT = FALSE;
		ALPHA = FALSE;
		WHITESPACE = FALSE;

		numberofperiods = 0;
		for (j = 0; j < k; j++)
		{
			if (_istalpha(tbuffer[j])) ALPHA = TRUE;

			if (_istdigit(tbuffer[j])) DIGIT = TRUE;

			if (_istspace(tbuffer[j])) WHITESPACE = TRUE;

			if (tbuffer[j] == '.') numberofperiods++;

			if (tbuffer[j] == '+' && j > 0) ALPHA = TRUE;

			if (tbuffer[j] == '-' && j > 0) ALPHA = TRUE;
		}
		if ((ALPHA) || (WHITESPACE)) return FALSE;

		if ((DIGIT) && (!ALPHA) && (!WHITESPACE))
			return !(numberofperiods > 1);
		return FALSE;
	}
} //namespace mainwind
