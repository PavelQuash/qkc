/**************************************************************/
/*           qshKeyCommander1 - DlgSettings.cpp               */
/*------------------------------------------------------------*/
/* ������ � �������� �������� ��������� � ����� ��������������*/
/*                      ��� ���������                         */
/*****2004***************************Copyright by <Quash!>*****/

#include "DlgSettings.h"

#define plg_extension "qkc" // ���������� ������������ �������
#define lng_extension "lng" // ���������� ������ ������

// ������� ����������
extern char plgdir[MAX_PATH];         // ������� � qkc_main.cpp
extern CPlugins Plugins;              // ������� ��� ��
extern stSettings Settings;           // ������� ��� ��
extern HINSTANCE hInst;               // ������� ��� ��
extern char lngdir[MAX_PATH];         // ������� ��� ��
extern SOCKET hWinLIRCsocket;         // ������� ��� ��
extern char sWinLIRCstring[MAX_PATH]; // ������� ��� ��
extern HWND hwndServerWindow;         // ������� ��� ��

// HWND ���� ���������� ����
HWND hwndDlgSettings  = NULL,
     hwndDlgKbrd      = NULL,
     hwndDlgWirc      = NULL,
	 hwndDlgMouse     = NULL,
	 hwndDlgGestures  = NULL,
     hwndDlgPlugList  = NULL,
     hwndDlgConfigure = NULL;

// ���������� ������ ������� PlugList
void DlgSettings_PlugList_Update(HWND hwndDlg)
{
 // ���������� ������� ������� ��������
 SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_PLUGINSPATH,WM_SETTEXT,0,(LPARAM)(LPCTSTR)plgdir);
 // ��������� ���� ��������� ��������
 WIN32_FIND_DATA finddata;
 HANDLE          hfile;
 char srchfilename[MAX_PATH];
 wsprintf(srchfilename,"%s\\*.%s",plgdir,plg_extension);
 SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_FINDPLUGINS,LB_RESETCONTENT,0,0); // ������ �����
 hfile = FindFirstFile(srchfilename,&finddata);
 if (!(hfile==INVALID_HANDLE_VALUE)) // ���� ���-�� �������, �� �������, ��� �������
  if (!(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) // ���� ���� ������� �� ����������
   {
    SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_FINDPLUGINS,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)finddata.cFileName);
    while (FindNextFile(hfile,&finddata))
     if (!(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) // ���� ���� ������� �� ����������   }
      SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_FINDPLUGINS,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)finddata.cFileName);
   }
 // ��������� ���� ������������ ��������
 stPluginData plugindata;
 SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_ACTVPLUGINS,LB_RESETCONTENT,0,0); // ������ �����
 for (int i=0; i<Plugins.GetPluginsCount(); i++)
  {
   Plugins.GetPluginData(i,&plugindata);
   SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_ACTVPLUGINS,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)plugindata.FileName);
  }
}

// �������, ��������� �������
void SetDialog(int id)
{
 HWND hwndTemp = NULL;

 // �������� ��� ������� � ���� ���������
 ShowWindow(hwndDlgKbrd,     SW_HIDE);
 ShowWindow(hwndDlgWirc,     SW_HIDE);
 ShowWindow(hwndDlgMouse,    SW_HIDE);
 ShowWindow(hwndDlgGestures, SW_HIDE);
 ShowWindow(hwndDlgPlugList, SW_HIDE);
 ShowWindow(hwndDlgConfigure,SW_HIDE);

 // ��������� ����� ������ �����
 switch (id)
  {
   case IDC_SETTINGS_KBRD:      hwndTemp = hwndDlgKbrd;      break;
   case IDC_SETTINGS_WIRC:      hwndTemp = hwndDlgWirc;      break;
   case IDC_SETTINGS_MOUSE:     hwndTemp = hwndDlgMouse;     break;
   case IDC_SETTINGS_GESTURES:  hwndTemp = hwndDlgGestures;  break;
   case IDC_SETTINGS_PLUGLIST:  hwndTemp = hwndDlgPlugList; DlgSettings_PlugList_Update(hwndDlgPlugList); break;
   case IDC_SETTINGS_CONFIGURE: hwndTemp = hwndDlgConfigure; break;
  }

 // ���������� ������
 if (hwndTemp!=NULL) 
  {
   ShowWindow(hwndTemp,SW_SHOW);
   SetFocus(hwndTemp);
  }
}

// ���������� ������ ��� ���������� ������
void DlgSettings_Kbrd_PluginCommands_Keys_Update(HWND hwndDlg)
{
 int          i,k,l;
 char         buffer [MAX_PATH];
 char         buffer2[MAX_PATH];
 LV_ITEM      item;
 stPluginData plugindata;
 stKbrdKey    kbrdkey;

  // ����������� ����� �����, ���������� � ������
 l = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_ACTIVEPLUGINS,LB_GETCURSEL,0,0);
 SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_ACTIVEPLUGINS,LB_GETTEXT,(WPARAM)l,(LPARAM)(LPCTSTR)buffer);
 l = Plugins.FindByFileName(buffer); // ����������� ������ ������� � ������
 Plugins.GetPluginData(l,&plugindata); // ������ � �������

 // ��������� ������ ������ � ��������
 item.mask = LVIF_TEXT;
 for (i=0; i<*plugindata.CommandsCount; i++)
  {
   // ������ �������, ���� �� �������, �� ����������� ������ �����
   k = Plugins.PluginsKeys[l].KbrdKeys.FindByCmdID(i);
   buffer [0] = '\0';
   buffer2[0] = '\0';
   if (k!=-1) 
    {
     Plugins.PluginsKeys[l].KbrdKeys.GetKey(k,&kbrdkey);
     KeyToStr(kbrdkey.KbrdKey,buffer);
     lstrcat(buffer2,"[");
     if (LOBYTE(kbrdkey.CNS)&CNS_CAPSLOCK) 
      {
       lstrcat(buffer2,"c");
       if (HIBYTE(kbrdkey.CNS)&CNS_CAPSLOCK) lstrcat(buffer2,"+");
       else                                  lstrcat(buffer2,"-");
      }
     if (LOBYTE(kbrdkey.CNS)&CNS_NUMLOCK) 
      {
       lstrcat(buffer2,"n");
       if (HIBYTE(kbrdkey.CNS)&CNS_NUMLOCK) lstrcat(buffer2,"+");
       else                                 lstrcat(buffer2,"-");
      }
     if (LOBYTE(kbrdkey.CNS)&CNS_SCROLLLOCK) 
      {
       lstrcat(buffer2,"s");
       if (HIBYTE(kbrdkey.CNS)&CNS_SCROLLLOCK) lstrcat(buffer2,"+");
       else                                    lstrcat(buffer2,"-");
      }
     lstrcat(buffer2,"]");
    }
   // ���������� �������
   item.iItem    = i;
   item.iSubItem = 1;
   item.pszText  = buffer;
   SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_PLUGINCOMMANDS,LVM_SETITEM,0,(LPARAM)&item);
   // ���������� CNS
   item.iItem    = i;
   item.iSubItem = 2;
   item.pszText  = buffer2;
   SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_PLUGINCOMMANDS,LVM_SETITEM,0,(LPARAM)&item);
  }
}

// ���������� ������ ��� ���������� ������
void DlgSettings_Wirc_PluginCommands_Keys_Update(HWND hwndDlg)
{
 // ���� ��� ������ ������� � ������, �� ��� ���� ��������� ������ ������
 int          i,k,l;
 char         buffer [MAX_PATH];
 char         buffer2[MAX_PATH];
 LV_ITEM      item;
 stPluginData plugindata;
 stWircKey    wirckey;

 // ����������� ����� �����, ���������� � ������
 l = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_ACTIVEPLUGINS,LB_GETCURSEL,0,0);
 SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_ACTIVEPLUGINS,LB_GETTEXT,(WPARAM)l,(LPARAM)(LPCTSTR)buffer);
 l = Plugins.FindByFileName(buffer); // ����������� ������ ������� � ������ ������
 Plugins.GetPluginData(l,&plugindata); // ��������� ������ � �������

 // ��������� ������ ������
 item.mask = LVIF_TEXT;
 for (i=0; i<*plugindata.CommandsCount; i++)
  {
   // ������ �������, ���� �� �������, �� ����������� ������ �����
   k = Plugins.PluginsKeys[l].WircKeys.FindByCmdID(i);
   if (k==-1) 
    {
     buffer [0] ='\0';
     buffer2[0]='\0';
    }
   else 
    {
     Plugins.PluginsKeys[l].WircKeys.GetKey(k,&wirckey);
     wsprintf(buffer, "%s",wirckey.WircKey);
     wsprintf(buffer2,"%i",wirckey.Repeats);
    }

   //  ���������� ������
   item.iItem    = i;
   item.iSubItem = 1;
   item.pszText  = buffer;
   SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_PLUGINCOMMANDS,LVM_SETITEM,0,(LPARAM)&item);
   //  ���������� ���������� ��������
   item.iItem    = i;
   item.iSubItem = 2;
   item.pszText  = buffer2;
   SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_PLUGINCOMMANDS,LVM_SETITEM,0,(LPARAM)&item);
  }
}

