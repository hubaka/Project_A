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
#include <strsafe.h>
#include <commctrl.h> // included in order to use tool bar related functionalities
#include <sqlite3.h>  // included for database
#include "errhandle.h"
#include "igrid.h"
#include "resource.h"
#include "mainwind.h"

HWND g_hToolbar = NULL;
static grid::IGrid*	m_pIGrid;

extern "C" IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

namespace mainwind
{
	//---------------------------------------------------------------------------
	// Defines and Macros
	//---------------------------------------------------------------------------
	#define HANDLE_DLGMSG(hWnd,message,fn)  case (message): return SetDlgMsgResult((hWnd),(message),HANDLE_##message((hWnd),(wParam),(lParam),(fn)))  /* added 05-01-29 */
	static errhandle::ErrHandle g_errHandle;
	static BOOL Main_OnNotify(HWND hWnd, INT id, LPNMHDR pnm);

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	MainWind::MainWind(
		HINSTANCE&	hParentInstance,
		const char*	p_className,
		int			nCmdShow
		) : m_hParentInstance(hParentInstance),
			m_pClassName(p_className),
			m_nCmdShow(nCmdShow) {
			;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	MainWind::~MainWind(
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
	MainWind::createWind(
		void
		) {
			//Step 1: Registering the Window Class
			m_wndClassEx.cbSize        = sizeof(WNDCLASSEX);
			m_wndClassEx.style         = 0;
			m_wndClassEx.lpfnWndProc   = (WNDPROC)StaticWndProc;
			m_wndClassEx.cbClsExtra    = 0;
			m_wndClassEx.cbWndExtra    = 0;
			m_wndClassEx.hInstance     = m_hParentInstance;
			m_wndClassEx.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
			m_wndClassEx.hCursor       = LoadCursor(NULL, IDC_ARROW);
			m_wndClassEx.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
			m_wndClassEx.lpszMenuName  = NULL;
			m_wndClassEx.lpszClassName = (LPCWSTR)m_pClassName;
			m_wndClassEx.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

			if(!RegisterClassEx(&m_wndClassEx))
			{
				g_errHandle.getErrorInfo((LPTSTR)L"Window Registration Failed!");
			}

			// Step 2: Creating the Window
			m_hWnd = CreateWindowEx(
							WS_EX_CLIENTEDGE,					// Extended Style For The Window
							(LPCWSTR)m_pClassName,				// Class Name
							(LPCWSTR)L"The title of my window",	// Window Title
							WS_OVERLAPPEDWINDOW,				// Defined Window Style
							CW_USEDEFAULT,						// Window Position
							CW_USEDEFAULT,						// Window Position
							CW_USEDEFAULT ,								// Window Width
							CW_USEDEFAULT ,								// Window height
							NULL,								// No Parent Window
							NULL,								// No Menu
							m_hParentInstance,						// Instance
							this);								//  Pass this class To WM_CREATE

			if(m_hWnd == NULL)
			{
				g_errHandle.getErrorInfo((LPTSTR)L"Window Creation Failed!");
			}
			ShowWindow(m_hWnd, m_nCmdShow);
			UpdateWindow(m_hWnd);
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	LRESULT	CALLBACK 
		MainWind::StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		MainWind* pParent;

	   // Get pointer to window
	   if(uMsg == WM_CREATE)
	   {
			pParent = (MainWind*)((LPCREATESTRUCT)lParam)->lpCreateParams;
			SetWindowLongPtr(hWnd,GWL_USERDATA,(LONG_PTR)pParent);
			
			createWindowsMenu(hWnd);
			pParent->m_hWndToolbar = createToolBar(hWnd);
			setWindowsIcon(hWnd);

			HWND hStatus = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0,
						hWnd, (HMENU)IDC_MAIN_STATUS, GetModuleHandle(NULL), NULL);

			int statwidths[] = {100, -1};
			SendMessage(hStatus, SB_SETPARTS, sizeof(statwidths)/sizeof(int), (LPARAM)statwidths);
			SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)L"Hi there :)");
			m_pIGrid->createBabyGrid(hWnd);
			/*HWND hgrid1 = GetDlgItem(hWnd, ID_BABY_GRID);
			(BOOL)SNDMSG((hgrid1),SG_SETCOLAUTOWIDTH,(BOOL)(TRUE),0L);*/
	   }
	   else
	   {
		  pParent = (MainWind*)GetWindowLongPtr(hWnd,GWL_USERDATA);
		  if(!pParent) return DefWindowProc(hWnd,uMsg,wParam,lParam);
	   }

