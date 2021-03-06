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
#ifndef RESOURCE_H
#define RESOURCE_H

#define ID_MENU_EXIT			9001
#define ID_MENU_OPEN			9002
#define ID_SUBMENU_OPEN_FILE	9003
#define ID_SUBMENU_OPEN_FOLDER	9004
#define ID_MENU_HELP			9005
#define ID_TOOBAR				9006
#define ID_TOOL_OPEN_FILE		9007
#define ID_TOOL_OPEN_FOLDER		9008
#define IDC_STATUSBAR			9009

#define ID_BABY_GRID			9010
#define IDC_SIMPLEGRID1			9011
#define IDC_TAB					9012
#define ID_RECYCLE_BIN_IMAGE	9013

#define IDD_USERCREDENTIALS		9014
#define IDD_NEWUSERCREDENTIALS	9015
#define IDE_FIRSTNAMEEDIT		9016
#define IDE_LASTNAMEEDIT		9017
#define IDE_USERNAMEEDIT		9018
#define IDE_PASSWORDEDIT		9019
#define IDE_RETYPEPASSWORDEDIT	9020
#define IDE_NEWUSER				9021
#define IDE_OK					9022
#define IDE_CANCEL				9023
#define ID_LARGEICON			9024
#define ID_SMALLICON			9025
#define ID_MENU_ABOUT			9026
#define IDE_FIRSTNAMEEDIT_TEXT	9027
#define IDE_LASTNAMEEDIT_TEXT	9028
#define IDE_USERNAMEEDIT_TEXT	9029
#define IDE_PASSWORDEDIT_TEXT	9030
#define IDE_RETYPEPASSWORDEDIT_TEXT	9031

//Testing purpose
#define IDD_TOOLBAR		8001
#define IDC_PRESS		8002
#define IDC_OTHER		8003
#define ID_DIALOG_SHOW	8004
#define ID_DIALOG_HIDE	8005
#define IDC_SIMPLEGRID2 8006
#define IDC_SIMPLEGRID3 8007
#define IDC_SIMPLEGRID4 8008
#define IDC_SIMPLEGRID5 8009
#define IDM_ABOUT		8010
#define IDM_EXIT        8011
#define DLG_ABOUT		8012
#define IDR_BMP_OFF		8013
#define IDR_BMP_ON		8014
#define IDR_ICO_MAIN	8015
#define BMP_SIMPLEGRID_DEMO 8016
#define IDC_STATIC1		8017
#define IDC_STATIC2		8018
#define IDC_STATIC3		8019

#ifdef IDC_STATIC
#undef IDC_STATIC
#endif
#define IDC_STATIC              (-1)     // all static controls

#endif //RESOURCE_H
