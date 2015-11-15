/**************************************************************/
/*                 qshKeyCommander1 - main.cpp                */
/*------------------------------------------------------------*/
/*         ������ ���������� �� ��� qshKeyCommander1          */
/*****2004***************************Copyright by <Quash!>*****/

// ���������: Winmm.lib - ������ � cdaudio


#include "windows.h"
#include "system_ids.h"
#include "shlobj.h" // SHAddToRecentDocs
#include "..\qkc_plugin.h"


// *���������� Runtime Library
#pragma comment(linker,"/MERGE:.rdata=.text")
#pragma comment(linker,"/FILEALIGN:512 /SECTION:.text,EWRX /IGNORE:4078")
#pragma comment(linker,"/ENTRY:DllMain")
#pragma comment(linker,"/NODEFAULTLIB")


#define CMDCOUNT 49
int  CmdCount = CMDCOUNT;    // ���������� ������, ������������ � �������
char CmdList[CMDCOUNT][200]; // ������ ������

// �����������/���������� ������
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
 switch (ul_reason_for_call)
  {
   case DLL_PROCESS_ATTACH:
    // ���������� ������
    lstrcpy(CmdList[RUN_SCREENSAVER]                  ,"������ ������������");
    lstrcpy(CmdList[SH_ICONS_DESKTOP]                 ,"������/�������� ������ �������� �����");
    lstrcpy(CmdList[SH_TASK_PANEL]                    ,"������/�������� ������ �����");
    lstrcpy(CmdList[CD_AUDIO_OPEN]                    ,"������� ����� ����� CD");
    lstrcpy(CmdList[CD_AUDIO_CLOSE]                   ,"������� ����� ����� CD");
    lstrcpy(CmdList[CD_AUDIO_OPENCLOSE]               ,"*�������/������� ����� CD");
    lstrcpy(CmdList[CLEAR_RECYCLEDBIN]                ,"�������� �������");
    lstrcpy(CmdList[CLEAR_DOCUMENTS]                  ,"�������� ���������");
    lstrcpy(CmdList[VOLUME_SYSTEM_UP]                 ,"+��������� ���������");
    lstrcpy(CmdList[VOLUME_SYSTEM_DOWN]               ,"-��������� ���������");
    lstrcpy(CmdList[PC_SHUTDOWN]                      ,"��������� ���������");
    lstrcpy(CmdList[PC_RESTART]                       ,"������������ ����������");
    lstrcpy(CmdList[PC_QUICKRESTART]                  ,"������� ������������ ����������");
    lstrcpy(CmdList[PC_RESTARTOS]                     ,"*������������ ������������ �������");
    lstrcpy(CmdList[PC_LOGOUT]                        ,"���������� ������ ������������");
    lstrcpy(CmdList[CHANGE_WALLPAPER]                 ,"*������� ����");
    lstrcpy(CmdList[MONITOR_OFF]                      ,"��������� �������");
    lstrcpy(CmdList[MONITOR_ON]                       ,"�������� �������");
    lstrcpy(CmdList[KEYBOARD_ON_OFF]                  ,"*���������/�������� ����������");
    lstrcpy(CmdList[MOUSE_ON_OFF]                     ,"*���������/�������� ����");
    lstrcpy(CmdList[CONTROL_PANEL]                    ,"������ ����������");
    lstrcpy(CmdList[DIALOG_EXECUTE]                   ,"*������ ���������");
    lstrcpy(CmdList[MINIMIZE_ALL_WINDOWS]             ,">�������� ��� ����");
    lstrcpy(CmdList[SLEEP]                            ,"*������ ����� (������)");
    lstrcpy(CmdList[SHOW_TASK_LIST]                   ,"��������� ����");

    lstrcpy(CmdList[CONTROL_INSTALLPROGRAMS]          ,"���������/�������� ��������");
    lstrcpy(CmdList[CONTROL_SCREENSETTINGS]           ,"�����");
    lstrcpy(CmdList[CONTROL_FINDFAST]                 ,"����� ������");
    lstrcpy(CmdList[CONTROL_IESETTINGS]               ,"�������� ������������");
    lstrcpy(CmdList[CONTROL_LANGUAGE]                 ,"���� � ���������");
    lstrcpy(CmdList[CONTROL_GAMEDEVICES]              ,"������� ����������");
    lstrcpy(CmdList[CONTROL_MOUSE]                    ,"����");
    lstrcpy(CmdList[CONTROL_MULTIMEDIA]               ,"�����������");
    lstrcpy(CmdList[CONTROL_MODEM]                    ,"�����");
    lstrcpy(CmdList[CONTROL_NETWORK]                  ,"����");
    lstrcpy(CmdList[CONTROL_PASSWORDS]                ,"������");
    lstrcpy(CmdList[CONTROL_POWERSETTINGS]            ,"���������� ���������������");
    lstrcpy(CmdList[CONTROL_SCANERS_AND_CAMERAS]      ,"������� � ������");
    lstrcpy(CmdList[CONTROL_SYSTEM]                   ,"�������");
    lstrcpy(CmdList[CONTROL_TELEPHONE]                ,"���������� ����������");
    lstrcpy(CmdList[CONTROL_THEMES]                   ,"���� �������� �����");
    lstrcpy(CmdList[CONTROL_DATATIME]                 ,"���� � �����");
    lstrcpy(CmdList[CONTROL_KEYBOARD]                 ,"����������");
    lstrcpy(CmdList[CONTROL_PRINTERS]                 ,"��������");
    lstrcpy(CmdList[CONTROL_FONTS]                    ,"������");
    lstrcpy(CmdList[CONTROL_SOUND]                    ,"*����");
    lstrcpy(CmdList[CONTROL_USERS]                    ,"*������������");
    lstrcpy(CmdList[CONTROL_MAIL]                     ,"*�����");
    lstrcpy(CmdList[CONTROL_INSTALLHARDWARE]          ,"*��������� ������������");
    break;

   case DLL_PROCESS_DETACH:
    break;
  }
 return true;
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
   case RUN_SCREENSAVER: // debug
    PostMessage(GetDesktopWindow(),WM_SYSCOMMAND,SC_SCREENSAVE,0);
    break;
   case SH_ICONS_DESKTOP:
    if (IsWindowVisible(FindWindow("Progman",NULL))) ShowWindow(FindWindow("Progman",NULL),SW_HIDE);
    else                                             ShowWindow(FindWindow("Progman",NULL),SW_SHOW);
    break;
   case SH_TASK_PANEL:
    if (IsWindowVisible(FindWindow("Shell_TrayWnd",NULL))) ShowWindow(FindWindow("Shell_TrayWnd",NULL),SW_HIDE);
    else                                                   ShowWindow(FindWindow("Shell_TrayWnd",NULL),SW_SHOW);
    break;
   case CLEAR_DOCUMENTS:
    SHAddToRecentDocs(SHARD_PATH,NULL);
    break;


