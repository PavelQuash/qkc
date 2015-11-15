/**************************************************************/
/*             qshKeyCommander1 - Settings.cpp                */
/*------------------------------------------------------------*/
/*  ��������/���������� �������� ���������, �������� ������,  */
/*  ���������� ��������� ��������� � ����������� �� ��������  */
/*****2004***************************Copyright by <Quash!>*****/

#include "Settings.h"

extern stSettings Settings;       // ������� � qkc_main.cpp
extern char prgdir[MAX_PATH];     // qkc_main.cpp
extern char lngdir[MAX_PATH];     // qkc_main.cpp
char   currentlanguage[MAX_PATH]; // ��� ����������� ���� (������)

// ��������� ����������� ���������
bool SettingsLoad()
{
 // ������ ���� � ����� �������������
 char inifn[MAX_PATH]; 
 wsprintf(inifn,"%s\\%s",prgdir,ini_filename);
 // �������� ��������
 Settings.xMainMenuWindow = GetPrivateProfileInt(ini_mainsection,"xMainMenuWindow",10,inifn);
 Settings.yMainMenuWindow = GetPrivateProfileInt(ini_mainsection,"yMainMenuWindow",10,inifn);
 
 Settings.KbrdOn = GetPrivateProfileInt(ini_mainsection,"KbrdOn",0,inifn);
 Settings.WircOn = GetPrivateProfileInt(ini_mainsection,"WircOn",0,inifn);

 Settings.UseWinHooks            = GetPrivateProfileInt(ini_mainsection,"UseWinHooks",0,inifn);
 Settings.RegisterServiceProcess = GetPrivateProfileInt(ini_mainsection,"RegisterServiceProcess",0,inifn);
 Settings.ChangeKeyboardState    = GetPrivateProfileInt(ini_mainsection,"ChangeKeyboardState",0,inifn);
 Settings.ShowInTray             = GetPrivateProfileInt(ini_mainsection,"ShowInTray",1,inifn);
 Settings.ClearOnAnalog          = GetPrivateProfileInt(ini_mainsection,"ClearOnAnalog",1,inifn);

 GetPrivateProfileString(ini_mainsection,"Language",":Russian:",Settings.Language,sizeof(Settings.Language),inifn);
 // ���� ���� �� �� ���������, �� ��������� �� �������������
 if (!(lstrcmp(Settings.Language,lng_default)==0))
  {
   char lngpath[MAX_PATH];
   wsprintf(lngpath,"%s\\%s",lngdir,Settings.Language);
   // ���� ���� �� ����������, �� �������� ���� �� ���������, ����� ������ �� ������
   if (!FileExists(0,lngpath)) wsprintf(Settings.Language,"%s",lng_default);
  }
 GetPrivateProfileString(ini_mainsection,"WinLIRCaddress","127.0.0.1",Settings.WinLIRCaddress,sizeof(Settings.WinLIRCaddress),inifn);
 Settings.WinLIRCport = GetPrivateProfileInt(ini_mainsection,"WinLIRCport",8765,inifn);
 GetPrivateProfileStruct(ini_mainsection,"MainMenuAccess",&Settings.MainMenuAccess,sizeof(stMainMenuAccess),inifn);
 // �������� �� ������������ MainMenuAccess, ���� ��� �� �� ���, �� ������������ false
 // debug
return true;
}

