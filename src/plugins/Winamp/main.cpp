/**************************************************************/
/*                 qshKeyCommander1 - main.cpp                */
/*------------------------------------------------------------*/
/*      ������ ���������� Winamp2 ��� qshKeyCommander1        */
/*****2004***************************Copyright by <Quash!>*****/

#include "../qkc_plugin.h" // ��������� ������ ������� ��� QshKeyCommander
#include "windows.h"       // �������� :)
#include "Winamp_ids.h"    // �������������� ������
#include "Frontend.h"      // ������������ ���� Winamp
#include "resource.h"      // ���� ��������
#include "commctrl.h"      // ��� �������� ����
#include "Addiction.h"     // �������������� �������



// *���������� Runtime Library
#pragma comment(linker,"/MERGE:.rdata=.text")
#pragma comment(linker,"/FILEALIGN:512 /SECTION:.text,EWRX /IGNORE:4078")
#pragma comment(linker,"/ENTRY:DllMain")
#pragma comment(linker,"/NODEFAULTLIB")



#define plugin_section "Winamp" // ������ �������� ��� �������
#define CMDCOUNT 55             // ���������� ������, ������������ � �������

// �������������� ����
#define SHW_MAIN      1
#define SHW_EQUALIZER 2
#define SHW_PLAYLIST  4
#define SHW_BROWSER   8

struct stSettings
{
 char Winamp_FileName[MAX_PATH];  // ������ ���� � ������������ ����� Winamp
 char Winamp_ClassName[MAX_PATH]; // ��� ������ �������� ���� Winamp

 char LikeDir[MAX_PATH];          // ������ ���� � �������� ��� ���������������
 char LikeFile[MAX_PATH];         // ������ ���� � ����� ��� ���������������

 int  SH_flags;                   // debug ����� ������/������� ����

 int  CurrentSong_UpdateTime;     // ���������� ������� CurrentSong
 int  xCurSong, yCurSong;         // ���������� ���� ���������� � ������� �����
} Settings;                       // ��������� ������

HMODULE hDll;                   // ��������� �� dll
int     CmdCount = CMDCOUNT;    // ���������� ������
char    CmdList[CMDCOUNT][200]; // ������ ������

char    Ini_FileName[MAX_PATH]; // ������ ���� � ����� �������������
HWND    hwndCurrentSong = NULL; // ������, ������������ ��������� ������

// �������� �������
BOOL CALLBACK DlgConfigProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);


