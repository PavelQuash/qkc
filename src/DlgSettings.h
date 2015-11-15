/**************************************************************/
/*           qshKeyCommander1 - DlgSettings.h                 */
/*------------------------------------------------------------*/
/*      ������������ ���� ��� ������� �������� ���������      */
/*****2004***************************Copyright by <Quash!>*****/

#include "windows.h"
#include "resource.h"
#include "CPlugins.h" // stPluginData
#include "CKbrdKeys.h"
#include "CTray.h"
#include "qkc_main.h"
#include "commctrl.h"
#include "CGetKbrdKey.h"

// ������� ��������� ���������, ����������� �������� ������� ��������
BOOL CALLBACK DlgSettingsProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

// ������� ��������� ���������, ����������� ������� 
BOOL CALLBACK DlgSettingsProc_Kbrd(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

// ������� ��������� ���������, ����������� ������� ��������� ������ WinLIRC
BOOL CALLBACK DlgSettingsProc_Wirc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

// ������� ��������� ���������, ����������� ������� ������������ ��������� ����
BOOL CALLBACK DlgSettingsProc_Mouse(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

// ������� ��������� ���������, ����������� ������� ������������ ������ ����
BOOL CALLBACK DlgSettingsProc_Gestures(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

// ������� ��������� ���������, ����������� ������� 
BOOL CALLBACK DlgSettingsProc_PlugList(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

// ������� ��������� ���������, ����������� ������� 
BOOL CALLBACK DlgSettingsProc_Configure(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
