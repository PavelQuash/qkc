/**************************************************************/
/*              qshKeyCommander1 - qkc_main.h                 */
/*------------------------------------------------------------*/
/*       ������������ ���� ��� ������� ����� ���������        */
/*****2004***************************Copyright by <Quash!>*****/

#ifndef __QKC_MAIN
#define __QKC_MAIN

// *����������� ��������
#define program_version "v1.0 beta 10.03 (26.03.2005�.)" // ������ ���������
#define program_www     "none"                           // �������� ��������� ���������
#define program_email   "mailto:qshDeveloper@mail.ru"    // ����� � �������

#define serverclassname "qshKeyCommander_1" // ��� ������ ����-������� (�.�. ����������� � qshKeyCommanderHooks.dll)
#define MAX_PASSWORDSYMBOLS 25              // ������������ ����� �������� � ������ (�.�. ����������� � qshKeyCommanderHooks.dll)

#define prg_help        "Help"                     // ������� ������
#define prg_languages   "Languages"                // ������� ������
#define prg_plugins     "Plugins"                  // ������� ��������

#define hooks_filename  "qshKeyCommanderHooks.dll" // ���� ������������ ���������
#define ini_filename    "qshKeyCommander.ini"      // ��� ����� �������������
#define ini_mainsection "qshKeyCommander_1"        // �������� ������ �������� ������ ���������

#define lng_default ":Russian:" // ���� �� ��������� (������ ������������ � ������ ������ ������)

#define WM_MAINMENU   WM_USER+01 // ��������� ��� ������/������� �������� ����
#define WM_TRAYICON   WM_USER+02 // ��������� �������������� �������� �� ����

#define WM_KEYBOARDHOTKEYS WM_USER+10 // ��������� ��� ������������ ������ ���������� (HotKeys)
#define WM_KEYBOARDHOOKS   WM_USER+11 // ��������� ��� ������������ ������ ���������� (Hooks)
#define WM_MOUSE           WM_USER+12 // ��������� ��� ������������ ��������� �� ����
#define WM_GESTURES        WM_USER+13 // ��������� ��� ������������ ������ ����
#define WM_WIRCKEYS        WM_USER+14 // ��������� ��� ������� ������ WinLIRC
#define WM_TIMEEVENTS      WM_USER+15 // ��������� ��� ����������� ����������� ��������� �������

// ����� ������� � �������� ���� (4 �������)
#define AF_TRAY     1
#define AF_HOTKEY   2
#define AF_PASSWORD 4
#define AF_DBLRUN   8

// ��������� �������� ������� � ����
struct stMainMenuAccess
{
 BYTE AccessFlags;                   // ����� �������
 WORD HotKey;                        // ���������� ������
 char Password[MAX_PASSWORDSYMBOLS]; // ������ (��� ������)
};

// ��������� ������ �����
struct stLanguage
{
 // ***������
 char DATA_Author[20];

 // ***�������
 // ������ - ������� ����
 char MAINMENU_Window_Caption[20];
 char MAINMENU_Button_Settings[20];
 char MAINMENU_Button_Help[20];
 char MAINMENU_Button_About[20];
 char MAINMENU_Button_Hide[20];
 char MAINMENU_Button_Quit[20];

 // ������ - �������
 char HELP_Window_Caption[20];
 char HELP_Button_Close[20];
 char HELP_Button_Open[20];

 // ������ - � ���������...
 char ABOUT_Window_Caption[20];
 char ABOUT_Text_Program[20];
 char ABOUT_Text_Version[20];
 char ABOUT_Text_Status[20];
 char ABOUT_Status[20];
 char ABOUT_Text_Www[20];
 char ABOUT_Text_Email[20];
 char ABOUT_Text_LngAuthor_Caption[20];
 char ABOUT_Button_Close[20];

 // ������ - ���������
 char SETTINGS_Window_Caption[20];
 char SETTINGS_Text_Plugins_Caption[20];
 char SETTINGS_Button_Keyboard[20];
 char SETTINGS_Button_Winlirc[20];
 char SETTINGS_Button_Pluginslist[20];
 char SETTINGS_Text_Configure_Caption[20];
 char SETTINGS_Button_Configure[20];
 char SETTINGS_Button_Close[20];
 char SETTINGS_Button_Help[20];
 char SETTINGS_Text_Help[20];

 // ������ - ��������� ������
 char SETTINGS_KBRD_Window_Caption[20];
 char SETTINGS_KBRD_Button_KbrdOn[20];
 char SETTINGS_KBRD_Text_ActivePlugins[20];
 char SETTINGS_KBRD_Text_Commands[20];
 char SETTINGS_KBRD_List_Command[20];
 char SETTINGS_KBRD_List_HotKey[20];
 char SETTINGS_KBRD_List_CNS[20];
 char SETTINGS_KBRD_Button_ChangeKey[20];
 char SETTINGS_KBRD_Button_Clear[20];
 char SETTINGS_KBRD_Button_Configure[20];

