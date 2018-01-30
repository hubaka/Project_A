//!
//! $URL: $
//!
//!	\Project	
//!
//! \file       cryptomain.cpp
//! \author     Anand.Kathiresan
//! \date       Jan, 2018
//!
//! \brief      @ Source File for cryotmain.cpp
//!
//! @(#)$Id:    $
//!

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// This program is the confidential and proprietary product of			  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

#include <windows.h>	// Windows-specific header file
#include <WinCred.h>
#include "resource.h"	// reqd. for resource like icon, dialog features
#include "files.h"		// to include crypto's file declaration
#include "rsa.h"		// to include crypto's rsa declaration
#include "osrng.h"		// to include crypto's AutoSeededRandomPool declaration
#include "hex.h"		// to include crypto's HexEncoder declaration
#include "modes.h"		// to include crypto's CTR_Mode declaration
#include "errhandle.h"	// reqd. for error handling
#include "commonparam.h" // reqd. for mainwindow messages
#include "cryptomain.h" // reqd. for crypt class

#pragma comment(lib,"Credui.lib")

namespace encryptmain
{
	using namespace CryptoPP;
	using namespace std;

	//---------------------------------------------------------------------------
	// Defines and Macros
	//---------------------------------------------------------------------------
	static const uint32_t MAXFIRSTNAMECHAR = 10;
	static const uint32_t MAXUSERNAMECHAR = 8;
	static const uint32_t MAXPASSWORDCHAR = 8;
	static errhandle::ErrHandle g_errHandle;
	static bool	newUserCreation = FALSE;
	static HWND gParentWindow;
	char	gFileName[MAX_PATH] = {0};
	char	gFilePassword[MAX_PATH] = { 0 };

	//---------------------------------------------------------------------------
	//! @def MainWindow_Close(hWnd)
	//!
	//! @brief Sends WM_CLOSE signal to window handle hWnd
	//!
	//! @param hWnd Handle of window
	//!
	//! @returns The return value is not meaningful. 
	#define MainWindow_Close(hWnd) (BOOL)SNDMSG((hWnd),WM_CLOSE, 0L,0L)

	//---------------------------------------------------------------------------
	//! @def MainWindow_OpenExistingDatabase(hWnd)
	//!
	//! @brief Sends MN_OPENEXISTINGDATABASE signal to open existing database
	//!
	//! @param hWnd Handle of window
	//!
	//! @returns The return value is not meaningful. 
	#define MainWindow_OpenExistingDatabase(hWnd) (BOOL)SNDMSG((hWnd),MN_OPENEXISTINGDATABASE, 0L,0L)

	//---------------------------------------------------------------------------
	//! @def MainWindow_CreateNewDatabase(hWnd)
	//!
	//! @brief Sends MN_CREATENEWDATABASE signal to create new database
	//!
	//! @param hWnd Handle of window
	//!
	//! @returns The return value is not meaningful. 
	#define MainWindow_CreateNewDatabase(hWnd) (BOOL)SNDMSG((hWnd),MN_CREATENEWDATABASE, 0L,0L)

	//---------------------------------------------------------------------------
	//! @def MainWindow_VerifyUserCredentials(hWnd)
	//!
	//! @brief Sends MN_VERIFYUSERCREDENTIALS signal to verify user credentials
	//!
	//! @param hWnd Handle of window
	//!
	//! @returns The return value is TRUE, if the user credentials is correct
	#define MainWindow_VerifyUserCredentials(hWnd) (BOOL)SNDMSG((hWnd),MN_VERIFYUSERCREDENTIALS, 0L,0L)

	//---------------------------------------------------------------------------
	//! @def MainWindow_FindDatabase(hWnd)
	//!
	//! @brief Sends MN_FINDDATABASE signal to find database
	//!
	//! @param hWnd Handle of window
	//!
	//! @returns returns TRUE, if database found. Otherwise FALSE
	#define MainWindow_FindDatabase(hWnd) (BOOL)SNDMSG((hWnd),MN_FINDDATABASE, 0L,0L)

