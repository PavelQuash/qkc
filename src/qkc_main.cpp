/**************************************************************/
/*              qshKeyCommander1 - qkc_main.cpp               */
/*------------------------------------------------------------*/
/*  Запуск сервера, загрузка данных, отслеживание сообщений,  */
/*          вывод диалоговых окон, работа с треем             */
/*****03.2005************************Copyright by <Quash!>*****/


#include "windows.h"
#include "qkc_main.h"
#include "resource.h"
#include "CPlugins.h"
#include "DlgMainMenu.h"
#include "DlgSettings.h"
#include "Settings.h"
#include "CTray.h"



// ***Отключение Runtime Library***
// Уменьшение программы примерно на 23 килобайт
/*
#pragma comment(linker,"/MERGE:.rdata=.text")
#pragma comment(linker,"/FILEALIGN:512 /SECTION:.text,EWRX /IGNORE:4078")
//#pragma comment(linker,"/ENTRY:WinMain")
//#pragma comment(linker,"/NODEFAULTLIB")
*/



// *Глобальные переменные
HINSTANCE  hInst = GetModuleHandle(NULL); // Handle программы
HWND       hwndServerWindow,              // hwnd окна-сервера (главного)
           hwndMainMenu;                  // hwnd окна главного меню
char       prgdir[MAX_PATH],              // Каталог программы
           hlpdir[MAX_PATH],              // Каталог помощи
           lngdir[MAX_PATH],              // Каталог языков
           plgdir[MAX_PATH];              // Каталог плагинов
CPlugins   Plugins;                       // Класс работы с подключаемыми модулями
stSettings Settings;                      // Настройки программы

// Статус
SOCKET  hWinLIRCsocket = INVALID_SOCKET;  // Сокет, предназначенный для подключения к WinLIRC
char    sWinLIRCstring[MAX_PATH];         // Строка, возвращаемая WinLIRC
int     hotkeyscount = 0;                 // Количество горячих клавиш, зарегистрированных на окно hwndServerWindow
HMODULE hDllHooks = NULL;                 // Модуль для отслеживания клавиатурных сообщений
BOOL PasswordHooks;                       // Статус включения отслеживания пароля
BOOL KeyboardHooks;                       // Статус включения отслеживания клавиатуры
BOOL MouseHooks;                          // Статус включения отслеживания мышки
BOOL WinLIRCMessages;                     // Статус включения отслеживания WinLIRC

extern  HWND hwndDlgSettings; // Описана в DlgSettings.cpp
extern  HWND hwndDlgAbout;    // Описана в DlgAbout.cpp

// Функции в dll, отслеживающей сообщения клавиатуры
typedef void (*IH_Type)(HMODULE hdllinst, bool pswrdstart, char password[MAX_PASSWORDSYMBOLS], bool hotkeystart, HWND hwndSrvWnd);
typedef void (*QH_Type)(void);



// *Описание заголовков функций
LRESULT CALLBACK ServerProc(HWND hwndWnd, UINT message, WPARAM wParam, LPARAM lParam);