// �����������/���������� ������
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
 switch (ul_reason_for_call)
  {
   case DLL_PROCESS_ATTACH:
    {
     // ���������� ������
     lstrcpy(CmdList[STARTCLOSEWINAMP],      "���������/������� Winamp");
     lstrcpy(CmdList[RESTARTWINAMP],         "������������� Winamp");
     lstrcpy(CmdList[BACK5SEC],              "����� �� 5 ������");
     lstrcpy(CmdList[FORW5SEC],              "����� �� 5 ������");
     lstrcpy(CmdList[PLAY],                  "���������������");
     lstrcpy(CmdList[PAUSE],                 "�����");
     lstrcpy(CmdList[STOP],                  "���������� ���������������");
     lstrcpy(CmdList[STOPAFTERSONG],         "���������� ��������������� ����� ����� ������� �����");
     lstrcpy(CmdList[STOPFADEOUT],           "���������� ��������������� � ������� ���������� ��������� (Fadeout)");
     lstrcpy(CmdList[VOLUMEUP],              "�������� ���������");
     lstrcpy(CmdList[VOLUMEDOWN],            "�������� ���������");

     lstrcpy(CmdList[PREVSONG],              "���������� �����");
     lstrcpy(CmdList[NEXTSONG],              "��������� �����");
     lstrcpy(CmdList[GOTOFIRSTSONG],         "������� �� ������ ����� Playlist'�");
     lstrcpy(CmdList[GOTOLASTSONG],          "������� �� ��������� ����� Playlist'�");
     lstrcpy(CmdList[BACK10TRACKS],          "������� ����� �� 10 �����");
     lstrcpy(CmdList[FORW10TRACKS],          "������� ����� �� 10 �����");
     lstrcpy(CmdList[JUMPTOTIME],            "������� �� �������");
     lstrcpy(CmdList[JUMPTOFILE],            "����� �������� Playlist");

     lstrcpy(CmdList[HSCURSONGINFO],         "��������/������ ���������� � ������� �����");
     lstrcpy(CmdList[SHOWFILEINFO],          "�������� ���������� � �����");

     lstrcpy(CmdList[EXECUTEVISUALISATION],  "��������/��������� ������ ������������");
     lstrcpy(CmdList[CONFIGUREVISUALISATION],"��������� ������� ������ ������������");

     lstrcpy(CmdList[OPENADDRESSINTERNET],   "������� ����� Internet");
     lstrcpy(CmdList[OPENFILES],             "������� �����");
     lstrcpy(CmdList[OPENDIRECTORY],         "������� �������");
     lstrcpy(CmdList[PLAYAUDIOCD],           "������� ������� ����");
     lstrcpy(CmdList[ADDTOBOOKMARK],         "�������� ������� ����� � �������� (Bookmarks)");

     lstrcpy(CmdList[TIMEELAPSED],           "����������� ������� � ����� '������ �������'");
     lstrcpy(CmdList[TIMEREMAINING],         "����������� ������� � ����� '�������� �������'");
     lstrcpy(CmdList[SHUFFLE],               "��������/��������� ��������� ��������������� (Shuffle)");
     lstrcpy(CmdList[LOOP],                  "��������/��������� ��������������� ������ (Loop)");
     lstrcpy(CmdList[CLEARPLAYLIST],         "�������� ������ (Playlist)");
     lstrcpy(CmdList[SOUNDOFF],              "��������� ���� (����������� ���������)");
     lstrcpy(CmdList[SOUNDON],               "�������� ���� (������������ ���������)");

     lstrcpy(CmdList[HELP],                  "������� ������ �� Winamp");
     lstrcpy(CmdList[MAINMENU],              "������� ������� ���� Winamp");
     lstrcpy(CmdList[DOUBLESIZE],            "��������/��������� ������� ������");
     lstrcpy(CmdList[ALWAYSONTOP],           "��������/��������� ����� '������ ���� ����'");

     lstrcpy(CmdList[MINIMIZERESTORE],       "��������/���������� ���� Winamp");
     lstrcpy(CmdList[HSWINAMP],              "��������/������ Winamp");
     lstrcpy(CmdList[HSEQUALISER],           "��������/������ Equalizer");
     lstrcpy(CmdList[HSPLAYLIST],            "��������/������ Playlist");
     lstrcpy(CmdList[HSMINIBROWSER],         "��������/������ Minibrowser");
     lstrcpy(CmdList[HSWAEQPL],              "��������/������ ���� ��������� � ������������");
     lstrcpy(CmdList[WSWINAMP],              "����������� ������ ���� Winamp");
     lstrcpy(CmdList[WSEQUALISER],           "����������� ������ ���� Equaliser");
     lstrcpy(CmdList[WSPLAYLIST],            "����������� ������ ���� Playlist");

     lstrcpy(CmdList[HSOPTIONS],             "�������/������� ���� �������� (Preferences)");
     lstrcpy(CmdList[OPTINDICATOR],          "������� ��������� �������� ���� (Visualisation)");
     lstrcpy(CmdList[OPTVISULISATION],       "������� ��������� �������� ������������ (Visualisation Plugins)");
     lstrcpy(CmdList[OPTSKINS],              "������� ����� ����� (Skins)");
     lstrcpy(CmdList[OPTBOOKMARKS],          "������� �������� �������� (Bookmarks)");

     lstrcpy(CmdList[LIKEDIR],               "������� ������� �������");
     lstrcpy(CmdList[LIKEFILE],              "������� ������� ��������");

     // �������� ������ �� ini �����
     GetModuleFileName(hDll,Ini_FileName,sizeof(Ini_FileName));
     for (int i=lstrlen(Ini_FileName); i>=0; i--)
      if (Ini_FileName[i]=='\\') // ���������� �� ����� �����
       {
        Ini_FileName[i+1]='\0';
        break;
       }
     lstrcat(Ini_FileName,"Plugins\\plugins.ini");
     GetPrivateProfileString(plugin_section,"Winamp_FileName","",Settings.Winamp_FileName, sizeof(Settings.Winamp_FileName), Ini_FileName);
     GetPrivateProfileString(plugin_section,"Winamp_ClassName","Winamp v1.x",Settings.Winamp_ClassName,sizeof(Settings.Winamp_ClassName),Ini_FileName);

     Settings.SH_flags = GetPrivateProfileInt(plugin_section,"SH_flags",0,Ini_FileName);

     GetPrivateProfileString(plugin_section,"LikeDir", "",Settings.LikeDir,  sizeof(Settings.LikeDir), Ini_FileName);
     GetPrivateProfileString(plugin_section,"LikeFile","",Settings.LikeFile, sizeof(Settings.LikeFile),Ini_FileName);

     Settings.xCurSong = GetPrivateProfileInt(plugin_section,"xCurSong",0,Ini_FileName);
     Settings.yCurSong = GetPrivateProfileInt(plugin_section,"yCurSong",0,Ini_FileName);
     Settings.CurrentSong_UpdateTime = 400;
    }
    break;

   case DLL_PROCESS_DETACH:
    {
     // ���������� ������ � ini ����
     char buffer[MAX_PATH];

     WritePrivateProfileString("Winamp","Winamp_FileName", Settings.Winamp_FileName, Ini_FileName);
     WritePrivateProfileString("Winamp","Winamp_ClassName",Settings.Winamp_ClassName,Ini_FileName);

     wsprintf(buffer,"%i",Settings.SH_flags);
     WritePrivateProfileString(plugin_section,"SH_flags",buffer,Ini_FileName);
     WritePrivateProfileString("Winamp","LikeDir", Settings.LikeDir, Ini_FileName);
     WritePrivateProfileString("Winamp","LikeFile",Settings.LikeFile,Ini_FileName);
     
     wsprintf(buffer,"%i",Settings.xCurSong);
     WritePrivateProfileString(plugin_section,"xCurSong",buffer,Ini_FileName);
     wsprintf(buffer,"%i",Settings.yCurSong);
     WritePrivateProfileString(plugin_section,"yCurSong",buffer,Ini_FileName);
    }
    break;
  }
 return true;
}

