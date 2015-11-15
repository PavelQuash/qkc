/**************************************************************/
/*              qshKeyCommander1 - qkc_main.cpp               */
/*------------------------------------------------------------*/
/*  ������ �������, �������� ������, ������������ ���������,  */
/*          ����� ���������� ����, ������ � �����             */
/*****03.2005************************Copyright by <Quash!>*****/


#include "windows.h"
#include "qkc_main.h"
#include "resource.h"
#include "CPlugins.h"
#include "DlgMainMenu.h"
#include "DlgSettings.h"
#include "Settings.h"
#include "CTray.h"



// ***���������� Runtime Library***
// ���������� ��������� �������� �� 23 ��������
/*
#pragma comment(linker,"/MERGE:.rdata=.text")
#pragma comment(linker,"/FILEALIGN:512 /SECTION:.text,EWRX /IGNORE:4078")
//#pragma comment(linker,"/ENTRY:WinMain")
//#pragma comment(linker,"/NODEFAULTLIB")
*/



// *���������� ����������
HINSTANCE  hInst = GetModuleHandle(NULL); // Handle ���������
HWND       hwndServerWindow,              // hwnd ����-������� (��������)
           hwndMainMenu;                  // hwnd ���� �������� ����
char       prgdir[MAX_PATH],              // ������� ���������
           hlpdir[MAX_PATH],              // ������� ������
           lngdir[MAX_PATH],              // ������� ������
           plgdir[MAX_PATH];              // ������� ��������
CPlugins   Plugins;                       // ����� ������ � ������������� ��������
stSettings Settings;                      // ��������� ���������

// ������
SOCKET  hWinLIRCsocket = INVALID_SOCKET;  // �����, ��������������� ��� ����������� � WinLIRC
char    sWinLIRCstring[MAX_PATH];         // ������, ������������ WinLIRC
int     hotkeyscount = 0;                 // ���������� ������� ������, ������������������ �� ���� hwndServerWindow
HMODULE hDllHooks = NULL;                 // ������ ��� ������������ ������������ ���������
BOOL PasswordHooks;                       // ������ ��������� ������������ ������
BOOL KeyboardHooks;                       // ������ ��������� ������������ ����������
BOOL MouseHooks;                          // ������ ��������� ������������ �����
BOOL WinLIRCMessages;                     // ������ ��������� ������������ WinLIRC

extern  HWND hwndDlgSettings; // ������� � DlgSettings.cpp
extern  HWND hwndDlgAbout;    // ������� � DlgAbout.cpp

// ������� � dll, ������������� ��������� ����������
typedef void (*IH_Type)(HMODULE hdllinst, bool pswrdstart, char password[MAX_PASSWORDSYMBOLS], bool hotkeystart, HWND hwndSrvWnd);
typedef void (*QH_Type)(void);



// *�������� ���������� �������
LRESULT CALLBACK ServerProc(HWND hwndWnd, UINT message, WPARAM wParam, LPARAM lParam);



