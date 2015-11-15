/**************************************************************/
/*                qshKeyCommander1 - CPlugins.h               */
/*------------------------------------------------------------*/
/*         Заголовочный файл для класса хранения данных       */
/*             о подключенных плагинах и работа с ними        */
/*****2004***************************Copyright by <Quash!>*****/

#include "windows.h"
#include "CKbrdKeys.h"
#include "CWircKeys.h"
#include "Addiction.h"

#ifndef __CPLUGINS
#define __CPLUGINS

// Структура данных модуля
struct stPluginData
{                             // КОММЕНТАРИЙ              (ЗАПОЛНЕНИЕ)
 char FileName[MAX_PATH];     // Имя файла модуля         (программой)
 HMODULE hDll;                // Хэндл модуля             (программой)
 char *Caption;               // Заголовок dll, версия    (модулем)
 void (*Init)   (void);       // Инициализация            (модулем)
 void (*Quit)   (void);       // Завершение работы        (модулем)
 void (*Config) (void);       // Настройка модуля         (модулем)
 void (*Command)(int cmdid);  // Выполнение команды       (модулем)
 char (*CommandsList) [200];  // Список команд модуля     (модулем)
 int  *CommandsCount;         // Количество команд        (модулем)
};

// Тип функции модуля - GetModuleData - получение данных
typedef void (*GMD_Type)(stPluginData*);

// Структура для хранения клавиш, которые модуль может обрабатывать
struct stPluginKeys
{
 CKbrdKeys KbrdKeys;         // Список клавиш клавиатуры
 CWircKeys WircKeys;         // Список кнопок винлирк
};

// Класс работы с модулями
class CPlugins
{
 stPluginData *PluginsData;  // Информация по подключённым модулям в списке
 int PluginsCount;           // Количество элементов в списке

 char inifilename[MAX_PATH]; // Ini-файл
 char pluginsdir[MAX_PATH];  // Каталог плагинов

 public:
  stPluginKeys *PluginsKeys; // Обрабатываемые клавиши для каждого обрабатываемого модуля

  CPlugins();  // Конструктор
  ~CPlugins(); // Деструктор

  bool SetPaths(LPTSTR inifilename, LPTSTR pluginsdir); // Установка ini-файла и каталога плагинов

  bool AddPlugin(LPTSTR filename);                          // Подключение модуля
  bool DelPlugin(int num);                                  // Отключение модуля
  bool GetPluginData(int num, stPluginData *getplugindata); // Данные num плагина
  int  GetPluginsCount(void);                               // Возврашение количества подключенных модулей

  int  FindByFileName(LPTSTR filename); // Найти модуль по имени файла
  int  FindByHandle(HINSTANCE hdll);    // Найти модуль по хэндлу модуля

  bool LoadPlugList(LPTSTR section, LPTSTR keysize, LPTSTR keydata); // Загрузить список подключенных модулей
  bool SavePlugList(LPTSTR section, LPTSTR keysize, LPTSTR keydata); // Сохранить список подключенных модулей
};

#endif