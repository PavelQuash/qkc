/**************************************************************/
/*              qshKeyCommander1 - CWircKeys.h                */
/*------------------------------------------------------------*/
/*    Заголовочный файл для класса хранения списка кнопок     */
/*    WinLIRC, описание структуры хранимых данных и класса    */
/*     Данный класс полностью идентичен CKbrdKeys, различие   */
/*    состоит только в том, что используются разные структуры */
/*****03.2005************************Copyright by <Quash!>*****/

#include "windows.h"
#include "Addiction.h"

#ifndef __WIRCKEYS
#define __WIRCKEYS

#define WIRC_KEYSIZE "WircSize"
#define WIRC_KEYDATA "WircData"

// Информация о кнопке WinLIRC
struct stWircKey
{
 char WircKey[80]; // Имя кнопки
 int  Repeats;     // Количество повторений
 int  CmdID;       // Идентификатор команды в модуле
};
    
// Класс списка кнопок WinLIRC
class CWircKeys
{
 stWircKey *KeysData; // Массив(список) кнопок, исползуемых в модуле
 int KeysCount;       // Число исползуемых клавиш в списке

 public:
  CWircKeys();  // Конструктор
  ~CWircKeys(); // Деструктор

  void AddKey(stWircKey *newwirckey);             // Добавляется новый элемент
  bool DelKey(int num);                           // Удаляется элемент с индексом num
  bool ChangeKey(int num, stWircKey *newwirckey); // Изменение данных элемента под номером num
  bool GetKey(int num, stWircKey *getwirckey);    // Возвращяет элемент c индексом num
  int  GetKeysCount(void);                        // Возвращает количество элементов в списке

  int  FindByKey(char wirckey[80]); // Возвращает номер первого элемента по коду клавиши
  int  FindByCmdID(int cmdid);      // Возвращает номер первого элемента по id команды

  void LoadKeys(LPTSTR inifilename, LPTSTR section, LPTSTR keysize, LPTSTR keydata); // Загрузка списка кнопок из ini файла
  void SaveKeys(LPTSTR inifilename, LPTSTR section, LPTSTR keysize, LPTSTR keydata); // Сохранение списка кнопок в ini файл
};

#endif