/**************************************************************/
/*                qshKeyCommander1 - CTray.cpp                */
/*------------------------------------------------------------*/
/* ������ � ��������� �����, ����� ������, ���������� � ����� */
/*****2004***************************Copyright by <Quash!>*****/

#include "CTray.h"

// ���������� ������ � ����
void CTray::AddTrayIcon (HWND hwndWnd,UINT ID,UINT Flags,HICON hIcon,UINT Message,LPTSTR Caption)
{
 NOTIFYICONDATA nidTray;

 nidTray.cbSize           = sizeof(NOTIFYICONDATA);
 nidTray.hWnd             = hwndWnd;
 nidTray.uID              = ID;
 nidTray.uFlags           = Flags;
 nidTray.uCallbackMessage = Message;
 nidTray.hIcon            = hIcon;
 lstrcpy(nidTray.szTip,Caption);

 Shell_NotifyIcon(NIM_ADD,&nidTray);
}

// ��������� ������ � ����
void CTray::ChangeTrayIcon(HWND hwndWnd,UINT ID,UINT Flags,HICON hIcon,UINT Message,LPTSTR Caption)
{
 NOTIFYICONDATA nidTray;

 nidTray.cbSize           = sizeof(NOTIFYICONDATA);
 nidTray.hWnd             = hwndWnd;
 nidTray.uID              = ID;
 nidTray.uFlags           = Flags;
 nidTray.uCallbackMessage = Message;
 nidTray.hIcon            = hIcon;
 lstrcpy(nidTray.szTip,Caption);

 Shell_NotifyIcon(NIM_MODIFY,&nidTray);
}

// �������� ������ �� ����
void CTray::DeleteTrayIcon(HWND hwndWnd,UINT ID)
{
 NOTIFYICONDATA nidTray;

 nidTray.cbSize = sizeof(NOTIFYICONDATA);
 nidTray.hWnd   = hwndWnd;
 nidTray.uID    = ID;

 Shell_NotifyIcon(NIM_DELETE,&nidTray);
}