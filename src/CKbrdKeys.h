/**************************************************************/
/*              qshKeyCommander1 - CKbrdKeys.h                */
/*------------------------------------------------------------*/
/*    ������������ ���� ��� ������ �������� ������ ������     */
/*  ����������, �������� ��������� �������� ������ � ������   */
/*****03.2005************************Copyright by <Quash!>*****/

#include "windows.h"
#include "Addiction.h"

#ifndef __KBRDKEYS
#define __KBRDKEYS

#define KBRD_KEYSIZE "KbrdSize"
#define KBRD_KEYDATA "KbrdData"

#define CNS_CAPSLOCK   1
#define CNS_NUMLOCK    2
#define CNS_SCROLLLOCK 4

// ���������� � ������� ����������
struct stKbrdKey
{
 WORD KbrdKey; // L:����������� H:����������� ���
 WORD CNS;     // L:���./����. CNS  H:������ ������ CNS
 int  CmdID;   // ������������� ������� � ������
};

// ����� ������ ������ ����������
class CKbrdKeys
{
 stKbrdKey *KeysData; // ������(������) ������, ����������� � ������
 int KeysCount;       // ����� ����������� ������ � ������

 public:
  CKbrdKeys();  // �����������
  ~CKbrdKeys(); // ����������

  void AddKey(stKbrdKey *newkbrdkey);             // ����������� ����� �������
  bool DelKey(int num);                           // ��������� ������� � �������� num
  bool ChangeKey(int num, stKbrdKey *newkbrdkey); // ��������� ������ �������� ��� ������� num
  bool GetKey(int num, stKbrdKey *getkbrdkey);    // ���������� ������� c �������� num
  int  GetKeysCount(void);                        // ���������� ���������� ��������� � ������

  int  FindByKey(WORD kbrdkey); // ���������� ����� ������� �������� �� ���� �������
  int  FindByCmdID(int cmdid);  // ���������� ����� ������� �������� �� id �������

  void LoadKeys(LPTSTR inifilename, LPTSTR section, LPTSTR keysize, LPTSTR keydata); // �������� ������ ������ �� ini �����
  void SaveKeys(LPTSTR inifilename, LPTSTR section, LPTSTR keysize, LPTSTR keydata); // ���������� ������ ������ � ini ����
};

#endif