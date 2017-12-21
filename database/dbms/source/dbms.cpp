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
		   char* path = "path1";
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

		   /* Create SQL statement */
			char* pCmd = "INSERT INTO ";
			char* pTable = "ENCRYPTTABLE ";
			char* pColumns = "(ID,FILENAME,PATH,WRACS,RDACS) ";

			createCmdString(pCmd, pTable, pColumns, pFileName, pFilePath, 1, 1);
			pSql = &cmdString[0];
		   /*str = "INSERT INTO ENCRYPTTABLE (ID,FILENAME,PATH,WRACS,RDACS) "  \
				 "VALUES (1, + quoteString(pFileName) , 'path1' , writeAccess, readAccess);";*/
				 //"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
				 //"VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
				 //"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
				 //"VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
				 //"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
				 //"VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

		   /* Execute SQL statement */
		   retVal = sqlite3_exec(m_pData, pSql, NULL, 0, &pErrMsg);
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
	void
	Dbms::createCmdString(char* pCmd, char* pTable, char* pColumns, char* pFileName, char* pFilePath,
							bool writeAccess, bool readAccess)  
	{
		char* pStr = &cmdString[0];
		char* pValueStartStr = "VALUES (";
		char* pValueEndStr = ");";
		char* pValueSep = ", ";
		char* pstrQuote = "'";
		uint32_t idx = 1;
		char intString[sizeof(uint32_t)];
		uint32_t pathLength = strlen(pFilePath);
		uint32_t cmdLength = strlen(pCmd) + strlen(pTable) + strlen(pColumns) + strlen(pValueStartStr) + + sizeof(uint32_t) \
								+ strlen(pFileName) + pathLength + sizeof(writeAccess) + sizeof(readAccess) \
								+ strlen(pValueEndStr) + 1; // extra 1 for NULL terminator
		if ((cmdLength > MAXCMDLENGTH) || (pathLength > MAX_PATH))
		{
			g_errHandle.getErrorInfo((LPTSTR)L"cmd length is invalid!");
		}
				   /*str = "INSERT INTO ENCRYPTTABLE (ID,FILENAME,PATH,WRACS,RDACS) "  \
				 "VALUES (1, 'file1' , 'path1' , writeAccess, readAccess);";*/
		strcpy(pStr, pCmd);
		strcat(pStr, pTable);
		strcat(pStr, pColumns);
		strcat(pStr, pValueStartStr);
		itoa(idx, &intString[0], 10);
		strcat(pStr, intString);
		strcat(pStr, pValueSep);
		strcat(pStr, pstrQuote);
		strcat(pStr, pFileName);
		strcat(pStr, pstrQuote);
		strcat(pStr, pValueSep);
		strcat(pStr, pstrQuote);
		strcat(pStr, pFilePath);
		strcat(pStr, pstrQuote);
		strcat(pStr, pValueSep);
		itoa(writeAccess, &intString[0], 10);
		strcat(pStr, intString);
		strcat(pStr, pValueSep);
		itoa(readAccess, &intString[0], 10);
		strcat(pStr, intString);
		strcat(pStr, pValueEndStr);

		//return pStr;
	}

} //namespace dbms