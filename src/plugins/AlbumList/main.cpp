/**************************************************************/
/*                 qshKeyCommander1 - main.cpp                */
/*------------------------------------------------------------*/
/*      ������ ���������� AlbumList (��� Winamp2)             */
/*****2004***************************Copyright by <Quash!>*****/

#include "../qkc_plugin.h" // ��������� ������ ������� ��� QshKeyCommander
#include "windows.h"       // �������� :)
#include "AlbumList_ids.h" // �������������� ������
#include "alfront.h"       // ������������ ���� AlbumList



// *���������� Runtime Library
#pragma comment(linker,"/MERGE:.rdata=.text")
#pragma comment(linker,"/FILEALIGN:512 /SECTION:.text,EWRX /IGNORE:4078")
#pragma comment(linker,"/ENTRY:DllMain")
#pragma comment(linker,"/NODEFAULTLIB")



#define CMDCOUNT 6
int  CmdCount = CMDCOUNT;    // ���������� ������, ������������ � �������
char CmdList[CMDCOUNT][200]; // ������ ������
char AlbumListClassName[] = "Winamp AL"; // ����� ���� AlbumList

// �����������/���������� ������
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
 switch (ul_reason_for_call)
  {
   case DLL_PROCESS_ATTACH:
    {
     // ���������� ������
     lstrcpy(CmdList[PLAYALBUM],       "��������������� �������");
     lstrcpy(CmdList[PLAYRANDOMALBUM], "������������� ��������� ������");
     lstrcpy(CmdList[PLAYALLALBUMS],   "������������� ��� �������");

     lstrcpy(CmdList[PREVALBUM],       "���������� ������");
     lstrcpy(CmdList[NEXTALBUM],       "��������� ������");

     lstrcpy(CmdList[HSALBUMLIST],     "��������/������ ���� Album List");
    }
    break;

   case DLL_PROCESS_DETACH:
    break;
  }
 return true;
}

// (���) �������� ��������� ���� AlbumList ��� ���������/��������� ���������� Winamp
int AlbumListUserSend(int id, int data)
{
 // ����� ���� Winamp
	HWND hwndAL = FindWindow(AlbumListClassName,NULL);
		// ���� ���� �������, �� ��������� ����������, ����� ������ �� ����������
	if (hwndAL) return (SendMessage(hwndAL,WM_AL_IPC,id,data));
	else return (-1);
}

// (�� ���) �������� ��������� ���� AlbumList'� ��� ���������/��������� ����������
int AlbumListUserPost(int id, int data)
{
 // ����� ���� Winamp
	HWND hwndAL = FindWindow(AlbumListClassName,NULL);
	// ���� ���� �������, �� ��������� ����������, ����� ������ �� ����������
	if (hwndAL) return (PostMessage(hwndAL,WM_AL_IPC,id,data));
	else return (-1);
}

// ������� �������������
//void Init(void)
//{ MessageBox(0,"MyPlugin Init","Info:",0); }

// ������� ���������������
//void Quit(void)
//{ MessageBox(0,"MyPlugin Quit","Info:",0); }

// ������������ �������
//void Config(void)
//{ MessageBox(0,"MyPlugin Config","Info:",0); }

// ������ �� ���������� ������� ��� ������� cmdid
void Command(int cmdid)
{
 switch (cmdid)
  {
   // ���������� ����������������
   case PLAYALBUM:
    {
     int i = AlbumListUserSend(IPC_GETALBUMINDEX,0);
     if (i<0) AlbumListUserPost(IPC_PLAYALBUM,0);
     else     AlbumListUserPost(IPC_PLAYALBUM,i);
    }
    break;
   case PLAYRANDOMALBUM:
    AlbumListUserPost(IPC_PLAYRANDOMALBUM,0);
    break;
   case PLAYALLALBUMS:
    AlbumListUserPost(IPC_PLAYALLALBUMS,0);
    break;

   // ����������� �� playlist'�
   case PREVALBUM:
    AlbumListUserPost(IPC_PLAYPREVALBUM,0);
    break;
   case NEXTALBUM:
    AlbumListUserPost(IPC_PLAYNEXTALBUM,0);
    break;

   case HSALBUMLIST:
    {
     HWND hwndAL = FindWindow(AlbumListClassName,NULL);
     if (IsWindowVisible(hwndAL)) 
      ShowWindow(hwndAL,SW_HIDE);
     else                         
     {
      ShowWindow(hwndAL,SW_SHOW);
      SetForegroundWindow(hwndAL);
     }
    }
    break;

   // ��������� �� ���������, ������ �� ����������
   default:
    break;
  }
}


#ifdef __cplusplus
extern "C"
{
#endif

// ������� �� �������, ���������� �������� �� ���������
__declspec (dllexport) void GetModuleData(struct stPluginData *PluginData)
{
 // ���������� ���������
 PluginData->Caption = "Album List v1.00 (21.05.2004�.)\nAlbumList  ������ ��� Winamp\n� <Quash!>, �.������������)";

 // �������� ������� ������� qshKeyCommander'�
 PluginData->Init    = NULL;//Init;
 PluginData->Quit    = NULL;//Quit;
 PluginData->Config  = NULL;//Config;
 PluginData->Command = Command;

 // �������� ������� ������ � ���������� ��� ��������� � ������
 PluginData->CommandsCount = &CmdCount;
 PluginData->CommandsList  = CmdList;
}

#ifdef __cplusplus
}
#endif