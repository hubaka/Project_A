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
#include "errhandle.h"
#include "dbms.h"


namespace dbms
{

	//---------------------------------------------------------------------------
	// Defines and Macros
	//---------------------------------------------------------------------------
	static errhandle::ErrHandle g_errHandle;

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	Dbms::Dbms(
		void
		) {
			
			int32_t retVal = sqlite3_open("MyDb.db", &m_pData);
			if (retVal)
			{
			  //cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(m_pData) << endl << endl;
			  sqlite3_close(m_pData);
			  g_errHandle.getErrorInfo((LPTSTR)L"Database opening Failed!");
			}

			createDb();

		   ///* Create SQL statement */
		   //sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
				 //"VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
				 //"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
				 //"VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
				 //"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
				 //"VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
				 //"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
				 //"VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

		   ///* Execute SQL statement */
		   //retVal = sqlite3_exec(m_pData, sql, NULL, 0, &zErrMsg);
		   //if( retVal != SQLITE_OK ){
			  // g_errHandle.getErrorInfo((LPTSTR)L"Database creation Failed!");
		   //}
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	Dbms::~Dbms(
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
	Dbms::createDb(
		void
		) {
		
			char*	pSql;
			int32_t retVal;
			char*	pErrMsg;

			/* Create SQL statement */
		   pSql = "CREATE TABLE IF NOT EXISTS ENCRYPTTABLE("  \
				 "ID INT PRIMARY KEY     NOT NULL," \
				 "FILENAME           TEXT    NOT NULL," \
				 "PATH            TEXT     NOT NULL," \
				 "WRACS        BIT," \
				 "RDACS         BIT );";

		   /* Execute SQL statement */
		   retVal = sqlite3_exec(m_pData, pSql, NULL, 0, &pErrMsg);
		   
		   if( retVal != SQLITE_OK ){
			   g_errHandle.getErrorInfo((LPTSTR)L"Database creation Failed!");
		   }

		   char* str1 = "file1";
		   char path[12] = "path1";
		   addTableData(str1, path, 1, 1);

	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void
	Dbms::addTableData(
		char* pFileName,
		char* pFilePath,
		bool  writeAccess,
		bool  readAccess
		) {
		
			char*	pSql;
			int32_t retVal;
			char*	pErrMsg;
			std::string str;

			str = quoteString(pFileName);
		   /* Create SQL statement */
		   /*str = "INSERT INTO ENCRYPTTABLE (ID,FILENAME,PATH,WRACS,RDACS) "  \
				 "VALUES (1, " + quoteString(pFileName) + ", 'path1' ," + writeAccess + "," + readAccess + ");";*/
				 //"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
				 //"VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
				 //"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
				 //"VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
				 //"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
				 //"VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

		   /* Execute SQL statement */
		   retVal = sqlite3_exec(m_pData, str.c_str(), NULL, 0, &pErrMsg);
		   if( retVal != SQLITE_OK ){
			   g_errHandle.getErrorInfo((LPTSTR)L"Database creation Failed!");
		   }
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	char*
	Dbms::quoteString(char* pString) 
	{
		char* pCmd = "INSERT INTO";
		char* pStr = &filePath[0];
		strcpy(pStr, pCmd);
		return pStr;
	}

} //namespace dbms