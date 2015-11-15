/**************************************************************/
/*            qshKeyCommander1 - Addiction.cpp                */
/*------------------------------------------------------------*/
/*    �������������� ������� ��� ������ ������� Winamp.qkc    */
/*****2004***************************Copyright by <Quash!>*****/

#include "Addiction.h"
#include "winbase.h"


char FLdirectorystart[MAX_PATH]; // ���������� ���������� ��� ���������� ��������



// ������� ��������� ��������� �� ���� ������ ����� (��� ������� ����)
int CALLBACK FLGetDirectoryProc(HWND hwndDlg, UINT msg, LPARAM wParam, LPARAM lParam)
{
 switch(msg)
  {
   // ��� ������������� �������
   case BFFM_INITIALIZED:
   // ���� ������ ���������� �������� �� ������, �� �������� �������
   if (lstrlen(FLdirectorystart)) 
    SendMessage(hwndDlg, BFFM_SETSELECTION,1,(LPARAM)FLdirectorystart);
   // ������� ������ ������ ���� ����
   SetForegroundWindow(hwndDlg);
   break;
 }
 return 0;
}

// ����������� � szDir ��� ���������� ��������
BOOL FLGetDirectory(HWND hwndParent, LPTSTR szDir,LPTSTR text)
{ 
 BOOL fRet;
 TCHAR szPath[MAX_PATH];
 LPITEMIDLIST pidl;
 LPITEMIDLIST pidlRoot;
 LPMALLOC lpMalloc;
 BROWSEINFO bi;

 // ���������� ���������
 bi.hwndOwner      = hwndParent;                      // ����������� ����
 bi.pidlRoot       = NULL;                            // ID ��������
 bi.pszDisplayName = szPath;                          // ?
 bi.lpszTitle      = text;                            // ���������
 bi.ulFlags        = BIF_RETURNONLYFSDIRS;            // ������������ ��������
 bi.lpfn           = (BFFCALLBACK)FLGetDirectoryProc; // ����� �������������� ��������� ������� �������
 bi.lParam         = NULL;                            // ?
 bi.iImage         = 0;                               // ����������, ��������������� � ��������� ���������

 // ��������� ��������� szDir
 lstrcpy(FLdirectorystart,szDir);

 // ���� ���-�� �� ���, �� ������ ������� �����������
 if (SHGetSpecialFolderLocation(HWND_DESKTOP,CSIDL_DRIVES,&pidlRoot)!=0)
  return false;
 // ���� ������ �� ���� ����������, �� - �����
 if (pidlRoot==NULL) return false;

 // ��������� ���������, ��� ������ � ���������������
 bi.pidlRoot = pidlRoot;

 // �������� �������
 pidl = SHBrowseForFolder(&bi);
  if (pidl!=NULL) fRet = SHGetPathFromIDList(pidl, szDir); // ��� ����� ���� �����������
  else            fRet = false;                            // ��� ������� - �����

 // ���� ���������� �� ������, �� ��� �� �����������
 if (!SHGetMalloc(&lpMalloc)&&(lpMalloc!=NULL))
  {
   if (pidlRoot!=NULL) lpMalloc->Free(pidlRoot);
   if (NULL != pidl)   lpMalloc->Free(pidl);
   lpMalloc->Release();
  }
 return fRet;
}



// ������� ��������� ��������� �� ���� ������ ������
UINT APIENTRY FLGetFileProc(HWND hwndDlg,UINT msg,WPARAM wParam,LPARAM lParam)
{
 switch (msg)
  {
   // ����� ��� ������������� ������� ������ ���� ����
   case WM_INITDIALOG:
    SetForegroundWindow(hwndDlg); 
    break;
  }
 return 0;
}

// �������� ������� ������ �����
BOOL FLGetFile(HWND hwndParent,LPSTR szFileName,LPCSTR caption,LPCSTR filter,DWORD filterid)
{
 OPENFILENAME ofn;

 char FileName[MAX_PATH] = "";     // ��������� ����
 char InitDir[MAX_PATH]  = "C:\\"; // ��������� �������

 // ����������� ���������� ��������
 if (szFileName[0]!='\0')
  {
   lstrcpy(InitDir,szFileName);
   for (int i=lstrlen(InitDir); i>=0; i--)
    if (InitDir[i]=='\\')
     {
      InitDir[i+1]='\0';
      break;
     }
  }

 ofn.hInstance       = NULL;
 ofn.lpstrFileTitle  = NULL;
 ofn.nMaxCustFilter  = 0;    // Not supported
 ofn.lCustData       = 0;
 ofn.lpstrDefExt     = NULL;
 ofn.lpTemplateName  = NULL;
 ofn.nFileExtension  = NULL;
 ofn.nFileOffset     = 0;
 ofn.nMaxFileTitle   = NULL;

 // ���������� ���������
 ofn.lpstrInitialDir   = InitDir;                      // ��������� �������
 ofn.hwndOwner         = hwndParent;                   // ������������ ����
 ofn.lStructSize       = sizeof(OPENFILENAME);         // ������ ���������
 ofn.lpstrFilter       = filter;                       // ������
 ofn.nFilterIndex      = filterid;                     // ������ ���������� �������
 ofn.lpstrFile         = FileName;                     // ��� ���������� �����
 ofn.nMaxFile          = MAX_PATH;                     // ���������� �������� � ����� �����
 ofn.lpstrTitle        = caption;                      // ��������� ����
 ofn.Flags             = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY|OFN_ENABLEHOOK; // ����� �������
 ofn.lpfnHook          = (LPOFNHOOKPROC)FLGetFileProc; // ����� �������, �������������� ��������� �� ������� (��� ����, ����� ������� ������ ���� ����)
 ofn.lpstrCustomFilter = NULL;                         // ������, ����� ����������� �������

 // ����� ������� ������ �����
 if (GetOpenFileName(&ofn)) 
  {
   lstrcpy(szFileName,FileName);
   return true;  // �� ������
  }
 else
  return false; // ������� ������ �� ����������
}

// �������� ����� �� �������������
// true - ������ ����; false - ���� �� ������
BOOL FileExists(BOOL dir, LPTSTR pathname)
{
 WIN32_FIND_DATA finddata;
 HANDLE          hfile;

 // ������� ������
 hfile = FindFirstFile(pathname,&finddata);
 if (hfile==INVALID_HANDLE_VALUE) return false;
        if ((dir)&&(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))     return true;
   else if ((!dir)&&(!(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))) return true;
 // ����������� ������
  while (FindNextFile(hfile,&finddata))
        if ((dir)&&(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))     return true;
   else if ((!dir)&&(!(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))) return true;
 return false; // ���� �� ������
}