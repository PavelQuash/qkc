/**************************************************************/
/*              qshKeyCommander1 - Addiction.h                */
/*------------------------------------------------------------*/
/* �������� �������������� ��������, ����������� ��� ������   */
/*                           ���������                        */
/*****2004***************************Copyright by <Quash!>*****/

#include "windows.h"

// �������� ����� �� �������������
// true - ������ ����; false - ���� �� ������
bool FileExists(bool dir, LPTSTR pathname);

// �������������� ����� ����� �� ��������� � ������� (�������)
// true - �������; false - ���� �� ������
bool GetLongFileName(LPTSTR filepath, LPTSTR filename, LPTSTR longfilename);

// ���� ����������� � ��� ������
// L: ������������; H: ����������� ���
void KeyToStr(WORD key, LPTSTR strkey);

// �������������� ������ � ����� �����
int stoi(LPTSTR str);

// ��������� ������ � �������
// ��������� ������ ��� ���������� �������� size (���� ������� ��������, �� ������������ NULL)
void* memalloc(int size);
// ������������ ������ ���������� (���� �� �������, �������� ������� ���������)
int memfree(void* var);