// ��������� ����� � ���������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
 Settings.LanguageStrings = NULL;

 // DEBUG - �������� ���������� �� ���������� ���������� ������
 // char buffer[10];
 // itoa(sizeof(CGetKbrdKey),buffer,10);
 // stPluginData     = 292
 // stPluginKeys     = 16
 // stKbrdKey        = 8
 // stWircKey        = 88 // 80 �������� + 2*4 ����� int
 // CKbrdKeys        = 8
 // CWircKeys        = 8
 // CPlugins         = 532
 // stLanguage       = 1680
 // stMainMenuAccess = 30
 // stSettings       = 352
 // MessageBox(0,buffer,"",0);

 // ����������� ��������������� ���������
 GetModuleFileName(hInst,prgdir,sizeof(prgdir));
 for (int i=lstrlen(prgdir); i>=0; i--)
  if (prgdir[i]=='\\') // ���������� �� ����� �����
   {
    prgdir[i]='\0';
    break;
   }

 // ����������� ������� ��������� ���������
 wsprintf(hlpdir,"%s\\%s",prgdir,prg_help);
 wsprintf(lngdir,"%s\\%s",prgdir,prg_languages);
 wsprintf(plgdir,"%s\\%s",prgdir,prg_plugins);
 // �������� �� ������������� ���������
 // (���� ������� �� ����������, �� �������� �������)
 if (!FileExists(1,hlpdir)) wsprintf(hlpdir,"%s",prgdir);
 if (!FileExists(1,lngdir)) wsprintf(lngdir,"%s",prgdir);
 if (!FileExists(1,plgdir)) wsprintf(plgdir,"%s",prgdir);
 
 // ����� �� ���������, ���� ini-���� �� ��� ������
 char inifn[MAX_PATH];
 wsprintf(inifn,"%s\\%s",prgdir,ini_filename);
 if (FileExists(0,inifn)) 
  { if (!SettingsLoad()) return 1; } // �������� �������� ���������, ���� ������ ���, �� �������
 else
  {
   // ����� �� ���������
   MessageBox(0,"���� ������������� �� ��� ������!!!\n������ ��������� ����� ��������.","������",MB_ICONERROR|MB_OK);
   return 1;
  }

 // ����� �� ���������, ���� ��� ��� ��������� � ������ � ��������
 // ���� ������, ����� ����������� ������� ���� � ��������� ��������� ������
 hwndServerWindow = FindWindow(serverclassname,NULL);
	if (hwndServerWindow)
  {
   if (Settings.MainMenuAccess.AccessFlags&AF_DBLRUN)
    PostMessage(hwndServerWindow,WM_MAINMENU,0,0);
   MessageBeep(0xFFFFFFFF); // ������ �������
   return 1;
  }

 // ��������� �������� �� ���������� �����
 if (Settings.RegisterServiceProcess)
  {
   typedef DWORD (WINAPI *RSP_Type)(DWORD, DWORD);  
   HMODULE hKernel;
   RSP_Type RegisterServiceProcess;
   hKernel = LoadLibrary("KERNEL32.DLL");
   if (hKernel)
    {
     RegisterServiceProcess = (RSP_Type)GetProcAddress(hKernel,"RegisterServiceProcess");
     if (RegisterServiceProcess) RegisterServiceProcess(NULL,1);
     FreeLibrary(hKernel);
    }
  }

 // �������� ��������� ����������
 if (Settings.ChangeKeyboardState)
  {
   /* debug ��������� ����������
   // rus - 0x0419
   // eng - 0x0409 (usa)
   char lng[KL_NAMELENGTH];
   LoadKeyboardLayout("00000409",KLF_ACTIVATE);
   GetKeyboardLayoutName(lng);
   MessageBox(0,lng,NULL,NULL);
   */
   //LoadKeyboardLayout("00000409",KLF_ACTIVATE);
   //ActivateKeyboardLayout(GetKeyboardLayout(0),KLF_REORDER);
  }

 // ������������ ������� ��������� => ����������� ��� dll � �� �������
 Plugins.SetPaths(inifn,plgdir);

 // ������������ ����� �������� ����
 WNDCLASS  wc;
 wc.style         = CS_HREDRAW|CS_VREDRAW ;
 wc.lpfnWndProc   = (WNDPROC)ServerProc;
 wc.hInstance     = hInst;                          
 wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
 wc.lpszClassName = serverclassname;
 wc.hCursor       = LoadCursor(NULL,IDC_ARROW);     
 wc.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
 wc.lpszMenuName  = NULL;
 wc.cbClsExtra    = 0;                                   
 wc.cbWndExtra    = 0;                                   
 RegisterClass(&wc);
 // �������� �������� ����
 hwndServerWindow = CreateWindow(wc.lpszClassName,"qkcServer",WS_BORDER|WS_CAPTION|WS_SYSMENU,10,10,140,1,NULL,NULL,hInst,NULL);

 SettingsUpdate(); // ���������� ��������
       
 // ��������� ������ � ��������� ����
 if (Settings.ShowInTray)
  {
   CTray systemtray;
   systemtray.AddTrayIcon(hwndServerWindow,1,NIF_ICON|NIF_MESSAGE|NIF_TIP,LoadIcon(hInst,MAKEINTRESOURCE(IDI_LOGO)),WM_TRAYICON,"qshKeyCommander");
  }

 MonitoringBegin(); // ��������� ������������� ���������

 // ��������� ��������� �������� �������� ����
 MSG message;
 while (GetMessage(&message,NULL,0,0)) 
  TranslateMessage(&message),DispatchMessage(&message);   
 return (message.wParam);
}