// ���������� ������ ������ ��� ���������� ������
void DlgSettings_Kbrd_PluginCommands_List_Update(HWND hwndDlg)
{
 // ���� ��� ������ ������� � ������, �� ��� ���� ��������� ������ ������
 int          i,l;
 char         buffer[MAX_PATH];
 LV_ITEM      item;
 stPluginData plugindata;

 // ����������� ����� �����, ���������� � ������
 l = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_ACTIVEPLUGINS,LB_GETCURSEL,0,0);
 SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_ACTIVEPLUGINS,LB_GETTEXT,(WPARAM)l,(LPARAM)(LPCTSTR)buffer);
 l = Plugins.FindByFileName(buffer); // ����������� ������ ������� � ������
 Plugins.GetPluginData(l,&plugindata); // ������ � �������
 // ���������� ������� � ������ ������������, ���� ������� ����������, ����� ������ �����������
 if (plugindata.Config==NULL) EnableWindow(GetDlgItem(hwndDlgKbrd,IDC_SETTINGS_KBRD_CONFIGURE),0);
 else                         EnableWindow(GetDlgItem(hwndDlgKbrd,IDC_SETTINGS_KBRD_CONFIGURE),1);
 // ��������� ������ ����������� ���� ������� �� �������
 EnableWindow(GetDlgItem(hwndDlgKbrd,IDC_SETTINGS_KBRD_CHANGEKEY),0);
 EnableWindow(GetDlgItem(hwndDlgKbrd,IDC_SETTINGS_KBRD_CLEAR),0);
 // ��������� ������ ������
 SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_PLUGINCOMMANDS,LVM_DELETEALLITEMS,0,0);
 item.iSubItem = 0;
 for (i=0; i<*plugindata.CommandsCount; i++)
  {
   // ���������� �������
   item.iItem    = i;
   if (plugindata.CommandsList[i][0]=='+')        // ������������ ������� ���������
    { 
     item.mask    = LVIF_TEXT|LVIF_STATE;
     item.state   = LVIS_DROPHILITED;
     item.pszText = plugindata.CommandsList[i]+1;
    }
   else if (plugindata.CommandsList[i][0]=='-')   // ������� ������� ���������
    {
     item.mask    = LVIF_TEXT;
     item.pszText = plugindata.CommandsList[i]+1;
    }
   else                                           // ��������� �������
    {
     item.mask    = LVIF_TEXT;
     item.pszText = plugindata.CommandsList[i];
    }
   SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_PLUGINCOMMANDS,LVM_INSERTITEM,0,(LPARAM)&item);
  }

 // ����� ������ ��� ������
 DlgSettings_Kbrd_PluginCommands_Keys_Update(hwndDlg);
}

// ���������� ������ ������ ��� ���������� ������
void DlgSettings_Wirc_PluginCommands_List_Update(HWND hwndDlg)
{
 // ���� ��� ������ ������� � ������, �� ��� ���� ��������� ������ ������
 int          i,l;
 char         buffer[MAX_PATH];
 LV_ITEM      item;
 stPluginData plugindata;

 // ����������� ����� �����, ���������� � ������
 l = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_ACTIVEPLUGINS,LB_GETCURSEL,0,0);
 SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_ACTIVEPLUGINS,LB_GETTEXT,(WPARAM)l,(LPARAM)(LPCTSTR)buffer);
 l = Plugins.FindByFileName(buffer); // ����������� ������ ������� � ������
 Plugins.GetPluginData(l,&plugindata); // ������ � �������
 // ���������� ������� � ������ ������������, ���� ������� ����������, ����� ������ �����������
 if (plugindata.Config==NULL) EnableWindow(GetDlgItem(hwndDlgKbrd,IDC_SETTINGS_WIRC_CONFIGURE),0);
 else                         EnableWindow(GetDlgItem(hwndDlgKbrd,IDC_SETTINGS_WIRC_CONFIGURE),1);
 // ��������� ������ ����������� ���� ������� �� �������
 EnableWindow(GetDlgItem(hwndDlgKbrd,IDC_SETTINGS_WIRC_CHANGEKEY),0);
 EnableWindow(GetDlgItem(hwndDlgKbrd,IDC_SETTINGS_WIRC_CLEAR),0);
 // ��������� ������ ������
 item.mask = LVIF_TEXT;
 SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_PLUGINCOMMANDS,LVM_DELETEALLITEMS,0,0);
 item.iSubItem = 0;
 for (i=0; i<*plugindata.CommandsCount; i++)
  {
   // ���������� �������
   item.iItem    = i;
   // ���������� �������
   item.iItem    = i;
   if (plugindata.CommandsList[i][0]=='+')        // ������������ ������� ���������
    { 
     item.mask    = LVIF_TEXT|LVIF_STATE;
     item.state   = LVIS_DROPHILITED;
     item.pszText = plugindata.CommandsList[i]+1;
    }
   else if (plugindata.CommandsList[i][0]=='-')   // ������� ������� ���������
    {
     item.mask    = LVIF_TEXT;
     item.pszText = plugindata.CommandsList[i]+1;
    }
   else                                           // ��������� �������
    {
     item.mask    = LVIF_TEXT;
     item.pszText = plugindata.CommandsList[i];
    }
   SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_PLUGINCOMMANDS,LVM_INSERTITEM,0,(LPARAM)&item);
  }

 // ����� ������ ��� ������
 DlgSettings_Wirc_PluginCommands_Keys_Update(hwndDlg);
}

// ���������� ������ ������������ ������� ��� ������� Kbrd
void DlgSettings_Kbrd_ActivePlugins_Update(HWND hwndDlg)
{
 // ������� ������ ����
 SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_PLUGINCOMMANDS,LVM_DELETEALLITEMS,0,0);
 // "�������" ������
 EnableWindow(GetDlgItem(hwndDlgKbrd,IDC_SETTINGS_KBRD_CHANGEKEY),0);
 EnableWindow(GetDlgItem(hwndDlgKbrd,IDC_SETTINGS_KBRD_CONFIGURE),0);
 EnableWindow(GetDlgItem(hwndDlgKbrd,IDC_SETTINGS_KBRD_CLEAR),0);

 stPluginData plugindata;
 SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_ACTIVEPLUGINS,LB_RESETCONTENT,0,0); // ������ �����
 for (int i=0; i<Plugins.GetPluginsCount(); i++)
  {
   Plugins.GetPluginData(i,&plugindata);
   SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_ACTIVEPLUGINS,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)plugindata.FileName);
  }
}

// ���������� ������ ������������ ������� ��� ������� Wirc
void DlgSettings_Wirc_ActivePlugins_Update(HWND hwndDlg)
{
 // ������� ������ ����
 SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_PLUGINCOMMANDS,LVM_DELETEALLITEMS,0,0);
 // "�������" ������
 EnableWindow(GetDlgItem(hwndDlgWirc,IDC_SETTINGS_WIRC_CHANGEKEY),0);
 EnableWindow(GetDlgItem(hwndDlgWirc,IDC_SETTINGS_WIRC_CONFIGURE),0);
 EnableWindow(GetDlgItem(hwndDlgWirc,IDC_SETTINGS_WIRC_CLEAR),0);

 stPluginData plugindata;
 SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_ACTIVEPLUGINS,LB_RESETCONTENT,0,0); // ������ �����
 for (int i=0; i<Plugins.GetPluginsCount(); i++)
  {
   Plugins.GetPluginData(i,&plugindata);
   SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_ACTIVEPLUGINS,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)plugindata.FileName);
  }
}

