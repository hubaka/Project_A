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

#define ID_FILE_EXIT	9001
#define ID_OPEN_FILE	9002
#define ID_HELP_ABOUT	9003
#define IDC_MAIN_TOOL	9004
#define ID_FILE_NEW		9005
#define ID_FILE_OPEN	9006
#define ID_FILE_SAVEAS	9007
#define IDC_MAIN_STATUS	9008

#define ID_BABY_GRID	9009
#define IDC_SIMPLEGRID1 9010
#define IDC_TAB			9011

//Testing purpose
#define IDD_TOOLBAR		8001
#define IDC_PRESS		8002
#define IDC_OTHER		8003
#define ID_DIALOG_SHOW	8004
#define ID_DIALOG_HIDE	8005
#define IDC_SIMPLEGRID2 8006

#ifdef IDC_STATIC
#undef IDC_STATIC
#endif
#define IDC_STATIC              (-1)     // all static controls

#endif //RESOURCE_H
