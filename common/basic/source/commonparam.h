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
#define MN_ADDEXISTINGDATATOGRID WM_USER + 0x04			//!<Add existing database entry to grid
#define MN_VERIFYUSERCREDENTIALS WM_USER + 0x05			//!<verify user credentials

//---------------------------------------------------------------------------
//! @var copyTcharToChar
//! @brief copy the string from CHAR variabel to TCHAR variable
//! @Param[in]	pCharVar	input char string
//! @Param[out]	pTcharVar	output TCHAR string
//! @Param[in]	sizeinbytes	size of the string to be copied, in bytes
#define copyTcharToChar(pCharVar, pTcharVar, sizeinbytes) wcstombs(pCharVar, pTcharVar, sizeinbytes);

//---------------------------------------------------------------------------
//! @var copyCharToTchar
//! @brief copy the string from TCHAR variabel to CHAR variable
//! @Param[in]	pTcharVar	input TCHAR string
//! @Param[out]	pCharVar	output CHAR string
//! @Param[in]	sizeinbytes	size of the string to be copied, in bytes
#define copyCharToTchar(pTcharVar, pCharVar, sizeinbytes) mbstowcs(pTcharVar, pCharVar, sizeinbytes);

//---------------------------------------------------------------------------
//! @var copyTcharToTchar
//! @brief copy the string from TCHAR variabel to TCHAR variable
//! @Param[in]	pTcharSrc	input TCHAR string
//! @Param[out]	pTcharDes	output TCHAR string
//! @Param[in]	sizeinbytes	size of the string to be copied, in bytes
#define copyTcharToTchar(pTcharDes, pTcharSrc, sizeinbytes) StringCchCopy(pTcharDes, sizeinbytes, pTcharSrc);

#endif //COMMONPARAM_H
