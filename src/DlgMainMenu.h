/**************************************************************/
/*             qshKeyCommander1 - DlgMainMenu.h               */
/*------------------------------------------------------------*/
/*        Заголовочный файл для диалога главного меню         */
/*****2004***************************Copyright by <Quash!>*****/

#include "windows.h"
#include "resource.h"
#include "qkc_main.h"
#include "DlgSettings.h"
#include "DlgAbout.h"
#include "DlgHelp.h"
#include "Settings.h"

// Функция обработки сообщений, поступающих диалогу главного меню
BOOL CALLBACK DlgMainMenuProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);