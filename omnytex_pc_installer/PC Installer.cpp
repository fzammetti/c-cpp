
// Includes
#include "stdafx.h"
#include <windows.h>
#include <direct.h>
#include "resource.h"
#include "shlobj.h"
#include <string>
#include <iostream>
#include <fstream>


// Global variables
HINSTANCE hInst						   = NULL;
TCHAR			szAppName[256]	   = TEXT("");
TCHAR			szAppVersion[256]	 = TEXT("");
TCHAR			szTitle[256]			 = TEXT("");
TCHAR			szFullTitle[256]   = TEXT("");
TCHAR			szWindowClass[256] = TEXT("");
TCHAR			destPath[256]			 = TEXT("C:\\Program Files\\IO Lander");


// Displays a dialog allowing the user to choose a directory and returns the selected path.
TCHAR ChooseDirectory(TCHAR* inPathDest) {
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = TEXT("Pick a Directory");
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl != 0) {
		if (!SHGetPathFromIDList(pidl, inPathDest)) {
			MessageBox(NULL, TEXT("SHGetPathFromIDList - Failure"), TEXT("ChooseDirectory Error"), MB_OK);
		}
		IMalloc* imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc))) {
			imalloc->Free(pidl);
			imalloc->Release();
		} else {
			MessageBox(NULL, TEXT("SHGetMalloc - Failure"), TEXT("ChooseDirectory Error"), MB_OK);
		}
	} else { // Problem opening browse dialog
		MessageBox(NULL, TEXT("SHBrowseForFolder() - Failure"), TEXT("ChooseDirectory Error"), MB_OK);
	}
	return inPathDest[0];
}


// Creates a shortcut link to a given file.
HRESULT CreateLink(const TCHAR* strPathObj, const TCHAR* strPathLink, TCHAR* strDesc) {
	HRESULT hres;
	IShellLink* psl;
	TCHAR strMyPath[255] = TEXT("");
	strcpy(strMyPath, strPathLink);
	CoInitialize(NULL);
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*) &psl);
	if (SUCCEEDED(hres)) {
		IPersistFile* ppf;
		psl->SetPath(strPathObj);
		psl->SetDescription(strDesc);
		hres = psl->QueryInterface(IID_IPersistFile, (LPVOID *) &ppf);
		if (SUCCEEDED(hres)) {
			WORD wsz[MAX_PATH];
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, strMyPath, -1, wsz, MAX_PATH);
			hres = ppf->Save(wsz, TRUE);
			if (hres != S_OK ) {
				if (hres == E_FAIL) {
					MessageBox(NULL, TEXT("IPersistFile->Save() - E_FAIL"), TEXT("CreateLink Error"), MB_OK);
				} else {
					MessageBox(NULL, TEXT("IPersistFile->Save() - Unknown Failure"), TEXT("CreateLink Error"), MB_OK);
				}
			}
			ppf->Release();
		} else { // QueryInterface Failed
			if (hres == E_NOINTERFACE) {
				MessageBox(NULL, TEXT("QueryInterface() - E_NOINTERFACE"), TEXT("CreateLink Error"), MB_OK);
			} else {
				MessageBox(NULL, TEXT("QueryInterface() - Unknown Failure"), TEXT("CreateLink Error"), MB_OK);
			}
		}
		psl->Release();
	} else { // CoCreateInstance Failed
		switch (hres) {
			case REGDB_E_CLASSNOTREG:
				MessageBox(NULL, TEXT("CoCreateInstance() - REGDB_E_CLASSNOTREG"), TEXT("CreateLink Error"), MB_OK);
			break;
			case CLASS_E_NOAGGREGATION:
				MessageBox(NULL, TEXT("CoCreateInstance() - CLASS_E_NOAGGREGATION"), TEXT("CreateLink Error"), MB_OK);
			break;
			default:
				MessageBox(NULL, TEXT("CoCreateInstance() - Unknown Failure"), TEXT("CreateLink Error"), MB_OK);
			break;
		}
	}
	return hres;
}