// (���) �������� ��������� ���� Winamp'� ��� ���������� �������
bool WinampCommandSend(int command)
{
 // ����� ���� Winamp
	HWND hwndWA	= FindWindow(Settings.Winamp_ClassName,NULL);
 // ���� ���� �������, �� ��������� ����������, ����� ������ �� ����������
	if (hwndWA) 
  {
   SendMessage(hwndWA,WM_COMMAND,command,0);
   return true;
  }
 else
  return false;
}

// (�� ���) �������� ��������� ���� Winamp'� ��� ���������� �������
bool WinampCommandPost(int command)
{
 // ����� ���� Winamp
	HWND hwndWA	= FindWindow(Settings.Winamp_ClassName,NULL);
 // ���� ���� �������, �� ��������� ����������, ����� ������ �� ����������
	if (hwndWA) 
  {
   PostMessage(hwndWA,WM_COMMAND,command,0);
   return true;
  }
 else
  return false;
}

// (���) �������� ��������� ���� Winamp'� ��� ���������/��������� ���������� Winamp
int WinampUserSend(int data, int id)
{
 // ����� ���� Winamp
	HWND hwndWA = FindWindow(Settings.Winamp_ClassName,NULL);
		// ���� ���� �������, �� ��������� ����������, ����� ������ �� ����������
	if (hwndWA) return (SendMessage(hwndWA,WM_USER,data,id));
	else return (-1);
}

// (�� ���) �������� ��������� ���� Winamp'� ��� ���������/��������� ���������� Winamp
int WinampUserPost(int data, int id)
{
 // ����� ���� Winamp
	HWND hwndWA = FindWindow(Settings.Winamp_ClassName,NULL);
	// ���� ���� �������, �� ��������� ����������, ����� ������ �� ����������
	if (hwndWA) return (PostMessage(hwndWA,WM_USER,data,id));
	else return (-1);
}

// ���������/������� Winamp
// -1   - ���� ����������� ��� ��� ������� ������
// 0-31 - ������
// >31  - ���� �� ���� �������, ������� �������
int WinampStartClose()
{
 // ���� ���� ����������, �� ����� �������, ����� ����������� ���������
 if (FindWindow(Settings.Winamp_ClassName,NULL)) 
  {
   WinampCommandPost(IPC_CLOSEWINAMP); // ������� ��������� Winamp
   return (-1);
  }
 else
  {
   if (FileExists(0,Settings.Winamp_FileName))
    return WinExec(Settings.Winamp_FileName,SW_NORMAL);
   else
    {
     if (MessageBox(0,"����������� ���� Winamp �� ��� ������. �� ������ ��� �������?","������ Winamp",MB_SETFOREGROUND|MB_ICONQUESTION|MB_YESNO)==IDYES)
      DialogBox(hDll,MAKEINTRESOURCE(IDD_CONFIG),0,(DLGPROC)DlgConfigProc);
     return (-1);
    }
  }
}

