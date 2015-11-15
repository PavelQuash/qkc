/**************************************************************/
/*             qshKeyCommander1 - Settings.cpp                */
/*------------------------------------------------------------*/
/*  Загрузка/сохранение настроек программы, загрузка языков,  */
/*  обновление состояний программы в зависимости от настроек  */
/*****2004***************************Copyright by <Quash!>*****/

#include "Settings.h"

extern stSettings Settings;       // Описано в qkc_main.cpp
extern char prgdir[MAX_PATH];     // qkc_main.cpp
extern char lngdir[MAX_PATH];     // qkc_main.cpp
char   currentlanguage[MAX_PATH]; // Уже загруженный язык (память)

// Загрузить программные настройки
bool SettingsLoad()
{
 // Полный путь к файлу инициализации
 char inifn[MAX_PATH]; 
 wsprintf(inifn,"%s\\%s",prgdir,ini_filename);
 // Загрузка настроек
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
 // Если язык не по умолчанию, то проверяем на существование
 if (!(lstrcmp(Settings.Language,lng_default)==0))
  {
   char lngpath[MAX_PATH];
   wsprintf(lngpath,"%s\\%s",lngdir,Settings.Language);
   // Если файл не существует, то ставится язык по умолчанию, иначе ничего не делаем
   if (!FileExists(0,lngpath)) wsprintf(Settings.Language,"%s",lng_default);
  }
 GetPrivateProfileString(ini_mainsection,"WinLIRCaddress","127.0.0.1",Settings.WinLIRCaddress,sizeof(Settings.WinLIRCaddress),inifn);
 Settings.WinLIRCport = GetPrivateProfileInt(ini_mainsection,"WinLIRCport",8765,inifn);
 GetPrivateProfileStruct(ini_mainsection,"MainMenuAccess",&Settings.MainMenuAccess,sizeof(stMainMenuAccess),inifn);
 // Проверка на правильность MainMenuAccess, если что то не так, то возвращается false
 // debug
return true;
}

