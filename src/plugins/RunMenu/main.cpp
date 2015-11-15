/**************************************************************/
/*                 qshKeyCommander1 - main.cpp                */
/*------------------------------------------------------------*/
/*      Плагин управления Winamp2 для qshKeyCommander1        */
/*****2004***************************Copyright by <Quash!>*****/


#include "windows.h"
#include "..\qkc_plugin.h"
#include "runmenu_ids.h"
#include "resource.h"
#include "commctrl.h"


// *Отключение Runtime Library
#pragma comment(linker,"/MERGE:.rdata=.text")
#pragma comment(linker,"/FILEALIGN:512 /SECTION:.text,EWRX /IGNORE:4078")
#pragma comment(linker,"/ENTRY:DllMain")
#pragma comment(linker,"/NODEFAULTLIB")


#define CMDCOUNT 1
int  CmdCount = CMDCOUNT;    // Количество команд, используемое в плагине
char CmdList[CMDCOUNT][200]; // Список команд
HMODULE qkcDll = NULL;       // Указатель на плагин

// Подключение/отключение модуля
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
 switch (ul_reason_for_call)
  {
   case DLL_PROCESS_ATTACH:
    // Заполнение списка
    lstrcpy(CmdList[SHOWMENU],"Показать меню");
    break;

   case DLL_PROCESS_DETACH:
    break;
  }
 return true;
}

// Функция инициализации
//void Init(void)
//{ MessageBox(0,"MyPlugin Init","Info:",0); }

// Функция деинициализации
//void Quit(void)
//{ MessageBox(0,"MyPlugin Quit","Info:",0); }

// Конфигурация плагина
//void Config(void)
//{ MessageBox(0,"MyPlugin Config","Info:",0); }



// ************Удалить для второй версии
HWND hwndTmp=NULL;

// Извлекаем из данной строки заголовок меню
void RMgetcaption(LPTSTR str2in1,LPTSTR caption)
{
 // Если первый же символ '|', то обнуляем заголовок
 if (str2in1[0]!='|')
  {
   // Считаем количество символов, которое нам нужно скопировать
   int k=lstrlen(str2in1);
   for (int i=0; i<k; i++)
    if (str2in1[i]=='|') break; // Если нашли служебный символ, то выходим из цикла
   lstrcpyn(caption,str2in1,i+1);
   caption[i]='\0';
  }
 else caption[0]='\0';
}

