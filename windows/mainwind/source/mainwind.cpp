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
#include <strsafe.h>
#include <commctrl.h> // included in order to use tool bar related functionalities
#include <sqlite3.h>  // included for database
#include <Shlwapi.h>	// for stripping filename for full file path
#include <time.h>
#include "sys.h"
#include "babygrid.h"
#include "errhandle.h"
#include "igrid.h"
#include "resource.h"
#include "toolbar.h"
#include "dbms.h"
#include "mainwind.h"

#pragma comment(lib,"Shlwapi.lib")

HWND g_hToolbar = NULL;
static grid::IGrid*	m_pIGrid;
static bar::ToolBar*	m_pBar;

extern "C" IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

namespace mainwind
{

	static HINSTANCE ghInstance;
	HWND hgrid1, hgrid2, hgrid3, hgrid4, hgrid5, htab;

	//---------------------------------------------------------------------------
	// Defines and Macros
	//---------------------------------------------------------------------------
	/// @def SimpleGrid_SetProtectColor(hGrid, clrProtect)
	///
	/// @brief Sets the background color of protected cells.
	///
	/// @param hGrid The handle of the grid.
	/// @param clrProtect A COLORREF value. 
	///
	/// @returns The return value is not meaningful. 
	#define SimpleDialog_ReSize(hDialog, clrProtect) (BOOL)SNDMSG((hDialog),WM_SIZE,(WPARAM)(UINT)(clrProtect),0L)