// ���������� ���� CurrentSong
void CurrentSong_Update()
{
 // ��������� ����������
	char buffer [MAX_PATH],
      buffer2[MAX_PATH];
 int  i1,i2;

	// ����� ���� Winamp
	HWND hwndWA = FindWindow(Settings.Winamp_ClassName,NULL);
 
 // *���������� �������, ���� ���� ����������

 // ��������� ������ => ���������������, �����, ����������
 if (IsWindow(hwndWA))
  switch(WinampUserSend(0,IPC_ISPLAYING))
   {
    case 1:  lstrcpy(buffer,"���������������"); break;
    case 3:  lstrcpy(buffer,"�����");           break;
    default: lstrcpy(buffer,"����������");      break;
   }
 else lstrcpy(buffer,"�� �������");
	GetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_STATUS,buffer2,sizeof(buffer2));
 if (lstrcmp(buffer,buffer2)!=0)
		SetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_STATUS,buffer);

 // ����� ����� => "(##/##)"
 if (IsWindow(hwndWA))
  {
   i1 = WinampUserSend(0,IPC_GETLISTPOS)+1;  // ����� ������� �����
   i2 = WinampUserSend(0,IPC_GETLISTLENGTH); // ����� � ����� �����
   wsprintf(buffer,"(%i/%i)",i1,i2);
  }
 else lstrcpy(buffer,"");
 GetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_SONGSCOUNTER,buffer2,sizeof(buffer2));
		if (lstrcmp(buffer,buffer2)!=0)
			SetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_SONGSCOUNTER,buffer);

 // ��������� ����� => ��������� ���� Winamp
 if (IsWindow(hwndWA))
  {
			GetWindowText(hwndWA,buffer,sizeof(buffer));
   // ���������� �� ������ �����
 	 i1 = WinampUserSend(0,IPC_GETLISTPOS)+1; // ����� ������� �����
   wsprintf(buffer2,"%i",i1);
   lstrcpy(buffer2,buffer+lstrlen(buffer2)+2); // ����� ������ �� ���������� �������� � ������ + ��� 2 ������� ". "
   // ���������� �� ������� � ����� ������
   lstrcpy(buffer,buffer2);
   for (i1 = lstrlen(buffer)-1; i1>=0; i1--)
    if (buffer[i1]=='-') 
     {
      buffer[i1-1]='\0';
      break;
     }
  }
 else lstrcpy(buffer,"");
 GetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_SONGTITLE,buffer2,sizeof(buffer2));
	if (lstrcmp(buffer,buffer2)!=0) 
		SetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_SONGTITLE,buffer);

 // ����� �����
 if (IsWindow(hwndWA))
  {
   // ��������
   i1 = WinampUserSend(1,IPC_GETOUTPUTTIME); // ����� ����� � ��������
   i2 = i1/60;                               // ������� � ������
			SendDlgItemMessage(hwndCurrentSong,IDD_CURRENTSONGINFO_PROGRESS,PBM_SETRANGE,0,MAKELPARAM(0, i1)); // �������� - ������������ ��������
   // �����
   wsprintf(buffer,"%i:",i2);
   i2 = i1-60*i2;
   if (i2<10) lstrcat(buffer,"0");
   wsprintf(buffer2,"%i",i2);
   lstrcat(buffer,buffer2);
  }
 else lstrcpy(buffer,"0:00");
	GetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_SONGLENGHT,buffer2,sizeof(buffer2));
	if (lstrcmp(buffer,buffer2)!=0)
	 SetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_SONGLENGHT,buffer);

 // ������� �������
 if (IsWindow(hwndWA))
  {
   // ��������
	  i1 = WinampUserSend(0,105)/1000; // ������� ������� ������������, ������� � �������
   i2 = i1/60;                  // ������� � ������
			SendDlgItemMessage(hwndCurrentSong,IDD_CURRENTSONGINFO_PROGRESS,PBM_SETPOS, WPARAM(i1),0); // �������� - �������
   // �����
   wsprintf(buffer,"%i:",i2);
   i2 = i1-60*i2;
   if (i2<10) lstrcat(buffer,"0");
   wsprintf(buffer2,"%i",i2);
   lstrcat(buffer,buffer2);
  }
 else 
  {
  	SendDlgItemMessage(hwndCurrentSong,IDD_CURRENTSONGINFO_PROGRESS,PBM_SETPOS, WPARAM(0),0); // �������� - �������
   lstrcpy(buffer,"0:00");
  }
	GetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_POSITION,buffer2,sizeof(buffer2));
	if (lstrcmp(buffer,buffer2)!=0)
	 SetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_POSITION,buffer);

 // �������
 if (IsWindow(hwndWA))
  {
			i1 = WinampUserSend(1,IPC_GETINFO); // �������
   wsprintf(buffer,"%i ����/���.",i1);
			if (i1>=1400) lstrcat(buffer," (Audio CD quality)");
  }
 else lstrcpy(buffer,"");
 GetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_BITRATE,buffer2,sizeof(buffer2));
 if (lstrcmp(buffer,buffer2)!=0)
		SetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_BITRATE,buffer);

 // ��������� � ���������� �������
 if (IsWindow(hwndWA))
  {
   i1 = WinampUserSend(0,IPC_GETINFO); // ���������
   i2 = WinampUserSend(2,IPC_GETINFO); // ���������� �������
   wsprintf(buffer,"%i  ���",i1);
			if (i2==1) lstrcat(buffer," (����)");
			else       lstrcat(buffer," (������)");
  }
 else lstrcpy(buffer,"");
 GetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_SAMPLERATE,buffer2,sizeof(buffer2));
 if (lstrcmp(buffer,buffer2)!=0)
		SetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_SAMPLERATE,buffer);

 // ������ �������� Shuffle � Loop
 if (IsWindow(hwndWA))
  {
   if (WinampUserSend(0,IPC_GET_SHUFFLE)==1) SetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_SHUFFLE,"Sh+");
   else                                      SetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_SHUFFLE,"Sh-");
   if (WinampUserSend(0,IPC_GET_REPEAT)==1)  SetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_LOOP,   "Lp+");
   else                                      SetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_LOOP,   "Lp-");
  }
 else
  {
   SetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_SHUFFLE,"");
   SetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_LOOP,   "");
  }
}

