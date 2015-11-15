/**************************************************************/
/*                qshKeyCommander1 - CPlugins.h               */
/*------------------------------------------------------------*/
/*         ������������ ���� ��� ������ �������� ������       */
/*             � ������������ �������� � ������ � ����        */
/*****2004***************************Copyright by <Quash!>*****/

#include "windows.h"
#include "CKbrdKeys.h"
#include "CWircKeys.h"
#include "Addiction.h"

#ifndef __CPLUGINS
#define __CPLUGINS

// ��������� ������ ������
struct stPluginData
{                             // �����������              (����������)
 char FileName[MAX_PATH];     // ��� ����� ������         (����������)
 HMODULE hDll;                // ����� ������             (����������)
 char *Caption;               // ��������� dll, ������    (�������)
 void (*Init)   (void);       // �������������            (�������)
 void (*Quit)   (void);       // ���������� ������        (�������)
 void (*Config) (void);       // ��������� ������         (�������)
 void (*Command)(int cmdid);  // ���������� �������       (�������)
 char (*CommandsList) [200];  // ������ ������ ������     (�������)
 int  *CommandsCount;         // ���������� ������        (�������)
};

// ��� ������� ������ - GetModuleData - ��������� ������
typedef void (*GMD_Type)(stPluginData*);

// ��������� ��� �������� ������, ������� ������ ����� ������������
struct stPluginKeys
{
 CKbrdKeys KbrdKeys;         // ������ ������ ����������
 CWircKeys WircKeys;         // ������ ������ �������
};

// ����� ������ � ��������
class CPlugins
{
 stPluginData *PluginsData;  // ���������� �� ������������ ������� � ������
 int PluginsCount;           // ���������� ��������� � ������

 char inifilename[MAX_PATH]; // Ini-����
 char pluginsdir[MAX_PATH];  // ������� ��������

 public:
  stPluginKeys *PluginsKeys; // �������������� ������� ��� ������� ��������������� ������

  CPlugins();  // �����������
  ~CPlugins(); // ����������

  bool SetPaths(LPTSTR inifilename, LPTSTR pluginsdir); // ��������� ini-����� � �������� ��������

  bool AddPlugin(LPTSTR filename);                          // ����������� ������
  bool DelPlugin(int num);                                  // ���������� ������
  bool GetPluginData(int num, stPluginData *getplugindata); // ������ num �������
  int  GetPluginsCount(void);                               // ����������� ���������� ������������ �������

  int  FindByFileName(LPTSTR filename); // ����� ������ �� ����� �����
  int  FindByHandle(HINSTANCE hdll);    // ����� ������ �� ������ ������

  bool LoadPlugList(LPTSTR section, LPTSTR keysize, LPTSTR keydata); // ��������� ������ ������������ �������
  bool SavePlugList(LPTSTR section, LPTSTR keysize, LPTSTR keydata); // ��������� ������ ������������ �������
};

#endif