 // ������ - ��������� WinLIRC
 char SETTINGS_WIRC_Window_Caption[20];
 char SETTINGS_WIRC_Button_WircOn[20];
 char SETTINGS_WIRC_Text_ActivePlugins[20];
 char SETTINGS_WIRC_Text_Commands[20];
 char SETTINGS_WIRC_List_Command[20];
 char SETTINGS_WIRC_List_Button[20];
 char SETTINGS_WIRC_List_Repeats[20];
 char SETTINGS_WIRC_Text_Server[20];
 char SETTINGS_WIRC_Text_Port[20];
 char SETTINGS_WIRC_Button_Connect[20];
 char SETTINGS_WIRC_Button_Disconnect[20];
 char SETTINGS_WIRC_Text_Connected[20];
 char SETTINGS_WIRC_Text_Disconnected[20];
 char SETTINGS_WIRC_Text_ConnectFailed[20];
 char SETTINGS_WIRC_Button_ChangeKey[20];
 char SETTINGS_WIRC_Button_Clear[20];
 char SETTINGS_WIRC_Button_Configure[20];

 // ������ - �����������
 char SETTINGS_PLUGLIST_Window_Caption[20];
 char SETTINGS_PLUGLIST_Text_PluginsPath[20];
 char SETTINGS_PLUGLIST_Text_FindPlugins[20];
 char SETTINGS_PLUGLIST_Text_ActvPlugins[20];
 char SETTINGS_PLUGLIST_Button_AddOne[20];
 char SETTINGS_PLUGLIST_Button_DelOne[20];
 char SETTINGS_PLUGLIST_Button_AddAll[20];
 char SETTINGS_PLUGLIST_Button_DelAll[20];
 char SETTINGS_PLUGLIST_Button_Accept[20];
 char SETTINGS_PLUGLIST_Button_Update[20];


 // ������ - ������������
 char SETTINGS_CONFIGURE_Window_Caption[20];
 char SETTINGS_CONFIGURE_Button_UseWinHooks[20];
 char SETTINGS_CONFIGURE_Button_RegisterServiceProcess[20];
 char SETTINGS_CONFIGURE_Button_ChangeKeyboardState[20];
 char SETTINGS_CONFIGURE_Button_ShowInTray[20];
 char SETTINGS_CONFIGURE_Button_Analogs[20];
 char SETTINGS_CONFIGURE_Text_InstallGroup[20];
 char SETTINGS_CONFIGURE_Button_Install[20];
 char SETTINGS_CONFIGURE_Button_Uninstall[20];
 char SETTINGS_CONFIGURE_Text_LanguageGroup[20];
 char SETTINGS_CONFIGURE_Text_AccessGroup[20];
 char SETTINGS_CONFIGURE_Access_Button_Tray[20];
 char SETTINGS_CONFIGURE_Access_Button_HotKey[20];
 char SETTINGS_CONFIGURE_Access_Button_ChangeHotKey[20];
 char SETTINGS_CONFIGURE_Access_Button_Password[20];
 char SETTINGS_CONFIGURE_Access_Text_Password[20];
 char SETTINGS_CONFIGURE_Access_Text_Confirm[20];
 char SETTINGS_CONFIGURE_Access_Button_DblRun[20];

 // ***���������
};

// ����������� ���������
struct stSettings
{
 stMainMenuAccess MainMenuAccess;
 BOOL KbrdOn;                 // ���������� �� ������������ ������ ����������
 BOOL MouseOn;                // ���������� �� ������������ ��������� ����
 BOOL GesturesOn;             // ���������� �� ������������ ������ ����
 BOOL WircOn;                 // ���������� �� ������������ ������ WinLIRC
 BOOL TimeEventsOn;           // ���������� �� ������������ ��������� ���������
 BOOL UseWinHooks;            // ������������ ��� ������������ �������
 BOOL RegisterServiceProcess; // ������� ������� �� Task Manager'�
 BOOL ChangeKeyboardState;    // �������� ��������� ����������
 BOOL ShowInTray;             // �������� � ����
 BOOL ClearOnAnalog;          // ����������, ���� ���� ����������

 char        Language[MAX_PATH]; // ��� ����� ����� ����������
 stLanguage *LanguageStrings;    // ������ ��� �������� ����������

 int  xMainMenuWindow;        // x ��������� �������� ����
 int  yMainMenuWindow;        // y ��������� �������� ����

 char WinLIRCaddress[15];     // ����� ������� WinLIRC
 int  WinLIRCport;            // ���� ������� WinLIRC
};

// *�������� ���������� �������
void MonitoringBegin();
void MonitoringEnd();
bool WinLIRCconnect(LPTSTR address, int port, HWND hwndWnd);
void WinLIRCdisconnect();
bool WinLIRCgetstr(LPTSTR winlircstring, int num, LPTSTR buffer);

#endif