	   pParent->m_hWnd = hWnd;
	   return pParent->mainWndProc(uMsg,wParam,lParam);
	}

	
	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	LRESULT CALLBACK 
		MainWind::mainWndProc(				// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
	{
		switch(uMsg)
		{
			HANDLE_DLGMSG(m_hWnd, WM_NOTIFY, Main_OnNotify);
			case WM_COMMAND: 
				{
					switch(LOWORD(wParam))
					{
					case ID_FILE_EXIT:
						{
							PostMessage(m_hWnd, WM_CLOSE, 0, 0);
							break;
						}
					case ID_FILE_OPEN:
					case ID_OPEN_FILE:
						{
							getFileName();
							break;
						}
					case ID_HELP_ABOUT:
						{
							MessageBox(m_hWnd, (LPCWSTR)L"No help document", (LPCWSTR)L"Message",
								MB_OK | MB_ICONINFORMATION);
							break;
						}
					default:
						{
							break;
						}
					}
					break;
				}
			case WM_CLOSE:
				{
					DestroyWindow(m_hWnd);
					break;
				}
			case WM_DESTROY:
				{
					DestroyWindow(g_hToolbar);
					PostQuitMessage(0);
					break;
				}
			//case WM_LBUTTONDOWN:
			//	{
			//		//MAX_PATH is a handy macro included via <windows.h> that 
			//		//is defined to the maximum length of a buffer needed to store a filename under Win32
			//		char szFileName[MAX_PATH];
			//		HINSTANCE hCurWindInstance = GetModuleHandle(NULL);
			//		GetModuleFileName(hCurWindInstance, (LPWCH)szFileName, MAX_PATH);
			//		MessageBox(m_hWnd, (LPCWSTR)szFileName, (LPCWSTR)L"This program is:", MB_OK | MB_ICONINFORMATION);
			//		break;
			//	}
			case WM_SIZE :
				{
					HWND hTool;
					RECT rcTool;
					int iToolHeight;

				    HWND hStatus;
					RECT rcStatus;
					int iStatusHeight;

					// Size toolbar and get height
					hTool = GetDlgItem(m_hWnd, IDC_MAIN_TOOL);
					SendMessage(hTool, TB_AUTOSIZE, 0, 0);
					GetWindowRect(hTool, &rcTool);
					iToolHeight = rcTool.bottom - rcTool.top;

					// Size status bar and get height
					hStatus = GetDlgItem(m_hWnd, IDC_MAIN_STATUS);
					SendMessage(hStatus, WM_SIZE, 0, 0);
					GetWindowRect(hStatus, &rcStatus);
					iStatusHeight = rcStatus.bottom - rcStatus.top;
					break;
				}
			/*case WM_INITDIALOG: 
			{
				break;
			}*/
			/*case WM_NOTIFY:
			{

				break;
			}*/
			default:
				return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
		}
		return 0;
	}


	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void 
		MainWind::getFileName(
			void
		) { 
			OPENFILENAME ofn;
			char szFileName[MAX_PATH] = "";

			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
			ofn.hwndOwner = m_hWnd;
			ofn.lpstrFilter = (LPCWSTR)L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
			ofn.lpstrFile = (LPWSTR) szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = (LPCWSTR)L"txt";

			if(GetOpenFileName(&ofn))
			{
				MessageBox(NULL, (LPWSTR)szFileName, (LPCWSTR)L"FileName!",
					MB_ICONEXCLAMATION | MB_OK);
			}
		}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	HWND*
		MainWind::createToolBar(
			HWND hWnd
		) { 
				
				static HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
									hWnd, (HMENU)IDC_MAIN_TOOL, GetModuleHandle(NULL), NULL);

				// Send the TB_BUTTONSTRUCTSIZE message, which is required for
				// backward compatibility.
				SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

				TBBUTTON tbb[3];
				TBADDBITMAP tbab;

				tbab.hInst = HINST_COMMCTRL;
				tbab.nID = IDB_STD_SMALL_COLOR;
				SendMessage(hWndToolbar, TB_ADDBITMAP, 0, (LPARAM)&tbab);

				ZeroMemory(tbb, sizeof(tbb));
				tbb[0].iBitmap = STD_FILENEW;
				tbb[0].fsState = TBSTATE_ENABLED;
				tbb[0].fsStyle = TBSTYLE_BUTTON;
				tbb[0].idCommand = ID_FILE_NEW;

				tbb[1].iBitmap = STD_FILEOPEN;
				tbb[1].fsState = TBSTATE_ENABLED;
				tbb[1].fsStyle = TBSTYLE_BUTTON;
				tbb[1].idCommand = ID_FILE_OPEN;

				tbb[2].iBitmap = STD_FILESAVE;
				tbb[2].fsState = TBSTATE_ENABLED;
				tbb[2].fsStyle = TBSTYLE_BUTTON;
				tbb[2].idCommand = ID_FILE_SAVEAS;

				SendMessage(hWndToolbar, TB_ADDBUTTONS, sizeof(tbb)/sizeof(TBBUTTON), (LPARAM)&tbb);
				return (&hWndToolbar);
		}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void 
		MainWind::createWindowsMenu(
			HWND hWnd
		) { 
				HMENU hMenu, hSubMenu;
				hMenu = CreateMenu();
				hSubMenu = CreatePopupMenu();
				AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, (LPCWSTR)L"&Exit");
				AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, (LPCWSTR)L"&File");
				AppendMenu(hMenu, MF_STRING, ID_OPEN_FILE, (LPCWSTR)L"&Open");
				AppendMenu(hMenu, MF_STRING, ID_HELP_ABOUT, (LPCWSTR)L"&Help");
				SetMenu(hWnd, hMenu);

		}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void 
		MainWind::setWindowsIcon(
			HWND hWnd
		) { 
				HICON hIcon, hIconSm;
				hIcon = static_cast<HICON> (LoadImage(NULL, (LPCWSTR)L"C:\\Users\\akathire\\Desktop\\t114_make\\common\\icon\\if_Batman_73346.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE));
				if(hIcon) {
					SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
				}
				else {
					MessageBox(hWnd, (LPCWSTR)L"Could not load large icon!", (LPCWSTR)L"Error", MB_OK | MB_ICONERROR);
				}

				hIconSm = static_cast<HICON> (LoadImage(NULL, (LPCWSTR)L"C:\\Users\\akathire\\Desktop\\t114_make\\common\\icon\\if_James_Bond_Beamers_73384.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE));
				if(hIconSm) {
					SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);
				}
				else {
					MessageBox(hWnd, (LPCWSTR)L"Could not load small icon!", (LPCWSTR)L"Error", MB_OK | MB_ICONERROR);
				}
		}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void 
		MainWind::attachGrid(
			grid::IGrid *p_grid
		) { 
			if (p_grid != NULL) {
				m_pIGrid = p_grid;
			}
			else {
				MessageBox(NULL, (LPCWSTR)L"Unable to attach Grid", (LPCWSTR)L"Error!",
					MB_ICONEXCLAMATION | MB_OK);
			}
		}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	static BOOL 
	Main_OnNotify(HWND hWnd, INT id, LPNMHDR pnm)
	{

		HWND hgrid1 = GetDlgItem(hWnd, ID_BABY_GRID);
		ShowWindow(hgrid1, SW_SHOW);
		//if (IDC_TAB == id)
		//{
		//	if (TCN_SELCHANGE == pnm->code)
		//	{
		//		switch(TabCtrl_GetCurSel(pnm->hwndFrom))
		//		{
		//			case 0:
		//				ShowWindow(hgrid5, SW_HIDE);
		//				ShowWindow(hgrid4, SW_HIDE);
		//				ShowWindow(hgrid3, SW_HIDE);
		//				ShowWindow(hgrid2, SW_SHOW);
		//				ShowWindow(hgrid1, SW_SHOW);
		//				break;
		//			case 1:
		//				ShowWindow(hgrid2, SW_HIDE);
		//				ShowWindow(hgrid1, SW_HIDE);
		//				ShowWindow(hgrid5, SW_HIDE);
		//				ShowWindow(hgrid4, SW_HIDE);
		//				ShowWindow(hgrid3, SW_SHOW);
		//				break;
		//			case 2:
		//				ShowWindow(hgrid3, SW_HIDE);
		//				ShowWindow(hgrid2, SW_HIDE);
		//				ShowWindow(hgrid1, SW_HIDE);
		//				ShowWindow(hgrid5, SW_HIDE);
		//				ShowWindow(hgrid4, SW_SHOW);
		//				break;
		//			case 3:
		//				ShowWindow(hgrid4, SW_HIDE);
		//				ShowWindow(hgrid3, SW_HIDE);
		//				ShowWindow(hgrid2, SW_HIDE);
		//				ShowWindow(hgrid1, SW_HIDE);
		//				InitializeGrid5Demo();
		//				ShowWindow(hgrid5, SW_SHOW);
		//				break;
		//		}
		//	}
		//}
		//if (IDC_SIMPLEGRID1 == id)
		//{
		//	if (pnm->code == SGN_ITEMCLICK) //a cell was clicked in the properties grid
		//	{
		//		DWORD dwType = ((LPNMGRID)pnm)->dwType;

		//		if(GCT_CHECK == dwType)
		//		{
		//			SGITEM sgi;
		//			sgi.col = ((LPNMGRID)pnm)->col;
		//			sgi.row = ((LPNMGRID)pnm)->row;
		//			SimpleGrid_GetItemData(pnm->hwndFrom, &sgi);

		//			if (FALSE == (BOOL) sgi.lpCurValue)
		//			{
		//				//send appropriate control message to the grid based
		//				//on the row of the cell that was toggled
		//				if (sgi.row == 0)
		//					SimpleGrid_SetAllowColResize(hgrid2, FALSE);
		//				if (sgi.row == 1)
		//					SimpleGrid_EnableEdit(hgrid2, FALSE);
		//				if (sgi.row == 2)
		//					SimpleGrid_SetEllipsis(hgrid2, FALSE);
		//				if (sgi.row == 3)
		//					SimpleGrid_SetColAutoWidth(hgrid2, FALSE);
		//				if (sgi.row == 4)
		//					SimpleGrid_ExtendLastColumn(hgrid2, FALSE);
		//				if (sgi.row == 5)
		//				{
		//					SimpleGrid_SetColsNumbered(hgrid2, FALSE);
		//					SimpleGrid_SetHeaderRowHeight(hgrid2, 61);
		//				}
		//				if (sgi.row == 6)
		//					SimpleGrid_SetRowsNumbered(hgrid2, FALSE);
		//				if (sgi.row == 7)
		//					SimpleGrid_SetSelectionMode(hgrid2, GSO_ROWHEADER);
		//				if (sgi.row == 8)
		//					SimpleGrid_SetGridLineColor(hgrid2, RGB(255, 255, 255));
		//			}
		//			else //TRUE
		//			{
		//				//send appropriate control message to the grid based
		//				//on the row of the cell that was toggled
		//				if (sgi.row == 0)
		//					SimpleGrid_SetAllowColResize(hgrid2, TRUE);
		//				if (sgi.row == 1)
		//					SimpleGrid_EnableEdit(hgrid2, TRUE);
		//				if (sgi.row == 2)
		//					SimpleGrid_SetEllipsis(hgrid2, TRUE);
		//				if (sgi.row == 3)
		//					SimpleGrid_SetColAutoWidth(hgrid2, TRUE);
		//				if (sgi.row == 4)
		//					SimpleGrid_ExtendLastColumn(hgrid2, TRUE);
		//				if (sgi.row == 5)
		//				{
		//					SimpleGrid_SetColsNumbered(hgrid2, TRUE);
		//					SimpleGrid_SetHeaderRowHeight(hgrid2, 21);
		//				}
		//				if (sgi.row == 6)
		//					SimpleGrid_SetRowsNumbered(hgrid2, TRUE);
		//				if (sgi.row == 7)
		//					SimpleGrid_SetSelectionMode(hgrid2, GSO_FULLROW);
		//				if (sgi.row == 8)
		//					SimpleGrid_SetGridLineColor(hgrid2, RGB(220, 220, 220));
		//			}
		//		}
		//	}   //if(pnm.code==BGN_CELLCLICKED)
		//	return TRUE;
		//}
		//else if (IDC_SIMPLEGRID3 == id)
		//{
		//	if(pnm->code == SGN_KEYDOWN)
		//	{
		//		LPNMSGKEYDOWN pnmkd = (LPNMSGKEYDOWN)pnm;
		//		if(VK_F2 == pnmkd->wVKey && pnmkd->dwType == GCT_EDIT)
		//		{
		//			SimpleGrid_SelectCell(pnm->hwndFrom, pnmkd->col, pnmkd->row, FALSE);
		//		}
		//	}
		//}
		//else if (IDC_SIMPLEGRID4 == id)
		//{
		//	if(pnm->code == SGN_ITEMCLICK)
		//	{
		//		DWORD dwType = ((LPNMGRID)pnm)->dwType;

		//		if(GCT_BUTTON == dwType)
		//		{
		//			int col = ((LPNMGRID)pnm)->col;
		//			int row = ((LPNMGRID)pnm)->row;
		//			int len = SimpleGrid_GetItemDataLen(pnm->hwndFrom, col, row);
		//			LPTSTR buf = (LPTSTR) _alloca(sizeof(TCHAR) * (len + 1));
		//			SimpleGrid_GetItemText(pnm->hwndFrom, col, row, buf);

		//			if(row == 0)
		//			{
		//				if(0 == _tcsicmp((LPTSTR) buf, _T("#1 On")))
		//				{
		//					// Column number
		//					// Row number
		//					// Item (cell) value
		//					SGITEM lpItems[] = {
		//						// Button column
		//						1, 0, (LPARAM)_T("#1 Off"),
		//						// Image column
		//						2, 0, (LPARAM) 1 //On light
		//					};

		//					for(int i = 0; i < NELEMS(lpItems); ++i)
		//					{
		//						SimpleGrid_SetItemData(pnm->hwndFrom, &lpItems[i]);
		//					}
		//				}
		//				else
		//				{
		//					// Column number
		//					// Row number
		//					// Item (cell) value
		//					SGITEM lpItems[] = {
		//						// Button column
		//						1, 0, (LPARAM)_T("#1 On"),
		//						// Image column
		//						2, 0, (LPARAM) 0 //Off light
		//					};

		//					for(int i = 0; i < NELEMS(lpItems); ++i)
		//					{
		//						SimpleGrid_SetItemData(pnm->hwndFrom, &lpItems[i]);
		//					}

		//				}
		//			}
		//			if(row == 1)
		//			{
		//				if(0 == _tcsicmp((LPTSTR) buf, _T("#2 On")))
		//				{
		//					// Column number
		//					// Row number
		//					// Item (cell) value
		//					SGITEM lpItems[] = {
		//						// Button column
		//						1, 1, (LPARAM)_T("#2 Off"),
		//						// Image column
		//						2, 1, (LPARAM) 1 //On light
		//					};

		//					for(int i = 0; i < NELEMS(lpItems); ++i)
		//					{
		//						SimpleGrid_SetItemData(pnm->hwndFrom, &lpItems[i]);
		//					}
		//				}
		//				else
		//				{
		//					// Column number
		//					// Row number
		//					// Item (cell) value
		//					SGITEM lpItems[] = {
		//						// Button column
		//						1, 1, (LPARAM)_T("#2 On"),
		//						// Image column
		//						2, 1, (LPARAM) 0 //Off light
		//					};

		//					for(int i = 0; i < NELEMS(lpItems); ++i)
		//					{
		//						SimpleGrid_SetItemData(pnm->hwndFrom, &lpItems[i]);
		//					}

		//				}
		//			}
		//			SimpleGrid_SetCursorPos(pnm->hwndFrom,col,row);
		//		}

		//	} //if(pnm.code==BGN_CELLCLICKED)
		//	return TRUE;
		//}
		//else if (IDC_SIMPLEGRID5 == id)
		//{
		//	if(pnm->code == SGN_ITEMCLICK)
		//	{
		//		DWORD dwType = ((LPNMGRID)pnm)->dwType;

		//		if(GCT_BUTTON == dwType)
		//		{
		//			if(0 == ((LPNMGRID)pnm)->col)// "-"
		//				SimpleGrid_DeleteRow(pnm->hwndFrom, ((LPNMGRID)pnm)->row);
		//			else // "+"
		//			{
		//				SGITEM sgi;
		//				sgi.row = ((LPNMGRID)pnm)->row + 1;
		//				SimpleGrid_InsertRow(pnm->hwndFrom, sgi.row, "");

		//				sgi.col = 0;
		//				sgi.lpCurValue = (LPARAM)_T("-"); // Button text
		//				SimpleGrid_SetItemData(pnm->hwndFrom, &sgi);

		//				sgi.col = 1;
		//				sgi.lpCurValue = (LPARAM)_T("+"); // Button text
		//				SimpleGrid_SetItemData(pnm->hwndFrom, &sgi);

		//				sgi.col = 2;
		//				sgi.lpCurValue = (LPARAM)_T("Inserted Item");
		//				SimpleGrid_SetItemData(pnm->hwndFrom, &sgi);
		//			}

		//			SimpleGrid_RefreshGrid(pnm->hwndFrom);
		//		}
		//		return TRUE;
		//	}
		//}
		/*else {
			return FALSE;
		}*/
		return true;
	}
} //namespace mainwind