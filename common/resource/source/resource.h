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

//#define IDR_MYMENU 101
//#define IDI_MYICON 201

#define ID_FILE_EXIT	9001
#define ID_STUFF_GO		9002

#define ID_HELP_ABOUT	9003
#define IDD_ABOUT		9004

#define ID_DIALOG_TOOL	9005
#define IDD_DIALOG		9006
#define ID_SHOW_MESSAGE 9007
#define ID_HIDE_MESSAGE 9008
#define IDC_PRESS		9009
#define IDC_OTHER		9010

#ifdef IDC_STATIC
#undef IDC_STATIC
#endif
#define IDC_STATIC              (-1)     // all static controls

#endif //RESOURCE_H
