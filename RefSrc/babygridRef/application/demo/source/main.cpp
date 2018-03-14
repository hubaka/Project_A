/****************************************************************************
 *                                                                          *
 * File    : main.c                                                         *
 *                                                                          *
 * Purpose : Generic dialog based Win32 application.                        *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

/* 
 * Either define WIN32_LEAN_AND_MEAN, or one or more of NOCRYPT,
 * NOSERVICE, NOMCX and NOIME, to decrease compile time (if you
 * don't need these defines -- see windows.h).
 */

// Begin MSVC++ Support
#define HANDLE_DLGMSG(hwnd,message,fn)  case (message): return SetDlgMsgResult((hwnd),(message),HANDLE_##message((hwnd),(wParam),(lParam),(fn)))  /* added 05-01-29 */
// End MSVC++ Support

#define WIN32_LEAN_AND_MEAN
/* #define NOCRYPT */
/* #define NOSERVICE */
/* #define NOMCX */
/* #define NOIME */

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include "resource.h"

#include "simplegrid.h"

#pragma comment(lib,"COMCTL32.LIB")

/** Macroes *****************************************************************/
#define NELEMS(a)  (sizeof(a) / sizeof((a)[0]))

/** Global variables ********************************************************/

static HINSTANCE ghInstance;

HWND hgrid1, hgrid2, hgrid3, hgrid4, hgrid5, htab;

/** Functions ***************************************************************/

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

void LoadGrid1(HWND hGrid)
{
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

void Main_OnClose(HWND hwnd)
{
    EndDialog(hwnd, 0);
}

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

static BOOL Main_OnNotify(HWND hwnd, INT id, LPNMHDR pnm)
{
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

BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    //Get window handles
    hgrid1 = GetDlgItem(hwnd, IDC_SIMPLEGRID1);
    hgrid2 = GetDlgItem(hwnd, IDC_SIMPLEGRID2);
    hgrid3 = GetDlgItem(hwnd, IDC_SIMPLEGRID3);
    hgrid4 = GetDlgItem(hwnd, IDC_SIMPLEGRID4);
	hgrid5 = GetDlgItem(hwnd, IDC_SIMPLEGRID5);
    htab =  GetDlgItem(hwnd, IDC_TAB);

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
    GetClientRect(hwnd, &rect);
    MoveWindow(hgrid2, rect.right / 3, 0, 
        rect.right - rect.right / 3, rect.bottom + 1, FALSE);

    return TRUE;
}

void Main_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
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

/****************************************************************************
 *                                                                          *
 * Function: MainDlgProc                                                    *
 *                                                                          *
 * Purpose : Process messages for the Main dialog.                          *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

BOOL CALLBACK Main_DlgProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        HANDLE_DLGMSG(hwndDlg, WM_CLOSE, Main_OnClose);
        HANDLE_DLGMSG(hwndDlg, WM_COMMAND, Main_OnCommand);
        HANDLE_DLGMSG(hwndDlg, WM_INITDIALOG, Main_OnInitDialog);
        HANDLE_DLGMSG(hwndDlg, WM_SIZE, Main_OnSize);
        HANDLE_DLGMSG(hwndDlg, WM_NOTIFY, Main_OnNotify);

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

/****************************************************************************
 *                                                                          *
 * Function: WinMain                                                        *
 *                                                                          *
 * Purpose : Initialize the application.  Register a window class,          *
 *           create and display the main window and enter the               *
 *           message loop.                                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    WNDCLASSEX wcx;

    ghInstance = hInstance;

    // Initialize common controls. Also needed for MANIFEST's.
    InitCommonControls();

    // Initialize Simple Grid
    InitSimpleGrid(hInstance);  //initializes SimpleGrid control
    //only call this function once in your program 

    // Get system dialog information.
    wcx.cbSize = sizeof(wcx);
    if (!GetClassInfoEx(NULL, MAKEINTRESOURCE(32770), &wcx))
        return 0;

    // Add our own stuff.
    wcx.hInstance = hInstance;
    wcx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_ICO_MAIN));
    wcx.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_ICO_SMALL));
    wcx.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wcx.lpszClassName = _T("Demo1");
    if (!RegisterClassEx(&wcx))
        return 0;


    // The user interface is a modal dialog box.
    return DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)Main_DlgProc);
}