// ������� ��������� ���������, ����������� �������� ������� ��������
BOOL CALLBACK DlgSettingsProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
 HINSTANCE hInst;
 switch (message)
  {
   case WM_INITDIALOG: // �������� ����������� ��� ������ �������� 
    {
     // ***������� ����������***
     if (lstrcmp(Settings.Language,lng_default)!=0)
      {
       char buffer[MAX_PATH];
       wsprintf(buffer,"%s - %s",Settings.LanguageStrings->SETTINGS_Window_Caption,ini_filename);
       SendMessage(hwndDlg,WM_SETTEXT,0,(LPARAM)(LPCTSTR)buffer);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGINS_CAPTION,  WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_Text_Plugins_Caption);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD,             WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_Button_Keyboard);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC,             WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_Button_Winlirc);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST,         WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_Button_Pluginslist);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_CAPTION,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_Text_Configure_Caption);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE,        WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_Button_Configure);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CLOSE,            WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_Button_Close);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_HELP,             WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_Button_Help);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_HELP_CAPTION,     WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_Text_Help);
      }
     // ***
     hInst = GetModuleHandle(NULL);
     hwndDlgSettings  = hwndDlg;
     hwndDlgKbrd      = CreateDialog(hInst,MAKEINTRESOURCE(IDD_SETTINGS_KBRD),     hwndDlg,(DLGPROC)DlgSettingsProc_Kbrd);
     hwndDlgWirc      = CreateDialog(hInst,MAKEINTRESOURCE(IDD_SETTINGS_WIRC),     hwndDlg,(DLGPROC)DlgSettingsProc_Wirc);
     hwndDlgMouse     = CreateDialog(hInst,MAKEINTRESOURCE(IDD_SETTINGS_MOUSE),    hwndDlg,(DLGPROC)DlgSettingsProc_Mouse);
     hwndDlgGestures  = CreateDialog(hInst,MAKEINTRESOURCE(IDD_SETTINGS_GESTURES), hwndDlg,(DLGPROC)DlgSettingsProc_Gestures);
     hwndDlgPlugList  = CreateDialog(hInst,MAKEINTRESOURCE(IDD_SETTINGS_PLUGLIST), hwndDlg,(DLGPROC)DlgSettingsProc_PlugList);
     hwndDlgConfigure = CreateDialog(hInst,MAKEINTRESOURCE(IDD_SETTINGS_CONFIGURE),hwndDlg,(DLGPROC)DlgSettingsProc_Configure);
    }
    return true;

   case WM_COMMAND: // ��������� ������� ������ � ������� �������
    switch (LOWORD(wParam))
     {
      case IDC_SETTINGS_KBRD:
      case IDC_SETTINGS_WIRC:
	  case IDC_SETTINGS_MOUSE:
      case IDC_SETTINGS_GESTURES:
      case IDC_SETTINGS_PLUGLIST:
      case IDC_SETTINGS_CONFIGURE:
      case IDC_SETTINGS_HELP:
       SetDialog(LOWORD(wParam));
       break;
      case IDC_SETTINGS_CLOSE:     
       EndDialog(hwndDlg,0);
       break;
     }
    return true;

  case WM_CLOSE: // �������� �������
   EndDialog(hwndDlg,0);
			return true;
 }
 return false;
}

// ��������� ���������� ������
void DlgSettingsProc_Kbrd_ChangeKey(HWND hwndDlg)
{
 if (!IsWindowEnabled(GetDlgItem(hwndDlg,IDC_SETTINGS_KBRD_CHANGEKEY))) return;

 int i;
 // ����� ���������� ��������
 i = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_PLUGINCOMMANDS,LVM_GETSELECTIONMARK,0,0);
 // ���� ������� ������, �� ������ ���������� ������
 if (i>=0) 
  {
   ShowWindow(hwndDlgSettings,SW_HIDE);
   int k,l;
   char buffer[MAX_PATH];
   WORD hotkey = 0;
   WORD cns    = 0;
   CGetKbrdKey getkbrdkey;
   stKbrdKey kbrdkey;

   // ����������� ������ �������, � ������� ��� ������
   l = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_ACTIVEPLUGINS,LB_GETCURSEL,0,0);
   SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_ACTIVEPLUGINS,LB_GETTEXT,(WPARAM)l,(LPARAM)(LPCTSTR)buffer);
   l = Plugins.FindByFileName(buffer); // ����������� ������ ������� � ������ ������
   // ����������� ������ ������� � ������ ������ Kbrd
   k = Plugins.PluginsKeys[l].KbrdKeys.FindByCmdID(i);
   // ��������� ���������� ������
   getkbrdkey.GetHotKey(1,&hotkey,&cns);
   kbrdkey.KbrdKey = hotkey;
   kbrdkey.CNS     = cns;
   kbrdkey.CmdID   = i;

   // ����� ����������� ������, ���� ����� ����
   if (IsDlgButtonChecked(hwndDlgConfigure,IDC_SETTINGS_CONFIGURE_ANALOGS))
    {
     int k2,l2;
     stKbrdKey checkkbrdkey;
     // ������� ���� ��������
     for (l2=0; l2<Plugins.GetPluginsCount(); l2++)
      {
       bool exitwhile = false;
       // ������ �����, ���������� � �� ���� �� ��������� �� ����� ����������� �������
       while (!exitwhile)
        {
         exitwhile = true;
         // ������� ���� ������� � ���� ���� �������, �� ���� ������������
         for (k2=0; k2<Plugins.PluginsKeys[l2].KbrdKeys.GetKeysCount(); k2++)
          {
           // ��������� ������ � ������
           Plugins.PluginsKeys[l2].KbrdKeys.GetKey(k2,&checkkbrdkey);
           // ���� ������ �������, �� ���� ������������
           if (kbrdkey.KbrdKey==checkkbrdkey.KbrdKey)
           if (!((l==l2)&&(kbrdkey.CmdID==checkkbrdkey.CmdID)))
            {
             // �������� ������
             Plugins.PluginsKeys[l2].KbrdKeys.DelKey(k2);
             exitwhile = false;
             break;
            }
          }
        }
      }
    }
   // ���������� ������
   EnableWindow(GetDlgItem(hwndDlg,IDC_SETTINGS_KBRD_CLEAR),0);
   if (kbrdkey.KbrdKey!=Settings.MainMenuAccess.HotKey)
    {
     if (HIBYTE(kbrdkey.KbrdKey)!=0)
      if (k<0) Plugins.PluginsKeys[l].KbrdKeys.AddKey(&kbrdkey);
      else     Plugins.PluginsKeys[l].KbrdKeys.ChangeKey(k,&kbrdkey);
     EnableWindow(GetDlgItem(hwndDlg,IDC_SETTINGS_KBRD_CLEAR),1);
    }
   else MessageBox(0,"���������� ������ �� ����� ���� ���������, ��� ��� ��������� � ����������� ������� � �������� ����. �������������� �������� ����������.","������",MB_ICONERROR|MB_OK);
   DlgSettings_Kbrd_PluginCommands_Keys_Update(hwndDlg);
   ShowWindow(hwndDlgSettings,SW_SHOW);
  }
}

// ������� ������� �� �������
void DlgSettingsProc_Kbrd_Clear(HWND hwndDlg)
{
 if (!IsWindowEnabled(GetDlgItem(hwndDlg,IDC_SETTINGS_KBRD_CLEAR))) return;

 int l;
 char buffer[MAX_PATH];
 // ����������� ������ �������, � ������� ��� ������
 l = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_ACTIVEPLUGINS,LB_GETCURSEL,0,0);
 SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_ACTIVEPLUGINS,LB_GETTEXT,(WPARAM)l,(LPARAM)(LPCTSTR)buffer);
 l = Plugins.FindByFileName(buffer); // ����������� ������ ������� � ������ ������
 // ����������� ������ ������� � ������ ������ Kbrd
 int k = Plugins.PluginsKeys[l].KbrdKeys.FindByCmdID(SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_PLUGINCOMMANDS,LVM_GETSELECTIONMARK,0,0));
 if (k>=0) Plugins.PluginsKeys[l].KbrdKeys.DelKey(k);
 DlgSettings_Kbrd_PluginCommands_Keys_Update(hwndDlg);
 EnableWindow(GetDlgItem(hwndDlg,IDC_SETTINGS_KBRD_CLEAR),0);
}