// ���������� ������� ����� �������� ���������� �������
void CALLBACK CurrentSong_TimerProc(HWND hwnd, UINT msg, UINT idTimer, DWORD dwTime)
{	CurrentSong_Update(); }

// ������� �������������
//void Init(void)
//{ MessageBox(0,"Winamp2 Init","Info:",0); }

// ������� ���������������
void Quit(void)
{
 // �������� ������� ������� �����, ���� �� ������
 if (IsWindow(hwndCurrentSong)) DestroyWindow(hwndCurrentSong);
}

// ������� ��������� ���������, ����������� ������� 
BOOL CALLBACK DlgCurrentSongProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
 switch (message)
  {
   case WM_INITDIALOG:
   	// ��������� ������� ���������� ����
    SetTimer(hwndDlg,1,Settings.CurrentSong_UpdateTime,(TIMERPROC)CurrentSong_TimerProc);
    break;
   case WM_COMMAND: // ��������� ������� ������ � ������� �������
    switch (LOWORD(wParam))
     {
      case IDD_CURRENTSONGINFO_OK:
       // ���������� ��������� �������
       RECT rct;
       GetWindowRect(hwndDlg,&rct);
       Settings.xCurSong = rct.left;
       Settings.yCurSong = rct.top;
       DestroyWindow(hwndDlg);
       break;
     }
    return true;

   case WM_CLOSE:   
    DestroyWindow(hwndDlg);
    EndDialog(hwndDlg,0);
	 		return true;

   case WM_DESTROY:
    KillTimer(hwndCurrentSong,1);
    return true;
  }
 return false;
}