// Сохранить программные настройки
void SettingsSave()
{
 char str[10];         // Строка для временных данных
 // Полный путь к файлу инициализации
 char inifn[MAX_PATH]; 
 wsprintf(inifn,"%s\\%s",prgdir,ini_filename);
 // Сохранение настроек
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

// Загрузка нового языка (если он существует)
// true - уже установлен; false - ставим новый
bool SettingsLanguageLoad(LPTSTR newlng)
{
 // Если выбран тот же, что и в прошлый раз, то выход из функции
 if (lstrcmp(currentlanguage,newlng)==0) return true;

 if (lstrcmp(newlng,lng_default)==0) 
  {
   // Освобождение памяти, если есть какие-то данные
   if (Settings.LanguageStrings!=NULL)
    {
     delete Settings.LanguageStrings;
     Settings.LanguageStrings = NULL;
    }
  }
 else
  {
   // Выделение памяти для массива
   if (Settings.LanguageStrings!=NULL) delete Settings.LanguageStrings;
   Settings.LanguageStrings = new stLanguage;

   // Загрузка текущего языка
   lstrcpy(Settings.LanguageStrings->DATA_Author,"© <Quash!>");

   lstrcpy(Settings.LanguageStrings->MAINMENU_Window_Caption ,"Главное меню qKC");
   lstrcpy(Settings.LanguageStrings->MAINMENU_Button_Settings,"Настройка программы");
   lstrcpy(Settings.LanguageStrings->MAINMENU_Button_Help    ,"Помощь");
   lstrcpy(Settings.LanguageStrings->MAINMENU_Button_About   ,"Версия");
   lstrcpy(Settings.LanguageStrings->MAINMENU_Button_Hide    ,"Спрятать");
   lstrcpy(Settings.LanguageStrings->MAINMENU_Button_Quit    ,"Закончить работу");

   lstrcpy(Settings.LanguageStrings->HELP_Window_Caption,"Имеющиеся файлы помощи:");
   lstrcpy(Settings.LanguageStrings->HELP_Button_Close  ,"Закрыть диалог");
   lstrcpy(Settings.LanguageStrings->HELP_Button_Open   ,"Открыть файл");

   lstrcpy(Settings.LanguageStrings->ABOUT_Window_Caption        ,"Версия программы");
   lstrcpy(Settings.LanguageStrings->ABOUT_Text_Program          ,"Прога:");
   lstrcpy(Settings.LanguageStrings->ABOUT_Text_Version          ,"Версион:");
   lstrcpy(Settings.LanguageStrings->ABOUT_Text_Status           ,"Распр-ние:");
   lstrcpy(Settings.LanguageStrings->ABOUT_Status                ,"Бесплатная");
   lstrcpy(Settings.LanguageStrings->ABOUT_Text_Www              ,"Ввв:");
   lstrcpy(Settings.LanguageStrings->ABOUT_Text_Email            ,"Мыло:");
   lstrcpy(Settings.LanguageStrings->ABOUT_Text_LngAuthor_Caption,"Автор:");
   lstrcpy(Settings.LanguageStrings->ABOUT_Button_Close          ,"Okay");

   lstrcpy(Settings.LanguageStrings->SETTINGS_Window_Caption        ,"Конфигурирование программы");
   lstrcpy(Settings.LanguageStrings->SETTINGS_Text_Plugins_Caption  ,"Плагины:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_Button_Keyboard       ,"Клава");
   lstrcpy(Settings.LanguageStrings->SETTINGS_Button_Winlirc        ,"Пульт");
   lstrcpy(Settings.LanguageStrings->SETTINGS_Button_Pluginslist    ,"Подключить/отключить");
   lstrcpy(Settings.LanguageStrings->SETTINGS_Text_Configure_Caption,"Программа:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_Button_Configure      ,"Параметры");
   lstrcpy(Settings.LanguageStrings->SETTINGS_Button_Close          ,"Принять");
   lstrcpy(Settings.LanguageStrings->SETTINGS_Button_Help           ,"Справка");
   lstrcpy(Settings.LanguageStrings->SETTINGS_Text_Help             ,"YO!");

   lstrcpy(Settings.LanguageStrings->SETTINGS_KBRD_Window_Caption     ,"Настройка клавиатуры");
   lstrcpy(Settings.LanguageStrings->SETTINGS_KBRD_Button_KbrdOn      ,"Включить отслеживание");
   lstrcpy(Settings.LanguageStrings->SETTINGS_KBRD_Text_ActivePlugins ,"Активные:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_KBRD_Text_Commands      ,"Действия:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_KBRD_List_Command       ,"Что делать");
   lstrcpy(Settings.LanguageStrings->SETTINGS_KBRD_List_HotKey        ,"Нажать");
   lstrcpy(Settings.LanguageStrings->SETTINGS_KBRD_List_CNS           ,"ЦНС");
   lstrcpy(Settings.LanguageStrings->SETTINGS_KBRD_Button_ChangeKey   ,"Установить");
   lstrcpy(Settings.LanguageStrings->SETTINGS_KBRD_Button_Clear       ,"Удалить");
   lstrcpy(Settings.LanguageStrings->SETTINGS_KBRD_Button_Configure   ,"Настроить");
/*
   lstrcpy(Settings.LanguageStrings->SETTINGS_MOUSE_Window_Caption ,"Настройка сообщений от мыши:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_
   lstrcpy(Settings.LanguageStrings->SETTINGS_
   lstrcpy(Settings.LanguageStrings->SETTINGS_
   lstrcpy(Settings.LanguageStrings->SETTINGS_

   lstrcpy(Settings.LanguageStrings->SETTINGS_GESTURES_Window_Caption ,"Настройка жестов мыши:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_
   lstrcpy(Settings.LanguageStrings->SETTINGS_
   lstrcpy(Settings.LanguageStrings->SETTINGS_
*/
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Window_Caption     ,"Настройка пульта");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Button_WircOn      ,"Включить");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Text_ActivePlugins ,"Активные:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Text_Commands      ,"Команды:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_List_Command       ,"Что делать");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_List_Button        ,"Батон");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_List_Repeats       ,"#");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Text_Server        ,"Сервер");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Text_Port          ,"Порт");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Button_Connect     ,"Соединить");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Button_Disconnect  ,"Ансоединить");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Text_Connected     ,"Все ок - con");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Text_Disconnected  ,"Все ок - discon");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Text_ConnectFailed ,"Блин не получилось!");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Button_ChangeKey   ,"Установить");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Button_Clear       ,"Удалить");
   lstrcpy(Settings.LanguageStrings->SETTINGS_WIRC_Button_Configure   ,"Настроить");

   lstrcpy(Settings.LanguageStrings->SETTINGS_PLUGLIST_Window_Caption   ,"Подключение модулей");
   lstrcpy(Settings.LanguageStrings->SETTINGS_PLUGLIST_Text_PluginsPath ,"Путь:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_PLUGLIST_Text_FindPlugins ,"Найденные:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_PLUGLIST_Text_ActvPlugins ,"Активные:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_PLUGLIST_Button_AddOne    ,"Добавить 1");
   lstrcpy(Settings.LanguageStrings->SETTINGS_PLUGLIST_Button_DelOne    ,"Удалить 1");
   lstrcpy(Settings.LanguageStrings->SETTINGS_PLUGLIST_Button_AddAll    ,"Установить все");
   lstrcpy(Settings.LanguageStrings->SETTINGS_PLUGLIST_Button_DelAll    ,"Удалить все");
   lstrcpy(Settings.LanguageStrings->SETTINGS_PLUGLIST_Button_Accept    ,"Accept");
   lstrcpy(Settings.LanguageStrings->SETTINGS_PLUGLIST_Button_Update    ,"Update");

   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Window_Caption                ,"Параметры");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Button_UseWinHooks            ,"Использовать хуки");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Button_RegisterServiceProcess ,"Прятать");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Button_ChangeKeyboardState    ,"Изменить раскладку");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Button_ShowInTray             ,"В трэе");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Button_Analogs                ,"Удалять аналоги");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Text_InstallGroup             ,"Автозагрузка");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Button_Install                ,"Установить");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Button_Uninstall              ,"Убрать");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Text_LanguageGroup            ,"Говорилки:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Text_AccessGroup              ,"Доступ:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Access_Button_Tray            ,"По трэю");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Access_Button_HotKey          ,"Горячей клавише");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Access_Button_ChangeHotKey    ,"Поменять");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Access_Button_Password        ,"По паролю");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Access_Text_Password          ,"Код:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Access_Text_Confirm           ,"Снова код:");
   lstrcpy(Settings.LanguageStrings->SETTINGS_CONFIGURE_Access_Button_DblRun          ,"По второму запуску");
  }
 lstrcpy(currentlanguage,newlng); // Установка текущего языка
 return false;
}

// Реакция программы в зависимости от настроек
void SettingsUpdate()
{
 // Обновление загрузка нового языка, если уже установлен, то пропускается
 SettingsLanguageLoad(Settings.Language);
}
