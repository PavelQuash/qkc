/**************************************************************/
/*                qshKeyCommander1 - CTray.h                  */
/*------------------------------------------------------------*/
/*       ������������ ���� ��� ������ � ��������� �����       */
/*****2004***************************Copyright by <Quash!>*****/

#include "windows.h"

#ifndef __SYSTEMTRAY
#define __SYSTEMTRAY

class CTray
{
 public:
 void AddTrayIcon (HWND hwndWnd,UINT ID,UINT Flags,HICON hIcon,UINT Message,LPTSTR Caption);
 void ChangeTrayIcon(HWND hwndWnd,UINT ID,UINT Flags,HICON hIcon,UINT Message,LPTSTR Caption);
 void DeleteTrayIcon(HWND hwndWnd,UINT ID);
};

#endif