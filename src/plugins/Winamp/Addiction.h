/**************************************************************/
/*            qshKeyCommander1 - Addiction.h                  */
/*------------------------------------------------------------*/
/*       �������� ������� ��� ������ ������� Winamp.qkc       */
/*****2004***************************Copyright by <Quash!>*****/

#include "windows.h"
#include "shlobj.h"
#include "memory.h"

#ifndef __ADDICTION
#define __ADDICTION

// ����������� � szDir ��� ���������� ��������
BOOL FLGetDirectory(HWND hwndParent,LPTSTR szDir,LPTSTR text);

// �������� ������� ������ �����
BOOL FLGetFile(HWND hwndParent,LPSTR szFileName,LPCSTR caption,LPCSTR filter,DWORD filterid);

// �������� ����� �� �������������
// true - ������ ����; false - ���� �� ������
BOOL FileExists(BOOL dir, LPTSTR pathname);

#endif