// ������� ��������� ���������, ����������� ������� ��������� ������ ����������
BOOL CALLBACK DlgSettingsProc_Kbrd(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
 switch (message)
  {
   case WM_INITDIALOG: // �������� ����������� ��� ������ �������� 
    {
     // ***������� ����������***
     if (lstrcmp(Settings.Language,lng_default)!=0)
      {
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_CAPTION,              WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_KBRD_Window_Caption);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_KBRDON,               WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_KBRD_Button_KbrdOn);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_ACTIVEPLUGINS_CAPTION,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_KBRD_Text_ActivePlugins);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_COMMANDS_CAPTION,     WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_KBRD_Text_Commands);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_CHANGEKEY,            WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_KBRD_Button_ChangeKey);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_CLEAR,                WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_KBRD_Button_Clear);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_CONFIGURE,            WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_KBRD_Button_Configure);
      }
     // ***

     // "�������" ������
     EnableWindow(GetDlgItem(hwndDlg,IDC_SETTINGS_KBRD_CHANGEKEY),0);
     EnableWindow(GetDlgItem(hwndDlg,IDC_SETTINGS_KBRD_CONFIGURE),0);
     EnableWindow(GetDlgItem(hwndDlg,IDC_SETTINGS_KBRD_CLEAR),0);

     // ��������� ���������� - �������, �������, CNS + ��������� ���� �������
     LV_COLUMN column;
     column.mask     = LVCF_TEXT|LVCF_WIDTH;
     column.cx       = 278;
     column.iSubItem = 1;
     // ***������� ����������***
     if (lstrcmp(Settings.Language,lng_default)!=0) column.pszText = Settings.LanguageStrings->SETTINGS_KBRD_List_Command;
     else                                           column.pszText = "�������";
     // ***
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_PLUGINCOMMANDS,LVM_INSERTCOLUMN,0,(LPARAM)&column);
     column.cx       = 110;
     column.iSubItem = 2;
     // ***������� ����������***
     if (lstrcmp(Settings.Language,lng_default)!=0) column.pszText = Settings.LanguageStrings->SETTINGS_KBRD_List_HotKey;
     else                                           column.pszText = "�������";
     // ***
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_PLUGINCOMMANDS,LVM_INSERTCOLUMN,1,(LPARAM)&column);
     column.cx       = 47;
     column.iSubItem = 3;
     // ***������� ����������***
     if (lstrcmp(Settings.Language,lng_default)!=0) column.pszText = Settings.LanguageStrings->SETTINGS_KBRD_List_CNS;
     else                                           column.pszText = "CNS";
     // ***
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_PLUGINCOMMANDS,LVM_INSERTCOLUMN,2,(LPARAM)&column);
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_PLUGINCOMMANDS,LVM_SETEXTENDEDLISTVIEWSTYLE,(WPARAM)LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES,(LPARAM)LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
     // ����������
     CheckDlgButton(hwndDlg,IDC_SETTINGS_KBRD_KBRDON,Settings.KbrdOn);    
     DlgSettings_Kbrd_ActivePlugins_Update(hwndDlg);
    }
    return true;

   case WM_COMMAND: // ��������� ������� ������ � �������
    switch (LOWORD(wParam))
     {
      case IDC_SETTINGS_KBRD_ACTIVEPLUGINS: // ����� ������ ������ � ����� ������ ������
       if (HIWORD(wParam)==LBN_SELCHANGE) DlgSettings_Kbrd_PluginCommands_List_Update(hwndDlg);
       break;

      case IDC_SETTINGS_KBRD_CHANGEKEY: // ��������� �������
       DlgSettingsProc_Kbrd_ChangeKey(hwndDlg);
       break;

      case IDC_SETTINGS_KBRD_CLEAR:     // ������� �������
       DlgSettingsProc_Kbrd_Clear(hwndDlg);
       break;

      case IDC_SETTINGS_KBRD_CONFIGURE: // ������������ ������
       if (!IsWindowEnabled(GetDlgItem(hwndDlg,IDC_SETTINGS_KBRD_CONFIGURE))) break;
       {
        int i;
        char buffer[MAX_PATH];
        stPluginData cfgplugindata;
        // ����������� ������ �������, � ������� ��� ������
        i = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_ACTIVEPLUGINS,LB_GETCURSEL,0,0);
        SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_ACTIVEPLUGINS,LB_GETTEXT,(WPARAM)i,(LPARAM)(LPCTSTR)buffer);
        i = Plugins.FindByFileName(buffer); // ����������� ������ ������� � ������ ������
        Plugins.GetPluginData(i,&cfgplugindata);
        ShowWindow(hwndDlgSettings,SW_HIDE);
        cfgplugindata.Config();
        ShowWindow(hwndDlgSettings,SW_SHOW);
       }
       break;
     }
    return true;

   case WM_NOTIFY:
    switch ((int) wParam)
    {
      case IDC_SETTINGS_KBRD_ACTIVEPLUGINS:  // ��������� �� ������ �������
       {
        if ((((LPNMHDR)lParam)->code)==NM_RETURN) MessageBox(0,"OK",NULL,NULL);
       }
       break;
      case IDC_SETTINGS_KBRD_PLUGINCOMMANDS: // ��������� �� ������ ������
       {
        switch (((LPNMHDR)lParam)->code)
         {
          case LVN_ITEMCHANGED:
           {
            int l,k;
            char buffer[MAX_PATH];
            stPluginData plgdata;
            // ����������� ������ �������, � ������� ��� ������
            l = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_ACTIVEPLUGINS,LB_GETCURSEL,0,0);
            SendDlgItemMessage(hwndDlg,IDC_SETTINGS_KBRD_ACTIVEPLUGINS,LB_GETTEXT,(WPARAM)l,(LPARAM)(LPCTSTR)buffer);
            l = Plugins.FindByFileName(buffer); // ����������� ������ ������� � ������ ������
            // ����������� ���������� ��������
            k = ((LPNMLISTVIEW)lParam)->iItem;
            Plugins.GetPluginData(l,&plgdata);
            if ((plgdata.CommandsList[k][0]=='+')||(plgdata.CommandsList[k][0]=='-')) 
             EnableWindow(GetDlgItem(hwndDlgKbrd,IDC_SETTINGS_KBRD_CHANGEKEY),0);
            else               
             EnableWindow(GetDlgItem(hwndDlgKbrd,IDC_SETTINGS_KBRD_CHANGEKEY),1);
            if (Plugins.PluginsKeys[l].KbrdKeys.FindByCmdID(k)<0) EnableWindow(GetDlgItem(hwndDlgKbrd,IDC_SETTINGS_KBRD_CLEAR),0);
            else                                                  EnableWindow(GetDlgItem(hwndDlgKbrd,IDC_SETTINGS_KBRD_CLEAR),1);
           }
           break;
          case LVN_ITEMACTIVATE:
           {
            // ��� ������� ����� ���������� ������ "��������"
            DlgSettingsProc_Kbrd_ChangeKey(hwndDlg);
           }
           break;
          case LVN_KEYDOWN:
           {
            NMLVKEYDOWN *keydown = ((LV_KEYDOWN FAR *)lParam);
            switch (keydown->wVKey)
             {
              case VK_INSERT: // ��������� �������
               DlgSettingsProc_Kbrd_ChangeKey(hwndDlg);
               break;
              case VK_DELETE: // ������� �������
               DlgSettingsProc_Kbrd_Clear(hwndDlg);
               break;
             }
           }
           break;
          case NM_RCLICK:
           {
            // �������� �� ������ ������� ����
            DlgSettingsProc_Kbrd_Clear(hwndDlg);
           }
           break;
         }
       }
       break;
     }
    return true;

   case WM_CLOSE: // �������� �������
    EndDialog(hwndDlg,0);
	 		return true;

   case WM_DESTROY:
    Settings.KbrdOn = IsDlgButtonChecked(hwndDlg,IDC_SETTINGS_KBRD_KBRDON);
    return true;
  }
 return false;
}

// ������� ��������� ���������, ����������� ������� ���������� ���������� ��� WinLIRC
BOOL CALLBACK DlgSettingsProc_Wirc_EnterRepeatsProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
 switch (message)
  {
   case WM_COMMAND: // ��������� ������� ������ � �������
    switch (LOWORD(wParam))
     {
      case IDC_SETTINGS_WIRC_ENTERDATA_OK:
      char str[100];
      int  i;
      SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_ENTERDATA_REPEATS,WM_GETTEXT,(WPARAM)sizeof(str),(LPARAM)str);
      i = stoi(str);
      EndDialog(hwndDlg,i);
     }
    return true;

   case WM_CLOSE: // �������� �������
    EndDialog(hwndDlg,-1);
	 		return true;
  }
 return false;
}

// ������� ��������� ���������, ����������� ������� ������������ ��������� ����
BOOL CALLBACK DlgSettingsProc_Mouse(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
 switch (message)
  {
   case WM_CLOSE:   
    EndDialog(hwndDlg,0);
	return true;
  }
 return false;
}

// ������� ��������� ���������, ����������� ������� ������������ ������ ����
BOOL CALLBACK DlgSettingsProc_Gestures(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
 switch (message)
  {
   case WM_CLOSE:   
    EndDialog(hwndDlg,0);
	return true;
  }
 return false;
}

// ��������� ���������� ������
void DlgSettingsProc_Wirc_ChangeKey(HWND hwndDlg)
{
 if (!IsWindowEnabled(GetDlgItem(hwndDlg,IDC_SETTINGS_WIRC_CHANGEKEY))) return;

 ShowWindow(hwndDlgSettings,SW_HIDE);

 int i;
 // ����� ���������� ��������
 i = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_PLUGINCOMMANDS,LVM_GETSELECTIONMARK,0,0);
 // ���� ������� ������, �� ������ ���������� ������
 if (i>=0) 
  {
   ShowWindow(hwndDlgSettings,SW_HIDE);
   int k,l;
   stWircKey wirckey;
   char buffer[MAX_PATH];
   // ����������� ������ �������, � ������� ��� ������
   l = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_ACTIVEPLUGINS,LB_GETCURSEL,0,0);
   SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_ACTIVEPLUGINS,LB_GETTEXT,(WPARAM)l,(LPARAM)(LPCTSTR)buffer);
   l = Plugins.FindByFileName(buffer); // ����������� ������ ������� � ������ ������
   // ����������� ������ ������� � ������ ������ Wirc
   k = Plugins.PluginsKeys[l].WircKeys.FindByCmdID(i);
   // ��������� ���������� ��������
   Plugins.PluginsKeys[l].WircKeys.GetKey(k,&wirckey);
   wirckey.Repeats = DialogBox(hInst,MAKEINTRESOURCE(IDD_SETTINGS_WIRC_ENTERDATA),0,(DLGPROC)DlgSettingsProc_Wirc_EnterRepeatsProc);
   // ��������� �� ������
   if (wirckey.Repeats<0) wirckey.Repeats = -1;
   Plugins.PluginsKeys[l].WircKeys.ChangeKey(k,&wirckey);
   DlgSettings_Wirc_PluginCommands_Keys_Update(hwndDlg);
  }
 ShowWindow(hwndDlgSettings,SW_SHOW);
}