	static INT_PTR CALLBACK PasswordProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK NewUserProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static void createNewFileName(char* pUserName, char* pUserPassword);
	static SecByteBlock HexDecodeString(const char *hex);

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	EncryptDBFile::EncryptDBFile(
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
	EncryptDBFile::~EncryptDBFile(
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
	EncryptDBFile::getUserCredentials(
		HINSTANCE	hParentInstance,
		HWND hParentWindow
	) {

		bool retVal = FALSE;
		gParentWindow = hParentWindow;
		retVal = DialogBox(hParentInstance,                   // application instance
			MAKEINTRESOURCE(IDD_USERCREDENTIALS), // dialog box resource
			hParentWindow,                          // owner window
			PasswordProc                    // dialog box window procedure
		);
		//g_errHandle.getErrorInfo((LPTSTR)L"dialog Registration Failed!");

		while ((newUserCreation == TRUE) && (retVal == FALSE)) {
			DialogBox(hParentInstance, MAKEINTRESOURCE(IDD_NEWUSERCREDENTIALS), hParentWindow, NewUserProc);
			retVal = DialogBox(hParentInstance, MAKEINTRESOURCE(IDD_USERCREDENTIALS), hParentWindow, PasswordProc);
		}
	}


	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	INT_PTR CALLBACK PasswordProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		TCHAR lpszPassword[16];
		TCHAR lpszUsername[16];
		WORD passWord;
		WORD userName;
		char  fileName[16];
		char  filePassword[16];
		bool retVal = FALSE;

		switch (message)
		{
			case WM_INITDIALOG:
			{
				// Set username size limit
				SendDlgItemMessage(hDlg, IDE_USERNAMEEDIT, EM_SETLIMITTEXT, (WPARAM)(sizeof(CHAR) * MAXUSERNAMECHAR), (LPARAM)0);
				// Set password character to a plus sign (*) 
				SendDlgItemMessage(hDlg, IDE_PASSWORDEDIT, EM_SETPASSWORDCHAR, (WPARAM) '*', (LPARAM)0);
				// Set password size limit
				SendDlgItemMessage(hDlg, IDE_PASSWORDEDIT, EM_SETLIMITTEXT, (WPARAM)(sizeof(CHAR) * MAXPASSWORDCHAR), (LPARAM)0);
				retVal = TRUE;
				break;
			}
			case WM_COMMAND:
			{
				switch (wParam)
				{
					case IDCANCEL:
					{
						EndDialog(hDlg, TRUE);
						retVal = TRUE;
						MainWindow_Close(gParentWindow);
						break;
					}
					case IDE_OK:
					{
						// Get number of characters. 
						userName = (WORD)SendDlgItemMessage(hDlg, IDE_USERNAMEEDIT, EM_LINELENGTH, (WPARAM)0, (LPARAM)0);
						passWord = (WORD)SendDlgItemMessage(hDlg, IDE_PASSWORDEDIT, EM_LINELENGTH, (WPARAM)0, (LPARAM)0);
						if (passWord >= 16) 
						{
							MessageBox(hDlg, L"Password can't be more than 16 characters", L"Error", MB_OK);
							retVal = FALSE;
						}
						else if (userName >= 16)
						{
							MessageBox(hDlg, L"Username can't be more than 16 characters", L"Error", MB_OK);
							retVal = FALSE;
						}
						else if (passWord == 0)
						{
							MessageBox(hDlg, L"Password can't be empty", L"Error", MB_OK);
							retVal = FALSE;
						}
						else if (userName == 0)
						{
							MessageBox(hDlg, L"Username can't be empyty", L"Error", MB_OK);
							retVal = FALSE;
						}
						else
						{

							// Put the number of characters into first word of buffer. 
							*((LPWORD)lpszPassword) = passWord;
							*((LPWORD)lpszUsername) = userName;

							// Get the characters. 
							SendDlgItemMessage(hDlg, IDE_USERNAMEEDIT, EM_GETLINE, (WPARAM)0, (LPARAM)lpszUsername);
							SendDlgItemMessage(hDlg, IDE_PASSWORDEDIT, EM_GETLINE, (WPARAM)0, (LPARAM)lpszPassword);
							// Null-terminate the string. 
							lpszUsername[userName] = 0;
							lpszPassword[passWord] = 0;
							copyTcharToChar(fileName, lpszUsername, 16); //copying to local char array
							copyTcharToChar(filePassword, lpszPassword, 16); //copying to local char array
							// Call a local password-parsing function. 
							createNewFileName(fileName, filePassword);

							bool isExistDatabase = MainWindow_FindDatabase(gParentWindow);
							if (isExistDatabase) {

								bool retVal = MainWindow_VerifyUserCredentials(gParentWindow);
								if (retVal == TRUE) {
									// message to mainwindow to open existing database
									MainWindow_OpenExistingDatabase(gParentWindow);

									newUserCreation = FALSE;
									EndDialog(hDlg, TRUE);
									retVal = TRUE;
								}
								else {
									MessageBox(hDlg, L"Password Mismatch\nPlease verify and re-enter password\n", L"Error", MB_OK);
									newUserCreation = FALSE;
									retVal = TRUE;
								}
							}
							else {
								MessageBox(hDlg, L"User name doesn't exist", L"Error", MB_OK);
								newUserCreation = FALSE;
								retVal = TRUE;
							}
						}
						break;
					}
					case IDE_NEWUSER:
					{
						newUserCreation = TRUE;
						EndDialog(hDlg, FALSE);
						retVal = TRUE;
						break;
					}
					default:
					{
						retVal = FALSE;
						break;
					}
				}
			}
		}
		return retVal;

		UNREFERENCED_PARAMETER(lParam);
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	
	//!
	//! \return		
	//!
	INT_PTR CALLBACK NewUserProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		TCHAR lpszUsername[16];
		TCHAR lpszPassword[16];
		TCHAR lpszRePassword[16];
		TCHAR lpsznameCredentials[MAX_PATH];
		WORD userNameSize;
		WORD passWordSize;
		WORD rePassWordSize;
		WORD nameCredentialsSize;
		char  fileName[16];
		char  filePassword[16];
		bool retVal = FALSE;

		switch (message)
		{
			case WM_INITDIALOG:
			{
				SendDlgItemMessage(hDlg, IDE_FIRSTNAMEEDIT, EM_SETLIMITTEXT, (WPARAM)(sizeof(CHAR) * MAXFIRSTNAMECHAR), (LPARAM)0);
				SendDlgItemMessage(hDlg, IDE_LASTNAMEEDIT, EM_SETLIMITTEXT, (WPARAM)(sizeof(CHAR) * MAXFIRSTNAMECHAR), (LPARAM)0);
				SendDlgItemMessage(hDlg, IDE_USERNAMEEDIT, EM_SETLIMITTEXT, (WPARAM)(sizeof(CHAR) * MAXUSERNAMECHAR), (LPARAM)0);

				// Set password character to a plus sign (*) 
				SendDlgItemMessage(hDlg, IDE_PASSWORDEDIT, EM_SETPASSWORDCHAR, (WPARAM) '*', (LPARAM)0);
				SendDlgItemMessage(hDlg, IDE_PASSWORDEDIT, EM_SETLIMITTEXT, (WPARAM)(sizeof(CHAR) * MAXPASSWORDCHAR), (LPARAM)0);

				SendDlgItemMessage(hDlg, IDE_RETYPEPASSWORDEDIT, EM_SETPASSWORDCHAR, (WPARAM) '*', (LPARAM)0);
				SendDlgItemMessage(hDlg, IDE_RETYPEPASSWORDEDIT, EM_SETLIMITTEXT, (WPARAM)(sizeof(CHAR) * MAXPASSWORDCHAR), (LPARAM)0);
				retVal = TRUE;
				break;
			}
			case WM_COMMAND:
			{
				switch (wParam)
				{
					case IDE_OK:
					{
						// Get number of characters. 
						userNameSize = (WORD)SendDlgItemMessage(hDlg, IDE_USERNAMEEDIT, EM_LINELENGTH, (WPARAM)0, (LPARAM)0);
						passWordSize = (WORD)SendDlgItemMessage(hDlg, IDE_PASSWORDEDIT, EM_LINELENGTH, (WPARAM)0, (LPARAM)0);
						rePassWordSize = (WORD)SendDlgItemMessage(hDlg, IDE_RETYPEPASSWORDEDIT, EM_LINELENGTH, (WPARAM)0, (LPARAM)0);

						// Put the number of characters into first word of buffer. 
						*((LPWORD)lpszUsername) = userNameSize;
						*((LPWORD)lpszPassword) = passWordSize;
						*((LPWORD)lpszRePassword) = rePassWordSize;

						// Get the characters. 
						SendDlgItemMessage(hDlg, IDE_USERNAMEEDIT, EM_GETLINE, (WPARAM)0, (LPARAM)lpszUsername);
						SendDlgItemMessage(hDlg, IDE_PASSWORDEDIT, EM_GETLINE, (WPARAM)0, (LPARAM)lpszPassword);
						SendDlgItemMessage(hDlg, IDE_RETYPEPASSWORDEDIT, EM_GETLINE, (WPARAM)0, (LPARAM)lpszRePassword);

						// Null-terminate the string. 
						lpszUsername[userNameSize] = 0;
						lpszPassword[passWordSize] = 0;
						lpszRePassword[rePassWordSize] = 0;

						if ((passWordSize >= 16) || (rePassWordSize >= 16))
						{
							MessageBox(hDlg, L"Password can't be more than 16 characters", L"Error", MB_OK);
							retVal = FALSE;
						}
						else if (userNameSize >= 16)
						{
							MessageBox(hDlg, L"Username can't be more than 16 characters", L"Error", MB_OK);
							retVal =  FALSE;
						}
						else if (passWordSize == 0)
						{
							MessageBox(hDlg, L"Password can't be empty", L"Error", MB_OK);
							retVal =  FALSE;
						}
						else if (userNameSize == 0)
						{
							MessageBox(hDlg, L"Username can't be empty", L"Error", MB_OK);
							retVal=  FALSE;
						}
						else if (wcscmp(lpszPassword, lpszRePassword))
						{
							MessageBox(hDlg, L"Password doesn't match", L"Error", MB_OK);
							retVal = FALSE;
						}
						else
						{
							copyTcharToChar(fileName, lpszUsername, 16); //copying to local char array
							copyTcharToChar(filePassword, lpszPassword, 16); //copying to local char array
							// Call a local password-parsing function. 
							createNewFileName(fileName, filePassword);

							bool isExistDatabase = MainWindow_FindDatabase(gParentWindow);
							if (isExistDatabase == FALSE) {
								// message to mainwindow to open existing database
								MainWindow_CreateNewDatabase(gParentWindow);

								newUserCreation = FALSE;
								EndDialog(hDlg, TRUE);
								retVal = TRUE;
							}
							else {
								MessageBox(hDlg, L"Username already Exists", L"Error", MB_OK);
								retVal = TRUE;
							}
						}
						break;
					}
					case IDE_CANCEL:
					{
						newUserCreation = FALSE;
						EndDialog(hDlg, TRUE);
						retVal = FALSE;
						break;
					}
					default:
					{
						retVal = FALSE;
						break;
					}
				}
			}
		}
		return retVal;

		UNREFERENCED_PARAMETER(lParam);
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	pFileName
	//!
	//! \return		
	//!
	SecByteBlock HexDecodeString(const char *hex)
	{
		StringSource ss(hex, true, new HexDecoder);
		SecByteBlock result((size_t)ss.MaxRetrievable());
		ss.Get(result, result.size());
		return result;
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	pFileName
	//!
	//! \return		
	//!
	void createNewFileName(char* pUserName, char* pUserPassword)
	{	
		SHA256 hash;
		string encFileName;
		string encFilePassword;
		StringSource s1(pUserName, true, new HashFilter(hash, new HexEncoder(new StringSink(encFileName))));
		strcpy(gFileName, encFileName.c_str());
		StringSource s2(pUserPassword, true, new HashFilter(hash, new HexEncoder(new StringSink(encFilePassword))));
		strcpy(gFilePassword, encFilePassword.c_str());
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	nothing
	//!
	//! \return		
	//!
	char* 
		EncryptDBFile::getDatabaseName(void) {
		return (gFileName);
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	nothing
	//!
	//! \return		
	//!
	char*
		EncryptDBFile::getDatabasePassword(void) {
		return (gFilePassword);
	}

	//---------------------------------------------------------------------------------------------------
	//! \brief		
	//!
	//! \param[in]	nothing
	//!
	//! \return		
	//!
	void
		EncryptDBFile::cryptDatabase(const char* pEncryptFilePath, TCHAR* pFilePath, const char *pPassPhraseOne, const char *pPassPhraseTwo) {
		
		char encryptFileName[MAX_PATH] = { 0 };
		TCHAR fileName[MAX_PATH] = { 0 };

		strcpy(encryptFileName, pEncryptFilePath);
		strcat(encryptFileName, ".crypt");
		mbstowcs(fileName, encryptFileName, MAX_PATH);

		SecByteBlock key = HexDecodeString(pPassPhraseOne);
		SecByteBlock iv = HexDecodeString(pPassPhraseTwo);
		CTR_Mode<AES>::Encryption aes(key, key.size(), iv);
		CryptoPP::FileSource(pEncryptFilePath, true, new CryptoPP::StreamTransformationFilter(aes, new CryptoPP::FileSink(encryptFileName)));

		int32_t retVal = DeleteFile(pFilePath);
		if (retVal == 0) {
			g_errHandle.getErrorInfo((LPTSTR)L"File Deletion Failed");
		}
		MoveFile(fileName, pFilePath);

	}

}