/**************************************************************/
/*                 qshKeyCommander1 - main.cpp                */
/*------------------------------------------------------------*/
/*      Плагин управления Winamp2 для qshKeyCommander1        */
/*****2004***************************Copyright by <Quash!>*****/

#include "../qkc_plugin.h" // Структура данных плагина для QshKeyCommander
#include "windows.h"       // Стандарт :)
#include "Winamp_ids.h"    // Идентификаторы команд
#include "Frontend.h"      // Заголовочный файл Winamp
#include "resource.h"      // Файл ресурсов
#include "commctrl.h"      // Для прогресс бара
#include "Addiction.h"     // Дополнительные функции



// *Отключение Runtime Library
#pragma comment(linker,"/MERGE:.rdata=.text")
#pragma comment(linker,"/FILEALIGN:512 /SECTION:.text,EWRX /IGNORE:4078")
#pragma comment(linker,"/ENTRY:DllMain")
#pragma comment(linker,"/NODEFAULTLIB")



#define plugin_section "Winamp" // Секция настроек для плагина
#define CMDCOUNT 55             // Количество команд, используемое в плагине

// Идентификаторы окон
#define SHW_MAIN      1
#define SHW_EQUALIZER 2
#define SHW_PLAYLIST  4
#define SHW_BROWSER   8

struct stSettings
{
 char Winamp_FileName[MAX_PATH];  // Полный путь к исполняемому файлу Winamp
 char Winamp_ClassName[MAX_PATH]; // Имя класса главного окна Winamp

 char LikeDir[MAX_PATH];          // Полный путь к каталогу для воспроизведения
 char LikeFile[MAX_PATH];         // Полный путь к файлу для воспроизведения

 int  SH_flags;                   // debug Флаги показа/скрытия окон

 int  CurrentSong_UpdateTime;     // Обновление диалога CurrentSong
 int  xCurSong, yCurSong;         // Координаты окна информации о текущей песне
} Settings;                       // Настройки модуля

HMODULE hDll;                   // Указатель на dll
int     CmdCount = CMDCOUNT;    // Количество команд
char    CmdList[CMDCOUNT][200]; // Список команд

char    Ini_FileName[MAX_PATH]; // Полный путь к файлу инициализации
HWND    hwndCurrentSong = NULL; // Дмалог, отображающий состояние плеера

// Описание функций
BOOL CALLBACK DlgConfigProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);