// ������� ������� �� �������
void DlgSettingsProc_Wirc_Clear(HWND hwndDlg)
{
 if (!IsWindowEnabled(GetDlgItem(hwndDlg,IDC_SETTINGS_WIRC_CLEAR))) return;

 int l;
 char buffer[MAX_PATH];
 // ����������� ������ �������, � ������� ��� ������
 l = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_ACTIVEPLUGINS,LB_GETCURSEL,0,0);
 SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_ACTIVEPLUGINS,LB_GETTEXT,(WPARAM)l,(LPARAM)(LPCTSTR)buffer);
 l = Plugins.FindByFileName(buffer); // ����������� ������ ������� � ������ ������
 // ����������� ������ ������ � ������ ������ Wirc
 int k = Plugins.PluginsKeys[l].WircKeys.FindByCmdID(SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_PLUGINCOMMANDS,LVM_GETSELECTIONMARK,0,0));
 if (k>=0) Plugins.PluginsKeys[l].WircKeys.DelKey(k);
 // ����������
 DlgSettings_Wirc_PluginCommands_Keys_Update(hwndDlg);
 EnableWindow(GetDlgItem(hwndDlgWirc,IDC_SETTINGS_WIRC_CHANGEKEY),0);
 EnableWindow(GetDlgItem(hwndDlgWirc,IDC_SETTINGS_WIRC_CLEAR),0);
}

// ������� ��������� ���������, ����������� ������� ��������� ������ WinLIRC
BOOL CALLBACK DlgSettingsProc_Wirc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
 switch (message)
  {
   case WM_INITDIALOG: // �������� ����������� ��� ������ �������� 
    {
     // ***������� ����������***
     if (lstrcmp(Settings.Language,lng_default)!=0)
      {
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_CAPTION,       WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_WIRC_Window_Caption);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_WIRCON,        WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_WIRC_Button_WircOn);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_ACTIVEPLUGINS, WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_WIRC_Text_ActivePlugins);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_COMMANDS,      WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_WIRC_Text_Commands);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_SERVER_CAPTION,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_WIRC_Text_Server);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_PORT_CAPTION,  WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_WIRC_Text_Port);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_CHANGEKEY,     WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_WIRC_Button_ChangeKey);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_CLEAR,         WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_WIRC_Button_Clear);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_CONFIGURE,     WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_WIRC_Button_Configure);
      }
     // ***

     // "�������" ������
     EnableWindow(GetDlgItem(hwndDlg,IDC_SETTINGS_WIRC_CHANGEKEY),0);
     EnableWindow(GetDlgItem(hwndDlg,IDC_SETTINGS_WIRC_CONFIGURE),0);
     EnableWindow(GetDlgItem(hwndDlg,IDC_SETTINGS_WIRC_CLEAR),0);

     // ��������� ����������
     LV_COLUMN column;
     column.mask     = LVCF_TEXT|LVCF_WIDTH;
     column.cx       = 278;
     column.iSubItem = 1;
     // ***������� ����������***
     if (lstrcmp(Settings.Language,lng_default)!=0) column.pszText = Settings.LanguageStrings->SETTINGS_WIRC_List_Command;
     else                                           column.pszText = "�������";
     // ***
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_PLUGINCOMMANDS,LVM_INSERTCOLUMN,0,(LPARAM)&column);
     column.cx       = 133;
     column.iSubItem = 2;
     // ***������� ����������***
     if (lstrcmp(Settings.Language,lng_default)!=0) column.pszText = Settings.LanguageStrings->SETTINGS_WIRC_List_Button;
     else                                           column.pszText = "������";
     // ***
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_PLUGINCOMMANDS,LVM_INSERTCOLUMN,1,(LPARAM)&column);
     column.cx       = 24;
     column.iSubItem = 3;
     // ***������� ����������***
     if (lstrcmp(Settings.Language,lng_default)!=0) column.pszText = Settings.LanguageStrings->SETTINGS_WIRC_List_Repeats;
     else                                           column.pszText = "�";
     // ***
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_PLUGINCOMMANDS,LVM_INSERTCOLUMN,2,(LPARAM)&column);
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_PLUGINCOMMANDS,LVM_SETEXTENDEDLISTVIEWSTYLE,(WPARAM)LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES,(LPARAM)LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
     // ����������
     CheckDlgButton(hwndDlg,IDC_SETTINGS_WIRC_WIRCON,Settings.WircOn);    
     DlgSettings_Wirc_ActivePlugins_Update(hwndDlg);
     // ��������� ����� � ���� WinLIRC �������
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_SERVER,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.WinLIRCaddress);
     char str[30];
     wsprintf(str,"%i%",Settings.WinLIRCport);
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_PORT,WM_SETTEXT,0,(LPARAM)(LPCTSTR)str);
     // ����������� �� ���� �� ���������� ��������
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_SERVER,EM_SETLIMITTEXT,(WPARAM)sizeof(Settings.WinLIRCaddress),0);
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_PORT,EM_SETLIMITTEXT,(WPARAM)10,0);
     // ������� ����������� � ����������� ��������� ������
     if (Settings.WircOn)
      SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_CONDISCON,BM_CLICK,0,0);
     else
      // ***������� ����������***
      if (lstrcmp(Settings.Language,lng_default)!=0) SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_CONDISCON,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_WIRC_Button_Connect); // ���� ����������� �� �����
      else                                           SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_CONDISCON,WM_SETTEXT,0,(LPARAM)(LPCTSTR)"���������� ����������");
      // ***
    }
    return true;

   case WM_COMMAND: // ��������� ������� ������ � �������
    switch (LOWORD(wParam))
     {
      case IDC_SETTINGS_WIRC_ACTIVEPLUGINS:
       if (HIWORD(wParam)==LBN_SELCHANGE) DlgSettings_Wirc_PluginCommands_List_Update(hwndDlg);
       break;

      case IDC_SETTINGS_WIRC_CONDISCON:
       if (hWinLIRCsocket==INVALID_SOCKET) 
        {
         // ���� ��� ���������� � ��������
         char buffer [MAX_PATH];
         char buffer2[MAX_PATH];
         int tempport;
         SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_SERVER,WM_GETTEXT,MAX_PATH,(LPARAM)buffer);
         SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_PORT,WM_GETTEXT,MAX_PATH,(LPARAM)buffer2);
         tempport = stoi(buffer2);
         if (WinLIRCconnect(buffer,tempport,hwndDlg)) 
          {
           // ***������� ����������***
           if (lstrcmp(Settings.Language,lng_default)!=0)
            {
             SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_CONDISCON,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_WIRC_Button_Disconnect); // ���� ������� �����������
             SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_STATUS,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_WIRC_Text_Connected);
            }
           else 
            {
             SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_CONDISCON,WM_SETTEXT,0,(LPARAM)(LPCTSTR)"�������� ����������"); // ���� ������� �����������
             SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_STATUS,WM_SETTEXT,0,(LPARAM)(LPCTSTR)"���������� �����������");
            }
           // ***

           SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_SERVER,EM_SETREADONLY,(WPARAM)(BOOL)1,0);
           SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_PORT,EM_SETREADONLY,(WPARAM)(BOOL)1,0);
          }
         else
          {
           // ***������� ����������***
           if (lstrcmp(Settings.Language,lng_default)!=0)
            {
             SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_CONDISCON,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_WIRC_Button_Connect); // ���� �� ������� �����������
             SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_STATUS,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_WIRC_Text_ConnectFailed);
            }
           else 
            {
             SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_CONDISCON,WM_SETTEXT,0,(LPARAM)(LPCTSTR)"���������� ����������"); // ���� �� ������� �����������
             SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_STATUS,WM_SETTEXT,0,(LPARAM)(LPCTSTR)"���������� ����������");
            }
           // ***
           SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_SERVER,EM_SETREADONLY,(WPARAM)(BOOL)0,0);
           SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_PORT,EM_SETREADONLY,(WPARAM)(BOOL)0,0);
          }
        }
       else 
        {
         // ���� ���� ���������� � ��������, �� ��������� ���
         WinLIRCdisconnect();
         // ***������� ����������***
         if (lstrcmp(Settings.Language,lng_default)!=0)
          {
           SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_CONDISCON,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_WIRC_Button_Connect);
           SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_STATUS,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_WIRC_Text_Disconnected);
          }
         else 
          {
           SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_CONDISCON,WM_SETTEXT,0,(LPARAM)(LPCTSTR)"���������� ����������");
           SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_STATUS,WM_SETTEXT,0,(LPARAM)(LPCTSTR)"���������� ��������");
          }
         // ***
         SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_SERVER,EM_SETREADONLY,(WPARAM)(BOOL)0,0);
         SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_PORT,EM_SETREADONLY,(WPARAM)(BOOL)0,0);
        }
       break;

      case IDC_SETTINGS_WIRC_CHANGEKEY: // ��������� ������ ������
       DlgSettingsProc_Wirc_ChangeKey(hwndDlg);
       break;

      case IDC_SETTINGS_WIRC_CLEAR:     // ������� �������
       DlgSettingsProc_Wirc_Clear(hwndDlg);
      break;
 
      case IDC_SETTINGS_WIRC_CONFIGURE: // ������������ ������
       if (!IsWindowEnabled(GetDlgItem(hwndDlg,IDC_SETTINGS_WIRC_CONFIGURE))) break;
       {
        int i2;
        char buffer[MAX_PATH];
        stPluginData cfgplugindata;
        // ����������� ������ �������, � ������� ��� ������
        i2 = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_ACTIVEPLUGINS,LB_GETCURSEL,0,0);
        SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_ACTIVEPLUGINS,LB_GETTEXT,(WPARAM)i2,(LPARAM)(LPCTSTR)buffer);
        i2 = Plugins.FindByFileName(buffer); // ����������� ������ ������� � ������ ������
        Plugins.GetPluginData(i2,&cfgplugindata);
        ShowWindow(hwndDlgSettings,SW_HIDE);
        cfgplugindata.Config();
        ShowWindow(hwndDlgSettings,SW_SHOW);
       }
       break;
     }
    return true;

   case WM_NOTIFY:
    switch ((int) wParam)
     {
      case IDC_SETTINGS_WIRC_PLUGINCOMMANDS:
       {
        switch (((LPNMHDR)lParam)->code)
         {
          case LVN_ITEMCHANGED:
           {
            int l,k;
            char buffer[MAX_PATH];
            // ����������� ������ �������, � ������� ��� ������
            l = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_ACTIVEPLUGINS,LB_GETCURSEL,0,0);
            SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_ACTIVEPLUGINS,LB_GETTEXT,(WPARAM)l,(LPARAM)(LPCTSTR)buffer);
            l = Plugins.FindByFileName(buffer); // ����������� ������ ������� � ������ ������
            // ����������� ���������� ��������
            k = ((LPNMLISTVIEW)lParam)->iItem;
            if (Plugins.PluginsKeys[l].WircKeys.FindByCmdID(k)<0) 
             {
              // ������� �� ����������
              EnableWindow(GetDlgItem(hwndDlgWirc,IDC_SETTINGS_WIRC_CHANGEKEY),0);
              EnableWindow(GetDlgItem(hwndDlgWirc,IDC_SETTINGS_WIRC_CLEAR),0);
             }
            else
             {
              // ������� ����������
              EnableWindow(GetDlgItem(hwndDlgWirc,IDC_SETTINGS_WIRC_CHANGEKEY),1);
              EnableWindow(GetDlgItem(hwndDlgWirc,IDC_SETTINGS_WIRC_CLEAR),1);
             }
           }
           break;
          case LVN_ITEMACTIVATE:
           {
            // ��� ������� ����� ���������� ������ "��������"
            DlgSettingsProc_Wirc_ChangeKey(hwndDlg);
           }
           break;
          case LVN_KEYDOWN:
           {
            NMLVKEYDOWN *keydown = ((LV_KEYDOWN FAR *)lParam);
            switch (keydown->wVKey)
             {
              case VK_INSERT:
               DlgSettingsProc_Wirc_ChangeKey(hwndDlg);
               break;
              case VK_DELETE:
               DlgSettingsProc_Wirc_Clear(hwndDlg);
               break;
             }
           }
           break;
          case NM_RCLICK:
           {
            // �������� �� ������ ������� ����
            DlgSettingsProc_Wirc_Clear(hwndDlg);
           }
           break;
         }
       }
       break;
     }
    break;

   // ��������� ��������� �� WinLIRC
   case WM_WIRCKEYS:
    WORD WSAEvent;
    WSAEvent = WSAGETSELECTEVENT(lParam);
	   if (WSAEvent==FD_READ)
     {
  			 recv(hWinLIRCsocket,sWinLIRCstring,sizeof(sWinLIRCstring),0);
      // ���� ������ �������, �� ������������ �������� ���������
      if (IsWindowVisible(hwndDlg))
       { 
        // �����, ���� ������ ���������, � �� �������
        int l,k;
        char buffer[MAX_PATH];
        stPluginData plgdata;
        // ����������� ������ �������, � ������� ��� ������
        l = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_ACTIVEPLUGINS,LB_GETCURSEL,0,0);
        SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_ACTIVEPLUGINS,LB_GETTEXT,(WPARAM)l,(LPARAM)(LPCTSTR)buffer);
        l = Plugins.FindByFileName(buffer); // ����������� ������ ������� � ������ ������
        // ����������� ���������� ��������
        k = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_PLUGINCOMMANDS,LVM_GETSELECTIONMARK,0,0);
        Plugins.GetPluginData(l,&plgdata);
        if ((plgdata.CommandsList[k][0]=='+')||(plgdata.CommandsList[k][0]=='-')) break;

        // ���� ������� ������� � ������, �� ������ ��� �� ������, ����� ������ �� ������
        int i = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_PLUGINCOMMANDS,LVM_GETSELECTIONMARK,0,0);
        if (i>=0)
         {
          int k,l;
          WORD hotkey = 0;
          WORD cns    = 0;
          stWircKey wirckey;
          char buffer[MAX_PATH];
          // ����������� ������ �������, � ������� ��� ������
          l = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_ACTIVEPLUGINS,LB_GETCURSEL,0,0);
          SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_ACTIVEPLUGINS,LB_GETTEXT,(WPARAM)l,(LPARAM)(LPCTSTR)buffer);
          l = Plugins.FindByFileName(buffer); // ����������� ������ ������� � ������ ������
          // ����������� ������ ������� � ������ ������ Wirc
          k = Plugins.PluginsKeys[l].WircKeys.FindByCmdID(i);
          // �����������  ������� � ���������� ���������� ������
          WinLIRCgetstr(sWinLIRCstring,3,wirckey.WircKey);
          wirckey.Repeats = -1;
          wirckey.CmdID   = i;
          // �������� �� ����������� ������
          if (IsDlgButtonChecked(hwndDlgConfigure,IDC_SETTINGS_CONFIGURE_ANALOGS))
           {
            int k2,l2;
            stWircKey checkwirckey;
            // ������� ���� ��������
            for (l2=0; l2<Plugins.GetPluginsCount(); l2++)
             {
              bool exitwhile = false;
              // ������ �����, ���������� � �� ���� �� ��������� �� ����� ����������� �������
              while (!exitwhile)
               {
                exitwhile = true;
                // ������� ���� ������ � ���� ���� �������, �� ���� ������������
                for (k2=0; k2<Plugins.PluginsKeys[l2].WircKeys.GetKeysCount(); k2++)
                 {
                  // ��������� ������ � ������
                  Plugins.PluginsKeys[l2].WircKeys.GetKey(k2,&checkwirckey);
                  // ���� ������ �������, �� ���� ������������
                  if (lstrcmp(wirckey.WircKey,checkwirckey.WircKey)==0)
                   if (!((l==l2)&&(wirckey.CmdID==checkwirckey.CmdID)))
                    {
                     // �������� ������
                     Plugins.PluginsKeys[l2].WircKeys.DelKey(k2);
                     exitwhile = false;
                     break;
                    }
                 }
               }
             }
           }
          // ���������� ������
          if (k<0) Plugins.PluginsKeys[l].WircKeys.AddKey(&wirckey);
          else     Plugins.PluginsKeys[l].WircKeys.ChangeKey(k,&wirckey);
          DlgSettings_Wirc_PluginCommands_Keys_Update(hwndDlg);
          // ���������� ��������� ����������� ������
          EnableWindow(GetDlgItem(hwndDlgWirc,IDC_SETTINGS_WIRC_CHANGEKEY),1);
          EnableWindow(GetDlgItem(hwndDlgWirc,IDC_SETTINGS_WIRC_CLEAR),1);
         }
       }
     }
		  return true;

   case WM_CLOSE: // �������� �������
    EndDialog(hwndDlg,0);
	 		return true;

   case WM_DESTROY:
    {
     WinLIRCdisconnect();
     Settings.WircOn = IsDlgButtonChecked(hwndDlg,IDC_SETTINGS_WIRC_WIRCON);
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_SERVER,WM_GETTEXT,sizeof(Settings.WinLIRCaddress),(LPARAM)Settings.WinLIRCaddress);
     char strtemp[11];
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_WIRC_PORT,WM_GETTEXT,sizeof(strtemp),(LPARAM)strtemp);
     Settings.WinLIRCport = stoi(strtemp);
    }
    return true;
  }
 return false;
}