	#define HANDLE_DLGMSG(hWnd,message,fn)  case (message): return SetDlgMsgResult((hWnd),(message),HANDLE_##message((hWnd),(wParam),(lParam),(fn)))  /* added 05-01-29 */
	static errhandle::ErrHandle g_errHandle;
	//static LRESULT CALLBACK mainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK dlgWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static BOOL Main_OnInitDialog(HWND hWnd, HWND hwndFocus, LPARAM lParam);
	static BOOL Main_OnNotify(HWND hWnd, INT id, LPNMHDR pnm);
	static HWND dialogbar;
	static void LoadGrid1(HWND hGrid);
	static void LoadGrid2(HWND hGrid);
	static void Main_OnSize(HWND hwnd, UINT state, int cx, int cy);
	static void Main_OnClose(HWND hwnd);
	static void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	static void InitializeGrid5Demo(void);
	static void LoadGrid5(HWND hGrid, LPTSTR *lpszzItems, int count);
	static void LoadGrid3(HWND hGrid);
	static void LoadGrid4(HWND hGrid);
	static void ResetGrid5(HWND hGrid);
	static BOOL CALLBACK About_DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	MainWind::MainWind(
		HINSTANCE&	hParentInstance,
		int			nCmdShow,
		dbms::Dbms* pDbms
		) : m_hParentInstance(hParentInstance),
			m_nCmdShow(nCmdShow),
			m_pDbms(pDbms) {
			ghInstance = hParentInstance;
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
			m_wndClassEx.lpszClassName = _T("MainWindowClass");
			m_wndClassEx.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

			if(!RegisterClassEx(&m_wndClassEx))
			{
				g_errHandle.getErrorInfo((LPTSTR)L"Window Registration Failed!");
			}

			// Step 2: Creating the Window
			m_hWnd = CreateWindowEx(
							WS_EX_CLIENTEDGE,					// Extended Style For The Window
							_T("MainWindowClass"),				// Class Name
							(LPCWSTR)L"The Title",				// Window Title
							WS_OVERLAPPEDWINDOW,				// Defined Window Style
							CW_USEDEFAULT,						// Window Position
							CW_USEDEFAULT,						// Window Position
							CW_USEDEFAULT ,						// Window Width
							CW_USEDEFAULT ,						// Window height
							NULL,								// No Parent Window
							NULL,								// No Menu
							m_hParentInstance,					// Instance
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
			// Sets the user data associated with the window. 
			// This data is intended for use by the application that created the window. Its value is initially zero.
			SetWindowLongPtr(hWnd,GWL_USERDATA,(LONG_PTR)pParent);
			
			createWindowsMenu(hWnd);
			pParent->m_hWndToolbar = createToolBar(hWnd);
			setWindowsIcon(hWnd);

			HWND hStatus = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0,
						hWnd, (HMENU)IDC_MAIN_STATUS, GetModuleHandle(NULL), NULL);
			if(hStatus == NULL)
			{
				g_errHandle.getErrorInfo((LPTSTR)L"StatusBar creation!");
			}

			m_pIGrid->createBabyGrid(hWnd);
			WNDCLASSEX dialogClass;
			// Get system dialog information.
			dialogClass.cbSize = sizeof(dialogClass);
			if (!GetClassInfoEx(NULL, MAKEINTRESOURCE(32770), &dialogClass))
				return 0;

			dialogClass.hInstance = GetModuleHandle(NULL);
			//dialogClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_ICO_MAIN));
			//dialogClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_ICO_SMALL));
			dialogClass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
			dialogClass.lpszClassName = _T("DialogClass");
			if (!RegisterClassEx(&dialogClass))
				return 0;
			g_hToolbar = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_BABY_GRID), hWnd, (DLGPROC)dlgWndProc);
			if(g_hToolbar != NULL)
			{
				ShowWindow(g_hToolbar, SW_SHOW);
			}
			else {
				g_errHandle.getErrorInfo((LPTSTR)L"dialog Failed!");
			}

			// TODO - Need to set the status bar
			int statwidths[] = {100, -1};
			SendMessage(hStatus, SB_SETPARTS, sizeof(statwidths)/sizeof(int), (LPARAM)statwidths);
			SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)L"Hi there :)");
			//dialogbar = m_pBar->createToolBar(hWnd);
			//m_pIGrid->createBabyGrid(hWnd);
			/*HWND hgrid1 = GetDlgItem(hWnd, ID_BABY_GRID);
			(BOOL)SNDMSG((hgrid1),SG_SETCOLAUTOWIDTH,(BOOL)(TRUE),0L);*/
	   }
	   else
	   {
		  pParent = (MainWind*)GetWindowLongPtr(hWnd,GWL_USERDATA);
		  if(!pParent) {
			  return DefWindowProc(hWnd,uMsg,wParam,lParam);
		  }
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
		MainWind::mainWndProc(
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
					case ID_DIALOG_SHOW:
						ShowWindow(dialogbar, SW_SHOW);
					break;
					case ID_DIALOG_HIDE:
						ShowWindow(dialogbar, SW_HIDE);
					break;
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
					HWND hgrid1 = GetDlgItem(m_hWnd, IDC_SIMPLEGRID1);
					SendMessage(hTool, TB_AUTOSIZE, 0, 0);
					GetWindowRect(hTool, &rcTool);
					iToolHeight = rcTool.bottom - rcTool.top;

					// Size status bar and get height
					hStatus = GetDlgItem(m_hWnd, IDC_MAIN_STATUS);
					SendMessage(hStatus, WM_SIZE, 0, 0);
					GetWindowRect(hStatus, &rcStatus);
					iStatusHeight = rcStatus.bottom - rcStatus.top;
					SimpleDialog_ReSize(g_hToolbar, 0);
					break;
				}
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
			char szFileName[2*MAX_PATH] = "";

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
				stripFileName(ofn.lpstrFile);
				m_pDbms->addTableData(m_fileName, m_filePath, 1, 1);
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
				if(hWndToolbar == NULL)
				{
					g_errHandle.getErrorInfo((LPTSTR)L"ToolBar creation!");
				}

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
	void 
		MainWind::attachBar(
			bar::ToolBar *p_bar
		) { 
			if (p_bar != NULL) {
				m_pBar = p_bar;
			}
			else {
				MessageBox(NULL, (LPCWSTR)L"Unable to attach bar", (LPCWSTR)L"Error!",
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

		hgrid1 = GetDlgItem(hWnd, IDC_SIMPLEGRID1);
		ShowWindow(hgrid1, SW_SHOW);

		if (IDC_TAB == id)
		{
			if (TCN_SELCHANGE == pnm->code)
			{
				switch(TabCtrl_GetCurSel(pnm->hwndFrom))
				{
					case 0:
						ShowWindow(hgrid5, SW_HIDE);
						ShowWindow(hgrid4, SW_HIDE);
						ShowWindow(hgrid3, SW_HIDE);
						ShowWindow(hgrid2, SW_SHOW);
						ShowWindow(hgrid1, SW_SHOW);
						break;
					case 1:
						ShowWindow(hgrid2, SW_HIDE);
						ShowWindow(hgrid1, SW_HIDE);
						ShowWindow(hgrid5, SW_HIDE);
						ShowWindow(hgrid4, SW_HIDE);
						ShowWindow(hgrid3, SW_SHOW);
						break;
					case 2:
						ShowWindow(hgrid3, SW_HIDE);
						ShowWindow(hgrid2, SW_HIDE);
						ShowWindow(hgrid1, SW_HIDE);
						ShowWindow(hgrid5, SW_HIDE);
						ShowWindow(hgrid4, SW_SHOW);
						break;
					case 3:
						ShowWindow(hgrid4, SW_HIDE);
						ShowWindow(hgrid3, SW_HIDE);
						ShowWindow(hgrid2, SW_HIDE);
						ShowWindow(hgrid1, SW_HIDE);
						InitializeGrid5Demo();
						ShowWindow(hgrid5, SW_SHOW);
						break;
				}
			}
		}
		if (IDC_SIMPLEGRID1 == id)
		{
			if (pnm->code == SGN_ITEMCLICK) //a cell was clicked in the properties grid
			{
				DWORD dwType = ((LPNMGRID)pnm)->dwType;

				if(GCT_CHECK == dwType)
				{
					SGITEM sgi;
					sgi.col = ((LPNMGRID)pnm)->col;
					sgi.row = ((LPNMGRID)pnm)->row;
					SimpleGrid_GetItemData(pnm->hwndFrom, &sgi);

					if (FALSE == (BOOL) sgi.lpCurValue)
					{
						//send appropriate control message to the grid based
						//on the row of the cell that was toggled
						if (sgi.row == 0)
							SimpleGrid_SetAllowColResize(hgrid2, FALSE);
						if (sgi.row == 1)
							SimpleGrid_EnableEdit(hgrid2, FALSE);
						if (sgi.row == 2)
							SimpleGrid_SetEllipsis(hgrid2, FALSE);
						if (sgi.row == 3)
							SimpleGrid_SetColAutoWidth(hgrid2, FALSE);
						if (sgi.row == 4)
							SimpleGrid_ExtendLastColumn(hgrid2, FALSE);
						if (sgi.row == 5)
						{
							SimpleGrid_SetColsNumbered(hgrid2, FALSE);
							SimpleGrid_SetHeaderRowHeight(hgrid2, 61);
						}
						if (sgi.row == 6)
							SimpleGrid_SetRowsNumbered(hgrid2, FALSE);
						if (sgi.row == 7)
							SimpleGrid_SetSelectionMode(hgrid2, GSO_ROWHEADER);
						if (sgi.row == 8)
							SimpleGrid_SetGridLineColor(hgrid2, RGB(255, 255, 255));
					}
					else //TRUE
					{
						//send appropriate control message to the grid based
						//on the row of the cell that was toggled
						if (sgi.row == 0)
							SimpleGrid_SetAllowColResize(hgrid2, TRUE);
						if (sgi.row == 1)
							SimpleGrid_EnableEdit(hgrid2, TRUE);
						if (sgi.row == 2)
							SimpleGrid_SetEllipsis(hgrid2, TRUE);
						if (sgi.row == 3)
							SimpleGrid_SetColAutoWidth(hgrid2, TRUE);
						if (sgi.row == 4)
							SimpleGrid_ExtendLastColumn(hgrid2, TRUE);
						if (sgi.row == 5)
						{
							SimpleGrid_SetColsNumbered(hgrid2, TRUE);
							SimpleGrid_SetHeaderRowHeight(hgrid2, 21);
						}
						if (sgi.row == 6)
							SimpleGrid_SetRowsNumbered(hgrid2, TRUE);
						if (sgi.row == 7)
							SimpleGrid_SetSelectionMode(hgrid2, GSO_FULLROW);
						if (sgi.row == 8)
							SimpleGrid_SetGridLineColor(hgrid2, RGB(220, 220, 220));
					}
				}
			}   //if(pnm.code==BGN_CELLCLICKED)
			return TRUE;
		}
		else if (IDC_SIMPLEGRID3 == id)
		{
			if(pnm->code == SGN_KEYDOWN)
			{
				LPNMSGKEYDOWN pnmkd = (LPNMSGKEYDOWN)pnm;
				if(VK_F2 == pnmkd->wVKey && pnmkd->dwType == GCT_EDIT)
				{
					SimpleGrid_SelectCell(pnm->hwndFrom, pnmkd->col, pnmkd->row, FALSE);
				}
			}
		}
		else if (IDC_SIMPLEGRID4 == id)
		{
			if(pnm->code == SGN_ITEMCLICK)
			{
				DWORD dwType = ((LPNMGRID)pnm)->dwType;

				if(GCT_BUTTON == dwType)
				{
					int col = ((LPNMGRID)pnm)->col;
					int row = ((LPNMGRID)pnm)->row;
					int len = SimpleGrid_GetItemDataLen(pnm->hwndFrom, col, row);
					LPTSTR buf = (LPTSTR) _alloca(sizeof(TCHAR) * (len + 1));
					SimpleGrid_GetItemText(pnm->hwndFrom, col, row, buf);

					if(row == 0)
					{
						if(0 == _tcsicmp((LPTSTR) buf, _T("#1 On")))
						{
							// Column number
							// Row number
							// Item (cell) value
							SGITEM lpItems[] = {
								// Button column
								1, 0, (LPARAM)_T("#1 Off"),
								// Image column
								2, 0, (LPARAM) 1 //On light
							};

							for(int i = 0; i < NELEMS(lpItems); ++i)
							{
								SimpleGrid_SetItemData(pnm->hwndFrom, &lpItems[i]);
							}
						}
						else
						{
							// Column number
							// Row number
							// Item (cell) value
							SGITEM lpItems[] = {
								// Button column
								1, 0, (LPARAM)_T("#1 On"),
								// Image column
								2, 0, (LPARAM) 0 //Off light
							};

							for(int i = 0; i < NELEMS(lpItems); ++i)
							{
								SimpleGrid_SetItemData(pnm->hwndFrom, &lpItems[i]);
							}

						}
					}
					if(row == 1)
					{
						if(0 == _tcsicmp((LPTSTR) buf, _T("#2 On")))
						{
							// Column number
							// Row number
							// Item (cell) value
							SGITEM lpItems[] = {
								// Button column
								1, 1, (LPARAM)_T("#2 Off"),
								// Image column
								2, 1, (LPARAM) 1 //On light
							};

							for(int i = 0; i < NELEMS(lpItems); ++i)
							{
								SimpleGrid_SetItemData(pnm->hwndFrom, &lpItems[i]);
							}
						}
						else
						{
							// Column number
							// Row number
							// Item (cell) value
							SGITEM lpItems[] = {
								// Button column
								1, 1, (LPARAM)_T("#2 On"),
								// Image column
								2, 1, (LPARAM) 0 //Off light
							};

							for(int i = 0; i < NELEMS(lpItems); ++i)
							{
								SimpleGrid_SetItemData(pnm->hwndFrom, &lpItems[i]);
							}

						}
					}
					SimpleGrid_SetCursorPos(pnm->hwndFrom,col,row);
				}

			} //if(pnm.code==BGN_CELLCLICKED)
			return TRUE;
		}
		else if (IDC_SIMPLEGRID5 == id)
		{
			if(pnm->code == SGN_ITEMCLICK)
			{
				DWORD dwType = ((LPNMGRID)pnm)->dwType;

				if(GCT_BUTTON == dwType)
				{
					if(0 == ((LPNMGRID)pnm)->col)// "-"
						SimpleGrid_DeleteRow(pnm->hwndFrom, ((LPNMGRID)pnm)->row);
					else // "+"
					{
						SGITEM sgi;
						sgi.row = ((LPNMGRID)pnm)->row + 1;
						SimpleGrid_InsertRow(pnm->hwndFrom, sgi.row, "");

						sgi.col = 0;
						sgi.lpCurValue = (LPARAM)_T("-"); // Button text
						SimpleGrid_SetItemData(pnm->hwndFrom, &sgi);

						sgi.col = 1;
						sgi.lpCurValue = (LPARAM)_T("+"); // Button text
						SimpleGrid_SetItemData(pnm->hwndFrom, &sgi);

						sgi.col = 2;
						sgi.lpCurValue = (LPARAM)_T("Inserted Item");
						SimpleGrid_SetItemData(pnm->hwndFrom, &sgi);
					}

					SimpleGrid_RefreshGrid(pnm->hwndFrom);
				}
				return TRUE;
			}
		}
		return FALSE;
	}
	
	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void 
	MainWind::stripFileName(LPTSTR filePath)
	{
		memset(m_filePath, 0, (sizeof(m_filePath)* sizeof(char)));
		memset(m_fileName, 0, (sizeof(m_fileName)* sizeof(char)));
		wcstombs(m_filePath, filePath, MAX_PATH); //copying to local array
		char* plastSlash = strrchr(m_filePath, '\\'); //finding the last "\" to strip the filename from path
		plastSlash++; // to move forward from "\" to actual file name
		uint32_t pathLen = strlen(m_filePath);
		uint32_t fileLen = strlen(plastSlash);
		strncpy(m_fileName, plastSlash, fileLen);
		pathLen = (pathLen - fileLen);
		for(uint32_t idx=0; idx<fileLen; idx++) {
			m_filePath[pathLen+idx] = '\0';
		}
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	BOOL CALLBACK 
	dlgWndProc(
		HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam
	) {
		switch (msg)
		{
			HANDLE_DLGMSG(hwndDlg, WM_CLOSE, Main_OnClose);
			HANDLE_DLGMSG(hwndDlg, WM_COMMAND, Main_OnCommand);
			HANDLE_DLGMSG(hwndDlg, WM_INITDIALOG, Main_OnInitDialog);
			HANDLE_DLGMSG(hwndDlg, WM_NOTIFY, Main_OnNotify);
			HANDLE_DLGMSG(hwndDlg, WM_SIZE, Main_OnSize);

			case WM_NOTIFYFORMAT:
#ifdef UNICODE
				return SetDlgMsgResult(hwndDlg, WM_NOTIFYFORMAT, NFR_UNICODE);
#else
				return SetDlgMsgResult(hwndDlg, WM_NOTIFYFORMAT, NFR_ANSI);
#endif
				//// TODO: Add dialog message crackers here...
			default:
				return FALSE;
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
	Main_OnInitDialog(
		HWND hWnd, HWND hwndFocus, LPARAM lParam
	) {
		//Get window handles
		hgrid1 = GetDlgItem(hWnd, IDC_SIMPLEGRID1);
		hgrid2 = GetDlgItem(hWnd, IDC_SIMPLEGRID2);
		hgrid3 = GetDlgItem(hWnd, IDC_SIMPLEGRID3);
		hgrid4 = GetDlgItem(hWnd, IDC_SIMPLEGRID4);
		hgrid5 = GetDlgItem(hWnd, IDC_SIMPLEGRID5);
		htab =  GetDlgItem(hWnd, IDC_TAB);

		//
		//Configure tab
		//

		TCITEM tie;
		tie.mask = TCIF_TEXT;
		tie.pszText = _T("Original Baby Grid Demo");
		TabCtrl_InsertItem(htab, 0, &tie);
		tie.pszText = _T("Big Data");
		TabCtrl_InsertItem(htab, 1, &tie);
		tie.pszText = _T("Column Types");
		TabCtrl_InsertItem(htab, 2, &tie);
		tie.pszText = _T("Insert/Delete Rows");
		TabCtrl_InsertItem(htab, 3, &tie);

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

		//Set the heading font for Grid 2
		HFONT hFont = CreateFont(20, 0, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			PROOF_QUALITY, VARIABLE_PITCH | FF_MODERN, _T("ARIEL"));

		SimpleGrid_SetHeadingFont(hgrid2,hFont);

		//make grid2 header row to initial height of 21 pixels
		SimpleGrid_SetHeaderRowHeight(hgrid2, 21);
		//on row header selection hilight full row, otherwise individual cell
		SimpleGrid_SetSelectionMode(hgrid2, GSO_ROWHEADER);
		//do not allow in cell editing of all grid items initially
		SimpleGrid_EnableEdit(hgrid2,FALSE);

		//populate grid2 with initial demo data
		LoadGrid2(hgrid2);

		//make grid3 header row to initial height of 21 pixels
		SimpleGrid_SetHeaderRowHeight(hgrid3, 21);
		//use column header text
		SimpleGrid_SetColsNumbered(hgrid3,FALSE);
		//use row header text
		SimpleGrid_SetRowsNumbered(hgrid3,FALSE);
		//last column standard width
		SimpleGrid_ExtendLastColumn(hgrid3,FALSE);
		//vertical scroll set to non integral rows
		SimpleGrid_ShowIntegralRows(hgrid3,FALSE);
		//on row header selection hilight full row, otherwise individual cell
		SimpleGrid_SetSelectionMode(hgrid3, GSO_ROWHEADER);

		//Include a title for this grid
		SimpleGrid_SetTitleFont(hgrid3,hFont);
		SimpleGrid_SetTitleHeight(hgrid3, 21);
		SimpleGrid_SetTitle(hgrid3,_T("Grid's window text displayed here."));

		//populate grid3 with big data
		LoadGrid3(hgrid3);

		//make grid4 header row to initial height of 21 pixels
		SimpleGrid_SetHeaderRowHeight(hgrid4, 21);
		//allow column resizing
		SimpleGrid_SetAllowColResize(hgrid4, TRUE);
		//use column header text
		SimpleGrid_SetColsNumbered(hgrid4,FALSE);
		//on row header selection hilight full row, otherwise individual cell
		SimpleGrid_SetSelectionMode(hgrid4, GSO_CELL);

		//populate grid4 with different column types
		LoadGrid4(hgrid4);

		//Force the display of the vertical scroll in grids (if necessary)
		RECT rect;
		GetClientRect(hWnd, &rect);
		MoveWindow(hgrid2, rect.right / 3, 0, 
			rect.right - rect.right / 3, rect.bottom + 1, FALSE);

		return TRUE;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void 
	LoadGrid1(
		HWND hGrid
	) {
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
		SetFocus(hGrid);
	}

	void 
	Main_OnSize(
		HWND hwnd, UINT state, int cx, int cy
	) {
		RECT rect;
		INT iTabHeight = 20;

		GetClientRect(hwnd, &rect);
		MoveWindow(htab, 0, 0, rect.right + 1, iTabHeight, FALSE);
		iTabHeight+= 2;
		MoveWindow(hgrid1, 0, iTabHeight, rect.right / 3, rect.bottom - iTabHeight, TRUE);
		MoveWindow(hgrid2, rect.right / 3, iTabHeight, 
			rect.right - rect.right / 3, rect.bottom - iTabHeight, TRUE);
		MoveWindow(hgrid3, 0, iTabHeight, 
			rect.right, rect.bottom - iTabHeight, TRUE);
		MoveWindow(hgrid4, 0, iTabHeight, 
			rect.right, rect.bottom - iTabHeight, TRUE);
		MoveWindow(hgrid5, 0, iTabHeight, 
			rect.right, rect.bottom - iTabHeight, TRUE);
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void Main_OnClose(HWND hwnd)
	{
		EndDialog(hwnd, 0);
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
	{
		switch (id)
		{
			case IDM_ABOUT:
				DialogBox(ghInstance, MAKEINTRESOURCE(DLG_ABOUT), hwnd, (DLGPROC)About_DlgProc);
				break;
			case IDM_EXIT:
				Main_OnClose(hwnd);
				break;
		}
	}


	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void LoadGrid3(HWND hGrid)
	{
		// Column type
		// Column header text
		// Optional data (ex: combobox choices)
		SGCOLUMN lpColumns[] = {
			GCT_EDIT, _T("01"), NULL,
			GCT_EDIT, _T("02"), NULL,
			GCT_EDIT, _T("03"), NULL, 
			GCT_EDIT, _T("04"), NULL,
			GCT_EDIT, _T("05"), NULL,
			GCT_EDIT, _T("06"), NULL,
			GCT_EDIT, _T("07"), NULL,
			GCT_EDIT, _T("08"), NULL,
			GCT_EDIT, _T("09"), NULL,
			GCT_EDIT, _T("0A"), NULL,
			GCT_EDIT, _T("0B"), NULL,
			GCT_EDIT, _T("0C"), NULL,
			GCT_EDIT, _T("0D"), NULL,
			GCT_EDIT, _T("0E"), NULL,
			GCT_EDIT, _T("0F"), NULL
		};

		// Add the columns
		for(int k = NELEMS(lpColumns), m = 0; 0 < k; --k, ++m)
			SimpleGrid_AddColumn(hGrid, &lpColumns[m]);


		//Make some random data
		TCHAR buf[6];
		memset(&buf, 0, sizeof buf);

		TCHAR aryByte[85257][3];
		srand(clock());
		for(int k = 0, m = NELEMS(aryByte); k < m; ++k)
			_sntprintf(aryByte[k], 3, TEXT("%02hhX"),(BYTE)rand());

		// Add rows of random data
		for(int j = 0, colCount = NELEMS(lpColumns), itemCount = NELEMS(aryByte),
			limit = itemCount; j < itemCount; j += 0x10)
		{
			_sntprintf(buf,NELEMS(buf),_T("%05lX"),j);
			LPARAM temp[] = {
				(LPARAM)(j+0x1 < limit ? aryByte[j+0x0] : _T("")),
				(LPARAM)(j+0x2 < limit ? aryByte[j+0x1] : _T("")),
				(LPARAM)(j+0x0 < limit ? aryByte[j+0x2] : _T("")),
				(LPARAM)(j+0x3 < limit ? aryByte[j+0x3] : _T("")),
				(LPARAM)(j+0x4 < limit ? aryByte[j+0x4] : _T("")),
				(LPARAM)(j+0x5 < limit ? aryByte[j+0x5] : _T("")),
				(LPARAM)(j+0x6 < limit ? aryByte[j+0x6] : _T("")),
				(LPARAM)(j+0x7 < limit ? aryByte[j+0x7] : _T("")),
				(LPARAM)(j+0x8 < limit ? aryByte[j+0x8] : _T("")),
				(LPARAM)(j+0x9 < limit ? aryByte[j+0x9] : _T("")),
				(LPARAM)(j+0xA < limit ? aryByte[j+0xA] : _T("")),
				(LPARAM)(j+0xB < limit ? aryByte[j+0xB] : _T("")),
				(LPARAM)(j+0xC < limit ? aryByte[j+0xC] : _T("")),
				(LPARAM)(j+0xD < limit ? aryByte[j+0xD] : _T("")),
				(LPARAM)(j+0xE < limit ? aryByte[j+0xE] : _T("")),
				(LPARAM)(j+0xF < limit ? aryByte[j+0xF] : _T(""))};

			// Macro to simplify the addition of a row of values
			SimpleGrid_AddRowData(hGrid, buf, GSA_LEFT, temp, colCount);
		}
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void LoadGrid4(HWND hGrid)
	{
		//
		// Create image list
		//
		INT iImages[] = { IDR_BMP_OFF,
						  IDR_BMP_ON};

		HIMAGELIST hImageList = ImageList_Create(32, 32, ILC_COLOR32, NELEMS(iImages), 1);
		for(int i = 0; i < NELEMS(iImages); ++i){
			HBITMAP hbmp = (HBITMAP)LoadImage(ghInstance, MAKEINTRESOURCE(iImages[i]), IMAGE_BITMAP, 32, 32,
				   LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
			ImageList_Add(hImageList, hbmp, NULL);
		}

		// Set Row height to accomodate the graphics
		SimpleGrid_SetRowHeight(hGrid,34);

		//
		// Create Columns
		//

		// Column type
		// Column header text
		// Optional data (ex: combobox choices)
		SGCOLUMN lpColumns[] = {
			GCT_COMBO,  _T("Combo Column"),  _T("Ford\0Chevy\0Zaparozhets\0Volkswagen\0Toyota\0Honda\0Citroen\0Fiat\0Volvo\0"),
			GCT_BUTTON, _T("Button Column"), NULL, 
			GCT_IMAGE, _T("Image Column"),  hImageList,
			GCT_LINK, _T("Link Column"), NULL
		};

		// Add the columns
		for(int k = NELEMS(lpColumns), m = 0; 0 < k; --k, ++m)
		{
			SimpleGrid_AddColumn(hGrid, &lpColumns[m]);
			SimpleGrid_SetColWidth(hGrid, m, 100);
		}

		//
		// Add some rows
		//
		for(int i = 0; i < 2; ++i) 
			SimpleGrid_AddRow(hGrid, _T("")); //Don't care about row header text

		//
		// Set cells to data
		//

		// Column number
		// Row number
		// Item (cell) value
		SGITEM lpItems[] = {
			// Combo column
			0, 0, (LPARAM)_T("Zaparozhets"),
			0, 1, (LPARAM)_T("Citroen"),

			// Button column
			1, 0, (LPARAM)_T("#1 On"),
			1, 1, (LPARAM)_T("#2 On"),

			// Image column
			2, 0, (LPARAM) 0,
			2, 1, (LPARAM) 0,

			// Link column
			3, 0, (LPARAM)_T("The Code Project\0http:\\\\www.codeproject.com\0"),
			3, 1, (LPARAM)_T("The Daily WTF: Curious Perversions in Information Technology\0http:\\\\www.thedailywtf.com\0"),
		};

		for(int i = 0; i < NELEMS(lpItems); ++i)
		{
			SimpleGrid_SetItemData(hGrid, &lpItems[i]);
		}
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void ResetGrid5(HWND hGrid)
	{
		// Clear all
		SimpleGrid_ResetContent(hGrid);
		//set grid5 (remove rows) to automatically size columns 
		//based on the length of the text entered into the cells
		SimpleGrid_SetColAutoWidth(hgrid5, TRUE);
		//I don't want a row header, so make it 0 pixels wide
		SimpleGrid_SetRowHeaderWidth(hgrid5,0);
		//this grid won't use column headings, set header row height = 0
		SimpleGrid_SetHeaderRowHeight(hgrid5, 0);

		// Add the columns

		// Column type
		// Column header text
		// Optional data (ex: combobox choices)
		SGCOLUMN lpColumns[] = {
			GCT_BUTTON, _T(""),  NULL,
			GCT_BUTTON, _T(""),  NULL,
			GCT_EDIT, _T(""),  NULL
		 };

		for(int i = 0; i < NELEMS(lpColumns); ++i)
			SimpleGrid_AddColumn(hGrid, &lpColumns[i]);
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void InitializeGrid5Demo(void)
	{
		//create data records to display in grid5
		LPTSTR items[] = {
			_T("Record 1"),
			_T("Record 2"),
			_T("Record 3"),
			_T("Record 4"),
			_T("Record 5"),
			_T("Record 6"),
			_T("Record 7"),
			_T("Record 8"),
			_T("Record 9")
		};

		//populate grid5 with data
		LoadGrid5(hgrid5,items, NELEMS(items));
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void LoadGrid5(HWND hGrid, LPTSTR *lpszzItems, int count)
	{
		ResetGrid5(hGrid);

		SGITEM sgi;
		for(int i = 0; i < count; ++i)
		{
			SimpleGrid_AddRow(hGrid, _T("")); //Don't care about row header text
			sgi.row = i;
			sgi.col = 0;
			sgi.lpCurValue = (LPARAM)_T("-"); // Button text
			SimpleGrid_SetItemData(hGrid, &sgi);

			sgi.col = 1;
			sgi.lpCurValue = (LPARAM)_T("+"); // Button text
			SimpleGrid_SetItemData(hGrid, &sgi);

			sgi.col = 2;
			sgi.lpCurValue = (LPARAM)lpszzItems[i];
			SimpleGrid_SetItemData(hGrid, &sgi);
		}
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void LoadGrid2(HWND hGrid)
	{
		// Add some text columns

		// Column type
		// Column header text
		// Optional data (ex: combobox choices)
		SGCOLUMN lpColumns[] = {
			GCT_EDIT, _T("Multi-line \nHeadings \nSupported"),  NULL,
			GCT_EDIT, _T("\n\nName"),  NULL,
			GCT_EDIT, _T("\n\nAge"),  NULL,
			GCT_EDIT, _T(""),  NULL,
			GCT_EDIT, _T(""),  NULL
		};
		for(int i = 0; i < NELEMS(lpColumns); ++i)
			SimpleGrid_AddColumn(hGrid, &lpColumns[i]);

		// Add some rows
		for(int i = 0; i < 100; ++i) 
			SimpleGrid_AddRow(hGrid, 0 == i ? _T("Row Headers customizable") : _T(""));

		// Set cells to data

		// Column number
		// Row number
		// Item (cell) value
		SGITEM lpItems[] = {
			1, 0, (LPARAM)_T("David"),
			1, 1, (LPARAM)_T("Maggie"),
			1, 2, (LPARAM)_T("Chester"),
			1, 3, (LPARAM)_T("Molly"),
			1, 4, (LPARAM)_T("Bailey"),

			2, 0, (LPARAM)_T("43"),
			2, 1, (LPARAM)_T("41"),
			2, 2, (LPARAM)_T("3"),
			2, 3, (LPARAM)_T("3"),
			2, 4, (LPARAM)_T("1")
		};

		SimpleGrid_SetProtectColor(hGrid, RGB(210, 210, 210)); //Grey

		for(int i = 0; i < NELEMS(lpItems); ++i)
		{
			SimpleGrid_SetItemData(hGrid, &lpItems[i]);
			//Protect just these cells
			SimpleGrid_SetItemProtection(hGrid, &lpItems[i], TRUE);
		}
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	BOOL CALLBACK About_DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
			case WM_INITDIALOG:
				return TRUE;

			case WM_COMMAND:
				if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
				{
					EndDialog(hDlg, LOWORD(wParam));
					return TRUE;
				}
				break;
		}
		return FALSE;
	}

} //namespace mainwind