// Извлекаем из данной строки команду для элемента меню
BOOL RMgetcommand(LPTSTR str2in1,LPTSTR command)
{
 // Считаем количество символов, которое нам нужно скопировать
 int k = lstrlen(str2in1);
 for (int i=0; i<k; i++)
  if (str2in1[i]=='|') break; // Если нашли служебный символ, то выходим из цикла
 // Нет ни '|', ни команды; либо есть '|', но команды нет
 if ((str2in1[i]!='|')||((str2in1[i]=='|')&&(str2in1[i+1]=='\0')))
  {
   lstrcpy(command,"Спрашиваем команду");
   return (true); // Необходимо сохранить команду
  }
 // Всё правильно
 else 
  {
   lstrcpy(command,str2in1+i+1); // Возвращаем команду
   return (false);              // Сохранять команду не нужно
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

// Функция обработки сообщений, поступающих диалогу 
BOOL CALLBACK RunMenuProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
 switch (message)
  {
   case WM_INITDIALOG:
    {
     /* Наиболее правильное
     HDC      scrDC = GetDC(NULL);
     WORD     index = 0; 
     ICONINFO icoinfo;

     HICON ico = ExtractAssociatedIcon(qkcDll,"C:\\DirSoft\\WinCmd5\\Totalcmd.exe",&index);
     DrawIcon(scrDC,300,300,ico);

     GetIconInfo(ico,&icoinfo);
     HBITMAP memBM = icoinfo.hbmColor;
     */
     /* Пока самое лучшее
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


    

     BOOL mexit;                // Флаг, разрешающий выход из цикла while
     int  iget,iset;            // Счётчики элементов меню
     char istr[10];             // i в строковом виде
     char inikey[20];           // Текущий ключ в ини файле
     char str2in1[MAX_PATH+40]; // Считываемая строка
     char cptn[40];             // Заголовок элемента меню
     char cmd[MAX_PATH];
     HMENU rmenu;               // Меню
     MENUITEMINFO rmenuitem;    // Информация о добавляемом элементе меню

     iget=0;
     iset=0;
     mexit=false;

     char buffer[MAX_PATH];
     // Загрузка данных из ini файла
     GetModuleFileName(qkcDll,buffer,sizeof(buffer));
     for (int i=lstrlen(buffer); i>=0; i--)
     if (buffer[i]=='\\') // Избавление от имени файла
      {
       buffer[i+1]='\0';
       break;
      }
     lstrcat(buffer,"plugins.ini");

     rmenu   = CreatePopupMenu();
 
     // Заполняем поля, одинаковые для строкового типа и сепаратора
     rmenuitem.cbSize = sizeof(MENUITEMINFO);
     rmenuitem.fState = MFS_DEFAULT;
 
     // Добавляем в меню элементы
     while (!mexit)
      {
       iget++;
       wsprintf(istr,"%i",iget);
       lstrcpy(inikey,"item");
       lstrcat(inikey,istr);
       GetPrivateProfileString("RunMenu",inikey,"_",str2in1,sizeof(str2in1),buffer);
       // Если строка по умолчанию, то выходим из цикла
       if (lstrcmp(str2in1,"_")!=0)
        {
         // Если строка пустая, то пропускаем её
         if (lstrcmp(str2in1,"")==0) continue;
         else iset++;
         // Если строка равна одному символу '-', то ставим сепаратор
         if (lstrcmp(str2in1,"-")==0) 
          {     
           // Добавляем разделитель
           rmenuitem.fMask  = MIIM_TYPE;
           rmenuitem.fType  = MFT_SEPARATOR;
           InsertMenuItem(rmenu,iset-1,true,&rmenuitem);
          }
         else 
          {
           // Если строка не пустая, то обрабатываем её -> добавляем в меню
           RMgetcaption(str2in1,cptn); // Если строка пустая, то ничего не делаем
           RMgetcommand(str2in1,cmd);
 
           SHGetFileInfo(cmd,0,&shfi,sizeof(shfi),SHGFI_ICON|SHGFI_SMALLICON|SHGFI_SHELLICONSIZE);
           GetIconInfo(shfi.hIcon,&icoinfo);
           memCH   = icoinfo.hbmMask;
           memUNCH = icoinfo.hbmColor;

           // Добавляем строку MIIM_DATA|
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
       // Иначе - выходим из цикла
       else
        {
         mexit=true;
         iget--;
        }
      }
     // Определяем координаты вывода меню (координаты курсора)
     POINT curpos;
     GetCursorPos(&curpos);
     // Выводим готовое меню
     SetForegroundWindow(hwndDlg);
     TrackPopupMenu(rmenu,NULL,curpos.x+8,curpos.y+20,0,hwndDlg,NULL);
    }
    return true;

   case WM_EXITMENULOOP: // Закрытие меню
    SetForegroundWindow(hwndTmp);
    SendMessage(hwndDlg,WM_CLOSE,0,0);
    break;

   case WM_COMMAND:
    {
     // Обработка сообщений от меню
     if (HIWORD(wParam)==0)
      {
       char buffer[MAX_PATH];
       // Загрузка данных из ini файла
       GetModuleFileName(qkcDll,buffer,sizeof(buffer));
       for (int i=lstrlen(buffer); i>=0; i--)
       if (buffer[i]=='\\') // Избавление от имени файла
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
       char rdstr[MAX_PATH+40]; // Считываемая строка
       // Формируем ключ в cfg файле
       wsprintf(num,"%i",itemindex);
       lstrcpy(inikey,"item");
       lstrcat(inikey,num);
       GetPrivateProfileString("RunMenu",inikey,"",rdstr,sizeof(rdstr),buffer);
       if (RMgetcommand(rdstr,cmdstr))
        {
         // Если нужно сохранить команду, то делаем это DEBUG
        // rdstr+|+command
         MessageBox(0,"Сохранение команды в файл","debug",NULL);
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

// Запрос на выполнение команды под номером cmdid
void Command(int cmdid)
{
 switch (cmdid)
  {
   case SHOWMENU:
    {
     hwndTmp = GetForegroundWindow(); // Запоминаем активное окно
     CreateDialog(qkcDll,MAKEINTRESOURCE(IDD_SERVER),0,(DLGPROC)RunMenuProc);
    }
    break;

   // Реакция на постороннее сообщение (на всякий случай)
   default:
    break;
  }
}



#ifdef __cplusplus
extern "C"
{
#endif

// Функция на экспорт, вызываемая плагином по умолчанию
__declspec (dllexport) void GetModuleData(struct stPluginData *PluginData)
{
 // Заполнение заголовка
 PluginData->Caption = "RunMenu v1.05 (06.06.2004г.)\n\n© <Quash!>, г.Екатеринбург";

 // Передача адресов функций qshKeyCommander'у
 PluginData->Init          = NULL;//Init;
 PluginData->Quit          = NULL;//Quit;
 PluginData->Config        = NULL;//Config;
 PluginData->Command       = Command;

 // Передача адресов списка и количества его элементов в памяти
 PluginData->CommandsCount = &CmdCount;
 PluginData->CommandsList  = CmdList;

 qkcDll = PluginData->hDll; // Сохранение указателя на плагин
}

#ifdef __cplusplus
}
#endif