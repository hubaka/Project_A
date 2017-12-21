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

#define MAXCMDLENGTH (2*MAX_PATH)

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
		char		cmdString[MAXCMDLENGTH];

		void 
		createDb(
			void
		);

		void createCmdString(char* pCmd, char* pTable, char* pColumns, char* pFileName, char* pFilePath, bool writeAccess, bool readAccess);

	};

} //namespace dbms

#endif //DBMS_H