// ������� ��������� ���������, ����������� ������� 
BOOL CALLBACK DlgConfigProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
 switch (message)
  {
   case WM_INITDIALOG:
    {
     SendDlgItemMessage(hwndDlg,IDC_WINAMPFILENAME,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.Winamp_FileName);

     if (Settings.SH_flags&SHW_MAIN)      CheckDlgButton(hwndDlg,IDC_SH_MAIN,1);
     if (Settings.SH_flags&SHW_EQUALIZER) CheckDlgButton(hwndDlg,IDC_SH_EQUALIZER,1);
     if (Settings.SH_flags&SHW_PLAYLIST)  CheckDlgButton(hwndDlg,IDC_SH_PLAYLIST,1);
     if (Settings.SH_flags&SHW_BROWSER)   CheckDlgButton(hwndDlg,IDC_SH_BROWSER,1);
     SendDlgItemMessage(hwndDlg,IDC_LIKEDIR, WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LikeDir);
     SendDlgItemMessage(hwndDlg,IDC_LIKEFILE,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LikeFile);

     SendDlgItemMessage(hwndDlg,IDC_WINAMPCLASSNAME,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.Winamp_ClassName);
     char buffer[50];
     wsprintf(buffer,"%i",Settings.CurrentSong_UpdateTime);
     SendDlgItemMessage(hwndDlg,IDC_CURRENTSONGUPDATE,WM_SETTEXT,0,(LPARAM)(LPCTSTR)buffer);
    }
	 		return true;

   case WM_COMMAND:
    switch (LOWORD(wParam))
     {
      case IDC_BTN_GETWA_FILENAME_HD: // ����� exe �����
       FLGetFile(hwndDlg,Settings.Winamp_FileName,"�������� ����������� ���� Winamp","������������� Winamp (winamp.exe)\0winamp.exe\0����������� ����� (*.exe)\0*.exe\0\0",0);
       SendDlgItemMessage(hwndDlg,IDC_WINAMPFILENAME,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.Winamp_FileName);
       break;
	  case IDC_BTN_GETWA_FILENAME_REGISTRY: // ���������� �� �������
InstallDirRegKey HKLM\Software\Microsoft\Windows\CurrentVersion\Uninstall\Winamp ("UninstallString")
���
current_user\software\winamp
       break;
      case IDC_BTN_LIKEDIR:  // ����� ����������
       FLGetDirectory(hwndDlg,Settings.LikeDir,"�������� ���������� ��� �������� ������� � Winamp");
       SendDlgItemMessage(hwndDlg,IDC_LIKEDIR, WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LikeDir);
       break;
      case IDC_BTN_LIKEFILE: // ����� �����
       FLGetFile(hwndDlg,Settings.LikeFile,"�������� ����, ������� ������� ������� � Winamp","��������� Winamp (*.m3u)\0*.m3u\0����� mp3 (*.mp3)\0*.mp3\0��� ����� (*.*)\0*.*\0\0",0);
       SendDlgItemMessage(hwndDlg,IDC_LIKEFILE,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LikeFile);
       break;
      case IDC_CLOSE:
       EndDialog(hwndDlg,0);
       break;
     }
	 		return true;

   case WM_CLOSE:   
    EndDialog(hwndDlg,0);
	 		return true;

   case WM_DESTROY:
    {
     SendDlgItemMessage(hwndDlg,IDC_WINAMPFILENAME, WM_GETTEXT,(WPARAM)sizeof(Settings.Winamp_FileName), (LPARAM)(LPCTSTR)Settings.Winamp_FileName);

     Settings.SH_flags = 0;
     if (IsDlgButtonChecked(hwndDlg,IDC_SH_MAIN))      Settings.SH_flags |= SHW_MAIN;
     if (IsDlgButtonChecked(hwndDlg,IDC_SH_EQUALIZER)) Settings.SH_flags |= SHW_EQUALIZER;
     if (IsDlgButtonChecked(hwndDlg,IDC_SH_PLAYLIST))  Settings.SH_flags |= SHW_PLAYLIST;
     if (IsDlgButtonChecked(hwndDlg,IDC_SH_BROWSER))   Settings.SH_flags |= SHW_BROWSER;
     SendDlgItemMessage(hwndDlg,IDC_LIKEDIR, WM_GETTEXT,(WPARAM)sizeof(Settings.LikeDir), (LPARAM)(LPCTSTR)Settings.LikeDir);
     SendDlgItemMessage(hwndDlg,IDC_LIKEFILE,WM_GETTEXT,(WPARAM)sizeof(Settings.LikeFile),(LPARAM)(LPCTSTR)Settings.LikeFile);

     SendDlgItemMessage(hwndDlg,IDC_WINAMPCLASSNAME,WM_GETTEXT,(WPARAM)sizeof(Settings.Winamp_ClassName),(LPARAM)(LPCTSTR)Settings.Winamp_ClassName);
    }
	 		return true;
  }
 return false;
}

// ������������ �������
void Config(void)
{
 DialogBox(hDll,MAKEINTRESOURCE(IDD_CONFIG),0,(DLGPROC)DlgConfigProc);
}