// ��������/�������� ������ ��� ������������ ���������
void AttachDllHooks(BOOL onof)
{
 if (onoff)
  {
   if (hDllHooks!=NULL) exit;
   MessageBox(0,"����������� ������",Settings.MainMenuAccess.Password,0); // DEBUG
   // ����������� ��������������� ����� ������� (��� ������������ ���������)
   char buffer[MAX_PATH];
   wsprintf(buffer,"%s\\%s",prgdir,hooks_filename);
   // �������� ����� �� �������������
   if (FileExists(0,buffer))
    {
	 // ���� ����������, ������������ �����������
     hDllHooks = LoadLibrary(hooks_filename);
     if (hDllHooks)
      {
	   // ����������� �������, ���������� �������� ������ � �������� ���� �� ������
	   if (Settings.MainMenuAccess.AccessFlags&AF_PASSWORD)
		{
		 SETPASSWORDHOOK_Type SetPasswordHook = (SETPASSWORDHOOK_Type)GetProcAddress(hDllHooks,"SetPasswordHook");
         if (SetPasswordHook) // ������� ����� �������
		  {
           SetPasswordHook(true);
		   PasswordHook = true;
		  }
         else // ������, ����� �� ��� �������
		  MessageBox(0,"������������ ������ dll! ������� ������������� �� ���� �������.",hooks_filename,MB_ICONERROR|MB_OK);
		}
	  }
	 else
	  {
	   // ����������� �� ���������
	   MessageBox(0,"������ �� ����� ���� ���������!",hooks_filename,MB_ICONERROR|MB_OK);
	  }
    }
   else
    {
     // ����������� ������ - ���� dll �� ����������
     MessageBox(0,"���� ����������� ����������� ���������� �� ��� ������! �������� ������������ ������ ���������� (�� ������ RegisterHotKey), ������ ���� � ��������� ���������.",hooks_filename,MB_ICONERROR|MB_OK);
     if (Settings.MainMenuAccess.AccessFlags&AF_PASSWORD) Settings.MainMenuAccess.AccessFlags ^= AF_PASSWORD;
     Settings.UseWinHooks = false;
	 Settings.MouseOn = false;
    }
  }
 else // �������� dll
  {
   if (hDllHooks==NULL) exit;
   // ���������� ������� � �������� ���� �� ������
   if (Settings.MainMenuAccess.AccessFlags&AF_PASSWORD)
	{
	 SETPASSWORDHOOK_Type SetPasswordHook = (SETPASSWORDHOOK_Type)GetProcAddress(hDllHooks,"SetPasswordHook");
     if (SetPasswordHook) // ������� ����� �������
	  {
       SetPasswordHook(false);
       PasswordHook = false;
	  }
   FreeLibrary(hDllHooks);      
   hDllHooks = NULL;
  }
}


// ���������/���������� ������������ ������������ ���������
void MessagesKeyboard(BOOL onoff)
{
 if (onoff) // ���������
  {
   if (KeyboardHooks) exit;
   // ����������� ������, ���� ����� ����������� � ���������
   if ((Settings.UseWinHooks)&&(Settings.KbrdOn)) AttachDllHooks(true);
   // ��������� ������������ ������, ���� ������������ ��������� �� ���������
   hotkeyscount = 0;
   if (Settings.KbrdOn) // ������� �������
	{
     if (!Settings.UseWinHooks)
	  {
       int  keyscount = 0; // ���������� ������ ����� �� ������������
       WORD *hotkeys;      // ������ ������
       bool analog;
       stKbrdKey kbrdkey;
       int  i,k,l,num=0;
       // ������� ���������� ������� ������ (� �����������)
       for (l=0; l<Plugins.GetPluginsCount(); l++)
        keyscount += Plugins.PluginsKeys[l].KbrdKeys.GetKeysCount();
       // ��������� ������ ��� ������ ������ � ��� ��������� (���������� �� �����)
       hotkeys = new WORD[keyscount];
       for (i=0; i<keyscount; i++) hotkeys[i] = 0;
       // ���������� ������ ������ ������, ��� ���������� �������� "0"
       for (l=0; l<Plugins.GetPluginsCount(); l++)
		{
         for (k=0; k<Plugins.PluginsKeys[l].KbrdKeys.GetKeysCount(); k++)
		  {
           Plugins.PluginsKeys[l].KbrdKeys.GetKey(k,&kbrdkey);
           analog = false;
           for (i=0; i<keyscount; i++)
            if (kbrdkey.KbrdKey==hotkeys[i]) analog = true;
           if (!analog) 
			{
             hotkeys[num] = kbrdkey.KbrdKey;
             num++;
			}
		  }
		}
       keyscount = num;
       // ������ �����������, ���������� ������� ������ � ��������-�����
       analog = false;
       for (i=0; i<keyscount; i++)
		{
         if (RegisterHotKey(hwndServerWindow,hotkeyscount+1,LOBYTE(hotkeys[i]),HIBYTE(hotkeys[i]))) hotkeyscount++;
         else 
		  {
           char str[100];
           KeyToStr(hotkeys[i],str);
           MessageBox(0,"��������� ������� ������� ����������!",str,MB_ICONERROR|MB_OK);
           analog = true;
		  }
		}
       delete [] hotkeys;
       // ���� � ������� ��� ����������� ��������� ���������� ������, �� ��������� �������������� ������������
       if (analog) MessageBox(0,"���� ��� ������ \"�������\" ������ �� ����� ���� �����������, �.�. � ������� ��� ��� ����������� � ������������ ������ ����������!\n������������ ������������� �������, ����� �� ���� ����������.","������",MB_ICONERROR|MB_OK);
	  }
     else // �������
	  {
	   if (hDllHooks)
		{
		 SETKEYBOARDHOOK_Type SetKeyboardHook = (SETKEYBOARDHOOK_Type)GetProcAddress(hDllHooks,"SetKeyboardHook");
         if (SetKeyboardHook) // ������� ����� �������
		  {
           SetKeyboardHook(true);
		  }
         else // ������, ����� �� ��� �������
		  MessageBox(0,"������������ ������ dll! ������� SetKeyboardHook �� ���� �������.",hooks_filename,MB_ICONERROR|MB_OK);
		}
	   // else ���������, ��� ������������ ������ �� ����� ���� ��������
	  }
	 KeyboardHook = true;
	}
  }
 else       // ����������
  {
   if (!KeyboardHooks) exit;
   KeyboardHook = false;
  }
}

// ���������/���������� ������������ ��������� ����
void MessagesMouse(BOOL onoff)
{
 if (onoff) // ���������
  {
   if (MouseHooks) exit;
   if (Settings.MouseOn) AttachDllHooks(true);
  }
 else       // ����������
  {
   if (!MouseHooks) exit;
  }
}

// ���������/���������� ������������ ������ ����
void MessagesMouseGestures(BOOL onoff)
{
 if (onoff) // ���������
  {
  }
 else       // ����������
  {
  }
}

// ���������/���������� ������������ ��������� �� ���
void MessagesWinLIRC(BOOL onoff)
{
 if (onoff) // ���������
  {
   if (WinLIRC) exit;
   // ������������ ��������� �� WinLIRC
   if (Settings.WircOn)
    if (!(WinLIRCconnect(Settings.WinLIRCaddress,Settings.WinLIRCport,hwndServerWindow))) 
	 {
	  // ������������ �� �������
      char buffer[MAX_PATH];
      wsprintf(buffer,"%s : %i",Settings.WinLIRCaddress,Settings.WinLIRCport);
      MessageBox(0,"�� ������� ������������ � ������� WinLIRC. �������� ���� ����������� ������� ����� � ����. ���� ������ ���������, �� ������ ����� 127.0.0.1, � ���� �� ��������� 8765! ��������� ������������ �������� ������ � ����� �������� ������������.",buffer,MB_ICONERROR|MB_OK);
      Settings.WircOn = false;
	 }
	else WinLIRC = true; // ����������� �������
  }
 else       // ����������
  {
   if (!WinLIRC) exit;
   WinLIRCdisconnect();
   WinLIRC = false;
  }
}

/*
void MessagesTimeEvents(BOOL onoff)
{
 if (onoff) // ���������
  {
  }
 else       // ����������
  {
  }
}
*/

// ��������� ������ ������������
void MonitoringBegin()
{
 // ��������� ������������ �������������� ���������� �������:
 // 1) ��� ����������� ������� ����������, ���� ����� ������������ �� ������, �� ����������� SetPasswordHook
 // 2) ��������� ���� ��������� (�� �������� ����������)
 //    ���� ��� ����������, �� ������������ ������� ����������
 //    ��� ������������ � �������:
 //    1. ������������ ��������� �� ���������� �� WinHooks (SetKeyboardHook) - ������������ ��������
 //    2. ��������� �� ����� (SetMouseHook)


 // ����������� ������������ �� ���������, �����, ���� ����������, �� �������� � true
 PasswordHooks   = false;
 KeyboardHooks   = false;
 MouseHooks      = false;
 WinLIRCMessages = false;
 //TimeEvents      = false;

 // ���� ������ ������, � ��������� ������ � �������� ���� �� ������, �� ������������ ���� AF_PASSWORD
 if (Settings.MainMenuAccess.AccessFlags&AF_PASSWORD)
  if (lstrlen(Settings.MainMenuAccess.Password)<=0) 
   {
    MessageBox(0,"������ � �������� ���� �� ������ �� ����� ���� �������, ��� ��� �� �� ��� ����������!","������",MB_ICONERROR|MB_OK);
    if (Settings.MainMenuAccess.AccessFlags&AF_PASSWORD) Settings.MainMenuAccess.AccessFlags ^= AF_PASSWORD;
   }

 // *��������� ������� �������� ���� - ������ �� ���������� ������� ������
 if (Settings.MainMenuAccess.AccessFlags&AF_HOTKEY)
  {
   if (!RegisterHotKey(hwndServerWindow,0,LOBYTE(Settings.MainMenuAccess.HotKey),HIBYTE(Settings.MainMenuAccess.HotKey)))
    {
     // ������� ��� ���� ���������������� � �������
     char buffer[MAX_PATH];
     KeyToStr(Settings.MainMenuAccess.HotKey,buffer);
     if (Settings.MainMenuAccess.AccessFlags&AF_HOTKEY) Settings.MainMenuAccess.AccessFlags ^= AF_HOTKEY;
     MessageBox(0,"������������ ������� ��������� ������ ��� ������� � �������� ���� ��������� ����������, ��� ��� � ������� ��� ���������������� ����������� ���������.\n������������� �������� ���������� ������ �� ������ ���������, ����� ��� �� ������������� � �������.",buffer,MB_ICONERROR|MB_OK);
    }
  }

 // *��������� ���� ���������
 MessagesKeyboard(true);
// MessagesMouse(true);
// MessagesMouseGestures(true);
 MessagesWinLIRC(true);
//MessagesTimeEvents(true);

 // *������������� �������
 stPluginData plgdata;
 for (int i=0; i<Plugins.GetPluginsCount(); i++)
  {
   Plugins.GetPluginData(i,&plgdata);
   if (plgdata.Init!=NULL) plgdata.Init();
  }
}

// ���������� ������ ������������
void MonitoringEnd()
{
 /*// *��������������� ����������
 stPluginData plgdata;
 for (int i=0; i<Plugins.GetPluginsCount(); i++)
  {
   Plugins.GetPluginData(i,&plgdata);
   if (plgdata.Quit!=NULL) plgdata.Quit();
  }
*/
 // *�������� ������� �������� ����
 // �������� ������� �� ���������� ������� ������
 if (Settings.MainMenuAccess.AccessFlags&AF_HOTKEY)
  UnregisterHotKey(hwndServerWindow,0);
/*
 // *�������� ������������ ��������� �� ����������
 if (Settings.KbrdOn)
  if (!Settings.UseWinHooks)
   {
    for (int i=0; i<hotkeyscount; i++)
     UnregisterHotKey(hwndServerWindow,i+1);
   }

 // ���� ���� ��������� dll, �� �����������
 if ((Settings.MainMenuAccess.AccessFlags&AF_PASSWORD)||((Settings.UseWinHooks)&&(Settings.KbrdOn)))
  {
   QH_Type QuitHooks = (QH_Type)GetProcAddress(hDllHooks,"QuitHooks");
   if (QuitHooks) QuitHooks();
   FreeLibrary(hDllHooks);
  }

 // *�������� ������������ ��������� �� WinLIRC
 if (Settings.WircOn) WinLIRCdisconnect();
 */
}

// ��������� ���������, ����������� �������� ����
LRESULT CALLBACK ServerProc(HWND hwndWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 int res=0; // �� ���������
 switch (message)
  {
   case WM_HOTKEY: // ������� �� ������� ���������� ������� ������
    {
     // ���� ������ ���������� ������ ��� ������� � �������� ����
     if ((int)wParam==0) 
      {
       SendMessage(hwndServerWindow,WM_MAINMENU,0,0);
       break;
      }
     // ���� ������������ �� ��������, �� ������������ �������
     if (!Settings.KbrdOn) break;
     // ���������� ������, ������� ������������� ������� �������
     int k,l;
     bool act_cl,
          act_nl,
          act_sl;
     stPluginData plgdata;
     stKbrdKey    kbrdkey;
     WORD         hotkey = MAKEWORD(LOWORD(lParam),HIWORD(lParam));
     WORD         cns;

     BYTE keyboardstate[256];
     cns = 0;
/*
     char buffer [MAX_PATH];
     char buffer2[MAX_PATH];

     buffer [0]='\0';
     buffer2[0]='\0';
*/
     GetKeyboardState(keyboardstate);

     if (keyboardstate[VK_CAPITAL]&1) {cns |= CNS_CAPSLOCK;}
     if (keyboardstate[VK_NUMLOCK]&1) {cns |= CNS_NUMLOCK;}
     if (keyboardstate[VK_SCROLL] &1) {cns |= CNS_SCROLLLOCK;}
     
/*
     if (keyboardstate[VK_CAPITAL]&1) {cns |= CNS_CAPSLOCK;  lstrcat(buffer,"CAPSLOCK\n");}
     if (keyboardstate[VK_NUMLOCK]&1) {cns |= CNS_NUMLOCK;   lstrcat(buffer,"NUMLOCK\n");}
     if (keyboardstate[VK_SCROLL] &1) {cns |= CNS_SCROLLLOCK;lstrcat(buffer,"SCROLLLOCK\n");}

     if (keyboardstate[VK_CAPITAL]&0x8000) {lstrcat(buffer,"C ");}
     if (keyboardstate[VK_NUMLOCK]&0x8000) {lstrcat(buffer,"N ");}
     if (keyboardstate[VK_SCROLL] &0x8000) {lstrcat(buffer,"S ");}
     /*

     if (GetKeyState(VK_CAPITAL)&0x8000) {lstrcat(buffer2,"C");}
     if (GetKeyState(VK_NUMLOCK)&0x8000) {lstrcat(buffer2,"N");}
     if (GetKeyState(VK_SCROLL) &0x8000) {lstrcat(buffer2,"S");}

     MessageBox(0,buffer,buffer2,NULL);
*/

     for (l=0; l<Plugins.GetPluginsCount(); l++)
      {
       Plugins.GetPluginData(l,&plgdata);
       for (k=0; k<Plugins.PluginsKeys[l].KbrdKeys.GetKeysCount(); k++)
        {
         Plugins.PluginsKeys[l].KbrdKeys.GetKey(k,&kbrdkey);
         // ������� �������
         if (hotkey==kbrdkey.KbrdKey)
          {
           // � ����������� �� ������ ����������� �������
           act_cl = false;
           act_nl = false;
           act_sl = false;
           if (kbrdkey.CNS!=0)
            {
             BYTE cnslobyte = LOBYTE(kbrdkey.CNS);
             BYTE cnshibyte = HIBYTE(kbrdkey.CNS);
             // �������� ��������� cns
             if (cnslobyte&CNS_CAPSLOCK) 
              { if ((cnshibyte&CNS_CAPSLOCK)==(cns&CNS_CAPSLOCK)) act_cl = true; }
             else act_cl = true;
             if (cnslobyte&CNS_NUMLOCK) 
              { if ((cnshibyte&CNS_NUMLOCK)==(cns&CNS_NUMLOCK)) act_nl = true; }
             else act_nl = true;
             if (cnslobyte&CNS_SCROLLLOCK) 
              { if ((cnshibyte&CNS_SCROLLLOCK)==(cns&CNS_SCROLLLOCK)) act_sl = true; }
             else act_sl = true;
            }
           else
            {
             act_cl = true;
             act_nl = true;
             act_sl = true;
            }
           if (act_cl&&act_nl&&act_sl)
            if (plgdata.Command!=NULL)
             plgdata.Command(kbrdkey.CmdID);
          }
        }
      }
    }
   break;

   case WM_WIRCKEYS:
    // ���� ������������ �� ��������, �� ������������ ������� (�� ������ ������)
    if (!Settings.WircOn) break;
    WORD WSAEvent;
    WSAEvent = WSAGETSELECTEVENT(lParam);
	   if (WSAEvent==FD_READ)
     {
  			 recv(hWinLIRCsocket,sWinLIRCstring,sizeof(sWinLIRCstring),0);
      int k,l;
      bool activate;
      char btn_name[MAX_PATH];
      char btn_repeats[MAX_PATH];
      stPluginData plgdata;
      stWircKey    wirckey;
      WinLIRCgetstr(sWinLIRCstring,2,btn_repeats);
      WinLIRCgetstr(sWinLIRCstring,3,btn_name);
      for (l=0; l<Plugins.GetPluginsCount(); l++)
       {
        Plugins.GetPluginData(l,&plgdata);
        for (k=0; k<Plugins.PluginsKeys[l].WircKeys.GetKeysCount(); k++)
         {
          Plugins.PluginsKeys[l].WircKeys.GetKey(k,&wirckey);
          // ���� ������ ���� �������, �� ��������� �� ���������� ��������
          if (lstrcmp(wirckey.WircKey,btn_name)==0)
           {
            activate = false;
            if (wirckey.Repeats>=0)
             {
              char wirckeyrepeats[10];
              char str[10];

              wirckeyrepeats[0]='\0';
              wsprintf(str,"%i%",wirckey.Repeats);
              if (lstrlen(str)<2) lstrcat(wirckeyrepeats,"0");
              lstrcat(wirckeyrepeats,str);

              if (lstrcmp(wirckeyrepeats,btn_repeats)==0) activate = true;
             }
            else activate = true;
            if (activate) plgdata.Command(wirckey.CmdID);
           }
         }
       }
     }
    break;

   case WM_MAINMENU: // �������� ������� ����
    // ���� ���� ��� ����������, �� ��� ������������
    if (IsWindow(hwndMainMenu)) DestroyWindow(hwndMainMenu);
    // ���� ��� �� ����������, �� ��������
    else DialogBox(hInst,MAKEINTRESOURCE(IDD_MAINMENU),0,(DLGPROC)DlgMainMenuProc);
    break;

   case WM_TRAYICON: // ������� �� ��������� �� ������ � ��������� ����
    switch (lParam)
     {
	  // ������� �� ����� ������� ����
      case WM_LBUTTONDOWN:
       if (!IsWindow(hwndDlgSettings))
        if (Settings.MainMenuAccess.AccessFlags&AF_TRAY)
         SendMessage(hwndServerWindow,WM_MAINMENU,0,0);
       break;
	  // ������� �� ������ ������� ����
      case WM_RBUTTONDOWN:
       if (!IsWindow(hwndDlgAbout)) DialogBox(hInst,MAKEINTRESOURCE(IDD_ABOUT),0,(DLGPROC)DlgAboutProc);
       else                         
        {
         if (IsWindow(hwndMainMenu)) DestroyWindow(hwndMainMenu);
         DestroyWindow(hwndDlgAbout);
        }
       break;
     }
   break;

   case WM_QUERYENDSESSION: // ������� �� ���������� ������ � Windows
    DestroyWindow(hwndWnd);
   break;

   case WM_CLOSE: // ������� �� �������� ����
    DestroyWindow(hwndWnd);
   break;

   case WM_DESTROY: // ������� �� ����������� ����
    if (Settings.LanguageStrings!=NULL) delete Settings.LanguageStrings; // ������������ ������, ���� ��� ��������� ����
    CTray systemtray;
    if (Settings.ShowInTray) systemtray.DeleteTrayIcon(hwndServerWindow,1);
    MonitoringEnd();    // ���������� ������������ ����������
    SettingsSave();     // ���������� �������� ���������
    PostQuitMessage(0); // ����� �� ���������
   break;

   // ��������� �������������� �� ���������
   default: 
    res = DefWindowProc(hwndWnd,message,wParam,lParam); 
  }
 return res;
}

// �������, ������������� � WinLIRC �������� � ����������� ��� � �����
bool WinLIRCconnect(LPTSTR address, int port, HWND hwndWnd)
{
 int result;
 SOCKADDR_IN addr; // �������� ������ (��� �������� ��� ����������)

 WSADATA stWSAData; // ��������� ������ � �������� - �������������
 WSAStartup(0x0101, &stWSAData);

 hWinLIRCsocket = socket(AF_INET, SOCK_STREAM, 0); // ����������� ������
 // ���������� ��������� ������
 addr.sin_family      = AF_INET;            // ��� ����������
 addr.sin_addr.s_addr = inet_addr(address); // �����
 addr.sin_port        = htons(port);        // ����

 // ������� �����������...
 result = connect(hWinLIRCsocket,(struct sockaddr *)&addr,sizeof(addr));
 // ���� ����������� �� �������, �� false
 if (result) 
  {
   hWinLIRCsocket = INVALID_SOCKET;
   return false;
  }
 // ��� ����� ��������� ����������� � ����� (��� ������)
 WSAAsyncSelect(hWinLIRCsocket,hwndWnd,WM_WIRCKEYS,FD_READ);   
 return true;
}

// ���������� �� ������� WinLIRC
void WinLIRCdisconnect()
{
 if (hWinLIRCsocket!=INVALID_SOCKET)
  {
   WSACleanup();
   hWinLIRCsocket = INVALID_SOCKET;
  }
}

// ���������� ���� ��� ������� num �� ������ WinLIRC
bool WinLIRCgetstr(LPTSTR winlircstring, int num, LPTSTR buffer)
{
 int i,
     k = 1, l = 0; // ��������� ��������

 for (i=0; i<lstrlen(winlircstring); i++)
  {
   // ���� ������ ���� ����������
    if (k==num)
    {
     buffer[i-l]   = winlircstring[i];
     buffer[i-l+1] = '\0';
    }
   // ���� ������ ���� ���� ��������, �� ���������� ��������
   else if (k>num) return true;
   // ���� ������ ������� �� ������ ����, �� ���������� ���������� �������
   if (winlircstring[i+1]==' ') { k++; l=i+2; }
  }
 buffer[0]='\0';
 return false;
}