// ��������� ����������� ���������
void SettingsSave()
{
 char str[10];         // ������ ��� ��������� ������
 // ������ ���� � ����� �������������
 char inifn[MAX_PATH]; 
 wsprintf(inifn,"%s\\%s",prgdir,ini_filename);
 // ���������� ��������
 wsprintf(str,"%i",Settings.xMainMenuWindow);
 WritePrivateProfileString(ini_mainsection,"xMainMenuWindow",str,inifn);
 wsprintf(str,"%i",Settings.yMainMenuWindow);
 WritePrivateProfileString(ini_mainsection,"yMainMenuWindow",str,inifn);
 
 wsprintf(str,"%i",Settings.KbrdOn);
 WritePrivateProfileString(ini_mainsection,"KbrdOn",str,inifn);
 wsprintf(str,"%i",Settings.WircOn);
 WritePrivateProfileString(ini_mainsection,"WircOn",str,inifn);

 wsprintf(str,"%i",Settings.UseWinHooks);
 WritePrivateProfileString(ini_mainsection,"UseWinHooks",str,inifn);
 wsprintf(str,"%i",Settings.RegisterServiceProcess);
 WritePrivateProfileString(ini_mainsection,"RegisterServiceProcess",str,inifn);
 wsprintf(str,"%i",Settings.ChangeKeyboardState);
 WritePrivateProfileString(ini_mainsection,"ChangeKeyboardState",str,inifn);
 wsprintf(str,"%i",Settings.ShowInTray);
 WritePrivateProfileString(ini_mainsection,"ShowInTray",str,inifn);
 wsprintf(str,"%i",Settings.ClearOnAnalog);
 WritePrivateProfileString(ini_mainsection,"ClearOnAnalog",str,inifn);
 WritePrivateProfileString(ini_mainsection,"Language",Settings.Language,inifn);
 WritePrivateProfileString(ini_mainsection,"WinLIRCaddress",Settings.WinLIRCaddress,inifn);
 wsprintf(str,"%i",Settings.WinLIRCport);
 WritePrivateProfileString(ini_mainsection,"WinLIRCport",str,inifn);
 WritePrivateProfileStruct(ini_mainsection,"MainMenuAccess",&Settings.MainMenuAccess,sizeof(stMainMenuAccess),inifn);
}

