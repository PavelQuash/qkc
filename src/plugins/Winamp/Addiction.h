/**************************************************************/
/*            qshKeyCommander1 - Addiction.h                  */
/*------------------------------------------------------------*/
/*       Описание функций для работы плагина Winamp.qkc       */
/*****2004***************************Copyright by <Quash!>*****/

#include "windows.h"
#include "shlobj.h"
#include "memory.h"

#ifndef __ADDICTION
#define __ADDICTION

// Возвращение в szDir имя выбранного каталога
BOOL FLGetDirectory(HWND hwndParent,LPTSTR szDir,LPTSTR text);

// Открытие диалога выбора файла
BOOL FLGetFile(HWND hwndParent,LPSTR szFileName,LPCSTR caption,LPCSTR filter,DWORD filterid);

// Проверка файла на существование
// true - найден файл; false - файл не найден
BOOL FileExists(BOOL dir, LPTSTR pathname);

#endif