/**************************************************************/
/*           qshKeyCommander1 - DlgSettings.h                 */
/*------------------------------------------------------------*/
/*      Заголовочный файл для диалога настроек программы      */
/*****2004***************************Copyright by <Quash!>*****/

#include "windows.h"
#include "resource.h"
#include "CPlugins.h" // stPluginData
#include "CKbrdKeys.h"
#include "CTray.h"
#include "qkc_main.h"
#include "commctrl.h"
#include "CGetKbrdKey.h"

// Функция обработки сообщений, поступающих главному диалогу настроек
BOOL CALLBACK DlgSettingsProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

// Функция обработки сообщений, поступающих диалогу 
BOOL CALLBACK DlgSettingsProc_Kbrd(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

// Функция обработки сообщений, поступающих диалогу настройки кнопок WinLIRC
BOOL CALLBACK DlgSettingsProc_Wirc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

// Функция обработки сообщений, поступающих диалогу отслеживания сообщений мыши
BOOL CALLBACK DlgSettingsProc_Mouse(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

// Функция обработки сообщений, поступающих диалогу отслеживания жестов мыши
BOOL CALLBACK DlgSettingsProc_Gestures(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

// Функция обработки сообщений, поступающих диалогу 
BOOL CALLBACK DlgSettingsProc_PlugList(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

// Функция обработки сообщений, поступающих диалогу 
BOOL CALLBACK DlgSettingsProc_Configure(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