// ������� ��������� ���������, ����������� ������� 
BOOL CALLBACK DlgSettingsProc_PlugList(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
 int          i,k,l;

 switch (message)
  {
   case WM_INITDIALOG:
    {
     // ***������� ����������***
     if (lstrcmp(Settings.Language,lng_default)!=0)
      {
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_CAPTION,            WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_PLUGLIST_Window_Caption);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_PLUGINSPATH_CAPTION,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_PLUGLIST_Text_PluginsPath);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_FINDPLUGINS_CAPTION,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_PLUGLIST_Text_FindPlugins);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_ACTVPLUGINS_CAPTION,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_PLUGLIST_Text_ActvPlugins);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_ADDONE,             WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_PLUGLIST_Button_AddOne);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_DELONE,             WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_PLUGLIST_Button_DelOne);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_ADDALL,             WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_PLUGLIST_Button_AddAll);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_DELALL,             WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_PLUGLIST_Button_DelAll);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_ACCEPT,             WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_PLUGLIST_Button_Accept);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_UPDATE,             WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_PLUGLIST_Button_Update);
      }
     // ***
    }
    break;

   case WM_COMMAND: // ��������� ������� ������ � �������
    switch (LOWORD(wParam))
     {
      case IDC_SETTINGS_PLUGLIST_ADDONE:
       {
        char buffer[MAX_PATH];
        i = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_FINDPLUGINS,LB_GETCURSEL,0,0);
        // ���� ������ �� �������, �� �������
        if (i==LB_ERR) break;
        SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_FINDPLUGINS,LB_GETTEXT,(WPARAM)i,(LPARAM)(LPCTSTR)buffer);
        i = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_ACTVPLUGINS,LB_FINDSTRING,0,(LPARAM)(LPCTSTR)buffer);
        if (i==LB_ERR) SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_ACTVPLUGINS,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)buffer);
       }
       break;

      case IDC_SETTINGS_PLUGLIST_DELONE:
       i = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_ACTVPLUGINS,LB_GETCURSEL,0,0);
       // ���� ������ �� �������, �� �������
       if (i!=LB_ERR) SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_ACTVPLUGINS,LB_DELETESTRING,(WPARAM)i,0);
       break;

      case IDC_SETTINGS_PLUGLIST_ADDALL:
       {
        char buffer[MAX_PATH];
        for (i=0; i<SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_FINDPLUGINS,LB_GETCOUNT,0,0); i++)
         {
          SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_FINDPLUGINS,LB_GETTEXT,(WPARAM)i,(LPARAM)(LPCTSTR)buffer);
          k = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_ACTVPLUGINS,LB_FINDSTRING,0,(LPARAM)(LPCTSTR)buffer);
          if (k==LB_ERR) SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_ACTVPLUGINS,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)buffer);
         }
       }
       break;

      case IDC_SETTINGS_PLUGLIST_DELALL:
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_ACTVPLUGINS,LB_RESETCONTENT,0,0);
       break;

      case IDC_SETTINGS_PLUGLIST_ACCEPT:
       {
        k = Plugins.GetPluginsCount();
        stPluginData plgdata;
        char buffer[MAX_PATH];
        HMODULE *hdlldel = new HMODULE [k];
        // ����������� ������ ����������� �������
        for (i=0; i<k; i++)
         {
          Plugins.GetPluginData(i,&plgdata);
          l = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_ACTVPLUGINS,LB_FINDSTRING,0,(LPARAM)(LPCTSTR)plgdata.FileName);
          if (l==LB_ERR) hdlldel[i] = plgdata.hDll;
          else           hdlldel[i] = 0;
         }
        // ���������� ���� �������� ������� �� ������
        for (i=0; i<k; i++)
         if (hdlldel!=0)
          Plugins.DelPlugin(Plugins.FindByHandle(hdlldel[i]));
        // ������������ ��� �������, ������ ��� ������������ ������������ �������� AddPlugin
        for (i=0; i<SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_ACTVPLUGINS,LB_GETCOUNT,0,0); i++)
         {
          SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PLUGLIST_ACTVPLUGINS,LB_GETTEXT,(WPARAM)i,(LPARAM)(LPCTSTR)buffer);
          Plugins.AddPlugin(buffer);
         }
        delete [] hdlldel;
        DlgSettings_PlugList_Update(hwndDlg);
        // ��������� ������ � Kbrd � Wirc �����
        DlgSettings_Kbrd_ActivePlugins_Update(hwndDlgKbrd);
        DlgSettings_Wirc_ActivePlugins_Update(hwndDlgWirc);
       }
       break;

      case IDC_SETTINGS_PLUGLIST_UPDATE:
       DlgSettings_PlugList_Update(hwndDlg);
       break;
     }
    return true;
   case WM_CLOSE: // �������� �������
    EndDialog(hwndDlg,0);
	 		return true;
  }
 return false;
}