// Процедура входа в программу
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
 Settings.LanguageStrings = NULL;

 // DEBUG - проверка переменных на количество занимаемой памяти
 // char buffer[10];
 // itoa(sizeof(CGetKbrdKey),buffer,10);
 // stPluginData     = 292
 // stPluginKeys     = 16
 // stKbrdKey        = 8
 // stWircKey        = 88 // 80 символов + 2*4 байта int
 // CKbrdKeys        = 8
 // CWircKeys        = 8
 // CPlugins         = 532
 // stLanguage       = 1680
 // stMainMenuAccess = 30
 // stSettings       = 352
 // MessageBox(0,buffer,"",0);

 // Определение местонахождения программы
 GetModuleFileName(hInst,prgdir,sizeof(prgdir));
 for (int i=lstrlen(prgdir); i>=0; i--)
  if (prgdir[i]=='\\') // Избавление от имени файла
   {
    prgdir[i]='\0';
    break;
   }

 // Определение рабочих каталогов программы
 wsprintf(hlpdir,"%s\\%s",prgdir,prg_help);
 wsprintf(lngdir,"%s\\%s",prgdir,prg_languages);
 wsprintf(plgdir,"%s\\%s",prgdir,prg_plugins);
 // Проверка на существование каталогов
 // (если каталог не существует, то ставится текущий)
 if (!FileExists(1,hlpdir)) wsprintf(hlpdir,"%s",prgdir);
 if (!FileExists(1,lngdir)) wsprintf(lngdir,"%s",prgdir);
 if (!FileExists(1,plgdir)) wsprintf(plgdir,"%s",prgdir);
 
 // Выход из программы, если ini-файл не был найден
 char inifn[MAX_PATH];
 wsprintf(inifn,"%s\\%s",prgdir,ini_filename);
 if (FileExists(0,inifn)) 
  { if (!SettingsLoad()) return 1; } // Загрузка настроек программы, если данных нет, то выходим
 else
  {
   // Выход из программы
   MessageBox(0,"Файл инициализации не был найден!!!\nРабота программы будет прервана.","Ошибка",MB_ICONERROR|MB_OK);
   return 1;
  }

 // Выход из программы, если она уже загружена и доступ к главному
 // меню закрыт, иначе открывается главное меню и программа завершает работу
 hwndServerWindow = FindWindow(serverclassname,NULL);
	if (hwndServerWindow)
  {
   if (Settings.MainMenuAccess.AccessFlags&AF_DBLRUN)
    PostMessage(hwndServerWindow,WM_MAINMENU,0,0);
   MessageBeep(0xFFFFFFFF); // Подача сигнала
   return 1;
  }

 // Программа прячется от диспетчера задач
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

 // Меняется раскладка клавиатуры
 if (Settings.ChangeKeyboardState)
  {
   /* debug раскладка клавиатуры
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

 // Устанавление рабочих каталогов => загружаются все dll и их клавиши
 Plugins.SetPaths(inifn,plgdir);

 // Регистрируем класс главного окна
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
 // Создание главного окна
 hwndServerWindow = CreateWindow(wc.lpszClassName,"qkcServer",WS_BORDER|WS_CAPTION|WS_SYSMENU,10,10,140,1,NULL,NULL,hInst,NULL);

 SettingsUpdate(); // Обновление настроек
       
 // Установка иконки в системный трэй
 if (Settings.ShowInTray)
  {
   CTray systemtray;
   systemtray.AddTrayIcon(hwndServerWindow,1,NIF_ICON|NIF_MESSAGE|NIF_TIP,LoadIcon(hInst,MAKEINTRESOURCE(IDI_LOGO)),WM_TRAYICON,"qshKeyCommander");
  }

 MonitoringBegin(); // Включение остслеживания сообщений

 // Обработка сообщений процесса главному окну
 MSG message;
 while (GetMessage(&message,NULL,0,0)) 
  TranslateMessage(&message),DispatchMessage(&message);   
 return (message.wParam);
}

// Загрузка/выгрузка модуля для отслеживания сообщений
void AttachDllHooks(BOOL onof)
{
 if (onoff)
  {
   if (hDllHooks!=NULL) exit;
   MessageBox(0,"Подключение модуля",Settings.MainMenuAccess.Password,0); // DEBUG
   // Определение местонахождения файла ловушек (для отслеживания сообщений)
   char buffer[MAX_PATH];
   wsprintf(buffer,"%s\\%s",prgdir,hooks_filename);
   // Проверка файла на существование
   if (FileExists(0,buffer))
    {
	 // Файл существует, производится подключение
     hDllHooks = LoadLibrary(hooks_filename);
     if (hDllHooks)
      {
	   // Подключение успешно, необходимо включить доступ к главному окну по паролю
	   if (Settings.MainMenuAccess.AccessFlags&AF_PASSWORD)
		{
		 SETPASSWORDHOOK_Type SetPasswordHook = (SETPASSWORDHOOK_Type)GetProcAddress(hDllHooks,"SetPasswordHook");
         if (SetPasswordHook) // Получен адрес функции
		  {
           SetPasswordHook(true);
		   PasswordHook = true;
		  }
         else // Ошибка, адрес не был передан
		  MessageBox(0,"Неправильный формат dll! Функции инициализации не было найдено.",hooks_filename,MB_ICONERROR|MB_OK);
		}
	  }
	 else
	  {
	   // Подключения не произошло
	   MessageBox(0,"Модуль не может быть подключен!",hooks_filename,MB_ICONERROR|MB_OK);
	  }
    }
   else
    {
     // Сбрасывание флагов - файл dll не существует
     MessageBox(0,"Файл динамически загружаемой библиотеки не был найден! Возможно отслеживание только клавиатуры (по методу RegisterHotKey), жестов мыши и временных сообщений.",hooks_filename,MB_ICONERROR|MB_OK);
     if (Settings.MainMenuAccess.AccessFlags&AF_PASSWORD) Settings.MainMenuAccess.AccessFlags ^= AF_PASSWORD;
     Settings.UseWinHooks = false;
	 Settings.MouseOn = false;
    }
  }
 else // Выгрузка dll
  {
   if (hDllHooks==NULL) exit;
   // Выключение доступа к главному окну по паролю
   if (Settings.MainMenuAccess.AccessFlags&AF_PASSWORD)
	{
	 SETPASSWORDHOOK_Type SetPasswordHook = (SETPASSWORDHOOK_Type)GetProcAddress(hDllHooks,"SetPasswordHook");
     if (SetPasswordHook) // Получен адрес функции
	  {
       SetPasswordHook(false);
       PasswordHook = false;
	  }
   FreeLibrary(hDllHooks);      
   hDllHooks = NULL;
  }
}


// Включение/отключение отслеживания клавиатурных сообщений
void MessagesKeyboard(BOOL onoff)
{
 if (onoff) // Включение
  {
   if (KeyboardHooks) exit;
   // Подгружение модуля, если нужно отслеживать с ловушками
   if ((Settings.UseWinHooks)&&(Settings.KbrdOn)) AttachDllHooks(true);
   // Установка отслеживания клавиш, если отслеживание сообщений не ловушками
   hotkeyscount = 0;
   if (Settings.KbrdOn) // Горячие клавиши
	{
     if (!Settings.UseWinHooks)
	  {
       int  keyscount = 0; // Количество клавиш всего на отслеживание
       WORD *hotkeys;      // Список клавиш
       bool analog;
       stKbrdKey kbrdkey;
       int  i,k,l,num=0;
       // Подсчёт количества горячих клавиш (с повторением)
       for (l=0; l<Plugins.GetPluginsCount(); l++)
        keyscount += Plugins.PluginsKeys[l].KbrdKeys.GetKeysCount();
       // Выделение памяти для списка клавиш и его обнуление (повторений не будет)
       hotkeys = new WORD[keyscount];
       for (i=0; i<keyscount; i++) hotkeys[i] = 0;
       // Заполнение списка кодами клавиш, при повторении ставится "0"
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
       // Список сформирован, связывание горячих клавиш с сервером-окном
       analog = false;
       for (i=0; i<keyscount; i++)
		{
         if (RegisterHotKey(hwndServerWindow,hotkeyscount+1,LOBYTE(hotkeys[i]),HIBYTE(hotkeys[i]))) hotkeyscount++;
         else 
		  {
           char str[100];
           KeyToStr(hotkeys[i],str);
           MessageBox(0,"Установка горячей клавиши невозможна!",str,MB_ICONERROR|MB_OK);
           analog = true;
		  }
		}
       delete [] hotkeys;
       // Если в системе уже установлены некоторые комбинации клавиш, то выводится предупреждение пользователю
       if (analog) MessageBox(0,"Одна или больше \"горячих\" клавиш не могут быть установлены, т.к. в системе они уже установлены и используются другой программой!\nПереназначте перечисленные команды, чтобы не было конфликтов.","Ошибка",MB_ICONERROR|MB_OK);
	  }
     else // Ловушки
	  {
	   if (hDllHooks)
		{
		 SETKEYBOARDHOOK_Type SetKeyboardHook = (SETKEYBOARDHOOK_Type)GetProcAddress(hDllHooks,"SetKeyboardHook");
         if (SetKeyboardHook) // Получен адрес функции
		  {
           SetKeyboardHook(true);
		  }
         else // Ошибка, адрес не был передан
		  MessageBox(0,"Неправильный формат dll! Функции SetKeyboardHook не было найдено.",hooks_filename,MB_ICONERROR|MB_OK);
		}
	   // else Сообщение, что отслеживание клавиш не может быть включено
	  }
	 KeyboardHook = true;
	}
  }
 else       // Отключение
  {
   if (!KeyboardHooks) exit;
   KeyboardHook = false;
  }
}

// Включение/отключение отслеживания сообщений мыши
void MessagesMouse(BOOL onoff)
{
 if (onoff) // Включение
  {
   if (MouseHooks) exit;
   if (Settings.MouseOn) AttachDllHooks(true);
  }
 else       // Отключение
  {
   if (!MouseHooks) exit;
  }
}

// Включение/отключение отслеживания жестов мыши
void MessagesMouseGestures(BOOL onoff)
{
 if (onoff) // Включение
  {
  }
 else       // Отключение
  {
  }
}

// Включение/отключение отслеживания сообщений от ПДУ
void MessagesWinLIRC(BOOL onoff)
{
 if (onoff) // Включение
  {
   if (WinLIRC) exit;
   // Отслеживание сообщений от WinLIRC
   if (Settings.WircOn)
    if (!(WinLIRCconnect(Settings.WinLIRCaddress,Settings.WinLIRCport,hwndServerWindow))) 
	 {
	  // Подключиться не удалось
      char buffer[MAX_PATH];
      wsprintf(buffer,"%s : %i",Settings.WinLIRCaddress,Settings.WinLIRCport);
      MessageBox(0,"Не удалось подключиться к серверу WinLIRC. Возможно были неправильно введены адрес и порт. Если машина локальная, то обычно адрес 127.0.0.1, а порт по умолчанию 8765! Проверьте правильность введённых данных и снова включите отслеживание.",buffer,MB_ICONERROR|MB_OK);
      Settings.WircOn = false;
	 }
	else WinLIRC = true; // Подключение успешно
  }
 else       // Отключение
  {
   if (!WinLIRC) exit;
   WinLIRCdisconnect();
   WinLIRC = false;
  }
}

/*
void MessagesTimeEvents(BOOL onoff)
{
 if (onoff) // Включение
  {
  }
 else       // Отключение
  {
  }
}
*/

// Включение режима отслеживания
void MonitoringBegin()
{
 // Включение отслеживания осуществляется следующими этапами:
 // 1) При подключении внешней библиотеки, если стоит отслеживание по паролю, то выполняется SetPasswordHook
 // 2) Включение всех сообщений (по заданным настройкам)
 //    Если это необходимо, то подгружается внешняя библиотека
 //    Она подключается в случаях:
 //    1. Отслеживание сообщений от клавиатуры по WinHooks (SetKeyboardHook) - отслеживание включено
 //    2. Сообщения от мышки (SetMouseHook)


 // Выставление отслеживания по умолчанию, далее, если включаются, то ставятся в true
 PasswordHooks   = false;
 KeyboardHooks   = false;
 MouseHooks      = false;
 WinLIRCMessages = false;
 //TimeEvents      = false;

 // Если пароль пустой, а выставлен доступ к главному окну по паролю, то сбрасывается флаг AF_PASSWORD
 if (Settings.MainMenuAccess.AccessFlags&AF_PASSWORD)
  if (lstrlen(Settings.MainMenuAccess.Password)<=0) 
   {
    MessageBox(0,"Доступ к главному окну по паролю не может быть включен, так как он не был установлен!","Ошибка",MB_ICONERROR|MB_OK);
    if (Settings.MainMenuAccess.AccessFlags&AF_PASSWORD) Settings.MainMenuAccess.AccessFlags ^= AF_PASSWORD;
   }

 // *Включение доступа главному окну - доступ по комбинации горячих клавиш
 if (Settings.MainMenuAccess.AccessFlags&AF_HOTKEY)
  {
   if (!RegisterHotKey(hwndServerWindow,0,LOBYTE(Settings.MainMenuAccess.HotKey),HIBYTE(Settings.MainMenuAccess.HotKey)))
    {
     // Клавиша уже была зарегистрирована в системе
     char buffer[MAX_PATH];
     KeyToStr(Settings.MainMenuAccess.HotKey,buffer);
     if (Settings.MainMenuAccess.AccessFlags&AF_HOTKEY) Settings.MainMenuAccess.AccessFlags ^= AF_HOTKEY;
     MessageBox(0,"Отслеживание данного сочетания клавиш для доступа к главному меню программы невозможно, так как в системе уже зарегистрировано аналогичное сочетание.\nРекомендуется изменить комбинацию клавиш на другое сочетание, чтобы оно не конфликтовало с другими.",buffer,MB_ICONERROR|MB_OK);
    }
  }

 // *Включение всех сообщений
 MessagesKeyboard(true);
// MessagesMouse(true);
// MessagesMouseGestures(true);
 MessagesWinLIRC(true);
//MessagesTimeEvents(true);

 // *Инициализация модулей
 stPluginData plgdata;
 for (int i=0; i<Plugins.GetPluginsCount(); i++)
  {
   Plugins.GetPluginData(i,&plgdata);
   if (plgdata.Init!=NULL) plgdata.Init();
  }
}

// Выключение режима отслеживания
void MonitoringEnd()
{
 /*// *Деинициализация библиотеки
 stPluginData plgdata;
 for (int i=0; i<Plugins.GetPluginsCount(); i++)
  {
   Plugins.GetPluginData(i,&plgdata);
   if (plgdata.Quit!=NULL) plgdata.Quit();
  }
*/
 // *Удаление доступа главному окну
 // Удаление доступа от комбинации горячих клавиш
 if (Settings.MainMenuAccess.AccessFlags&AF_HOTKEY)
  UnregisterHotKey(hwndServerWindow,0);
/*
 // *Удаление отслеживания сообщений от Клавиатуры
 if (Settings.KbrdOn)
  if (!Settings.UseWinHooks)
   {
    for (int i=0; i<hotkeyscount; i++)
     UnregisterHotKey(hwndServerWindow,i+1);
   }

 // Если была загружена dll, то выгружается
 if ((Settings.MainMenuAccess.AccessFlags&AF_PASSWORD)||((Settings.UseWinHooks)&&(Settings.KbrdOn)))
  {
   QH_Type QuitHooks = (QH_Type)GetProcAddress(hDllHooks,"QuitHooks");
   if (QuitHooks) QuitHooks();
   FreeLibrary(hDllHooks);
  }

 // *Удаление отслеживания сообщений от WinLIRC
 if (Settings.WircOn) WinLIRCdisconnect();
 */
}

// Обработка сообщений, поступающих главному окну
LRESULT CALLBACK ServerProc(HWND hwndWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 int res=0; // По умолчанию
 switch (message)
  {
   case WM_HOTKEY: // Реакция на нажатие комбинации горячих клавиш
    {
     // Была нажата комбинация клавиш для доступа к главному меню
     if ((int)wParam==0) 
      {
       SendMessage(hwndServerWindow,WM_MAINMENU,0,0);
       break;
      }
     // Если отслеживание не включено, то игнорируется нажатие
     if (!Settings.KbrdOn) break;
     // Выполнение команд, которым соответствуют горячие клавиши
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
         // Клавиша найдена
         if (hotkey==kbrdkey.KbrdKey)
          {
           // В зависимости от флагов выполняется команда
           act_cl = false;
           act_nl = false;
           act_sl = false;
           if (kbrdkey.CNS!=0)
            {
             BYTE cnslobyte = LOBYTE(kbrdkey.CNS);
             BYTE cnshibyte = HIBYTE(kbrdkey.CNS);
             // Проверка состояний cns
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
    // Если отслеживание не включено, то игнорируется нажатие (на всякий случай)
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
          // Если кнопка была найдена, то проверяем на количество повторов
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

   case WM_MAINMENU: // Показать главное меню
    // Если окно уже существует, то оно уничтожается
    if (IsWindow(hwndMainMenu)) DestroyWindow(hwndMainMenu);
    // Если ещё не существует, то создаётся
    else DialogBox(hInst,MAKEINTRESOURCE(IDD_MAINMENU),0,(DLGPROC)DlgMainMenuProc);
    break;

   case WM_TRAYICON: // Реакция на сообщения от иконки в системном трэе
    switch (lParam)
     {
	  // Реакция на левую клавишу мыши
      case WM_LBUTTONDOWN:
       if (!IsWindow(hwndDlgSettings))
        if (Settings.MainMenuAccess.AccessFlags&AF_TRAY)
         SendMessage(hwndServerWindow,WM_MAINMENU,0,0);
       break;
	  // Реакция на правую клавишу мыши
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

   case WM_QUERYENDSESSION: // Реакция на завершение работы с Windows
    DestroyWindow(hwndWnd);
   break;

   case WM_CLOSE: // Реакция на закрытие окна
    DestroyWindow(hwndWnd);
   break;

   case WM_DESTROY: // Реакция на уничтожение окна
    if (Settings.LanguageStrings!=NULL) delete Settings.LanguageStrings; // Освобождение памяти, если был подключен язык
    CTray systemtray;
    if (Settings.ShowInTray) systemtray.DeleteTrayIcon(hwndServerWindow,1);
    MonitoringEnd();    // Выключение отслеживания сообщенний
    SettingsSave();     // Сохранение настроек программы
    PostQuitMessage(0); // Выход из программы
   break;

   // Сообщение обрабатываемое по умолчанию
   default: 
    res = DefWindowProc(hwndWnd,message,wParam,lParam); 
  }
 return res;
}

// Функция, соединяющаяся с WinLIRC сервером и связывающая его с окном
bool WinLIRCconnect(LPTSTR address, int port, HWND hwndWnd)
{
 int result;
 SOCKADDR_IN addr; // Хранение адреса (для передаче при соединении)

 WSADATA stWSAData; // Получение данных о драйвере - инициализация
 WSAStartup(0x0101, &stWSAData);

 hWinLIRCsocket = socket(AF_INET, SOCK_STREAM, 0); // Определение сокета
 // Заполнение структуры адреса
 addr.sin_family      = AF_INET;            // Тип соединения
 addr.sin_addr.s_addr = inet_addr(address); // Адрес
 addr.sin_port        = htons(port);        // Порт

 // Попытка соединиться...
 result = connect(hWinLIRCsocket,(struct sockaddr *)&addr,sizeof(addr));
 // Если соединиться не удалось, то false
 if (result) 
  {
   hWinLIRCsocket = INVALID_SOCKET;
   return false;
  }
 // При удаче сообщения связываются с окном (для чтения)
 WSAAsyncSelect(hWinLIRCsocket,hwndWnd,WM_WIRCKEYS,FD_READ);   
 return true;
}

// Отключение от сервера WinLIRC
void WinLIRCdisconnect()
{
 if (hWinLIRCsocket!=INVALID_SOCKET)
  {
   WSACleanup();
   hWinLIRCsocket = INVALID_SOCKET;
  }
}

// Извлечение поля под номером num из строки WinLIRC
bool WinLIRCgetstr(LPTSTR winlircstring, int num, LPTSTR buffer)
{
 int i,
     k = 1, l = 0; // Начальные значение

 for (i=0; i<lstrlen(winlircstring); i++)
  {
   // Если нужное поле достигнуто
    if (k==num)
    {
     buffer[i-l]   = winlircstring[i];
     buffer[i-l+1] = '\0';
    }
   // Если нужное поле было проидено, то возвращаем значение
   else if (k>num) return true;
   // Если найден переход на другое поле, то происходит сохранение позиции
   if (winlircstring[i+1]==' ') { k++; l=i+2; }
  }
 buffer[0]='\0';
 return false;
}
