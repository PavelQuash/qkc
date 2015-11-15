/**************************************************************/
/*            qshKeyCommander1 - DlgAbout.cpp                 */
/*------------------------------------------------------------*/
/*   Вывод диалога "О программе..." с координатами Хоумпаги   */
/*****2004***************************Copyright by <Quash!>*****/

#include "DlgAbout.h"

extern HWND hwndMainMenu;   // Описана в qkc_main.cpp
extern stSettings Settings; // Описана в qkc_main.cpp

HWND hwndDlgAbout = NULL; //Хранение hwnd диалога "О программе..."

// Функция обработки сообщений, поступающих диалогу 
BOOL CALLBACK DlgAboutProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
 switch (message)
  {
   case WM_INITDIALOG:
    // ***Перевод интерфейса***
    if (lstrcmp(Settings.Language,lng_default)!=0)
     {
      SendMessage(hwndDlg,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->ABOUT_Window_Caption);
      SendDlgItemMessage(hwndDlg,IDC_ABOUT_PROGRAM_CAPTION,  WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->ABOUT_Text_Program);
      SendDlgItemMessage(hwndDlg,IDC_ABOUT_VERSION_CAPTION,  WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->ABOUT_Text_Version);
      SendDlgItemMessage(hwndDlg,IDC_ABOUT_STATUS_CAPTION,   WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->ABOUT_Text_Status);
      SendDlgItemMessage(hwndDlg,IDC_ABOUT_STATUS,           WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->ABOUT_Status);
      SendDlgItemMessage(hwndDlg,IDC_ABOUT_WWW_CAPTION,      WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->ABOUT_Text_Www);
      SendDlgItemMessage(hwndDlg,IDC_ABOUT_EMAIL_CAPTION,    WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->ABOUT_Text_Email);
      SendDlgItemMessage(hwndDlg,IDC_ABOUT_LNGAUTHOR_CAPTION,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->ABOUT_Text_LngAuthor_Caption);
      SendDlgItemMessage(hwndDlg,IDC_ABOUT_LNGAUTHOR,        WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->DATA_Author);
      SendDlgItemMessage(hwndDlg,IDC_ABOUT_CLOSE,            WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->ABOUT_Button_Close);
     }
    // ***

    hwndDlgAbout = hwndDlg;
    SendDlgItemMessage(hwndDlg,IDC_ABOUT_VERSION,WM_SETTEXT,0,(LPARAM)(LPCTSTR)program_version);
    break;
   case WM_COMMAND: // Обработка нажатий кнопок в главном диалоге
    switch (LOWORD(wParam))
     {
      case IDC_ABOUT_CLOSE:
       EndDialog(hwndDlg,0);
       break;
      case IDC_ABOUT_WWW:
       ShellExecute(0,NULL,program_www,NULL,NULL,SW_MAXIMIZE);
       if (IsWindow(hwndMainMenu)) DestroyWindow(hwndMainMenu);
       else                        EndDialog(hwndDlg,0);
       break;
      case IDC_ABOUT_EMAIL:
       ShellExecute(0,"edit",program_email,NULL,NULL,SW_MAXIMIZE);
       if (IsWindow(hwndMainMenu)) DestroyWindow(hwndMainMenu);
       else                        EndDialog(hwndDlg,0);
       break;
     }
    return true;
   case WM_CLOSE:   
    EndDialog(hwndDlg,0);
	 		return true;
  }
 return false;
}
