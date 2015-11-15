/**************************************************************/
/*              qshKeyCommander1 - CWircKeys.h                */
/*------------------------------------------------------------*/
/*    ������������ ���� ��� ������ �������� ������ ������     */
/*    WinLIRC, �������� ��������� �������� ������ � ������    */
/*     ������ ����� ��������� ��������� CKbrdKeys, ��������   */
/*    ������� ������ � ���, ��� ������������ ������ ��������� */
/*****03.2005************************Copyright by <Quash!>*****/

#include "windows.h"
#include "Addiction.h"

#ifndef __WIRCKEYS
#define __WIRCKEYS

#define WIRC_KEYSIZE "WircSize"
#define WIRC_KEYDATA "WircData"

// ���������� � ������ WinLIRC
struct stWircKey
{
 char WircKey[80]; // ��� ������
 int  Repeats;     // ���������� ����������
 int  CmdID;       // ������������� ������� � ������
};
    
// ����� ������ ������ WinLIRC
class CWircKeys
{
 stWircKey *KeysData; // ������(������) ������, ����������� � ������
 int KeysCount;       // ����� ����������� ������ � ������

 public:
  CWircKeys();  // �����������
  ~CWircKeys(); // ����������

  void AddKey(stWircKey *newwirckey);             // ����������� ����� �������
  bool DelKey(int num);                           // ��������� ������� � �������� num
  bool ChangeKey(int num, stWircKey *newwirckey); // ��������� ������ �������� ��� ������� num
  bool GetKey(int num, stWircKey *getwirckey);    // ���������� ������� c �������� num
  int  GetKeysCount(void);                        // ���������� ���������� ��������� � ������

  int  FindByKey(char wirckey[80]); // ���������� ����� ������� �������� �� ���� �������
  int  FindByCmdID(int cmdid);      // ���������� ����� ������� �������� �� id �������

  void LoadKeys(LPTSTR inifilename, LPTSTR section, LPTSTR keysize, LPTSTR keydata); // �������� ������ ������ �� ini �����
  void SaveKeys(LPTSTR inifilename, LPTSTR section, LPTSTR keysize, LPTSTR keydata); // ���������� ������ ������ � ini ����
};

#endif