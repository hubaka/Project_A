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

//---------------------------------------------------------------------------
#ifndef DBMS_H
#define DBMS_H
//---------------------------------------------------------------------------
#include "sys.h"
#include "sqlite3.h"

//---------------------------------------------------------------------------
// Public structures
//---------------------------------------------------------------------------

namespace dbms
{

	class Dbms {
	public:
		Dbms(
			void
		);

		~Dbms(
			void
		);

		void 
		addTableData(
			char* pFileName,
			char* pFilePath,
			bool  writeAccess,
			bool  readAccess
		);
	private:
		sqlite3*	m_pData;
		char		filePath[MAX_PATH];

		void 
		createDb(
			void
		);

		char* quoteString(char* pString);

	};

} //namespace dbms

#endif //DBMS_H