// ������ �� ���������� �������
void Command(int cmdid)
{
 switch (cmdid)
  {
   // ������ Winamp
   case STARTCLOSEWINAMP:
    WinampStartClose();
    break;
   case RESTARTWINAMP:
    WinampUserPost(0,IPC_RESTARTWINAMP);
    break;

   // ���������� ������� ������
   case BACK5SEC:
    WinampCommandPost(WINAMP_REW5S);
    break;
   case FORW5SEC:
    WinampCommandPost(WINAMP_FFWD5S);
    break;
   case PLAY:
    // ���� ���� Winamp �� ���� �������, �� ������� ��������� Winamp
    if (!WinampCommandSend(WINAMP_PLAY))
     {
      int res = WinampStartClose();
      if (res>31)
      WinampCommandSend(WINAMP_PLAY);
     }
    break;
   case PAUSE:
    WinampCommandPost(WINAMP_PAUSE);
    break;
   case STOP:
    WinampCommandPost(WINAMP_STOP);
    break;
   case STOPAFTERSONG:
    WinampCommandPost(WINAMP_STOPAFTERTRACK);
    break;
   case STOPFADEOUT:
    WinampCommandPost(WINAMP_FADEOUT);
    break;
   case VOLUMEUP:
    WinampCommandPost(WINAMP_VOLUP);
    break;
   case VOLUMEDOWN:
    WinampCommandPost(WINAMP_VOLDOWN);
    break;

   // ����������� �� ������
   case PREVSONG:
    WinampCommandPost(WINAMP_PREVTRACK);
    break; 
   case NEXTSONG:
    WinampCommandPost(WINAMP_NEXTTRACK);
    break;
   case GOTOFIRSTSONG:
    WinampUserSend(0,IPC_SETPLAYLISTPOS); // ����� ������ �������
    WinampCommandPost(WINAMP_PLAY);
    break;
   case GOTOLASTSONG:
    WinampUserSend(WinampUserSend(0,IPC_GETLISTLENGTH)-1,IPC_SETPLAYLISTPOS); // ����� ��������� �������
    WinampCommandPost(WINAMP_PLAY); // Play
    break;
   case BACK10TRACKS:
    WinampCommandPost(WINAMP_BACK10TRACKS);
    break;
   case FORW10TRACKS:
    WinampCommandPost(WINAMP_FORW10TRACKS);
    break;
   case JUMPTOTIME:
    {
     HWND hwndOld = GetForegroundWindow();
     SetForegroundWindow(FindWindow(Settings.Winamp_ClassName,NULL));
     WinampCommandSend(WINAMP_JUMPTOTIME);
     SetForegroundWindow(hwndOld);
    }
    break;
   case JUMPTOFILE:
    {
     HWND hwndOld = GetForegroundWindow();
     SetForegroundWindow(FindWindow(Settings.Winamp_ClassName,NULL));
     WinampCommandSend(WINAMP_JUMPTOFILE);
     SetForegroundWindow(hwndOld);
    }
    break;

   // ����������
   case HSCURSONGINFO:
   	// ���� ������ ��� ������...
	   if (IsWindow(hwndCurrentSong)) 
	    {
      // ... �� ��� ����� �������
		    DestroyWindow(hwndCurrentSong);
     }
    else
     {
  	   // ... ����� ��� �������� ����                    
     	hwndCurrentSong = CreateDialog(hDll,MAKEINTRESOURCE(IDD_CURRENTSONGINFO),0,(DLGPROC)DlgCurrentSongProc);
     	// ��������� ������ ����
      CurrentSong_Update();
     	// ����� ���� ������ ���� ����
      SetWindowPos(hwndCurrentSong,HWND_TOPMOST,Settings.xCurSong,Settings.yCurSong,0,0,SWP_NOSIZE);
     	ShowWindow(hwndCurrentSong,SW_SHOW);
     }
    break;
   case SHOWFILEINFO:
    {
     HWND hwndOld = GetForegroundWindow();
     SetForegroundWindow(FindWindow(Settings.Winamp_ClassName,NULL));
     WinampCommandSend(WINAMP_FILEINFO);
     SetForegroundWindow(hwndOld);
    }
    break;

   // ������������
   case EXECUTEVISUALISATION:
    WinampCommandPost(WINAMP_EXECVISUAL);
    break;
   case CONFIGUREVISUALISATION:
    {
     HWND hwndOld = GetForegroundWindow();
     SetForegroundWindow(FindWindow(Settings.Winamp_ClassName,NULL));
     WinampCommandSend(WINAMP_CONFVISUAL);
     SetForegroundWindow(hwndOld);
    }
    break;

   // ��������
   case OPENADDRESSINTERNET:
    {
     HWND hwndOld = GetForegroundWindow();
     SetForegroundWindow(FindWindow(Settings.Winamp_ClassName,NULL));
     WinampCommandSend(WINAMP_OPENURL);
     SetForegroundWindow(hwndOld);
    }
    break;
   case OPENFILES:
    {
     HWND hwndOld = GetForegroundWindow();
     SetForegroundWindow(FindWindow(Settings.Winamp_ClassName,NULL));
     WinampCommandSend(WINAMP_OPENFILE);
     SetForegroundWindow(hwndOld);
    }
    break;
   case OPENDIRECTORY:
    {
     HWND hwndOld = GetForegroundWindow();
     SetForegroundWindow(FindWindow(Settings.Winamp_ClassName,NULL));
     WinampCommandSend(WINAMP_OPENDIR);
     SetForegroundWindow(hwndOld);
    }
    break;
   case PLAYAUDIOCD:
    WinampCommandPost(WINAMP_PLAYCD);
    break;
   case ADDTOBOOKMARK:
    WinampCommandPost(WINAMP_ADDTOBOOKMARK);
    break;

   // �������������
   case TIMEELAPSED:
    WinampCommandPost(WINAMP_TIMEELAPSED);
    break;
   case TIMEREMAINING:
    WinampCommandPost(WINAMP_TIMEREMAINING);
    break;
   case SHUFFLE:
    WinampCommandPost(WINAMP_SHUFFLE);
    break;
   case LOOP:
    WinampCommandPost(WINAMP_REPEAT);
    break;
   case CLEARPLAYLIST:
    WinampCommandPost(WINAMP_CLEARPL);
    break;
   case SOUNDOFF:
    WinampUserPost(0,IPC_SETVOLUME);
    break;
   case SOUNDON:
    WinampUserPost(255,IPC_SETVOLUME);
    break;

   // ���������
   case HELP:
    SetForegroundWindow(FindWindow(Settings.Winamp_ClassName,NULL));
    WinampCommandSend(WINAMP_ABOUT);
    break;
   case MAINMENU:
    {
     SetForegroundWindow(FindWindow(Settings.Winamp_ClassName,NULL));
     WinampCommandSend(WINAMP_MAINMENU);
    }
    break;
   case DOUBLESIZE:
    WinampCommandPost(WINAMP_DOUBLESIZE);
    break;
   case ALWAYSONTOP:
    WinampCommandPost(WINAMP_ONTOP);
    break;

   // ���������� ������ Winamp
   case MINIMIZERESTORE:
    {
     HWND            hwnd_wa;
     WINDOWPLACEMENT hwnd_wa_plac;
     hwnd_wa_plac.length = sizeof(WINDOWPLACEMENT);
     hwnd_wa = FindWindow(Settings.Winamp_ClassName,NULL);
     // ���� ���� �������, �� ��������������, ����� �����������������
     if (hwnd_wa)
      if (GetWindowPlacement(hwnd_wa,&hwnd_wa_plac))
       if (hwnd_wa_plac.showCmd==SW_SHOWMINIMIZED) ShowWindow(hwnd_wa,SW_RESTORE);
       else                                        ShowWindow(hwnd_wa,SW_MINIMIZE);
    }
    break;
   case HSWINAMP:
    WinampCommandPost(WINAMP_HSWINAMP);
    break;
   case HSEQUALISER:
    WinampCommandPost(WINAMP_HSEQUALIZER);
    break;
   case HSPLAYLIST:
    WinampCommandPost(WINAMP_HSPLAYLIST);
    break;
   case HSMINIBROWSER:
    WinampCommandPost(WINAMP_HSMINIBROWSER);
    break;
   case HSWAEQPL:
    if (Settings.SH_flags&SHW_MAIN)      WinampCommandPost(WINAMP_HSWINAMP);
    if (Settings.SH_flags&SHW_EQUALIZER) WinampCommandPost(WINAMP_HSEQUALIZER);
    if (Settings.SH_flags&SHW_PLAYLIST)  WinampCommandPost(WINAMP_HSPLAYLIST);
    if (Settings.SH_flags&SHW_BROWSER)   WinampCommandPost(WINAMP_HSMINIBROWSER);
    break;
   case WSWINAMP:
    WinampCommandPost(WINAMP_WSWINAMP);
    break;
   case WSEQUALISER:
    WinampCommandPost(WINAMP_WSEQUALIZER);
    break;
   case WSPLAYLIST:
    WinampCommandPost(WINAMP_WSPLAYLIST);
    break;

   // �������� ���� ��������
   case HSOPTIONS:
    {
     SetForegroundWindow(FindWindow(Settings.Winamp_ClassName,NULL));
     WinampCommandPost(WINAMP_HSPREFERENCES);
    }
    break;
   case OPTINDICATOR:
    {
     SetForegroundWindow(FindWindow(Settings.Winamp_ClassName,NULL));
     WinampCommandPost(WINAMP_PREFINDICATOR);
    }
    break;
   case OPTVISULISATION:
    {
     SetForegroundWindow(FindWindow(Settings.Winamp_ClassName,NULL));
     WinampCommandPost(WINAMP_PREFVISUALIZATION);
    }
    break;
   case OPTSKINS:
    {
     SetForegroundWindow(FindWindow(Settings.Winamp_ClassName,NULL));
     WinampCommandPost(WINAMP_SKINSELECTOR);
    }
    break;
   case OPTBOOKMARKS:
    {
     SetForegroundWindow(FindWindow(Settings.Winamp_ClassName,NULL));
     WinampCommandPost(WINAMP_EDITBOOKMARKS);
    }
    break;

   // ������� ������
   case LIKEDIR:
    {
     char buffer[MAX_PATH];
     wsprintf(buffer,"%s \"%s\"",Settings.Winamp_FileName,Settings.LikeDir);
     WinExec(buffer,SW_SHOWNORMAL);
    }
    break;
   case LIKEFILE:
    {
     char buffer[MAX_PATH];
     wsprintf(buffer,"%s \"%s\"",Settings.Winamp_FileName,Settings.LikeFile);
     WinExec(buffer,SW_SHOWNORMAL);
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
 hDll                      = PluginData->hDll;
 PluginData->Caption       = "Winamp v1.12 (20.05.2004�.)\n\n� <Quash!>, �.������������)";
 // �������� ������� �������
 PluginData->Init          = NULL;//Init;
 PluginData->Quit          = Quit;
 PluginData->Config        = Config;
 PluginData->Command       = Command;
 // �������� ������� � ������ ������ � ���������� ��� ���������
 PluginData->CommandsCount = &CmdCount;
 PluginData->CommandsList  = CmdList;
}

#ifdef __cplusplus
}
#endif
