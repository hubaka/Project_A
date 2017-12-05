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


	//---------------------------------------------------------------------------
	// Defines and Macros
	//---------------------------------------------------------------------------
	static errhandle::ErrHandle g_errHandle;

	//---------------------------------------------------------------------------
	// FUNCTION DECLARATIONS
	//---------------------------------------------------------------------------
	static LRESULT CALLBACK Grid_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static BOOL createGrid(HWND hWnd, LPCREATESTRUCT lpCreateStruct);

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

		//m_gridhWnd = CreateDialog(m_hParentInstance, MAKEINTRESOURCE(ID_BABY_GRID), hWnd, (DLGPROC)Grid_Proc);
		//g_errHandle.getErrorInfo((LPTSTR)L"createbabygrid!");
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

		//Only need to register the property grid once
		/*if (!aControl)
			aControl = InitSimpleGrid(hinst);*/
		//parentWind = hWnd;
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

		//Control_GetInstanceData(hWnd, &g_lpInst);   //Update the instance pointer
		////update the grid width and height variable
		//if (NULL != g_lpInst)
		//{
		//	GetClientRect(hWnd, &rect);
		//	g_lpInst->gridwidth = rect.right - rect.left;
		//	g_lpInst->gridheight = rect.bottom - rect.top;
		//}
		switch (uMsg) {
			//HANDLE_MSG(hWnd, WM_CREATE, createGrid);
			//case WM_INITDIALOG:
			//	{
			//		initGridDialog(hWnd);
			//		break;
			//	}
			//case SG_SETCOLAUTOWIDTH:
			//	{
			//		g_lpInst->COLAUTOWIDTH = (BOOL)wParam;
			//		break;
			//	}
			//case SG_SETROWHEADERWIDTH:
			//	{
			//		DWORD dwRtn = 0;
			//		dwRtn = Grid_OnSetColWidth(hWnd, 0, lParam);
			//		return dwRtn;
			//	}
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

		//INSTANCEDATA instance;
		//memset(&instance, 0, sizeof(INSTANCEDATA));
		//instance.hInstance = lpCreateStruct->hInstance;
		//instance.hWndParent = lpCreateStruct->hwndParent;
		////Create the data table
		//instance.data = createVector();
		////Add Column 0 (Row headers)
		//LPVECTOR temp = createVector();
		////First element (the column name) Should be blank for row header column
		//retVal = addVector(temp, createNewItem(_T("")));

		//if (retVal)
		//{
		//	SGCOLUMN sgc;
		//	sgc.dwType = GCT_ROWHEADER;
		//	sgc.lpszHeader = _T("");
		//	sgc.pOptional = NULL;
		//	retVal = addVector(instance.data, addNewColumn(&sgc, INITIAL_COL_WIDTH, temp));
		//}

		//if (retVal)
		//{
		//	instance.homerow = 1;
		//	instance.homecol = 1; 
		//	instance.rowheight = 21;
		//	instance.headerrowheight = 21;
		//	instance.ROWSNUMBERED = TRUE;
		//	instance.COLUMNSNUMBERED = TRUE;
		//	instance.EDITABLE = TRUE;
		//	instance.cursorcol = 1;
		//	instance.cursorrow = 1;
		//	instance.HIGHLIGHTFULLROW = FALSE;
		//	instance.DOUBLEBUFFER = TRUE;
		//	instance.cWheelRemainder = 0;
		//	instance.clrProtect = GetSysColor(COLOR_WINDOW);
		//	instance.clrHighlight = GetSysColor(COLOR_HIGHLIGHT);
		//	instance.clrLink = GetSysColor(COLOR_HIGHLIGHT);
		//	instance.LAUNCLINK = TRUE;
		//	instance.clrGridline = GetSysColor(COLOR_BTNFACE);
		//	instance.clrHighlightText = GetSysColor(COLOR_HIGHLIGHTTEXT);
		//	instance.clrBackground = GetSysColor(COLOR_WINDOW);
		//	instance.title = createNewString(_T(""));
		//	instance.titleheight = 0;
		//	instance.EXTENDLASTCOLUMN = TRUE;
		//	instance.SHOWINTEGRALROWS = TRUE;
		//	instance.SIZING = FALSE;
		//	instance.ELLIPSIS = TRUE;
		//	instance.COLAUTOWIDTH = FALSE;
		//	instance.COLUMNSIZING = FALSE;
		//	instance.ALLOWCOLUMNRESIZING = FALSE;
		//	instance.FORCELEFTJUSTIFY = FALSE;
		//	instance.cursortype = 0;
		//	instance.hcolumnheadingfont = NULL;
		//	instance.htitlefont = NULL;

		//	retVal = addWindowPropList(hWnd, &instance);
		//}
		return retVal;
	}


	////---------------------------------------------------------------------------------------------------
	////! \brief		
	////!
	////! \param[in]	
	////!
	////! \return		
	////!
	//static LPVECTOR 
	//createVector(void)
	//{
	//	LPVECTOR pVector;
	//	pVector = (LPVECTOR)SMALLOC(sizeof(VECTOR));
	//	pVector->_current_capacity = VECTOR_INITIAL_CAPACITY;
	//	pVector->_data = (PVOID*) SMALLOC(sizeof(PVOID) * pVector->_current_capacity);//DWM 1.9 Added cast
	//	pVector->_size = 0;
	//	return pVector;
	//}

	////---------------------------------------------------------------------------------------------------
	////! \brief		Add an item to a VECTOR
	////!
	////! \param[in]	 pVector a pointer to a VECTOR instance
	////! \param[in]	 object a pointer to an item to store in the vector
	////!
	////! \return		TRUE if successful
	////!
	//static BOOL 
	//addVector(const LPVECTOR pVector, PVOID object)
	//{
	//	bool retVal = true;
	//	int old_size = Vector_Size(pVector);
	//	int new_capacity;
	//	PVOID *new_data;//DWM 1.9: Added more explicit pointer

	//	(pVector->_size)++;
	//	if (old_size == pVector->_current_capacity)
	//	{
	//		new_capacity = pVector->_current_capacity + VECTOR_CAPACITY_DELTA;
	//		new_data = (PVOID *) SMALLOC(sizeof(PVOID) * new_capacity);//DWM 1.9: Added cast

	//		memcpy(new_data, pVector->_data, sizeof(PVOID) * old_size);
	//		free(pVector->_data);
	//		(pVector->_data) = new_data;
	//		pVector->_current_capacity = new_capacity;
	//	}
	//	(pVector->_data)[old_size] = object;
	//	return retVal;
	//}

	////---------------------------------------------------------------------------------------------------
	////! \brief		Gets the number of objects allocated in the array
	////!
	////! \param[in]	 pVector a pointer to a VECTOR instance
	////!
	////! \return		The number of objects
	////!
	//static int32_t 
	//Vector_Size(const LPVECTOR pVector)
	//{
	//	return (NULL == pVector) ? 0 : pVector->_size;
	//}

	////---------------------------------------------------------------------------------------------------
	////! \brief		Allocate and populate a grid item data structure.
	////!
	////! \param[in]	 szCurValue The item's current value.
	////!
	////! \return		 a Pointer to the allocated grid item.
	////!
	//static LPGRIDITEM 
	//createNewItem(LPTSTR szCurValue)
	//{
	//	LPGRIDITEM lpItem = (LPGRIDITEM)calloc(1, sizeof(GRIDITEM));

	//	lpItem->lpszMisc = NULL; // <- used to store pointer to data allocated
	//							 // to lpItem->lpszCurValue (if it is an array)
	//	lpItem->dwAllignment = GSA_GENERAL;
	//	lpItem->lpszCurValue = createNewString(szCurValue);
	//	return lpItem;
	//}

	////---------------------------------------------------------------------------------------------------
	////! \brief		Allocate and store a string.
	////!
	////! \param[in]	 str The string to store.
	////!
	////! \return		 a Pointer to the allocated string.
	////!
	//static LPTSTR 
	//createNewString(LPTSTR str)
	//{
	//	if (NULL == str || _T('\0') == *str)
	//		str = _T("");
	//	LPTSTR tmp = (LPTSTR)calloc(_tcslen(str) + 1, sizeof(TCHAR));

	//	if (NULL == tmp)
	//	{
	//		return (LPTSTR)calloc(1, sizeof(TCHAR));
	//	}
	//	return _tmemmove(tmp, str, _tcslen(str));
	//}

	////---------------------------------------------------------------------------------------------------
	////! \brief		Allocate and populate a grid column data structure.
	////!
	////! \param[in]	 lpColumn The column data.
	////! \param[in]	 iWidth The column width.
	////! \param[in]	 lpVector The column vector.
	////!
	////! \return		 a Pointer to the allocated grid column.
	////!
	//static LPGRIDCOLUMN 
	//	addNewColumn(LPSGCOLUMN lpColumn, uint32_t iWidth, LPVECTOR lpVector)
	//{
	//	LPGRIDCOLUMN lpCol = (LPGRIDCOLUMN)calloc(1, sizeof(GRIDCOLUMN));

	//	lpCol->dwType = lpColumn->dwType;
	//	switch(lpCol->dwType)
	//	{
	//		case GCT_ROWHEADER:
	//			lpCol->pOptional = NULL;
	//			break;
	//		case GCT_EDIT:
	//			lpCol->pOptional = NULL;
	//			break;
	//		//case GCT_COMBO:
	//		//	lpCol->pOptional = NewStringArray((LPTSTR)lpColumn->pOptional);
	//		//	break;
	//		case GCT_BUTTON:
	//			lpCol->pOptional = NULL;
	//			break;
	//		//case GCT_CHECK:
	//		//	lpCol->pOptional = NULL;
	//		//	break;
	//		//case GCT_LINK:
	//		//	lpCol->pOptional = NULL;
	//		//	break;
	//		//case GCT_IMAGE:
	//		//	lpCol->pOptional = (HIMAGELIST)lpColumn->pOptional;
	//		//	break;
	//	}
	//	lpCol->iWidth = iWidth;
	//	lpCol->items = lpVector;
	//	return lpCol;
	//}

	////---------------------------------------------------------------------------------------------------
	////! \brief		Allocate the Instance data associated with this instance.
	////!
	////! \param[in]	 lpColumn The column data.hControl Handle to current instance.
	////! \param[in]	 pInstanceData Pointer to an INSTANCEDATA struct
	////!
	////! \return		 TRUE if successful
	////!
	//static BOOL 
	//addWindowPropList(HWND hControl, LPINSTANCEDATA pInstanceData)
	//{
	//	LPINSTANCEDATA pInst = (LPINSTANCEDATA)SMALLOC(sizeof(INSTANCEDATA));
	//	memmove(pInst, pInstanceData, sizeof(INSTANCEDATA));
	//	return SetProp(hControl, (LPCTSTR)_T("lpInsData"), pInst);
	//}

	///// @brief Get the Instance data associated with this instance.
	/////
	///// @param hControl Handle to Current instance.
	///// @param ppInstanceData - Pointer to the address of an INSTANCEDATA struct. 
	/////
	///// @returns TRUE if successful
	//static BOOL 
	//Control_GetInstanceData(HWND hControl, LPINSTANCEDATA *ppInstanceData)
	//{
	//	*ppInstanceData = (LPINSTANCEDATA)GetProp(hControl, (LPCTSTR)_T("lpInsData"));
	//	if (NULL != *ppInstanceData)
	//	{
	//		return TRUE;
	//	}
	//	else {
	//		return FALSE;
	//	}
	//}

	////---------------------------------------------------------------------------------------------------
	////! \brief		
	////!
	////! \param[in]	 
	////!
	////! \return	
	////!
	//static BOOL 
	//initGridDialog(HWND hwnd)
	//{
	//	//Get window handles
	//	HWND hgrid1 = GetDlgItem(parentWind, IDC_SIMPLEGRID1);
	//	if (NULL == hgrid1) {
	//		g_errHandle.getErrorInfo((LPTSTR)L"initgriddialog");
	//	}

	//	//HWND hgrid1 = GetDlgItem(hwnd, IDC_SIMPLEGRID1);
	//	//hgrid2 = GetDlgItem(hwnd, IDC_SIMPLEGRID2);
	//	//hgrid3 = GetDlgItem(hwnd, IDC_SIMPLEGRID3);
	//	//hgrid4 = GetDlgItem(hwnd, IDC_SIMPLEGRID4);
	//	//hgrid5 = GetDlgItem(hwnd, IDC_SIMPLEGRID5);
	//	//htab =  GetDlgItem(hwnd, IDC_TAB);

	//	//
	//	//Configure tab
	//	//

	//	//TCITEM tie;
	//	//tie.mask = TCIF_TEXT;
	//	//tie.pszText = _T("Original Baby Grid Demo");
	//	//TabCtrl_InsertItem(htab, 0, &tie);
	//	//tie.pszText = _T("Big Data");
	//	//TabCtrl_InsertItem(htab, 1, &tie);
	//	//tie.pszText = _T("Column Types");
	//	//TabCtrl_InsertItem(htab, 2, &tie);
	//	//tie.pszText = _T("Insert/Delete Rows");
	//	//TabCtrl_InsertItem(htab, 3, &tie);

	//	//
	//	//Configure grids
	//	//

	//	//set grid1 (the properties grid) to automatically size columns 
	//	//based on the length of the text entered into the cells
	//	SimpleGrid_SetColAutoWidth(hgrid1, TRUE);
	//	//I don't want a row header, so make it 0 pixels wide
	//	SimpleGrid_SetRowHeaderWidth(hgrid1,0);
	//	//this grid won't use column headings, set header row height = 0
	//	SimpleGrid_SetHeaderRowHeight(hgrid1, 0);
	//	//on selection hilight full row
	//	SimpleGrid_SetSelectionMode(hgrid1, GSO_FULLROW);
	//	//populate grid1 with data
	//	LoadGrid1(hgrid1);

	//	////Set the heading font for Grid 2
	//	//HFONT hFont = CreateFont(20, 0, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE,
	//	//	ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
	//	//	PROOF_QUALITY, VARIABLE_PITCH | FF_MODERN, _T("ARIEL"));

	//	//SimpleGrid_SetHeadingFont(hgrid2,hFont);

	//	////make grid2 header row to initial height of 21 pixels
	//	//SimpleGrid_SetHeaderRowHeight(hgrid2, 21);
	//	////on row header selection hilight full row, otherwise individual cell
	//	//SimpleGrid_SetSelectionMode(hgrid2, GSO_ROWHEADER);
	//	////do not allow in cell editing of all grid items initially
	//	//SimpleGrid_EnableEdit(hgrid2,FALSE);

	//	////populate grid2 with initial demo data
	//	//LoadGrid2(hgrid2);

	//	////make grid3 header row to initial height of 21 pixels
	//	//SimpleGrid_SetHeaderRowHeight(hgrid3, 21);
	//	////use column header text
	//	//SimpleGrid_SetColsNumbered(hgrid3,FALSE);
	//	////use row header text
	//	//SimpleGrid_SetRowsNumbered(hgrid3,FALSE);
	//	////last column standard width
	//	//SimpleGrid_ExtendLastColumn(hgrid3,FALSE);
	//	////vertical scroll set to non integral rows
	//	//SimpleGrid_ShowIntegralRows(hgrid3,FALSE);
	//	////on row header selection hilight full row, otherwise individual cell
	//	//SimpleGrid_SetSelectionMode(hgrid3, GSO_ROWHEADER);

	//	////Include a title for this grid
	//	//SimpleGrid_SetTitleFont(hgrid3,hFont);
	//	//SimpleGrid_SetTitleHeight(hgrid3, 21);
	//	//SimpleGrid_SetTitle(hgrid3,_T("Grid's window text displayed here."));

	//	////populate grid3 with big data
	//	//LoadGrid3(hgrid3);

	//	////make grid4 header row to initial height of 21 pixels
	//	//SimpleGrid_SetHeaderRowHeight(hgrid4, 21);
	//	////allow column resizing
	//	//SimpleGrid_SetAllowColResize(hgrid4, TRUE);
	//	////use column header text
	//	//SimpleGrid_SetColsNumbered(hgrid4,FALSE);
	//	////on row header selection hilight full row, otherwise individual cell
	//	//SimpleGrid_SetSelectionMode(hgrid4, GSO_CELL);

	//	////populate grid4 with different column types
	//	//LoadGrid4(hgrid4);

	//	//Force the display of the vertical scroll in grids (if necessary)
	//	/*RECT rect;
	//	GetClientRect(hwnd, &rect);
	//	MoveWindow(hgrid2, rect.right / 3, 0, 
	//		rect.right - rect.right / 3, rect.bottom + 1, FALSE);*/

	//	return TRUE;
	//}

	//static void 
	//LoadGrid1(HWND hGrid)
	//{
	//	HWND retHandle;
	//	// Add some columns

	//	// Column type
	//	// Column header text
	//	// Optional data (ex: combobox choices)
	//	SGCOLUMN lpColumns[] = {
	//		GCT_EDIT, _T(""),  NULL,
	//		GCT_CHECK, _T(""),  NULL
	//	};
	//	for(int i = 0; i < NELEMS(lpColumns); ++i)
	//		SimpleGrid_AddColumn(hGrid, &lpColumns[i]);

	//	// Add some rows
	//	for(int i = 0; i < 9; ++i) 
	//		SimpleGrid_AddRow(hGrid, _T(""));

	//	// Column number
	//	// Row number
	//	// Item (cell) value
	//	SGITEM lpItems[] = {
	//		0, 0, (LPARAM)_T("User Column Resizing"),
	//		1, 0, (LPARAM) FALSE,
	//		0, 1, (LPARAM)_T("User Editable"),
	//		1, 1, (LPARAM) FALSE,
	//		0, 2, (LPARAM)_T("Show Ellipsis"),
	//		1, 2, (LPARAM) TRUE,
	//		0, 3, (LPARAM)_T("Auto Column Size"),
	//		1, 3, (LPARAM) FALSE,
	//		0, 4, (LPARAM)_T("Extend Last Column"),
	//		1, 4, (LPARAM) TRUE,
	//		0, 5, (LPARAM)_T("Numbered Columns"),
	//		1, 5, (LPARAM) TRUE,
	//		0, 6, (LPARAM)_T("Numbered Rows"),
	//		1, 6, (LPARAM) TRUE,
	//		0, 7, (LPARAM)_T("Highlight Row"),
	//		1, 7, (LPARAM) TRUE,
	//		0, 8, (LPARAM)_T("Show Gridlines"),
	//		1, 8, (LPARAM) TRUE
	//	};

	//	for(int i = 0; i < NELEMS(lpItems); ++i)
	//	{
	//		SimpleGrid_SetItemData(hGrid, &lpItems[i]);
	//	}

	//	//make the properties grid have the focus when the application starts
	//	retHandle = SetFocus(hGrid);
	//	if (NULL == retHandle) {
	//		g_errHandle.getErrorInfo((LPTSTR)L"SetFocus Failed!");
	//	}
	//}

	///// @brief Handles SG_SETCOLWIDTH message.
	/////
	///// @param hwnd The handle of the grid
	///// @param wParam The index of the column
	///// @param lParam The desired width (in pixels) of the column
	/////
	///// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds
	//static LRESULT Grid_OnSetColWidth(HWND hwnd, WPARAM wParam, LPARAM lParam)
	//{
	//	INT iCol = wParam;
	//	if (iCol > ColCount())
	//	{
	//		SetLastError(ERROR_INVALID_INDEX);
	//		return SG_ERROR;
	//	}
	//	RECT rect = {0,0,0,0};
	//	((LPGRIDCOLUMN)Vector_Get(g_lpInst->data, iCol))->iWidth = (INT)lParam;
	//	GetClientRect(hwnd, &rect);
	//	InvalidateRect(hwnd, &rect, FALSE);
	//	SetVisibleColumns(hwnd);
	//	return ERROR_SUCCESS;
	//}

	///// @brief wrapper to access column count (including the row header column 0)
	/////
	///// @returns The number of columns in the underlying data table
	//static int ColCount(VOID)
	//{
	//	return Vector_Size(g_lpInst->data);
	//}

	///// @brief Access an item stored in the VECTOR
	/////
	///// @param pVector a pointer to a VECTOR instance
	///// @param index the array index associated with the desired item
	/////
	///// @returns The desired item if successful, otherwise NULL
	//static PVOID Vector_Get(const LPVECTOR pVector, const int index)
	//{
	//	if (index < Vector_Size(pVector))
	//	{
	//		return pVector->_data[index];
	//	}
	//	else
	//	{
	//		return NULL;
	//	}
	//}

	///// @brief Sets the visible columns field to the number of visible columns.
	/////
	///// @param hwnd Handle of the grid
	/////
	///// @returns VOID
	//static void SetVisibleColumns(HWND hwnd)
	//{
	//	int j;
	//	int cols = ColCount();
	//	int value;
	//	value = 0;
	//	for (j = 1; j < cols; j++)
	//	{
	//		if (0 < GetColWidth(j))
	//		{
	//			value++;
	//		}
	//	}
	//	g_lpInst->visiblecolumns = value;
	//	SetScrollRange(hwnd, SB_HORZ, 1, value, TRUE);
	//}

	///// @brief wrapper to access column width
	/////
	///// @param col The column number
	/////
	///// @returns The width of the column
	//static int GetColWidth(INT col)
	//{
	//	LPGRIDCOLUMN lpgc = (LPGRIDCOLUMN)Vector_Get(g_lpInst->data, col);
	//	if(NULL == lpgc)
	//		return 0;

	//	return lpgc->iWidth;
	//}

} //namespace mainwind