/*
������: 

��� ��������� ������������� Windows? �����: ����������� ������� ExitWindows(). � �������� ������� ��������� �� ���������� ��� �� ���� ��������: 
   EW_RESTARTWINDOWS 
   EW_REBOOTSYSTEM 
   EW_EXITANDEXECAPP 
������ �������� ������������ ��� ������������ ���������� � ������ �������� MS DOS. 

������:
  ExitWindows(EW_RESTARTWINDOWS, 0 );  

    
����p����� ������: RUNDLL32.EXE shell32.dll,SHExitWindowsEx
����������:        RUNDLL32.EXE shell32.dll,SHExitWindowsEx 1
Reboot:            RUNDLL32.EXE shell32.dll,SHExitWindowsEx 2

��� ���p���
����p����� ������: RUNDLL32.EXE shell32.dll,SHExitWindowsEx 4
����������:        RUNDLL32.EXE shell32.dll,SHExitWindowsEx 5
Reboot:            RUNDLL32.EXE shell32.dll,SHExitWindowsEx 6
*/
   case PC_SHUTDOWN:
    ExitWindowsEx(EWX_SHUTDOWN,0);
    break;
   case PC_RESTART:
    ExitWindowsEx(EWX_REBOOT,0);
    break;
   case PC_QUICKRESTART:
    ExitWindowsEx(EWX_FORCE|EWX_REBOOT,0);
    break;
   case PC_RESTARTOS:
   // ExitWindowsEx(EW_RESTARTWINDOWS,0);
    break;
   case PC_LOGOUT:
    ExitWindowsEx(EWX_LOGOFF,0);
    break;
   case CD_AUDIO_OPEN:
    mciSendString("Set cdaudio door open wait",NULL,0,GetDesktopWindow());
    break;
   case CD_AUDIO_CLOSE:
    mciSendString("Set cdaudio door closed wait",NULL,0,GetDesktopWindow());
    break;
   case CLEAR_RECYCLEDBIN:
    SHEmptyRecycleBin(NULL,NULL,SHERB_NOPROGRESSUI);
    break;
   case MONITOR_OFF:
    PostMessage(GetDesktopWindow(),WM_SYSCOMMAND,SC_MONITORPOWER,1);
    break;
   case MONITOR_ON:
    PostMessage(GetDesktopWindow(),WM_SYSCOMMAND,SC_MONITORPOWER,-1); 
    break;
   case CONTROL_PANEL:
    WinExec("control.exe",SW_SHOWNORMAL);
    break;
   case MINIMIZE_ALL_WINDOWS:
    PostMessage(FindWindow("Shell_TrayWnd",NULL),WM_COMMAND,0x019F,0);
    break;
   case SHOW_TASK_LIST:
    PostMessage(GetDesktopWindow(),WM_SYSCOMMAND,SC_TASKLIST,0);
    break;

   case CONTROL_INSTALLPROGRAMS:
    WinExec("control.exe appwiz.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_SCREENSETTINGS:
    WinExec("control.exe desk.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_FINDFAST:
    WinExec("control.exe findfast.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_IESETTINGS:
    WinExec("control.exe inetcpl.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_LANGUAGE:
    WinExec("control.exe intl.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_GAMEDEVICES:
    WinExec("control.exe joy.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_MOUSE:
    WinExec("control.exe mouse",SW_SHOWNORMAL);
    break;
   case CONTROL_MULTIMEDIA:
    WinExec("control.exe mmsys.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_MODEM:
    WinExec("control.exe modem.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_NETWORK:
    WinExec("control.exe netcpl.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_PASSWORDS:
    WinExec("control.exe password.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_POWERSETTINGS:
    WinExec("control.exe powercfg.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_SCANERS_AND_CAMERAS:
    WinExec("control.exe sticpl.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_SYSTEM:
    WinExec("control.exe sysdm.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_TELEPHONE:
    WinExec("control.exe telephon.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_THEMES:
    WinExec("control.exe themes.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_DATATIME:
    WinExec("control.exe timedate.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_KEYBOARD:
    WinExec("control.exe keyboard",SW_SHOWNORMAL);
    break;
   case CONTROL_PRINTERS:
    WinExec("control.exe printers",SW_SHOWNORMAL);
    break;
   case CONTROL_FONTS:
    WinExec("control.exe fonts",SW_SHOWNORMAL);
    break;
   case CONTROL_SOUND:
    //WinExec("control.exe",SW_SHOWNORMAL);
    break;
   case CONTROL_USERS:
    //WinExec("control.exe",SW_SHOWNORMAL);
    break;
   case CONTROL_MAIL:
    //WinExec("control.exe",SW_SHOWNORMAL);
    break;
   case CONTROL_INSTALLHARDWARE:
    //WinExec("control.exe",SW_SHOWNORMAL);
    break;
    
    // ������� �� ����������� ��������� (�� ������ ������)
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
 PluginData->Caption = "System v0.24 (04.06.2004�.)\n\n� <Quash!>, �.������������)";

 // �������� ������� ������� qshKeyCommander'�
 PluginData->Init          = NULL;//Init;
 PluginData->Quit          = NULL;//Quit;
 PluginData->Config        = NULL;//Config;
 PluginData->Command       = Command;

 // �������� ������� ������ � ���������� ��� ��������� � ������
 PluginData->CommandsCount = &CmdCount;
 PluginData->CommandsList  = CmdList;
}

#ifdef __cplusplus
}
#endif