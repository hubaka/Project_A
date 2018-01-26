//!
//! $URL: $
//!
//!
//! \file       
//! \author     Anand.Kathiresan
//! \date       
//!
//! \brief      @ Header file of cryptomain. This file contains the cryto 
//!					class
//!
//! @(#)$Id:    $
//!

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// This program is the confidential and proprietary product of			  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
#ifndef ENCRYPTMAIN_H
#define ENCRYPTMAIN_H
//---------------------------------------------------------------------------
#include "sys.h"

namespace encryptmain
{
	class EncryptDBFile {
	public:
		EncryptDBFile(void);
		~EncryptDBFile(void);
		void getUserCredentials(HINSTANCE hParentInstance, HWND hParentWindow);
		char* getDatabaseName(void);

	private:

	};

} //namespace encryptmain

#endif //ENCRYPTMAIN_H