// Подключение/отключение модуля
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
 switch (ul_reason_for_call)
  {
   case DLL_PROCESS_ATTACH:
    {
     // Заполнение списка
     lstrcpy(CmdList[STARTCLOSEWINAMP],      "Запустить/закрыть Winamp");
     lstrcpy(CmdList[RESTARTWINAMP],         "Перезагрузить Winamp");
     lstrcpy(CmdList[BACK5SEC],              "Назад на 5 секунд");
     lstrcpy(CmdList[FORW5SEC],              "Вперёд на 5 секунд");
     lstrcpy(CmdList[PLAY],                  "Воспроизведение");
     lstrcpy(CmdList[PAUSE],                 "Пауза");
     lstrcpy(CmdList[STOP],                  "Остановить воспроизведение");
     lstrcpy(CmdList[STOPAFTERSONG],         "Остановить воспроизведение листа после текущей песни");
     lstrcpy(CmdList[STOPFADEOUT],           "Остановить воспроизведение с плавным понижением громкости (Fadeout)");
     lstrcpy(CmdList[VOLUMEUP],              "Повысить громкость");
     lstrcpy(CmdList[VOLUMEDOWN],            "Понизить громкость");

     lstrcpy(CmdList[PREVSONG],              "Предыдущая песня");
     lstrcpy(CmdList[NEXTSONG],              "Следующая песня");
     lstrcpy(CmdList[GOTOFIRSTSONG],         "Перейти на первую песню Playlist'а");
     lstrcpy(CmdList[GOTOLASTSONG],          "Перейти на последнюю песню Playlist'а");
     lstrcpy(CmdList[BACK10TRACKS],          "Перейти назад на 10 песен");
     lstrcpy(CmdList[FORW10TRACKS],          "Перейти вперёд на 10 песен");
     lstrcpy(CmdList[JUMPTOTIME],            "Перейти ко времени");
     lstrcpy(CmdList[JUMPTOFILE],            "Поиск элемента Playlist");

     lstrcpy(CmdList[HSCURSONGINFO],         "Показать/скрыть информацию о текущей песне");
     lstrcpy(CmdList[SHOWFILEINFO],          "Показать информацию о файле");

     lstrcpy(CmdList[EXECUTEVISUALISATION],  "Включить/выключить плагин визуализации");
     lstrcpy(CmdList[CONFIGUREVISUALISATION],"Настроить текущий плагин визуализации");

     lstrcpy(CmdList[OPENADDRESSINTERNET],   "Открыть адрес Internet");
     lstrcpy(CmdList[OPENFILES],             "Открыть файлы");
     lstrcpy(CmdList[OPENDIRECTORY],         "Открыть каталог");
     lstrcpy(CmdList[PLAYAUDIOCD],           "Открыть компакт диск");
     lstrcpy(CmdList[ADDTOBOOKMARK],         "Добавить текущую песню в закладки (Bookmarks)");

     lstrcpy(CmdList[TIMEELAPSED],           "Переключить счётчик в режим 'прошло времени'");
     lstrcpy(CmdList[TIMEREMAINING],         "Переключить счётчик в режим 'осталось времени'");
     lstrcpy(CmdList[SHUFFLE],               "Включить/выключить случайное воспроизведение (Shuffle)");
     lstrcpy(CmdList[LOOP],                  "Включить/выключить воспроизведение циклом (Loop)");
     lstrcpy(CmdList[CLEARPLAYLIST],         "Очистить список (Playlist)");
     lstrcpy(CmdList[SOUNDOFF],              "Выключить звук (минимальная громкость)");
     lstrcpy(CmdList[SOUNDON],               "Включить звук (максимальная громкость)");

     lstrcpy(CmdList[HELP],                  "Вывести помощь по Winamp");
     lstrcpy(CmdList[MAINMENU],              "Вывести главное меню Winamp");
     lstrcpy(CmdList[DOUBLESIZE],            "Включить/выключить двойной размер");
     lstrcpy(CmdList[ALWAYSONTOP],           "Включить/выключить режим 'поверх всех окон'");

     lstrcpy(CmdList[MINIMIZERESTORE],       "Свернуть/развернуть окна Winamp");
     lstrcpy(CmdList[HSWINAMP],              "Показать/скрыть Winamp");
     lstrcpy(CmdList[HSEQUALISER],           "Показать/скрыть Equalizer");
     lstrcpy(CmdList[HSPLAYLIST],            "Показать/скрыть Playlist");
     lstrcpy(CmdList[HSMINIBROWSER],         "Показать/скрыть Minibrowser");
     lstrcpy(CmdList[HSWAEQPL],              "Показать/скрыть окна указанные в конфигурации");
     lstrcpy(CmdList[WSWINAMP],              "Переключить размер окна Winamp");
     lstrcpy(CmdList[WSEQUALISER],           "Переключить размер окна Equaliser");
     lstrcpy(CmdList[WSPLAYLIST],            "Переключить размер окна Playlist");

     lstrcpy(CmdList[HSOPTIONS],             "Открыть/закрыть окно настроек (Preferences)");
     lstrcpy(CmdList[OPTINDICATOR],          "Открыть настройки внешнего вида (Visualisation)");
     lstrcpy(CmdList[OPTVISULISATION],       "Открыть настройки плагинов визуализации (Visualisation Plugins)");
     lstrcpy(CmdList[OPTSKINS],              "Открыть выбор шкуры (Skins)");
     lstrcpy(CmdList[OPTBOOKMARKS],          "Открыть редактор закладок (Bookmarks)");

     lstrcpy(CmdList[LIKEDIR],               "Открыть любимый каталог");
     lstrcpy(CmdList[LIKEFILE],              "Открыть любимый плейлист");

     // Загрузка данных из ini файла
     GetModuleFileName(hDll,Ini_FileName,sizeof(Ini_FileName));
     for (int i=lstrlen(Ini_FileName); i>=0; i--)
      if (Ini_FileName[i]=='\\') // Избавление от имени файла
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
     // Сохранение данных в ini файл
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

// (ждёт) Передача сообщения окну Winamp'а для выполнения команды
bool WinampCommandSend(int command)
{
 // Поиск окна Winamp
	HWND hwndWA	= FindWindow(Settings.Winamp_ClassName,NULL);
 // Если окно найдено, то сообщение посылается, иначе ничего не происходит
	if (hwndWA) 
  {
   SendMessage(hwndWA,WM_COMMAND,command,0);
   return true;
  }
 else
  return false;
}

// (не ждёт) Передача сообщения окну Winamp'а для выполнения команды
bool WinampCommandPost(int command)
{
 // Поиск окна Winamp
	HWND hwndWA	= FindWindow(Settings.Winamp_ClassName,NULL);
 // Если окно найдено, то сообщение посылается, иначе ничего не происходит
	if (hwndWA) 
  {
   PostMessage(hwndWA,WM_COMMAND,command,0);
   return true;
  }
 else
  return false;
}

// (ждёт) Передача сообщения окну Winamp'а для получения/установки параметров Winamp
int WinampUserSend(int data, int id)
{
 // Поиск окна Winamp
	HWND hwndWA = FindWindow(Settings.Winamp_ClassName,NULL);
		// Если окно найдено, то сообщение посылается, иначе ничего не происходит
	if (hwndWA) return (SendMessage(hwndWA,WM_USER,data,id));
	else return (-1);
}

// (не ждёт) Передача сообщения окну Winamp'а для получения/установки параметров Winamp
int WinampUserPost(int data, int id)
{
 // Поиск окна Winamp
	HWND hwndWA = FindWindow(Settings.Winamp_ClassName,NULL);
	// Если окно найдено, то сообщение посылается, иначе ничего не происходит
	if (hwndWA) return (PostMessage(hwndWA,WM_USER,data,id));
	else return (-1);
}

// Запустить/закрыть Winamp
// -1   - Окно закрывается или был выведен диалог
// 0-31 - Ошибка
// >31  - Окно не было найдено, попытка запуска
int WinampStartClose()
{
 // Если окно существует, то нужно закрыть, иначе запускается программа
 if (FindWindow(Settings.Winamp_ClassName,NULL)) 
  {
   WinampCommandPost(IPC_CLOSEWINAMP); // Попытка выключить Winamp
   return (-1);
  }
 else
  {
   if (FileExists(0,Settings.Winamp_FileName))
    return WinExec(Settings.Winamp_FileName,SW_NORMAL);
   else
    {
     if (MessageBox(0,"Исполняемый файл Winamp не был найден. Вы хотите его указать?","Запуск Winamp",MB_SETFOREGROUND|MB_ICONQUESTION|MB_YESNO)==IDYES)
      DialogBox(hDll,MAKEINTRESOURCE(IDD_CONFIG),0,(DLGPROC)DlgConfigProc);
     return (-1);
    }
  }
}

// Обновление окна CurrentSong
void CurrentSong_Update()
{
 // Временные переменные
	char buffer [MAX_PATH],
      buffer2[MAX_PATH];
 int  i1,i2;

	// Поиск окна Winamp
	HWND hwndWA = FindWindow(Settings.Winamp_ClassName,NULL);
 
 // *Обновление диалога, если окно существует

 // Состояние плеера => ВОСПРОИЗВЕДЕНИЕ, ПАУЗА, ОСТАНОВЛЕН
 if (IsWindow(hwndWA))
  switch(WinampUserSend(0,IPC_ISPLAYING))
   {
    case 1:  lstrcpy(buffer,"ВОСПРОИЗВЕДЕНИЕ"); break;
    case 3:  lstrcpy(buffer,"ПАУЗА");           break;
    default: lstrcpy(buffer,"ОСТАНОВЛЕН");      break;
   }
 else lstrcpy(buffer,"НЕ ЗАПУЩЕН");
	GetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_STATUS,buffer2,sizeof(buffer2));
 if (lstrcmp(buffer,buffer2)!=0)
		SetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_STATUS,buffer);

 // Номер песни => "(##/##)"
 if (IsWindow(hwndWA))
  {
   i1 = WinampUserSend(0,IPC_GETLISTPOS)+1;  // Номер текущей песни
   i2 = WinampUserSend(0,IPC_GETLISTLENGTH); // Всего в листе песен
   wsprintf(buffer,"(%i/%i)",i1,i2);
  }
 else lstrcpy(buffer,"");
 GetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_SONGSCOUNTER,buffer2,sizeof(buffer2));
		if (lstrcmp(buffer,buffer2)!=0)
			SetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_SONGSCOUNTER,buffer);

 // Заголовок песни => заголовок окна Winamp
 if (IsWindow(hwndWA))
  {
			GetWindowText(hwndWA,buffer,sizeof(buffer));
   // Избавление от номера песни
 	 i1 = WinampUserSend(0,IPC_GETLISTPOS)+1; // Номер текущей песни
   wsprintf(buffer2,"%i",i1);
   lstrcpy(buffer2,buffer+lstrlen(buffer2)+2); // Сдвиг строки на количество символов в номере + ещё 2 символа ". "
   // Избавление от лишнего в конце строки
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

 // Длина песни
 if (IsWindow(hwndWA))
  {
   // Прогресс
   i1 = WinampUserSend(1,IPC_GETOUTPUTTIME); // Длина песни в секундах
   i2 = i1/60;                               // Перевод в минуты
			SendDlgItemMessage(hwndCurrentSong,IDD_CURRENTSONGINFO_PROGRESS,PBM_SETRANGE,0,MAKELPARAM(0, i1)); // Прогресс - максимальное значение
   // Метка
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

 // Текущая позиция
 if (IsWindow(hwndWA))
  {
   // Прогресс
	  i1 = WinampUserSend(0,105)/1000; // Текущая позиция милисекундах, перевод в секунды
   i2 = i1/60;                  // Перевод в минуты
			SendDlgItemMessage(hwndCurrentSong,IDD_CURRENTSONGINFO_PROGRESS,PBM_SETPOS, WPARAM(i1),0); // Прогресс - позиция
   // Метка
   wsprintf(buffer,"%i:",i2);
   i2 = i1-60*i2;
   if (i2<10) lstrcat(buffer,"0");
   wsprintf(buffer2,"%i",i2);
   lstrcat(buffer,buffer2);
  }
 else 
  {
  	SendDlgItemMessage(hwndCurrentSong,IDD_CURRENTSONGINFO_PROGRESS,PBM_SETPOS, WPARAM(0),0); // Прогресс - позиция
   lstrcpy(buffer,"0:00");
  }
	GetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_POSITION,buffer2,sizeof(buffer2));
	if (lstrcmp(buffer,buffer2)!=0)
	 SetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_POSITION,buffer);

 // Битрейт
 if (IsWindow(hwndWA))
  {
			i1 = WinampUserSend(1,IPC_GETINFO); // Битрейт
   wsprintf(buffer,"%i Кбит/сек.",i1);
			if (i1>=1400) lstrcat(buffer," (Audio CD quality)");
  }
 else lstrcpy(buffer,"");
 GetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_BITRATE,buffer2,sizeof(buffer2));
 if (lstrcmp(buffer,buffer2)!=0)
		SetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_BITRATE,buffer);

 // Сэмплрейт и количество каналов
 if (IsWindow(hwndWA))
  {
   i1 = WinampUserSend(0,IPC_GETINFO); // Сэмплрейт
   i2 = WinampUserSend(2,IPC_GETINFO); // Количество каналов
   wsprintf(buffer,"%i  кГц",i1);
			if (i2==1) lstrcat(buffer," (Моно)");
			else       lstrcat(buffer," (Стерео)");
  }
 else lstrcpy(buffer,"");
 GetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_SAMPLERATE,buffer2,sizeof(buffer2));
 if (lstrcmp(buffer,buffer2)!=0)
		SetDlgItemText(hwndCurrentSong,IDD_CURRENTSONGINFO_SAMPLERATE,buffer);

 // Статус значений Shuffle и Loop
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