// This function is called when the user clicks Cancel to verify they want to really quit the setup
BOOL verifyCancel(HWND hDlg) {
	// Create the text string for the message of this popup
	TCHAR szMessage[256] = TEXT("");
	strcat(szMessage, TEXT("Setup is not complete.  If you quit now,\n"));
	strcat(szMessage, szAppName);
	strcat(szMessage, TEXT(" v"));
	strcat(szMessage, szAppVersion);
	strcat(szMessage, TEXT(" will not be installed.\n\nYou may run Setup at a later time to complete the\ninstallation.\n\nDo you wish to quit installing "));
	strcat(szMessage, szAppName);
	strcat(szMessage, TEXT(" v"));
	strcat(szMessage, szAppVersion);
	strcat(szMessage, TEXT("?"));
	if (MessageBox(hDlg, szMessage, TEXT("Exit Setup"), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 | MB_APPLMODAL) == IDYES) { 
		return true;
	} else {
		return false;
	}
}


// Message handler function for the Welcome dialog
int CALLBACK scrWelcomeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_INITDIALOG: {
			// Create the text string for the text next to the install icon and set it
			TCHAR szWelcome1[256] = TEXT("");
			strcat(szWelcome1, TEXT("Welcome to the "));
			strcat(szWelcome1, szFullTitle);
			strcat(szWelcome1, TEXT(" setup program.  This program will install "));
			strcat(szWelcome1, szFullTitle);
			strcat(szWelcome1, TEXT(" on your mobile device."));
			SetDlgItemText(hDlg, IDC_WELCOME1, szWelcome1);
			// Create the text string for the titlebar text and set it
			TCHAR szTitlebarText[256] = TEXT("");
			strcat(szTitlebarText, szFullTitle);
			strcat(szTitlebarText, TEXT(" Setup"));
			SetWindowText(hDlg, szTitlebarText);
		break; }
		case WM_COMMAND: {
			int wmId    = LOWORD(wParam); 
			int wmEvent = HIWORD(wParam); 
			switch (wmId) {
				case IDC_CANCEL: {
					// CANCEL was clicked, make sure they want to exit
					if (verifyCancel(hDlg)) {
						EndDialog(hDlg, 1);
					}
				break; }
				case IDC_NEXT: {
					EndDialog(hDlg, 0);
				break; }
			}
		break; }
	}
	return 0;
}

// Message handler function for the Info dialog
int CALLBACK scrInfoProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_INITDIALOG: {
			// Create the text string for the titlebar text and set it
			TCHAR szTitlebarText[256] = TEXT("");
			strcat(szTitlebarText, szFullTitle);
			strcat(szTitlebarText, TEXT(" Setup"));
			SetWindowText(hDlg, szTitlebarText);
			SetDlgItemText(hDlg, IDC_EDIT_DIRECTORY, destPath);
		break; }
		case WM_COMMAND: {
			int wmId    = LOWORD(wParam); 
			int wmEvent = HIWORD(wParam); 
			switch (wmId) {
				case IDC_DIRECTORY: {
					ChooseDirectory(destPath);
					SetDlgItemText(hDlg, IDC_EDIT_DIRECTORY, destPath);
				break; }
				case IDC_CANCEL: {
					// CANCEL was clicked, make sure they want to exit
					if (verifyCancel(hDlg)) {
						EndDialog(hDlg, 1);
					}
				break; }
				case IDC_BACK: {
					EndDialog(hDlg, 2);
				break; }
				case IDC_NEXT: {
					EndDialog(hDlg, 0);
				break; }
			}
		break; }
	}
	return 0;
}

