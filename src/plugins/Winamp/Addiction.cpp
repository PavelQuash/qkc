/**************************************************************/
/*            qshKeyCommander1 - Addiction.cpp                */
/*------------------------------------------------------------*/
/*    Дополнительные функции для работы плагина Winamp.qkc    */
/*****2004***************************Copyright by <Quash!>*****/

#include "Addiction.h"
#include "winbase.h"


char FLdirectorystart[MAX_PATH]; // Глобальная переменная для стартового каталога



// Функция обработки сообщений от окна выбора папок (для функции ниже)
int CALLBACK FLGetDirectoryProc(HWND hwndDlg, UINT msg, LPARAM wParam, LPARAM lParam)
{
 switch(msg)
  {
   // При инициализации диалога
   case BFFM_INITIALIZED:
   // Если строка стартового каталога не пустая, то выбираем каталог
   if (lstrlen(FLdirectorystart)) 
    SendMessage(hwndDlg, BFFM_SETSELECTION,1,(LPARAM)FLdirectorystart);
   // Выводим диалог поверх всех окон
   SetForegroundWindow(hwndDlg);
   break;
 }
 return 0;
}

// Возвращение в szDir имя выбранного каталога
BOOL FLGetDirectory(HWND hwndParent, LPTSTR szDir,LPTSTR text)
{ 
 BOOL fRet;
 TCHAR szPath[MAX_PATH];
 LPITEMIDLIST pidl;
 LPITEMIDLIST pidlRoot;
 LPMALLOC lpMalloc;
 BROWSEINFO bi;

 // Заполнение структуры
 bi.hwndOwner      = hwndParent;                      // Материнское окно
 bi.pidlRoot       = NULL;                            // ID каталога
 bi.pszDisplayName = szPath;                          // ?
 bi.lpszTitle      = text;                            // Заголовок
 bi.ulFlags        = BIF_RETURNONLYFSDIRS;            // Возвращаемые значения
 bi.lpfn           = (BFFCALLBACK)FLGetDirectoryProc; // Адрес обрабатываемой сообщения диалога функции
 bi.lParam         = NULL;                            // ?
 bi.iImage         = 0;                               // Переменная, ассоциированная с выбранным каталогом

 // Стартовым каталогом szDir
 lstrcpy(FLdirectorystart,szDir);

 // Если что-то не так, то работа функции завершается
 if (SHGetSpecialFolderLocation(HWND_DESKTOP,CSIDL_DRIVES,&pidlRoot)!=0)
  return false;
 // Если ничего не было возвращено, то - выход
 if (pidlRoot==NULL) return false;

 // Сообщение структуре, что ищется в вышеопределённом
 bi.pidlRoot = pidlRoot;

 // Открытие диалога
 pidl = SHBrowseForFolder(&bi);
  if (pidl!=NULL) fRet = SHGetPathFromIDList(pidl, szDir); // При удаче путь сохраняется
  else            fRet = false;                            // При неудаче - выход

 // Если переменные не пустые, то идёт их уничтожение
 if (!SHGetMalloc(&lpMalloc)&&(lpMalloc!=NULL))
  {
   if (pidlRoot!=NULL) lpMalloc->Free(pidlRoot);
   if (NULL != pidl)   lpMalloc->Free(pidl);
   lpMalloc->Release();
  }
 return fRet;
}



// Функция обработки сообщений от окна выбора файлов
UINT APIENTRY FLGetFileProc(HWND hwndDlg,UINT msg,WPARAM wParam,LPARAM lParam)
{
 switch (msg)
  {
   // Вывод при инициализации диалога поверх всех окон
   case WM_INITDIALOG:
    SetForegroundWindow(hwndDlg); 
    break;
  }
 return 0;
}

// Открытие диалога выбора файла
BOOL FLGetFile(HWND hwndParent,LPSTR szFileName,LPCSTR caption,LPCSTR filter,DWORD filterid)
{
 OPENFILENAME ofn;

 char FileName[MAX_PATH] = "";     // Выбранный файл
 char InitDir[MAX_PATH]  = "C:\\"; // Стартовый каталог

 // Определение стартового каталога
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

 // Заполнение структуры
 ofn.lpstrInitialDir   = InitDir;                      // Стартовый каталог
 ofn.hwndOwner         = hwndParent;                   // Родительское окно
 ofn.lStructSize       = sizeof(OPENFILENAME);         // Размер структуры
 ofn.lpstrFilter       = filter;                       // Фильтр
 ofn.nFilterIndex      = filterid;                     // Индекс выбранного фильтра
 ofn.lpstrFile         = FileName;                     // Имя выбранного файла
 ofn.nMaxFile          = MAX_PATH;                     // Количество символов в имени файла
 ofn.lpstrTitle        = caption;                      // Заголовок окна
 ofn.Flags             = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY|OFN_ENABLEHOOK; // Флаги диалога
 ofn.lpfnHook          = (LPOFNHOOKPROC)FLGetFileProc; // Адрес функции, обрабатывающей сообщения от диалога (для того, чтобы вывести поверх всех окон)
 ofn.lpstrCustomFilter = NULL;                         // Пустой, иначе посторонние символы

 // Вывод диалога выбора файла
 if (GetOpenFileName(&ofn)) 
  {
   lstrcpy(szFileName,FileName);
   return true;  // Всё хорошо
  }
 else
  return false; // Вывести диалог не получилось
}

// Проверка файла на существование
// true - найден файл; false - файл не найден
BOOL FileExists(BOOL dir, LPTSTR pathname)
{
 WIN32_FIND_DATA finddata;
 HANDLE          hfile;

 // Попытка искать
 hfile = FindFirstFile(pathname,&finddata);
 if (hfile==INVALID_HANDLE_VALUE) return false;
        if ((dir)&&(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))     return true;
   else if ((!dir)&&(!(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))) return true;
 // Продолжение поиска
  while (FindNextFile(hfile,&finddata))
        if ((dir)&&(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))     return true;
   else if ((!dir)&&(!(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))) return true;
 return false; // Файл не найден
}