// Обновление диалога через заданный промежуток времени
void CALLBACK CurrentSong_TimerProc(HWND hwnd, UINT msg, UINT idTimer, DWORD dwTime)
{	CurrentSong_Update(); }

// Функция инициализации
//void Init(void)
//{ MessageBox(0,"Winamp2 Init","Info:",0); }

// Функция деинициализации
void Quit(void)
{
 // Закрытие диалога текущей песни, если он открыт
 if (IsWindow(hwndCurrentSong)) DestroyWindow(hwndCurrentSong);
}

// Функция обработки сообщений, поступающих диалогу 
BOOL CALLBACK DlgCurrentSongProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
 switch (message)
  {
   case WM_INITDIALOG:
   	// Установка времени обновления окна
    SetTimer(hwndDlg,1,Settings.CurrentSong_UpdateTime,(TIMERPROC)CurrentSong_TimerProc);
    break;
   case WM_COMMAND: // Обработка нажатий кнопок в главном диалоге
    switch (LOWORD(wParam))
     {
      case IDD_CURRENTSONGINFO_OK:
       // Сохранение координат диалога
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

// Функция обработки сообщений, поступающих диалогу 
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
      case IDC_BTN_GETWA_FILENAME_HD: // Поиск exe файла
       FLGetFile(hwndDlg,Settings.Winamp_FileName,"Выберите исполняемый файл Winamp","Проигрыватель Winamp (winamp.exe)\0winamp.exe\0Исполняемые файлы (*.exe)\0*.exe\0\0",0);
       SendDlgItemMessage(hwndDlg,IDC_WINAMPFILENAME,WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.Winamp_FileName);
       break;
	  case IDC_BTN_GETWA_FILENAME_REGISTRY: // Извлечение из реестра
InstallDirRegKey HKLM\Software\Microsoft\Windows\CurrentVersion\Uninstall\Winamp ("UninstallString")
или
current_user\software\winamp
       break;
      case IDC_BTN_LIKEDIR:  // Выбор директории
       FLGetDirectory(hwndDlg,Settings.LikeDir,"Выберите директорию для быстрого доступа в Winamp");
       SendDlgItemMessage(hwndDlg,IDC_LIKEDIR, WM_SETTEXT,0,(LPARAM)(LPCTSTR)Settings.LikeDir);
       break;
      case IDC_BTN_LIKEFILE: // Выбор файла
       FLGetFile(hwndDlg,Settings.LikeFile,"Выберите файл, который следует открыть в Winamp","Плейлисты Winamp (*.m3u)\0*.m3u\0Файлы mp3 (*.mp3)\0*.mp3\0Все файлы (*.*)\0*.*\0\0",0);
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

// Конфигурация плагина
void Config(void)
{
 DialogBox(hDll,MAKEINTRESOURCE(IDD_CONFIG),0,(DLGPROC)DlgConfigProc);
}

// Запрос на выполнение команды
void Command(int cmdid)
{
 switch (cmdid)
  {
   // Запуск Winamp
   case STARTCLOSEWINAMP:
    WinampStartClose();
    break;
   case RESTARTWINAMP:
    WinampUserPost(0,IPC_RESTARTWINAMP);
    break;

   // Управление текущей песней
   case BACK5SEC:
    WinampCommandPost(WINAMP_REW5S);
    break;
   case FORW5SEC:
    WinampCommandPost(WINAMP_FFWD5S);
    break;
   case PLAY:
    // Если окно Winamp не было найдено, то попытка запустить Winamp
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

   // Перемещение по песням
   case PREVSONG:
    WinampCommandPost(WINAMP_PREVTRACK);
    break; 
   case NEXTSONG:
    WinampCommandPost(WINAMP_NEXTTRACK);
    break;
   case GOTOFIRSTSONG:
    WinampUserSend(0,IPC_SETPLAYLISTPOS); // Самый первый элемент
    WinampCommandPost(WINAMP_PLAY);
    break;
   case GOTOLASTSONG:
    WinampUserSend(WinampUserSend(0,IPC_GETLISTLENGTH)-1,IPC_SETPLAYLISTPOS); // Самый последний элемент
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

   // Информация
   case HSCURSONGINFO:
   	// Если диалог уже открыт...
	   if (IsWindow(hwndCurrentSong)) 
	    {
      // ... то его нужно закрыть
		    DestroyWindow(hwndCurrentSong);
     }
    else
     {
  	   // ... иначе идёт создание окна                    
     	hwndCurrentSong = CreateDialog(hDll,MAKEINTRESOURCE(IDD_CURRENTSONGINFO),0,(DLGPROC)DlgCurrentSongProc);
     	// Обновляем данные окна
      CurrentSong_Update();
     	// Вывод окна поверх всех окон
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

   // Визуализация
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

   // Открытие
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

   // Косметические
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

   // Служебные
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

   // Управление окнами Winamp
   case MINIMIZERESTORE:
    {
     HWND            hwnd_wa;
     WINDOWPLACEMENT hwnd_wa_plac;
     hwnd_wa_plac.length = sizeof(WINDOWPLACEMENT);
     hwnd_wa = FindWindow(Settings.Winamp_ClassName,NULL);
     // Если окно найдено, то минимизируется, иначе восстанавливается
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

   // Открытие окон настроек
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

   // Быстрый доступ
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

   // Сообщение по умолчанию, ничего не происходит
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
 hDll                      = PluginData->hDll;
 PluginData->Caption       = "Winamp v1.12 (20.05.2004г.)\n\n© <Quash!>, г.Екатеринбург)";
 // Передача адресов функций
 PluginData->Init          = NULL;//Init;
 PluginData->Quit          = Quit;
 PluginData->Config        = Config;
 PluginData->Command       = Command;
 // Передача адресов в памяти списка и количества его элементов
 PluginData->CommandsCount = &CmdCount;
 PluginData->CommandsList  = CmdList;
}

#ifdef __cplusplus
}
#endif