// ������� ��������� ���������, ����������� ������� 
BOOL CALLBACK DlgSettingsProc_Configure(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
 int i;
 char buffer[MAX_PATH];
 switch (message)
  {
   case WM_INITDIALOG: // ���������� ������ �������
    {
     // ***������� ����������***
     if (lstrcmp(Settings.Language,lng_default)!=0)
      {
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_CAPTION,               WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_CONFIGURE_Window_Caption);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_USEWINHOOKS,           WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_CONFIGURE_Button_UseWinHooks);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_REGISTERSERVICEPROCESS,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_CONFIGURE_Button_RegisterServiceProcess);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_CHANGEKEYBOARDSTATE,   WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_CONFIGURE_Button_ChangeKeyboardState);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_SHOWINTRAY,            WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_CONFIGURE_Button_ShowInTray);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_ANALOGS,               WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_CONFIGURE_Button_Analogs);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_INSTALLGROUP,          WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_CONFIGURE_Text_InstallGroup);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_INSTALL,               WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_CONFIGURE_Button_Install);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_UNINSTALL,             WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_CONFIGURE_Button_Uninstall);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_LANGUAGEGROUP,         WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_CONFIGURE_Text_LanguageGroup);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_ACCESSGROUP,           WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_CONFIGURE_Text_AccessGroup);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_ACCESS_TRAY,           WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_CONFIGURE_Access_Button_Tray);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_ACCESS_HOTKEY,         WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_CONFIGURE_Access_Button_HotKey);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_ACCESS_CHANGEHOTKEY,   WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_CONFIGURE_Access_Button_ChangeHotKey);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_ACCESS_PASSWORD,       WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_CONFIGURE_Access_Button_Password);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PASSWORD_PASSWORD,               WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_CONFIGURE_Access_Text_Password);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PASSWORD_CONFIRM,                WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_CONFIGURE_Access_Text_Confirm);
       SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_ACCESS_DBLRUN,         WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LanguageStrings->SETTINGS_CONFIGURE_Access_Button_DblRun);
      }
     // ***

     CheckDlgButton(hwndDlg,IDC_SETTINGS_CONFIGURE_USEWINHOOKS,Settings.UseWinHooks);
     CheckDlgButton(hwndDlg,IDC_SETTINGS_CONFIGURE_REGISTERSERVICEPROCESS,Settings.RegisterServiceProcess);
     CheckDlgButton(hwndDlg,IDC_SETTINGS_CONFIGURE_CHANGEKEYBOARDSTATE,Settings.ChangeKeyboardState);
     CheckDlgButton(hwndDlg,IDC_SETTINGS_CONFIGURE_SHOWINTRAY,Settings.ShowInTray);
     CheckDlgButton(hwndDlg,IDC_SETTINGS_CONFIGURE_ANALOGS,Settings.ClearOnAnalog);
     // ��������� ������ ���������� ������� � ������ �� ������� ������
     WIN32_FIND_DATA finddata;
     HANDLE          hfile;
     wsprintf(buffer,"%s\\*.%s",lngdir,lng_extension);
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_LANGUAGELIST,LB_RESETCONTENT,0,0); // ������ �����
     hfile = FindFirstFile(buffer,&finddata);
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_LANGUAGELIST,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)":Russian:");
     if (!(hfile==INVALID_HANDLE_VALUE)) // ���� ���-�� �������, �� �������, ��� �������
      if (!(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) // ���� ���� ������� �� ����������
       {
        SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_LANGUAGELIST,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)finddata.cFileName);
        while (FindNextFile(hfile,&finddata))
         if (!(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) // ���� ���� ������� �� ����������   }
          SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_LANGUAGELIST,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)finddata.cFileName);
       }
     // ������������� ������ �� ������� ����
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_LANGUAGELIST,LB_SELECTSTRING,0,(LPARAM)(LPCTSTR)Settings.Language);
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PSWRDENTER,EM_SETLIMITTEXT,(WPARAM)MAX_PASSWORDSYMBOLS,0);
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PSWRDREENTER,EM_SETLIMITTEXT,(WPARAM)MAX_PASSWORDSYMBOLS,0);
     if (Settings.MainMenuAccess.AccessFlags&AF_TRAY)   CheckDlgButton(hwndDlg,IDC_SETTINGS_CONFIGURE_ACCESS_TRAY,1);
     else                                               CheckDlgButton(hwndDlg,IDC_SETTINGS_CONFIGURE_ACCESS_TRAY,0);
     if (Settings.MainMenuAccess.AccessFlags&AF_HOTKEY) CheckDlgButton(hwndDlg,IDC_SETTINGS_CONFIGURE_ACCESS_HOTKEY,1);
     else                                               CheckDlgButton(hwndDlg,IDC_SETTINGS_CONFIGURE_ACCESS_HOTKEY,0);
     KeyToStr(Settings.MainMenuAccess.HotKey,buffer);
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_ACCESS_LBHOTKEY,WM_SETTEXT,0,(LPARAM)(LPCTSTR)buffer);
     if (Settings.MainMenuAccess.AccessFlags&AF_PASSWORD) CheckDlgButton(hwndDlg,IDC_SETTINGS_CONFIGURE_ACCESS_PASSWORD,1);
     else                                                 CheckDlgButton(hwndDlg,IDC_SETTINGS_CONFIGURE_ACCESS_PASSWORD,0);
     if (Settings.MainMenuAccess.AccessFlags&AF_DBLRUN)   CheckDlgButton(hwndDlg,IDC_SETTINGS_CONFIGURE_ACCESS_DBLRUN,1);
     else                                                 CheckDlgButton(hwndDlg,IDC_SETTINGS_CONFIGURE_ACCESS_DBLRUN,0);
    }
    return true;

   case WM_COMMAND:
    switch (LOWORD(wParam))
     {
      case IDC_SETTINGS_CONFIGURE_INSTALL: // ���������� � ������������
       {
        HKEY hKeyInst;
        char szPath[MAX_PATH];
        GetModuleFileName(NULL, szPath, sizeof(szPath));
        RegCreateKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,"",REG_OPTION_NON_VOLATILE,KEY_SET_VALUE,NULL,&hKeyInst,NULL);
        if (hKeyInst)
         {
          RegSetValueEx(hKeyInst,"qshKeyCommander",NULL,REG_SZ,(LPBYTE)szPath,lstrlen(szPath));
          RegCloseKey(hKeyInst);
         }
       }
       break;

      case IDC_SETTINGS_CONFIGURE_UNINSTALL:
       {
        HKEY hKeyUninst;
        RegCreateKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,"",REG_OPTION_NON_VOLATILE,KEY_SET_VALUE,NULL,&hKeyUninst,NULL);
        if (hKeyUninst)
         {
          RegDeleteValue(hKeyUninst,"qshKeyCommander");
          RegCloseKey(hKeyUninst);
         }
       }
       break;

      case IDC_SETTINGS_CONFIGURE_ACCESS_CHANGEHOTKEY:
       {
        ShowWindow(hwndDlgSettings,SW_HIDE);
        WORD temphotkey = 0;
        // ��������� ���������� ������
        CGetKbrdKey getaccesshotkey;
        getaccesshotkey.GetHotKey(0,&temphotkey,NULL);
        // �������� ������� �� ����������, ���� ����� ����
        bool exitfor = false;
        if (IsDlgButtonChecked(hwndDlgConfigure,IDC_SETTINGS_CONFIGURE_ANALOGS))
         {
          int l2,k2;
          char buffer [MAX_PATH];
          char buffer2[MAX_PATH];
          stKbrdKey    checkkbrdkey;
          stPluginData checkplugindata;
          for (l2=0; l2<Plugins.GetPluginsCount(); l2++)
           {
            Plugins.GetPluginData(l2,&checkplugindata);
            for (k2=0; k2<Plugins.PluginsKeys[l2].KbrdKeys.GetKeysCount(); k2++)
             {
              Plugins.PluginsKeys[l2].KbrdKeys.GetKey(k2,&checkkbrdkey);
              if (checkkbrdkey.KbrdKey==temphotkey)
               {
                KeyToStr(temphotkey,buffer);
                wsprintf(buffer2,"���������� ���������� � ������:\n%s\n��� �������:\n%s\n��� ����������� ������� � �������� ���� ���������� ���� �� ������!",checkplugindata.FileName,checkplugindata.CommandsList[checkkbrdkey.CmdID]);
                // ��������� ������������
                MessageBox(hwndDlg,buffer2,buffer,MB_ICONWARNING|MB_OK);
                exitfor = true;
               }
              if (exitfor) break;
             }
            if (exitfor) break;
           }
         }
        // ���� ������ �� ���� ��������� � �� ���������, �� ��� ���������� � ����������
        if ((HIBYTE(temphotkey)!=0)&&(!exitfor)) 
         {
          char strkey[100];
          Settings.MainMenuAccess.HotKey = temphotkey;
          KeyToStr(temphotkey,strkey);
          SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_ACCESS_LBHOTKEY,WM_SETTEXT,0,(LPARAM)strkey);
         }
        ShowWindow(hwndDlgSettings,SW_SHOW);
       }
       break;
     }
    break;

   case WM_CLOSE: // �������� �������
    EndDialog(hwndDlg,0);
	 		return true;

   case WM_DESTROY:
    {
     Settings.UseWinHooks            = IsDlgButtonChecked(hwndDlg,IDC_SETTINGS_CONFIGURE_USEWINHOOKS);
     Settings.RegisterServiceProcess = IsDlgButtonChecked(hwndDlg,IDC_SETTINGS_CONFIGURE_REGISTERSERVICEPROCESS);
     Settings.ChangeKeyboardState    = IsDlgButtonChecked(hwndDlg,IDC_SETTINGS_CONFIGURE_CHANGEKEYBOARDSTATE);
     if (Settings.ShowInTray!=(BOOL)IsDlgButtonChecked(hwndDlg,IDC_SETTINGS_CONFIGURE_SHOWINTRAY))
      {
       Settings.ShowInTray = IsDlgButtonChecked(hwndDlg,IDC_SETTINGS_CONFIGURE_SHOWINTRAY);
       CTray systemtray;
       if (Settings.ShowInTray) systemtray.AddTrayIcon(hwndServerWindow,1,NIF_ICON|NIF_MESSAGE|NIF_TIP,LoadIcon(hInst,MAKEINTRESOURCE(IDI_LOGO)),WM_TRAYICON,"qshKeyCommander");
       else                     systemtray.DeleteTrayIcon(hwndServerWindow,1);
      }
     Settings.ClearOnAnalog = IsDlgButtonChecked(hwndDlg,IDC_SETTINGS_CONFIGURE_ANALOGS);
     // ���������� ��������� ����
     i = SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_LANGUAGELIST,LB_GETCURSEL,0,0);
     // ���� ���-�� �������, �� �������� ������
     if (i!=LB_ERR) SendDlgItemMessage(hwndDlg,IDC_SETTINGS_CONFIGURE_LANGUAGELIST,LB_GETTEXT,(WPARAM)i,(LPARAM)(LPCTSTR)Settings.Language);
     // ����������� ������
     char buffer [MAX_PASSWORDSYMBOLS],
          buffer2[MAX_PASSWORDSYMBOLS];
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PSWRDENTER,WM_GETTEXT,(WPARAM)MAX_PASSWORDSYMBOLS,(LPARAM)buffer);
     SendDlgItemMessage(hwndDlg,IDC_SETTINGS_PSWRDREENTER,WM_GETTEXT,(WPARAM)MAX_PASSWORDSYMBOLS,(LPARAM)buffer2);
     if (Settings.MainMenuAccess.Password[0]=='\0')
      {
       // ���� ������ ��� �� ����������
       if (buffer[0]!='\0') // ������������ ����� ���������� ������
        {
         if (lstrcmp(buffer,buffer2)==0) // ���� ������ ����� � ����������, �� ��� ����������
          {
           lstrcpy(Settings.MainMenuAccess.Password,buffer);
           MessageBox(hwndDlg,"������ ��� ������� ����������!","��������� ������",MB_ICONINFORMATION|MB_OK);
          }
        }
      }
     else
      {
       // ������ ��� ����
       if (buffer[0]=='\0') // ���� ��� ���� ������, �� ������ �� ��������
        {
         if (buffer2[0]!='\0') // ������� ������, �� ������ ���� ������ ������ ������ �� ������ ����
          {
           if (lstrcmp(Settings.MainMenuAccess.Password,buffer2)==0)
            {
             Settings.MainMenuAccess.Password[0]='\0';
             MessageBox(hwndDlg,"������ ��� ������� ������!","��������� ������",MB_ICONINFORMATION|MB_OK);
            }
           else
            MessageBox(hwndDlg,"������ �� ����� ���� ������ ��-�� ������������� �������������!","��������� ������",MB_ICONWARNING|MB_OK);
          }
        }
       else // ��������� ������
        {
         if (lstrcmp(Settings.MainMenuAccess.Password,buffer2)==0)
          {
           lstrcpy(Settings.MainMenuAccess.Password,buffer);
           MessageBox(hwndDlg,"������ ��� ������� ������!","��������� ������",MB_ICONINFORMATION|MB_OK);
          }
         else 
          MessageBox(hwndDlg,"������ �� ����� ���� ������ ��-�� ������������� �������������!","��������� ������",MB_ICONWARNING|MB_OK);
        }
      }
     // ���������� ������
     Settings.MainMenuAccess.AccessFlags = 0; // ��������� ����� ����������
     if (IsDlgButtonChecked(hwndDlg,IDC_SETTINGS_CONFIGURE_ACCESS_TRAY))     Settings.MainMenuAccess.AccessFlags=Settings.MainMenuAccess.AccessFlags|AF_TRAY;
     if (IsDlgButtonChecked(hwndDlg,IDC_SETTINGS_CONFIGURE_ACCESS_HOTKEY))   Settings.MainMenuAccess.AccessFlags=Settings.MainMenuAccess.AccessFlags|AF_HOTKEY;
     if (IsDlgButtonChecked(hwndDlg,IDC_SETTINGS_CONFIGURE_ACCESS_PASSWORD)) Settings.MainMenuAccess.AccessFlags=Settings.MainMenuAccess.AccessFlags|AF_PASSWORD;
     if (IsDlgButtonChecked(hwndDlg,IDC_SETTINGS_CONFIGURE_ACCESS_DBLRUN))   Settings.MainMenuAccess.AccessFlags=Settings.MainMenuAccess.AccessFlags|AF_DBLRUN;
    }
    break;
  }
 return false;
}
