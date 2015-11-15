/**************************************************************/
/*            qshKeyCommander1 - DlgAbout.cpp                 */
/*------------------------------------------------------------*/
/*       ����� ������� ������, ����� � ������ ������          */
/*****2004***************************Copyright by <Quash!>*****/

#include "DlgHelp.h"

extern char hlpdir[MAX_PATH]; // ������� � qkc_main.cpp
extern HWND hwndMainMenu;     // ������� ��� ��
extern stSettings Settings;   // � qkc_main.cpp

void DlgHelp_AddFilesByExt(HWND hwndDlg, LPTSTR ext)
{
 // ��������� ���� ��������� ������
 WIN32_FIND_DATA finddata;
 HANDLE          hfile;
 char srchfilename[MAX_PATH];
 wsprintf(srchfilename,"%s\\*.%s",hlpdir,ext);
 hfile = FindFirstFile(srchfilename,&finddata);
 if (!(hfile==INVALID_HANDLE_VALUE)) // ���� ���-�� �������, �� �������, ��� �������
  if (!(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) // ���� ���� ������� �� ����������
   {
    SendDlgItemMessage(hwndDlg,IDC_HELP_LIST,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)finddata.cFileName);
    while (FindNextFile(hfile,&finddata))
     if (!(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) // ���� ���� ������� �� ����������   }
      SendDlgItemMessage(hwndDlg,IDC_HELP_LIST,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)finddata.cFileName);
   }
}


// ������� ��������� ���������, ����������� ������� 
BOOL CALLBACK DlgHelpProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
 switch (message)
  {
   case WM_INITDIALOG:
    // ***������� ����������***
    if (lstrcmp(Settings.Language,lng_default)!=0)
     {
      SendDlgItemMessage(hwndDlg,IDC_HELP_CAPTION,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->HELP_Window_Caption);
      SendDlgItemMessage(hwndDlg,IDC_HELP_CLOSE,  WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->HELP_Button_Close);
      SendDlgItemMessage(hwndDlg,IDC_HELP_OPEN,   WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->HELP_Button_Open);
     }
    // ***
    // ���������� ������
    SendDlgItemMessage(hwndDlg,IDC_HELP_LIST,LB_RESETCONTENT,0,0); // ������ �����
    DlgHelp_AddFilesByExt(hwndDlg,"doc");
    DlgHelp_AddFilesByExt(hwndDlg,"hlp");
    DlgHelp_AddFilesByExt(hwndDlg,"htm");
    DlgHelp_AddFilesByExt(hwndDlg,"nfo");
    DlgHelp_AddFilesByExt(hwndDlg,"txt");
    DlgHelp_AddFilesByExt(hwndDlg,"url");
    break;
   case WM_COMMAND: // ��������� ������� ������ � ������� �������
    switch (LOWORD(wParam))
     {
      case IDC_HELP_OPEN:
       {
        int i = SendDlgItemMessage(hwndDlg,IDC_HELP_LIST,LB_GETCURSEL,0,0);
        if (i>=0)
         {
          // ����������� ���������� ����� �����
          char buffer[MAX_PATH],filename[MAX_PATH];
          SendDlgItemMessage(hwndDlg,IDC_HELP_LIST,LB_GETTEXT,(WPARAM)i,(LPARAM)(LPCTSTR)buffer);
          wsprintf(filename,"%s\\%s",hlpdir,buffer);
          ShellExecute(0,NULL,filename,NULL,NULL,SW_MAXIMIZE);
          DestroyWindow(hwndMainMenu);
         }
       }
       break;
      case IDC_HELP_CLOSE:
       EndDialog(hwndDlg,0);
       break;
     }
    return true;
   case WM_CLOSE:   
    EndDialog(hwndDlg,0);
	 		return true;
  }
 return false;
}
