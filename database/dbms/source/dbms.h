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
//---------------------------------------------------------------------------
//! @var SGITEM
//! @brief A grid item structure
//! @var LPSGITEM
//! @brief Pointer to a grid item structure
//! @struct tagSGITEM
//! @brief This is the address data associated with a grid cell
typedef struct tagDBITEM {
	char sfileName[MAX_PATH];				//!< file name
	char sfilePath[MAX_PATH];				//!< file path
	bool sEncrypted;                 //!< file is encrypted or not
	bool sDecrypted;                 //!< file is decrypted or not 
} DBITEM, *LPDBITEM;

//---------------------------------------------------------------------------
namespace dbms
{

	static const uint32_t MAXCMDLENGTH = (2 * MAX_PATH);

	class Dbms {
	public:
		Dbms(void);
		~Dbms(void);
		bool createNewDatabase(const char* pFileName);
		bool openExistingDatabase(const char* pFileName);
		bool addDbData(char* pFullPath, char* pFileName, char* pFilePath, bool isEncrypted, bool isDecrypted);
		bool updateDbData(char* pFullPath, bool  isEncrypted, bool  isDecrypted);
		bool checkDbEncrypt(char* pFullPath);
		void deleteDbData(char* pFullPath);
		bool closeDatabase(void);
		bool readDbData(void);
		void attachWindHandle(HWND hWindHandle);
		bool validateExistingDatabase(const char* pFileName);
	private:
		sqlite3*	m_pData;
		char		cmdString[MAXCMDLENGTH];
		uint8_t     m_isDbOpened;

		void createDatabaseTable(void);
		void createInsertCmdString(char* pCmd, char* pTable, char* pColumns, char* pFullPath, char* pFileName, char* pFilePath, bool  isEncrypted, bool  isDecrypted);
		void createUpdateCmdString(char* pCmd, char* pTable, char* pEncrypted, char* pFullPath, char* pDecrypted, bool  isEncrypted, bool  isDecrypted);
		void createCmdString(char* pTable, char* pFullPath);
		void getRowCount(void);

	};

} //namespace dbms

#endif //DBMS_H