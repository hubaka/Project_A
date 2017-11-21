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
#include <strsafe.h>
#include <commctrl.h> // included in order to use tool bar related functionalities
#include "resource.h"
#include "mainwind.h"

HWND g_hToolbar = NULL;

extern "C" IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

namespace mainwind
{

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	MainWind::MainWind(
		HINSTANCE&	hInstance,
		const char*	p_className,
		int			nCmdShow
		) : m_hInstance(hInstance),
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
	MainWind::regClass(
		void
		) {
		    //Step 1: Registering the Window Class
			m_wndClassEx.cbSize        = sizeof(WNDCLASSEX);
			m_wndClassEx.style         = 0;
			m_wndClassEx.lpfnWndProc   = (WNDPROC)StaticWndProc;
			m_wndClassEx.cbClsExtra    = 0;
			m_wndClassEx.cbWndExtra    = 0;
			m_wndClassEx.hInstance     = m_hInstance;
			m_wndClassEx.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
			m_wndClassEx.hCursor       = LoadCursor(NULL, IDC_ARROW);
			m_wndClassEx.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
			m_wndClassEx.lpszMenuName  = NULL;
			m_wndClassEx.lpszClassName = (LPCWSTR)m_pClassName;
			m_wndClassEx.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

			if(!RegisterClassEx(&m_wndClassEx))
			{
				MessageBox(NULL, (LPCWSTR)L"Window Registration Failed!", (LPCWSTR)L"Error!",
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
	MainWind::createWind(
		void
		) {
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
							m_hInstance,						// Instance
							this);								//  Pass this class To WM_CREATE

			if(m_hWnd == NULL)
			{
				MessageBox(NULL, (LPCWSTR)L"Window Creation Failed!", (LPCWSTR)L"Error!",
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
	MainWind::showWind(
		void
		) {
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
			SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)"Hi there :)");
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
			case WM_LBUTTONDOWN:
				{
					//MAX_PATH is a handy macro included via <windows.h> that 
					//is defined to the maximum length of a buffer needed to store a filename under Win32
					char szFileName[MAX_PATH];
					HINSTANCE hInstance = GetModuleHandle(NULL);
					GetModuleFileName(hInstance, (LPWCH)szFileName, MAX_PATH);
					MessageBox(m_hWnd, (LPCWSTR)szFileName, (LPCWSTR)L"This program is:", MB_OK | MB_ICONINFORMATION);
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
		MainWind::ErrorExit(
			LPTSTR lpszFunction
		) { 
			// Retrieve the system error message for the last-error code

			LPVOID lpMsgBuf;
			LPVOID lpDisplayBuf;
			DWORD dw = GetLastError(); 

			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				dw,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf,
				0, NULL );

			// Display the error message and exit the process

			lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
				(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
			StringCchPrintf((LPTSTR)lpDisplayBuf, 
				LocalSize(lpDisplayBuf) / sizeof(TCHAR),
				TEXT("%s failed with error %d: %s"), 
				lpszFunction, dw, lpMsgBuf); 
			MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

			LocalFree(lpMsgBuf);
			LocalFree(lpDisplayBuf);
			ExitProcess(dw); 
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
			ofn.lpstrFilter = (LPCWSTR) "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
			ofn.lpstrFile = (LPWSTR) szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = (LPCWSTR) "txt";

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

} //namespace mainwind