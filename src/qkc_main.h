/**************************************************************/
/*              qshKeyCommander1 - qkc_main.h                 */
/*------------------------------------------------------------*/
/*       Заголовочный файл для главной части программы        */
/*****2004***************************Copyright by <Quash!>*****/

#ifndef __QKC_MAIN
#define __QKC_MAIN

// *Определение макросов
#define program_version "v1.0 beta 10.03 (26.03.2005г.)" // Версия программы
#define program_www     "none"                           // Домашняя страничка программы
#define program_email   "mailto:qshDeveloper@mail.ru"    // Связь с автором

#define serverclassname "qshKeyCommander_1" // Имя класса окна-сервера (д.б. согласовано с qshKeyCommanderHooks.dll)
#define MAX_PASSWORDSYMBOLS 25              // Максимальное число символов в пароле (д.б. согласовано с qshKeyCommanderHooks.dll)

#define prg_help        "Help"                     // Каталог помощи
#define prg_languages   "Languages"                // Каталог языков
#define prg_plugins     "Plugins"                  // Каталог плагинов

#define hooks_filename  "qshKeyCommanderHooks.dll" // Файл отслеживания сообщений
#define ini_filename    "qshKeyCommander.ini"      // Имя файла инициализации
#define ini_mainsection "qshKeyCommander_1"        // Название секции хранения данных программы

#define lng_default ":Russian:" // Язык по умолчанию (строка отображается в списке выбора языков)

#define WM_MAINMENU   WM_USER+01 // Сообщение для показа/скрытия главного меню
#define WM_TRAYICON   WM_USER+02 // Сообщение обрабатываемое сервером от трэя

#define WM_KEYBOARDHOTKEYS WM_USER+10 // Сообщения для отслеживания клавиш клавиатуры (HotKeys)
#define WM_KEYBOARDHOOKS   WM_USER+11 // Сообщения для отслеживания клавиш клавиатуры (Hooks)
#define WM_MOUSE           WM_USER+12 // Сообщения для отслеживания сообщений от мыши
#define WM_GESTURES        WM_USER+13 // Сообщения для отслеживания жестов мыши
#define WM_WIRCKEYS        WM_USER+14 // Сообщение при нажатии кнопки WinLIRC
#define WM_TIMEEVENTS      WM_USER+15 // Сообщение при наступлении определённых временных событий

// Флаги доступа к главному меню (4 способа)
#define AF_TRAY     1
#define AF_HOTKEY   2
#define AF_PASSWORD 4
#define AF_DBLRUN   8

// Структура главного доступа к окну
struct stMainMenuAccess
{
 BYTE AccessFlags;                   // Флаги доступа
 WORD HotKey;                        // Комбинация клавиш
 char Password[MAX_PASSWORDSYMBOLS]; // Пароль (без защиты)
};

// Структура данных языка
struct stLanguage
{
 // ***Данные
 char DATA_Author[20];

 // ***Диалоги
 // Диалог - Главное меню
 char MAINMENU_Window_Caption[20];
 char MAINMENU_Button_Settings[20];
 char MAINMENU_Button_Help[20];
 char MAINMENU_Button_About[20];
 char MAINMENU_Button_Hide[20];
 char MAINMENU_Button_Quit[20];

 // Диалог - Справка
 char HELP_Window_Caption[20];
 char HELP_Button_Close[20];
 char HELP_Button_Open[20];

 // Диалог - О программе...
 char ABOUT_Window_Caption[20];
 char ABOUT_Text_Program[20];
 char ABOUT_Text_Version[20];
 char ABOUT_Text_Status[20];
 char ABOUT_Status[20];
 char ABOUT_Text_Www[20];
 char ABOUT_Text_Email[20];
 char ABOUT_Text_LngAuthor_Caption[20];
 char ABOUT_Button_Close[20];

 // Диалог - Настройки
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

 // Диалог - Настройки клавиш
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

 // Диалог - Настройки WinLIRC
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

 // Диалог - Подключение
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


 // Диалог - Конфигурации
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

 // ***Сообщения
};

// Программные настройки
struct stSettings
{
 stMainMenuAccess MainMenuAccess;
 BOOL KbrdOn;                 // Разрешение на отслеживание клавиш Клавиатуры
 BOOL MouseOn;                // Разрешение на отслеживание сообщений Мыши
 BOOL GesturesOn;             // Разрешение на отслеживание жестов Мыши
 BOOL WircOn;                 // Разрешение на отслеживание кнопок WinLIRC
 BOOL TimeEventsOn;           // Разрешение на отслеживание временных сообщений
 BOOL UseWinHooks;            // Использовать для отслеживания ловушки
 BOOL RegisterServiceProcess; // Скрытие процеса от Task Manager'а
 BOOL ChangeKeyboardState;    // Изменить раскладку клавиатуры
 BOOL ShowInTray;             // Показать в трее
 BOOL ClearOnAnalog;          // Спрашивать, если было совпадение

 char        Language[MAX_PATH]; // Имя файла языка интерфейса
 stLanguage *LanguageStrings;    // Строки для перевода интерфейса

 int  xMainMenuWindow;        // x положение главного меню
 int  yMainMenuWindow;        // y положение главного меню

 char WinLIRCaddress[15];     // Адрес сервера WinLIRC
 int  WinLIRCport;            // Порт сервера WinLIRC
};

// *Описание заголовков функций
void MonitoringBegin();
void MonitoringEnd();
bool WinLIRCconnect(LPTSTR address, int port, HWND hwndWnd);
void WinLIRCdisconnect();
bool WinLIRCgetstr(LPTSTR winlircstring, int num, LPTSTR buffer);

#endif