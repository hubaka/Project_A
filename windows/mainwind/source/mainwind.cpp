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
			m_hwnd = CreateWindowEx(
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

			if(m_hwnd == NULL)
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
			ShowWindow(m_hwnd, m_nCmdShow);
			UpdateWindow(m_hwnd);
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

			HMENU hMenu, hSubMenu;
			HICON hIcon, hIconSm;

			hMenu = CreateMenu();

			hSubMenu = CreatePopupMenu();
			AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, (LPCWSTR)L"&Exit");
			AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, (LPCWSTR)L"&File");
			AppendMenu(hMenu, MF_STRING, ID_OPEN_FILE, (LPCWSTR)L"&Open");
			AppendMenu(hMenu, MF_STRING, ID_HELP_ABOUT, (LPCWSTR)L"&Help");
			SetMenu(hWnd, hMenu);


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
	   else
	   {
		  pParent = (MainWind*)GetWindowLongPtr(hWnd,GWL_USERDATA);
		  if(!pParent) return DefWindowProc(hWnd,uMsg,wParam,lParam);
	   }

	   pParent->m_hwnd = hWnd;
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
							PostMessage(m_hwnd, WM_CLOSE, 0, 0);
							break;
						}
					case ID_OPEN_FILE:
						{
							getFileName();
							break;
						}
					case ID_HELP_ABOUT:
						{
							MessageBox(m_hwnd, (LPCWSTR)L"No help document", (LPCWSTR)L"Message",
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
					DestroyWindow(m_hwnd);
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
					MessageBox(m_hwnd, (LPCWSTR)szFileName, (LPCWSTR)L"This program is:", MB_OK | MB_ICONINFORMATION);
					break;
				}
			default:
				return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
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
			ofn.hwndOwner = m_hwnd;
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

} //namespace mainwind