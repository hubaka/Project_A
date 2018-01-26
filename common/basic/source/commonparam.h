//!
//! $URL: $
//!
//!
//! \file       commonparam.h
//! \author     Anand.Kathiresan
//! \date       Jan, 2018
//!
//! \brief      @ Header file for common parameters
//!
//! @(#)$Id:    $
//!

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// This program is the confidential and proprietary product of			  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////
#ifndef COMMONPARAM_H
#define COMMONPARAM_H

/****************************************************************************/
//! @name Main Window messages.
//! @{

#define MN_CREATENEWDATABASE WM_USER + 0x01             //!<create New Database
#define MN_OPENEXISTINGDATABASE WM_USER + 0x02          //!<Open existing Database
#define MN_FINDDATABASE WM_USER + 0x03					//!<FIND Database 
#define MN_ADDEXISTINGDATATOGRID WM_USER + 0x04					//!<Add existing database entry to grid

#define copyTcharToChar(pCharVar, pTcharVar, sizeinbytes) wcstombs(pCharVar, pTcharVar, sizeinbytes);
#define copyCharToTchar(pTcharVar, pCharVar, sizeinbytes) mbstowcs(pTcharVar, pCharVar, sizeinbytes);
#define copyTcharToTchar(pTcharDes, pTcharSrc, sizeinbytes) StringCchCopy(pTcharDes, sizeinbytes, pTcharSrc);

#endif //COMMONPARAM_H
