/**************************************************************/
/*            qshKeyCommander1 - DlgMainMenu.cpp              */
/*------------------------------------------------------------*/
/*              Работа с диалогом главного меню               */
/*****2004***************************Copyright by <Quash!>*****/

#include "DlgMainMenu.h"

extern HINSTANCE hInst;        // Описано в qkc_main.cpp
extern HWND hwndServerWindow;  // Описано там же
extern HWND hwndMainMenu;      // Описано там же
extern stSettings Settings;    // Описано там же

// Функция обработки сообщений, поступающих диалогу главного меню
BOOL CALLBACK DlgMainMenuProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
 RECT rct;
 switch (message)
  {
   case WM_INITDIALOG:
    // ***Перевод интерфейса***
    if (lstrcmp(Settings.Language,lng_default)!=0)
     {
      SendMessage(hwndDlg,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->MAINMENU_Window_Caption);
      SendDlgItemMessage(hwndDlg,IDC_MAINMENU_SETTINGS,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->MAINMENU_Button_Settings);
      SendDlgItemMessage(hwndDlg,IDC_MAINMENU_HELP,    WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->MAINMENU_Button_Help);
      SendDlgItemMessage(hwndDlg,IDC_MAINMENU_ABOUT,   WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->MAINMENU_Button_About);
      SendDlgItemMessage(hwndDlg,IDC_MAINMENU_HIDE,    WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->MAINMENU_Button_Hide);
      SendDlgItemMessage(hwndDlg,IDC_MAINMENU_QUIT,    WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->MAINMENU_Button_Quit);
     }
    // ***
    // Ставим главное окно поверх всех окон
    hwndMainMenu = hwndDlg;
    SetWindowPos(hwndDlg,HWND_TOPMOST,Settings.xMainMenuWindow,Settings.yMainMenuWindow,0,0,SWP_NOSIZE);
    SetForegroundWindow(hwndDlg);
    ShowWindow(hwndDlg,SW_SHOW);
    break;
   case WM_COMMAND: // Обработка нажатий кнопок в диалоге
    switch (LOWORD(wParam))
     {
      case IDC_MAINMENU_SETTINGS:
       ShowWindow(hwndDlg,SW_HIDE);
       MonitoringEnd();   // Выключение отслеживания
       DialogBox(hInst,MAKEINTRESOURCE(IDD_SETTINGS),hwndDlg,(DLGPROC)DlgSettingsProc);
       SettingsUpdate(); // Обновление после изменения настроек
       MonitoringBegin(); // Включение отслеживания
       ShowWindow(hwndDlg,SW_SHOW);
       break;
      case IDC_MAINMENU_HELP:
       ShowWindow(hwndDlg,SW_HIDE);
       DialogBox(hInst,MAKEINTRESOURCE(IDD_HELP),hwndDlg,(DLGPROC)DlgHelpProc);
       ShowWindow(hwndDlg,SW_SHOW);
       break;
      case IDC_MAINMENU_ABOUT:
       ShowWindow(hwndDlg,SW_HIDE);
       DialogBox(hInst,MAKEINTRESOURCE(IDD_ABOUT),hwndDlg,(DLGPROC)DlgAboutProc);
       ShowWindow(hwndDlg,SW_SHOW);
       break;
      case IDC_MAINMENU_HIDE:
       EndDialog(hwndDlg,0);
       break;
      case IDC_MAINMENU_QUIT:
       // Приходится повторяться, т.к. уничтожение идёт сразу
       GetWindowRect(hwndDlg,&rct);
       Settings.xMainMenuWindow = rct.left;
       Settings.yMainMenuWindow = rct.top;
       SendMessage(hwndServerWindow,WM_CLOSE,0,0);
       break;
     }
    return true;
   case WM_CLOSE:   
    EndDialog(hwndDlg,0);
	 		return true;
   case WM_DESTROY:
    GetWindowRect(hwndDlg,&rct);
    Settings.xMainMenuWindow = rct.left;
    Settings.yMainMenuWindow = rct.top;
    break;
  }
 return false;
}
