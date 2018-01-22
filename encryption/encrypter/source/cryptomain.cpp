//!
//! $URL: $
//!
//!
//! \file       
//! \author     Anand.Kathiresan
//! \date       
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

#include <windows.h>
#include <WinCred.h>
#include <string.h>
#include "resource.h"
#include "files.h" // to include crypto's file declaration
#include "rsa.h" // to include crypto's rsa declaration
#include "osrng.h" // to include crypto's AutoSeededRandomPool declaration
#include "hex.h" // to include crypto's HexEncoder declaration
#include "modes.h" // to include crypto's CTR_Mode declaration
#include "errhandle.h"
#include "cryptomain.h"

#pragma comment(lib,"Credui.lib")

namespace encryptmain
{
	using namespace CryptoPP;
	using namespace std;

	// RSA Testing
	static const size_t keyLength = 1024;
	RSA::PrivateKey _secretKey;
	RSA::PublicKey  _pubKey;
	static string cipherText1;

	AutoSeededRandomPool& rng_get() {
		static AutoSeededRandomPool defRng;
		return defRng;
	}

	string rsa_encode(const string& plainText, const RSA::PublicKey& pubKey) {
		RSAES_OAEP_SHA_Encryptor rsaEnc(pubKey);
		string cipherText;
		StringSource ss1(reinterpret_cast< const byte* >(plainText.c_str()), plainText.size(), true,
			new PK_EncryptorFilter(rng_get(), rsaEnc,
				new StringSink(cipherText)
			) // PK_EncryptorFilter
		); // StringSource
		return move(cipherText);
	}

	string rsa_decode(const string& cipherText, const RSA::PrivateKey& secretKey) {
		RSAES_OAEP_SHA_Decryptor rsaDec(secretKey);
		string plainText;
		StringSource ss2(reinterpret_cast< const byte* >(cipherText.c_str()), cipherText.size(), true,
			new PK_DecryptorFilter(rng_get(), rsaDec,
				new StringSink(plainText)
			) // PK_DecryptorFilter
		); // StringSource
		return move(plainText);
	}

	bool test(const string& initialText, bool first) {

		if (first) {
			cipherText1 = rsa_encode(initialText, _pubKey);
			auto plainText = rsa_decode(cipherText1, _secretKey);
			return plainText == initialText;
		}
		else {
			string cipherText2 = rsa_encode(initialText, _pubKey);
			if (cipherText1.compare(cipherText2) == 0) {
				return true;
			}
			auto plainText = rsa_decode(cipherText2, _secretKey);
			return plainText == initialText;
		}
	}

	SecByteBlock HexDecodeString(const char *hex)
	{
		StringSource ss(hex, true, new HexDecoder);
		SecByteBlock result((size_t)ss.MaxRetrievable());
		ss.Get(result, result.size());
		return result;
	}

	// RSA Testing

	//---------------------------------------------------------------------------
	/// @def SimpleGrid_SetProtectColor(hGrid, clrProtect)
	///
	/// @brief Sets the background color of protected cells.
	///
	/// @param hGrid The handle of the grid.
	/// @param clrProtect A COLORREF value. 
	///
	/// @returns The return value is not meaningful. 
	#define MainWindow_Close(hWnd) (BOOL)SNDMSG((hWnd),WM_CLOSE, 0L,0L)

	static errhandle::ErrHandle g_errHandle;
	static bool	newUserCreation = FALSE;
	static HWND gParentWindow;

	static INT_PTR CALLBACK PasswordProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK NewUserProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static void createDbName(char* pFileName);

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
			DialogBox(hParentInstance, MAKEINTRESOURCE(IDD_USERCREDENTIALS), hParentWindow, PasswordProc);
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
		bool retVal = FALSE;

