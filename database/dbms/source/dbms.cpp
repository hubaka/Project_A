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
	static int32_t sqlCallback(void *data, int argc, char **argv, char **azColName);
	static uint32_t rowCount = 0;

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
			// create database
			createDb();
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

			getRowCount();
			createCmdString(pCmd, pTable, pColumns, (rowCount+1), pFileName, pFilePath, 1, 1);
			pSql = &cmdString[0];

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
	Dbms::createCmdString(char* pCmd, char* pTable, char* pColumns, uint32_t rowCount, char* pFileName, char* pFilePath,
							bool writeAccess, bool readAccess)  
	{
		char* pStr = &cmdString[0];
		char* pValueStartStr = "VALUES (";
		char* pValueEndStr = ");";
		char* pValueSep = ", ";
		char* pstrQuote = "'";
		char intString[sizeof(uint32_t)];
		uint32_t pathLength = strlen(pFilePath);
		uint32_t cmdLength = strlen(pCmd) + strlen(pTable) + strlen(pColumns) + strlen(pValueStartStr) + + sizeof(uint32_t) \
								+ strlen(pFileName) + pathLength + sizeof(writeAccess) + sizeof(readAccess) \
								+ strlen(pValueEndStr) + 1; // extra 1 for NULL terminator
		if ((cmdLength > MAXCMDLENGTH) || (pathLength > MAX_PATH))
		{
			g_errHandle.getErrorInfo((LPTSTR)L"cmd length is invalid!");
		}
		//sample insert command for coding
		   /*str = "INSERT INTO ENCRYPTTABLE (ID,FILENAME,PATH,WRACS,RDACS) "  \
		 "VALUES (1, 'file1' , 'path1' , writeAccess, readAccess);";*/
		strcpy(pStr, pCmd);
		strcat(pStr, pTable);
		strcat(pStr, pColumns);
		strcat(pStr, pValueStartStr);
		itoa(rowCount, &intString[0], 10);
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
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void 
	Dbms::getRowCount(void)
	{

		char*	pErrMsg;
		char* pStr = "SELECT COUNT(ID) FROM ENCRYPTTABLE;";
		/* Execute SQL statement */
		int32_t retVal = sqlite3_exec(m_pData, pStr, sqlCallback, 0, &pErrMsg);
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
	int32_t 
	sqlCallback(void *data, int argc, char **argv, char **azColName)
	{
		uint32_t idx = 0;
		while(argv[idx] != NULL) {
			rowCount = atoi(argv[idx]);
			idx++;
		}
	   return 0;
	}
} //namespace dbms