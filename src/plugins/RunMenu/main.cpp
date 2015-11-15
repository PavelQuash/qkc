/**************************************************************/
/*                 qshKeyCommander1 - main.cpp                */
/*------------------------------------------------------------*/
/*      ������ ���������� Winamp2 ��� qshKeyCommander1        */
/*****2004***************************Copyright by <Quash!>*****/


#include "windows.h"
#include "..\qkc_plugin.h"
#include "runmenu_ids.h"
#include "resource.h"
#include "commctrl.h"


// *���������� Runtime Library
#pragma comment(linker,"/MERGE:.rdata=.text")
#pragma comment(linker,"/FILEALIGN:512 /SECTION:.text,EWRX /IGNORE:4078")
#pragma comment(linker,"/ENTRY:DllMain")
#pragma comment(linker,"/NODEFAULTLIB")


#define CMDCOUNT 1
int  CmdCount = CMDCOUNT;    // ���������� ������, ������������ � �������
char CmdList[CMDCOUNT][200]; // ������ ������
HMODULE qkcDll = NULL;       // ��������� �� ������

// �����������/���������� ������
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
 switch (ul_reason_for_call)
  {
   case DLL_PROCESS_ATTACH:
    // ���������� ������
    lstrcpy(CmdList[SHOWMENU],"�������� ����");
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



// ************������� ��� ������ ������
HWND hwndTmp=NULL;

// ��������� �� ������ ������ ��������� ����
void RMgetcaption(LPTSTR str2in1,LPTSTR caption)
{
 // ���� ������ �� ������ '|', �� �������� ���������
 if (str2in1[0]!='|')
  {
   // ������� ���������� ��������, ������� ��� ����� �����������
   int k=lstrlen(str2in1);
   for (int i=0; i<k; i++)
    if (str2in1[i]=='|') break; // ���� ����� ��������� ������, �� ������� �� �����
   lstrcpyn(caption,str2in1,i+1);
   caption[i]='\0';
  }
 else caption[0]='\0';
}

// ��������� �� ������ ������ ������� ��� �������� ����
BOOL RMgetcommand(LPTSTR str2in1,LPTSTR command)
{
 // ������� ���������� ��������, ������� ��� ����� �����������
 int k = lstrlen(str2in1);
 for (int i=0; i<k; i++)
  if (str2in1[i]=='|') break; // ���� ����� ��������� ������, �� ������� �� �����
 // ��� �� '|', �� �������; ���� ���� '|', �� ������� ���
 if ((str2in1[i]!='|')||((str2in1[i]=='|')&&(str2in1[i+1]=='\0')))
  {
   lstrcpy(command,"���������� �������");
   return (true); // ���������� ��������� �������
  }
 // �� ���������
 else 
  {
   lstrcpy(command,str2in1+i+1); // ���������� �������
   return (false);              // ��������� ������� �� �����
  }
}

BOOL OsTypeNT(void)
{
 OSVERSIONINFO vi;
 vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
 GetVersionEx(&vi);
 if (vi.dwPlatformId==VER_PLATFORM_WIN32_NT) return true;
 return false;
}

// ������� ��������� ���������, ����������� ������� 
BOOL CALLBACK RunMenuProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
 switch (message)
  {
   case WM_INITDIALOG:
    {
     /* �������� ����������
     HDC      scrDC = GetDC(NULL);
     WORD     index = 0; 
     ICONINFO icoinfo;

     HICON ico = ExtractAssociatedIcon(qkcDll,"C:\\DirSoft\\WinCmd5\\Totalcmd.exe",&index);
     DrawIcon(scrDC,300,300,ico);

     GetIconInfo(ico,&icoinfo);
     HBITMAP memBM = icoinfo.hbmColor;
     */
     /* ���� ����� ������
     WORD  index = 0; 
     HICON ico = ExtractAssociatedIcon(qkcDll,"C:\\DirSoft\\WinCmd5\\Totalcmd.exe",&index);
     IMAGEINFO imgnfo;

     HIMAGELIST imglst = ImageList_Create(16,16,ILC_COLOR,0,1);
     ImageList_ReplaceIcon(imglst,-1,ico);
     ImageList_GetImageInfo(imglst,0,&imgnfo);
     HBITMAP memBM = imgnfo.hbmImage;
     */
     SHFILEINFO shfi;
     ICONINFO icoinfo;
     HBITMAP memCH;
     HBITMAP memUNCH;


    

     BOOL mexit;                // ����, ����������� ����� �� ����� while
     int  iget,iset;            // �������� ��������� ����
     char istr[10];             // i � ��������� ����
     char inikey[20];           // ������� ���� � ��� �����
     char str2in1[MAX_PATH+40]; // ����������� ������
     char cptn[40];             // ��������� �������� ����
     char cmd[MAX_PATH];
     HMENU rmenu;               // ����
     MENUITEMINFO rmenuitem;    // ���������� � ����������� �������� ����

     iget=0;
     iset=0;
     mexit=false;

     char buffer[MAX_PATH];
     // �������� ������ �� ini �����
     GetModuleFileName(qkcDll,buffer,sizeof(buffer));
     for (int i=lstrlen(buffer); i>=0; i--)
     if (buffer[i]=='\\') // ���������� �� ����� �����
      {
       buffer[i+1]='\0';
       break;
      }
     lstrcat(buffer,"plugins.ini");

     rmenu   = CreatePopupMenu();
 
     // ��������� ����, ���������� ��� ���������� ���� � ����������
     rmenuitem.cbSize = sizeof(MENUITEMINFO);
     rmenuitem.fState = MFS_DEFAULT;
 
     // ��������� � ���� ��������
     while (!mexit)
      {
       iget++;
       wsprintf(istr,"%i",iget);
       lstrcpy(inikey,"item");
       lstrcat(inikey,istr);
       GetPrivateProfileString("RunMenu",inikey,"_",str2in1,sizeof(str2in1),buffer);
       // ���� ������ �� ���������, �� ������� �� �����
       if (lstrcmp(str2in1,"_")!=0)
        {
         // ���� ������ ������, �� ���������� �
         if (lstrcmp(str2in1,"")==0) continue;
         else iset++;
         // ���� ������ ����� ������ ������� '-', �� ������ ���������
         if (lstrcmp(str2in1,"-")==0) 
          {     
           // ��������� �����������
           rmenuitem.fMask  = MIIM_TYPE;
           rmenuitem.fType  = MFT_SEPARATOR;
           InsertMenuItem(rmenu,iset-1,true,&rmenuitem);
          }
         else 
          {
           // ���� ������ �� ������, �� ������������ � -> ��������� � ����
           RMgetcaption(str2in1,cptn); // ���� ������ ������, �� ������ �� ������
           RMgetcommand(str2in1,cmd);
 
           SHGetFileInfo(cmd,0,&shfi,sizeof(shfi),SHGFI_ICON|SHGFI_SMALLICON|SHGFI_SHELLICONSIZE);
           GetIconInfo(shfi.hIcon,&icoinfo);
           memCH   = icoinfo.hbmMask;
           memUNCH = icoinfo.hbmColor;

           // ��������� ������ MIIM_DATA|
           rmenuitem.fMask      = MIIM_ID|MIIM_TYPE|MIIM_CHECKMARKS;
           rmenuitem.fType      = MFT_STRING;
           rmenuitem.cch        = lstrlen(cptn);
           rmenuitem.dwTypeData = cptn;
           rmenuitem.wID        = iget+60000;
           rmenuitem.hbmpChecked   = memUNCH;
           rmenuitem.hbmpUnchecked = memUNCH;
//           SetMenuItemBitmaps(rmenu,iset-1,MF_BYPOSITION,memUNCH,memCH);
           InsertMenuItem(rmenu,iset-1,true,&rmenuitem);
          }
        }
       // ����� - ������� �� �����
       else
        {
         mexit=true;
         iget--;
        }
      }
     // ���������� ���������� ������ ���� (���������� �������)
     POINT curpos;
     GetCursorPos(&curpos);
     // ������� ������� ����
     SetForegroundWindow(hwndDlg);
     TrackPopupMenu(rmenu,NULL,curpos.x+8,curpos.y+20,0,hwndDlg,NULL);
    }
    return true;

   case WM_EXITMENULOOP: // �������� ����
    SetForegroundWindow(hwndTmp);
    SendMessage(hwndDlg,WM_CLOSE,0,0);
    break;

   case WM_COMMAND:
    {
     // ��������� ��������� �� ����
     if (HIWORD(wParam)==0)
      {
       char buffer[MAX_PATH];
       // �������� ������ �� ini �����
       GetModuleFileName(qkcDll,buffer,sizeof(buffer));
       for (int i=lstrlen(buffer); i>=0; i--)
       if (buffer[i]=='\\') // ���������� �� ����� �����
        {
         buffer[i+1]='\0';
         break;
        }
       lstrcat(buffer,"plugins.ini");

       int itemindex;
       char cmdstr[MAX_PATH];
       itemindex=(UINT)LOWORD(wParam);

       itemindex=itemindex-60000;
       char num[10];
       char inikey[100];
       char rdstr[MAX_PATH+40]; // ����������� ������
       // ��������� ���� � cfg �����
       wsprintf(num,"%i",itemindex);
       lstrcpy(inikey,"item");
       lstrcat(inikey,num);
       GetPrivateProfileString("RunMenu",inikey,"",rdstr,sizeof(rdstr),buffer);
       if (RMgetcommand(rdstr,cmdstr))
        {
         // ���� ����� ��������� �������, �� ������ ��� DEBUG
        // rdstr+|+command
         MessageBox(0,"���������� ������� � ����","debug",NULL);
        }
       WinExec(cmdstr,SW_SHOWNORMAL);
      }
    }
    return true;

   case WM_CLOSE:   
    EndDialog(hwndDlg,0);
	 		return true;
  }
 return false;
}

// ********************************

// ������ �� ���������� ������� ��� ������� cmdid
void Command(int cmdid)
{
 switch (cmdid)
  {
   case SHOWMENU:
    {
     hwndTmp = GetForegroundWindow(); // ���������� �������� ����
     CreateDialog(qkcDll,MAKEINTRESOURCE(IDD_SERVER),0,(DLGPROC)RunMenuProc);
    }
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
 PluginData->Caption = "RunMenu v1.05 (06.06.2004�.)\n\n� <Quash!>, �.������������";

 // �������� ������� ������� qshKeyCommander'�
 PluginData->Init          = NULL;//Init;
 PluginData->Quit          = NULL;//Quit;
 PluginData->Config        = NULL;//Config;
 PluginData->Command       = Command;

 // �������� ������� ������ � ���������� ��� ��������� � ������
 PluginData->CommandsCount = &CmdCount;
 PluginData->CommandsList  = CmdList;

 qkcDll = PluginData->hDll; // ���������� ��������� �� ������
}

#ifdef __cplusplus
}
#endif