// �������� ������ ����� (���� �� ����������)
// true - ��� ����������; false - ������ �����
bool SettingsLanguageLoad(LPTSTR newlng)
{
 // ���� ������ ��� ��, ��� � � ������� ���, �� ����� �� �������
 if (lstrcmp(currentlanguage,newlng)==0) return true;

 if (lstrcmp(newlng,lng_default)==0) 
  {
   // ������������ ������, ���� ���� �����-�� ������
   if (Settings.LanguageStrings!=NULL)
    {
     delete Settings.LanguageStrings;
     Settings.LanguageStrings = NULL;
    }
  }
 else
  {
   // ��������� ������ ��� �������
   if (Settings.LanguageStrings!=NULL) delete Settings.LanguageStrings;
   Settings.LanguageStrings = new stLanguage;

   // �������� �������� �����
   lstrcpy(Settings.LanguageStrings->DATA_Author,"� <Quash!>");

   lstrcpy(Settings.LanguageStrings->MAINMENU_Window_Caption ,"������� ���� qKC");
   lstrcpy(Settings.LanguageStrings->MAINMENU_Button_Settings,"��������� ���������");
   lstrcpy(Settings.LanguageStrings->MAINMENU_Button_Help    ,"������");
   lstrcpy(Settings.LanguageStrings->MAINMENU_Button_About   ,"������");
   lstrcpy(Settings.LanguageStrings->MAINMENU_Button_Hide    ,"��������");
   lstrcpy(Settings.LanguageStrings->MAINMENU_Button_Quit    ,"��������� ������");

   lstrcpy(Settings.LanguageStrings->HELP_Window_Caption,"��������� ����� ������:");
   lstrcpy(Settings.LanguageStrings->HELP_Button_Close  ,"������� ������");
   lstrcpy(Settings.LanguageStrings->HELP_Button_Open   ,"������� ����");

   lstrcpy(Settings.LanguageStrings->ABOUT_Window_Caption        ,"������ ���������");
   lstrcpy(Settings.LanguageStrings->ABOUT_Text_Program          ,"�����:");
   lstrcpy(Settings.LanguageStrings->ABOUT_Text_Version          ,"�������:");
   lstrcpy(Settings.LanguageStrings->ABOUT_Text_Status           ,"�����-���:");
   lstrcpy(Settings.LanguageStrings->ABOUT_Status                ,"����������");
   lstrcpy(Settings.LanguageStrings->ABOUT_Text_Www              ,"���:");
   lstrcpy(Settings.LanguageStrings->ABOUT_Text_Email            ,"����:");
   lstrcpy(Settings.LanguageStrings->ABOUT_Text_LngAuthor_Caption,"�����:");
   lstrcpy(Settings.LanguageStrings->ABOUT_Button_Close          ,"Okay");

   lstrcpy(Settings.LanguageStrings->SETTINGS_Window_Caption        ,"���������������� ���������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_Text_Plugins_Caption  ,"�������:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_Button_Keyboard       ,"�����");
   lstrcpy(Settings.LanguageStrings->SETTINGS_Button_Winlirc        ,"�����");
   lstrcpy(Settings.LanguageStrings->SETTINGS_Button_Pluginslist    ,"����������/���������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_Text_Configure_Caption,"���������:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_Button_Configure      ,"���������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_Button_Close          ,"�������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_Button_Help           ,"�������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_Text_Help             ,"YO!");

   lstrcpy(Settings.LanguageStrings->SETTINGS_KBRD_Window_Caption     ,"��������� ����������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_KBRD_Button_KbrdOn      ,"�������� ������������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_KBRD_Text_ActivePlugins ,"��������:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_KBRD_Text_Commands      ,"��������:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_KBRD_List_Command       ,"��� ������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_KBRD_List_HotKey        ,"������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_KBRD_List_CNS           ,"���");
   lstrcpy(Settings.LanguageStrings->SETTINGS_KBRD_Button_ChangeKey   ,"����������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_KBRD_Button_Clear       ,"�������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_KBRD_Button_Configure   ,"���������");
/*
   lstrcpy(Settings.LanguageStrings->SETTINGS_MOUSE_Window_Caption ,"��������� ��������� �� ����:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_
   lstrcpy(Settings.LanguageStrings->SETTINGS_
   lstrcpy(Settings.LanguageStrings->SETTINGS_
   lstrcpy(Settings.LanguageStrings->SETTINGS_

   lstrcpy(Settings.LanguageStrings->SETTINGS_GESTURES_Window_Caption ,"��������� ������ ����:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_
   lstrcpy(Settings.LanguageStrings->SETTINGS_
   lstrcpy(Settings.LanguageStrings->SETTINGS_
*/
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Window_Caption     ,"��������� ������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Button_WircOn      ,"��������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Text_ActivePlugins ,"��������:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Text_Commands      ,"�������:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_List_Command       ,"��� ������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_List_Button        ,"�����");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_List_Repeats       ,"#");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Text_Server        ,"������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Text_Port          ,"����");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Button_Connect     ,"���������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Button_Disconnect  ,"�����������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Text_Connected     ,"��� �� - con");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Text_Disconnected  ,"��� �� - discon");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Text_ConnectFailed ,"���� �� ����������!");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Button_ChangeKey   ,"����������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Button_Clear       ,"�������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Button_Configure   ,"���������");

   lstrcpy(Settings.LanguageStrings->SETTINGS_PLUGLIST_Window_Caption   ,"����������� �������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_PLUGLIST_Text_PluginsPath ,"����:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_PLUGLIST_Text_FindPlugins ,"���������:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_PLUGLIST_Text_ActvPlugins ,"��������:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_PLUGLIST_Button_AddOne    ,"�������� 1");
   lstrcpy(Settings.LanguageStrings->SETTINGS_PLUGLIST_Button_DelOne    ,"������� 1");
   lstrcpy(Settings.LanguageStrings->SETTINGS_PLUGLIST_Button_AddAll    ,"���������� ���");
   lstrcpy(Settings.LanguageStrings->SETTINGS_PLUGLIST_Button_DelAll    ,"������� ���");
   lstrcpy(Settings.LanguageStrings->SETTINGS_PLUGLIST_Button_Accept    ,"Accept");
   lstrcpy(Settings.LanguageStrings->SETTINGS_PLUGLIST_Button_Update    ,"Update");

   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Window_Caption                ,"���������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Button_UseWinHooks            ,"������������ ����");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Button_RegisterServiceProcess ,"�������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Button_ChangeKeyboardState    ,"�������� ���������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Button_ShowInTray             ,"� ����");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Button_Analogs                ,"������� �������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Text_InstallGroup             ,"������������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Button_Install                ,"����������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Button_Uninstall              ,"������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Text_LanguageGroup            ,"���������:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Text_AccessGroup              ,"������:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Access_Button_Tray            ,"�� ����");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Access_Button_HotKey          ,"������� �������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Access_Button_ChangeHotKey    ,"��������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Access_Button_Password        ,"�� ������");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Access_Text_Password          ,"���:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Access_Text_Confirm           ,"����� ���:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Access_Button_DblRun          ,"�� ������� �������");
  }
 lstrcpy(currentlanguage,newlng); // ��������� �������� �����
 return false;
}

// ������� ��������� � ����������� �� ��������
void SettingsUpdate()
{
 // ���������� �������� ������ �����, ���� ��� ����������, �� ������������
 SettingsLanguageLoad(Settings.Language);
}