// Message handler function for the EULA dialog
int CALLBACK scrEULAProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_INITDIALOG: {
			// Create the text string for the text below the EULA text and set it
			TCHAR szEULA1[256] = TEXT("");
			strcat(szEULA1, TEXT("Do you accept all the terms of the preceeding License Agreement?  If you choose Decline, Setup will close.  To install "));
			strcat(szEULA1, szFullTitle);
			strcat(szEULA1, TEXT(", you must accept this agreement."));
			SetDlgItemText(hDlg, IDC_EULA1, szEULA1);
			// Set the actual EULA text
			TCHAR szEULA[14500]   = TEXT("");
			TCHAR szEULAP1[2050] = TEXT("");
			TCHAR szEULAP2[2050] = TEXT("");
			TCHAR szEULAP3[2050] = TEXT("");
			TCHAR szEULAP4[2050] = TEXT("");
			TCHAR szEULAP5[2050] = TEXT("");
			TCHAR szEULAP6[2050] = TEXT("");
			TCHAR szEULAP7[2050] = TEXT("");
			LoadString(hInst, IDS_EULA1, szEULAP1, 2048);
			LoadString(hInst, IDS_EULA2, szEULAP2, 2048);
			LoadString(hInst, IDS_EULA3, szEULAP3, 2048);
			LoadString(hInst, IDS_EULA4, szEULAP4, 2048);
			LoadString(hInst, IDS_EULA5, szEULAP5, 2048);
			LoadString(hInst, IDS_EULA6, szEULAP6, 2048);
			LoadString(hInst, IDS_EULA7, szEULAP7, 2048);
			strcat(szEULA, szEULAP1);
			strcat(szEULA, szEULAP2);
			strcat(szEULA, szEULAP3);
			strcat(szEULA, szEULAP4);
			strcat(szEULA, szEULAP5);
			strcat(szEULA, szEULAP6);
			strcat(szEULA, szEULAP7);
			SetDlgItemText(hDlg, IDC_EULA_TEXT, szEULA);
		break; }

		case WM_COMMAND: {
			int wmId    = LOWORD(wParam); 
			int wmEvent = HIWORD(wParam);
			switch (wmId) {
				case IDC_BACK: {
					EndDialog(hDlg, 2);
				break; }
				case IDC_DECLINE: {
					EndDialog(hDlg, 1);
				break; }
				case IDC_ACCEPT: {
					EndDialog(hDlg, 0);
				break; }
			}
		break; }
	}
	return 0;
}


// Message handler function for the main background window
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_PAINT: {
			TCHAR szText[256] = TEXT("");
			strcpy(szText, szFullTitle);
			strcat(szText, TEXT(" Setup"));
			PAINTSTRUCT ps;
			RECT rt;
			HDC hDC	= NULL;
			hDC = BeginPaint(hWnd, &ps);
				rt.top    = 20;
				rt.left   = 20;
				rt.bottom = 1000;
				rt.right  = 1000;
				// Set the font
				LOGFONT lf;
				HGDIOBJ oldObject;
				lf.lfWeight					= FW_BOLD;
				lf.lfEscapement			= 0;
				lf.lfOrientation		= 0;
				lf.lfHeight					= 48;
				lf.lfWidth					= 0;
				lf.lfCharSet				= OEM_CHARSET;
				lf.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
				lf.lfItalic					= TRUE;
				lf.lfUnderline			= FALSE;
				lf.lfStrikeOut			= FALSE;
				lf.lfQuality				= DEFAULT_QUALITY;
				lf.lfOutPrecision		= OUT_DEFAULT_PRECIS;
				lf.lfPitchAndFamily = FF_ROMAN | DEFAULT_PITCH;
				strcpy(lf.lfFaceName, TEXT("MS Serif"));
				oldObject = SelectObject(hDC, CreateFontIndirect(&lf));
				// Set the current text color and background
				SetTextColor(hDC, 0x00ffffff);
				SetBkColor  (hDC, 0x00000000);
				DrawText(hDC, szText, strlen(szText), &rt, DT_LEFT);
			EndPaint(hWnd, &ps);
		break; }
		default: {
			return DefWindowProc(hWnd, message, wParam, lParam);
    break; }
  }
  return 0;
}