		switch (message)
		{
			case WM_INITDIALOG:
			{
				// Set password character to a plus sign (*) 
				SendDlgItemMessage(hDlg, IDE_PASSWORDEDIT, EM_SETPASSWORDCHAR, (WPARAM) '*', (LPARAM)0);
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
						userName = (WORD)SendDlgItemMessage(hDlg, IDE_PASSWORDEDIT, EM_LINELENGTH, (WPARAM)0, (LPARAM)0);
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

							MessageBox(hDlg, lpszUsername, L"Did it work?", MB_OK);
							MessageBox(hDlg, lpszPassword, L"Did it work?", MB_OK);

							// Call a local password-parsing function. 
							//ParsePassword(lpszPassword);

							EndDialog(hDlg, TRUE);
							retVal = TRUE;
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
		TCHAR lpszPassword[16];
		TCHAR lpszUsername[16];
		char  fileName[16];
		TCHAR lpsznameCredentials[MAX_PATH];
		WORD passWordSize;
		WORD userNameSize;
		WORD nameCredentials;
		bool retVal = FALSE;

		switch (message)
		{
			case WM_INITDIALOG:
			{
				// Set password character to a plus sign (*) 
				SendDlgItemMessage(hDlg, IDE_PASSWORDEDIT, EM_SETPASSWORDCHAR, (WPARAM) '*', (LPARAM)0);
				SendDlgItemMessage(hDlg, IDE_RETYPEPASSWORDEDIT, EM_SETPASSWORDCHAR, (WPARAM) '*', (LPARAM)0);
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
						if (passWordSize >= 16)
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
							MessageBox(hDlg, L"Username can't be empyty", L"Error", MB_OK);
							retVal=  FALSE;
						}
						else
						{
							// Put the number of characters into first word of buffer. 
							*((LPWORD)lpszPassword) = passWordSize;
							*((LPWORD)lpszUsername) = userNameSize;

							// Get the characters. 
							SendDlgItemMessage(hDlg, IDE_USERNAMEEDIT, EM_GETLINE, (WPARAM)0, (LPARAM)lpszUsername);
							SendDlgItemMessage(hDlg, IDE_PASSWORDEDIT, EM_GETLINE, (WPARAM)0, (LPARAM)lpszPassword);
							// Null-terminate the string. 
							lpszUsername[userNameSize] = 0;
							lpszPassword[passWordSize] = 0;

							//MessageBox(hDlg, lpszUsername, L"Did it work?", MB_OK);
							//MessageBox(hDlg, lpszPassword, L"Did it work?", MB_OK);

							wcstombs(fileName, lpszUsername, 16); //copying to local char array
							// Call a local password-parsing function. 
							createDbName(fileName);

							EndDialog(hDlg, TRUE);
							retVal = TRUE;
						}
						newUserCreation = FALSE;
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
	void createDbName(char* pFileName)
	{
		// RSA testing
		// TEST - 1
		//_secretKey.GenerateRandomWithKeySize(rng_get(), keyLength);
		//new(&_pubKey) RSA::PublicKey(_secretKey);
		//string initialText86 = "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111";
		//auto testResult = test(initialText86, true);
		//testResult = test(initialText86, false);
		////assert(testResult);
		////return testResult ? 0 : -1;

		// TEST-2
		/*SecByteBlock key = HexDecodeString(pFileName);
		key = HexDecodeString(pFileName);
		key = HexDecodeString(pFileName);
		key = HexDecodeString(pFileName);
		key = HexDecodeString(pFileName);*/

		// TEST-3
		char infile[MAX_PATH] = "C:\\Users\\akathire\\Desktop\\test\\testout.txt";
		char outfile[MAX_PATH] = "C:\\Users\\akathire\\Desktop\\test\\testdec.txt";
		SHA256 hash;
		string message = "abcdABCDEF`123-=[]\;'/.,~!@#$%^&*()_+{}|:";
		string message2 = "efgh";
		string digest1, digest2, digest3;
		StringSource s1(message, true, new HashFilter(hash, new HexEncoder(new StringSink(digest1))));
		StringSource s2(message2, true, new HashFilter(hash, new HexEncoder(new StringSink(digest2))));
		
		SecByteBlock key = HexDecodeString(digest1.c_str());
		SecByteBlock iv = HexDecodeString(digest2.c_str());
		CTR_Mode<AES>::Encryption aes(key, key.size(), iv);
		FileSource(infile, true, new StreamTransformationFilter(aes, new FileSink(outfile)));

		// RSA testing
	}
}