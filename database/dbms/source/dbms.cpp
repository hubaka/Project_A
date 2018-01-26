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
#include "commonparam.h"
#include "errhandle.h"
#include "dbms.h"


namespace dbms
{

	static HWND g_hMainWindHandle;

	//---------------------------------------------------------------------------
	//! @def MainWindow_AddExistingDatabasetoGrid(hWnd)
	//!
	//! @brief Sends MN_ADDEXISTINGDATATOGRID signal to find mainwindow
	//!
	//! @param hWnd Handle of window
	//!
	//! @returns returns TRUE, if database found. Otherwise FALSE
	#define MainWindow_AddExistingDatabasetoGrid(hWnd, pItem) (BOOL)SNDMSG((hWnd),MN_ADDEXISTINGDATATOGRID, 0L,(LPARAM)(pItem))

	//---------------------------------------------------------------------------
	// Defines and Macros
	//---------------------------------------------------------------------------
	static errhandle::ErrHandle g_errHandle;
	static int32_t sqlCallback(void *data, int argc, char **argv, char **azColName);
	static int32_t sqlReadCallback(void *data, int argc, char **argv, char **azColName);
	static uint32_t gRowCount = 0;

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
		;
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
	Dbms::createDatabaseTable(
		void
		) {
		
			char*	pSql;
			int32_t retVal;
			char*	pErrMsg;

			/* Create SQL statement */
		   pSql = "CREATE TABLE IF NOT EXISTS ENCRYPTTABLE("  \
				 "FULLPATH TEXT PRIMARY KEY     NOT NULL," \
				 "FILENAME           TEXT    NOT NULL," \
				 "PATH            TEXT     NOT NULL," \
				 "ENCRYPTED        BIT," \
				 "DECRYPTED         BIT );";

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
	bool
	Dbms::addDbData(char* pFullPath, char* pFileName, char* pFilePath, bool  isEncrypted, bool  isDecrypted) {
			char*	pSql;
			int32_t sqRetVal;
			char*	pErrMsg;
			std::string str;
			bool retVal = FALSE;

		   /* Create SQL statement */
			char* pCmd = "INSERT INTO ";
			char* pTable = "ENCRYPTTABLE ";
			char* pColumns = "(FULLPATH,FILENAME,PATH,ENCRYPTED,DECRYPTED) ";

			createInsertCmdString(pCmd, pTable, pColumns, pFullPath, pFileName, pFilePath, isEncrypted, isDecrypted);
			pSql = &cmdString[0];

		   /* Execute SQL statement */
			sqRetVal = sqlite3_exec(m_pData, pSql, NULL, 0, &pErrMsg);
		   if ((sqRetVal != SQLITE_OK ) && (pErrMsg != 0)) {
			   MessageBox(NULL, (LPCWSTR)L"File already exists", (LPCWSTR)L"Error!", MB_OK | MB_ICONERROR);
		   }
		   else {
			   retVal = TRUE;
		   }
		   return retVal;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void
	Dbms::createInsertCmdString(char* pCmd, char* pTable, char* pColumns, char* pFullPath, char* pFileName, char* pFilePath,
								bool  isEncrypted, bool  isDecrypted)
	{
		char* pStr = &cmdString[0];
		char* pValueStartStr = "VALUES (";
		char* pValueEndStr = ");";
		char* pValueSep = ", ";
		char* pstrQuote = "'";
		char intString[sizeof(uint32_t)];

		memset(cmdString, 0, (MAXCMDLENGTH * sizeof(char)));
		uint32_t pathLength = strlen(pFilePath);
		uint32_t cmdLength = strlen(pCmd) + strlen(pTable) + strlen(pColumns) + strlen(pValueStartStr)
							+ strlen(pstrQuote) + strlen(pFullPath) + strlen(pstrQuote) + strlen(pValueSep) \
							+ strlen(pstrQuote) + strlen(pFileName) + strlen(pstrQuote) + strlen(pValueSep) \
							+ strlen(pstrQuote) + strlen(pFilePath) + strlen(pstrQuote) + strlen(pValueSep) \
							+ sizeof(isEncrypted) + strlen(pValueSep) + sizeof(isDecrypted) + strlen(pValueEndStr) + 1; // extra 1 for NULL terminator

		if ((cmdLength > MAXCMDLENGTH) || (pathLength > MAX_PATH))
		{
			g_errHandle.getErrorInfo((LPTSTR)L"cmd length is invalid!");
		}
		//sample insert command for coding
		   /*str = "INSERT INTO ENCRYPTTABLE (FULLPATH,FILENAME,PATH,WRACS,RDACS) "  \
		 "VALUES ('fullpath', 'file1' , 'path1' , writeAccess, readAccess);";*/
		strcpy(pStr, pCmd);
		strcat(pStr, pTable);
		strcat(pStr, pColumns);
		strcat(pStr, pValueStartStr);
		strcat(pStr, pstrQuote);
		strcat(pStr, pFullPath);
		strcat(pStr, pstrQuote);
		strcat(pStr, pValueSep);
		strcat(pStr, pstrQuote);
		strcat(pStr, pFileName);
		strcat(pStr, pstrQuote);
		strcat(pStr, pValueSep);
		strcat(pStr, pstrQuote);
		strcat(pStr, pFilePath);
		strcat(pStr, pstrQuote);
		strcat(pStr, pValueSep);
		itoa(isEncrypted, &intString[0], 10);
		strcat(pStr, intString);
		strcat(pStr, pValueSep);
		itoa(isDecrypted, &intString[0], 10);
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
		char* pStr = "SELECT COUNT(FULLPATH) FROM ENCRYPTTABLE;";
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
		gRowCount = atoi(argv[idx]);
	   return 0;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	pFileName
	//!
	//! \return		
	//!
	bool
		Dbms::createNewDatabase(const char* pFileName) {
		
		bool retVal = FALSE;
		// return value is ZERO, on successful creation of db file by sqlite3_open_v2
		int32_t sqRetVal = sqlite3_open_v2(pFileName, &m_pData, (SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE), NULL);
		if (sqRetVal)
		{
			closeDatabase(m_pData);
			g_errHandle.getErrorInfo((LPTSTR)L"Database opening Failed!");
			retVal = FALSE;
		}
		else {
			// create database
			createDatabaseTable();
			closeDatabase(m_pData);
			retVal = TRUE;
		}
		return retVal;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	pFileName
	//!
	//! \return		
	//!
	bool
		Dbms::openExistingDatabase(const char* pFileName) {

		bool retVal = FALSE;
		// return value is ZERO, on successful creation of db file by sqlite3_open_v2
		int32_t sqRetVal = sqlite3_open(pFileName, &m_pData);
		if (sqRetVal)
		{
			closeDatabase(m_pData);
			g_errHandle.getErrorInfo((LPTSTR)L"Database opening Failed!");
			retVal = FALSE;
		}
		else {
			//getRowCount();
			retVal = TRUE;
		}
		return retVal;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	pFileName
	//!
	//! \return		
	//!
	void
		Dbms::closeDatabase(sqlite3* pDatabase) {
		if (pDatabase != NULL) {
			sqlite3_close(pDatabase);
		}
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	pFileName
	//! \param[in]	pFilePath
	//! \param[in]	isEncrypted
	//! \param[in]	isDecrypted
	//!
	//! \return		
	//!
	bool
		Dbms::updateDbData(char* pFullPath, bool  isEncrypted, bool  isDecrypted) {
		bool retVal = FALSE;
		char*	pSql;
		int32_t sqRetVal;
		char*	pErrMsg;
		std::string str;

		/* Create SQL statement */
		char* pCmd = "UPDATE ";
		char* pTable = "ENCRYPTTABLE ";
		char* pEncrypted = " ENCRYPTED ";
		char* pDecrypted = " DECRYPTED ";

		createUpdateCmdString(pCmd, pTable, pFullPath, pEncrypted, pDecrypted, isEncrypted, isDecrypted);
		pSql = &cmdString[0];

		/* Execute SQL statement */
		sqRetVal = sqlite3_exec(m_pData, pSql, NULL, 0, &pErrMsg);
		if (sqRetVal != SQLITE_OK) {
			g_errHandle.getErrorInfo((LPTSTR)L"Database creation Failed!");
		}
		else {
			retVal = TRUE;
		}
		return retVal;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void
		Dbms::createUpdateCmdString(char* pCmd, char* pTable, char* pFullPath, char* pEncrypted, char* pDecrypted, bool  isEncrypted, bool  isDecrypted)
	{
		char* pStr = &cmdString[0];
		char* pSetStr = "SET";
		char* pEqualStr = "= ";
		char* pWhereStr = " WHERE ";
		char* pFullPathStr = "FULLPATH ";
		char* pEndStr = ";";
		char* pCommaStr = ", ";
		char* pstrQuote = "'";
		char intString[sizeof(uint32_t)];

		memset(cmdString, 0, (MAXCMDLENGTH * sizeof(char)));
		uint32_t cmdLength = strlen(pCmd) + strlen(pTable) + strlen(pSetStr) +  strlen(pEncrypted) + strlen(pEqualStr) + sizeof(isEncrypted) \
							 + strlen(pCommaStr) + strlen(pDecrypted) + strlen(pEqualStr) + sizeof(isDecrypted) + sizeof(pWhereStr) + strlen(pFullPathStr) \
							 + strlen(pEqualStr) + strlen(pstrQuote) + strlen(pFullPath) + strlen(pstrQuote) + strlen(pEndStr) + 1; // extra 1 for NULL terminator

		if ((cmdLength > MAXCMDLENGTH))
		{
			g_errHandle.getErrorInfo((LPTSTR)L"cmd length is invalid!");
		}
		//sample update command for coding
		// "UPDATE ENCRYPTTABLE set ENCRYPTED = 1, DECRYPTED = 1 where ID = rowCount;"
		strcpy(pStr, pCmd);
		strcat(pStr, pTable);
		strcat(pStr, pSetStr);
		strcat(pStr, pEncrypted);
		strcat(pStr, pEqualStr);
		itoa(isEncrypted, &intString[0], 10);
		strcat(pStr, intString);
		strcat(pStr, pCommaStr);
		strcat(pStr, pDecrypted);
		strcat(pStr, pEqualStr);
		itoa(isDecrypted, &intString[0], 10);
		strcat(pStr, intString);
		strcat(pStr, pWhereStr);
		strcat(pStr, pFullPathStr);
		strcat(pStr, pEqualStr);
		strcat(pStr, pstrQuote);
		strcat(pStr, pFullPath);
		strcat(pStr, pstrQuote);
		strcat(pStr, pEndStr);
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	pFileName
	//! \param[in]	pFilePath
	//! \param[in]	isEncrypted
	//! \param[in]	isDecrypted
	//!
	//! \return		
	//!
	bool
		Dbms::deleteDbData(char* pFullPath) {
		bool retVal = FALSE;
		char*	pSql;
		int32_t sqRetVal;
		char*	pErrMsg;
		std::string str;

		/* Create SQL statement */
		char* pCmd = "DELETE ";
		char* pTable = "ENCRYPTTABLE ";

		createDeleteCmdString(pCmd, pTable, pFullPath);
		pSql = &cmdString[0];

		/* Execute SQL statement */
		sqRetVal = sqlite3_exec(m_pData, pSql, NULL, 0, &pErrMsg);
		if (sqRetVal != SQLITE_OK) {
			g_errHandle.getErrorInfo((LPTSTR)L"Database creation Failed!");
		}
		else {
			retVal = TRUE;
		}
		return retVal;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void
		Dbms::createDeleteCmdString(char* pCmd, char* pTable, char* pFullPath)
	{
		char* pStr = &cmdString[0];
		char* pFromStr = "from ";
		char* pWhereStr = "WHERE ";
		char* pFullPathStr = "FULLPATH ";
		char* pEqualStr = "= ";
		char* pEndStr = "; ";
		char* pstrQuote = "'";
		char intString[sizeof(uint32_t)];

		memset(cmdString, 0, (MAXCMDLENGTH * sizeof(char)));
		uint32_t cmdLength = strlen(pCmd) + strlen(pFromStr) + strlen(pTable) + strlen(pWhereStr) + strlen(pFullPathStr) + sizeof(pEqualStr) \
			+ strlen(pstrQuote) + strlen(pFullPath) + strlen(pstrQuote) + strlen(pEndStr) + 1; // extra 1 for NULL terminator

		if ((cmdLength > MAXCMDLENGTH))
		{
			g_errHandle.getErrorInfo((LPTSTR)L"cmd length is invalid!");
		}
		//sample update command for coding
		// "DELETE from COMPANY where ID=2;"
		strcpy(pStr, pCmd);
		strcat(pStr, pFromStr);
		strcat(pStr, pTable);
		strcat(pStr, pWhereStr);
		strcat(pStr, pFullPathStr);
		strcat(pStr, pEqualStr);
		strcat(pStr, pstrQuote);
		strcat(pStr, pFullPath);
		strcat(pStr, pstrQuote);
		strcat(pStr, pEndStr);
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	pFileName
	//! \param[in]	pFilePath
	//! \param[in]	isEncrypted
	//! \param[in]	isDecrypted
	//!
	//! \return		
	//!
	bool
		Dbms::readDbData(void) {
		bool retVal = FALSE;
		char*	pSql;
		int32_t sqRetVal;
		char*	pErrMsg;
		std::string str;

		/* Create SQL statement */
		char* pCmd = "DELETE ";
		char* pTable = "ENCRYPTTABLE ";

		/*createDeleteCmdString(pCmd, pTable, pFullPath);
		pSql = &cmdString[0];*/

		pSql = "SELECT * FROM ENCRYPTTABLE";
		/* Execute SQL statement */
		sqRetVal = sqlite3_exec(m_pData, pSql, sqlReadCallback, 0, &pErrMsg);
		if (sqRetVal != SQLITE_OK) {
			g_errHandle.getErrorInfo((LPTSTR)L"Database creation Failed!");
		}
		else {
			retVal = TRUE;
		}
		return retVal;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void
		Dbms::createReadCmdString(char* pCmd, char* pTable, char* pFullPath)
	{
		char* pStr = &cmdString[0];
		char* pFromStr = "from ";
		char* pWhereStr = "WHERE ";
		char* pFullPathStr = "FULLPATH ";
		char* pEqualStr = "= ";
		char* pEndStr = "; ";
		char* pstrQuote = "'";
		char intString[sizeof(uint32_t)];

		memset(cmdString, 0, (MAXCMDLENGTH * sizeof(char)));
		uint32_t cmdLength = strlen(pCmd) + strlen(pFromStr) + strlen(pTable) + strlen(pWhereStr) + strlen(pFullPathStr) + sizeof(pEqualStr) \
			+ strlen(pstrQuote) + strlen(pFullPath) + strlen(pstrQuote) + strlen(pEndStr) + 1; // extra 1 for NULL terminator

		if ((cmdLength > MAXCMDLENGTH))
		{
			g_errHandle.getErrorInfo((LPTSTR)L"cmd length is invalid!");
		}
		//sample update command for coding
		// "DELETE from COMPANY where ID=2;"
		strcpy(pStr, pCmd);
		strcat(pStr, pFromStr);
		strcat(pStr, pTable);
		strcat(pStr, pWhereStr);
		strcat(pStr, pFullPathStr);
		strcat(pStr, pEqualStr);
		strcat(pStr, pstrQuote);
		strcat(pStr, pFullPath);
		strcat(pStr, pstrQuote);
		strcat(pStr, pEndStr);
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	int32_t
		sqlReadCallback(void *data, int argc, char **argv, char **azColName)
	{
		DBITEM dbItem;
		strcpy(dbItem.sfileName, argv[1]);
		strcpy(dbItem.sfilePath, argv[2]);
		dbItem.sEncrypted = atoi(argv[3]);
		dbItem.sDecrypted = atoi(argv[4]);

		MainWindow_AddExistingDatabasetoGrid(g_hMainWindHandle, &dbItem);
		return 0;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	void
		Dbms::attachWindHandle(HWND hWindowHandle)
	{
		g_hMainWindHandle = hWindowHandle;
	}

} //namespace dbms