// EXECUTION BEGINS HERE
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	int retVal;
	// Get our instance for later
	hInst = hInstance;
	// Load strings from string table for later
	LoadString(hInst, IDS_APP_NAME,    szAppName,    256);
	LoadString(hInst, IDS_APP_VERSION, szAppVersion, 256);
	// Create the main background window
	LoadString(hInstance, IDS_APP_CLASS, szWindowClass, 256);
	strcat(szTitle, szAppName);
	strcat(szTitle, TEXT(" v"));
	strcat(szTitle, szAppVersion);
	strcpy(szFullTitle, szTitle);
	HWND hWnd = FindWindow(szWindowClass, szTitle);	
	if (hWnd) {
		SetForegroundWindow((HWND)(((DWORD)hWnd) | 0x01));    
	} else {
		WNDCLASS wc;
			wc.style			    = CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE;
			wc.lpfnWndProc		= (WNDPROC)WndProc;
			wc.cbClsExtra		  = 0;
			wc.cbWndExtra		  = 0;
			wc.hInstance		  = hInstance;
			wc.hIcon			    = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
			wc.hCursor			  = 0;
			wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
			wc.lpszMenuName		= 0;
			wc.lpszClassName	= szWindowClass;
		RegisterClass(&wc);
		hWnd = CreateWindow(szWindowClass, TEXT("Omnytex Technologies"), NULL, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
		if (!hWnd) { 
			return FALSE; 
		}
		ShowWindow(hWnd, SW_MAXIMIZE);
}
	// Show the welcome screen, exit program if 1 returned (CANCEL clicked)
doWelcome:
	retVal = DialogBox(hInst, MAKEINTRESOURCE(IDD_WELCOME), hWnd, (DLGPROC)scrWelcomeProc);
	if (retVal == 1) {
		return -1;
	}
	// Show the EULA, exit program if FALSE returned (DECLINE clicked)
doEULA:
	retVal = DialogBox(hInst, MAKEINTRESOURCE(IDD_EULA),    hWnd, (DLGPROC)scrEULAProc);
	if (retVal == 1) {
		return -1;
	} else if (retVal == 2) {
		goto doWelcome;
	}
	// Show the info screen
	retVal = DialogBox(hInst, MAKEINTRESOURCE(IDD_INFO),    hWnd, (DLGPROC)scrInfoProc);
	if (retVal == 1) {
		return -1;
	} else if (retVal == 2) {
		goto doEULA;
	}
	// Install the application
	// -----------------------
  // First, create the destination directory, no problem if it exists already
	CreateDirectory(destPath, NULL);
	// Now copy the source file into that directory
	strcat(destPath, TEXT("\\IO Lander.swf"));
  FILE *fpnew = fopen(destPath, "wb");
  FILE *fpold = fopen(TEXT("iolander.swf"), "rb");
  if (fpnew && fpold) {
		int ch;
    while (ch = fgetc(fpold), !feof(fpold)) {
			fputc(ch, fpnew);
		}
    if (ferror(fpnew) || ferror(fpold)) {
			MessageBox(NULL, TEXT("Error copying file"), TEXT("Error"), MB_OK);
    }
	} else { // Error opening one or both files
		MessageBox(NULL, TEXT("Error opening files"), TEXT("Error"), MB_OK);
	}
  fclose(fpold);
  fclose(fpnew);
	// Create the shortcut on the desktop to the file
  TCHAR crEnVar[255] = TEXT("");
  GetEnvironmentVariable("USERPROFILE", crEnVar, 255);
  TCHAR sc_destPath[255] = TEXT("");
  strcpy(sc_destPath, crEnVar);
  strcat(sc_destPath, TEXT("\\Desktop\\"));
  strcat(sc_destPath, TEXT("IO Lander"));
  strcat(sc_destPath, TEXT(".lnk"));
  CreateLink(destPath, sc_destPath, TEXT("IO Lander"));
	// Tell the user we're done
	MessageBox(NULL, TEXT("IO Lander has been successfully installed.  Enjoy!"), TEXT("Install Complete"), MB_OK | MB_ICONINFORMATION);
	// Exit successfully
	return 0; 
}