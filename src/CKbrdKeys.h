/**************************************************************/
/*              qshKeyCommander1 - CKbrdKeys.h                */
/*------------------------------------------------------------*/
/*    Заголовочный файл для класса хранения списка клавиш     */
/*  клавиатуры, описание структуры хранимых данных и класса   */
/*****03.2005************************Copyright by <Quash!>*****/

#include "windows.h"
#include "Addiction.h"

#ifndef __KBRDKEYS
#define __KBRDKEYS

#define KBRD_KEYSIZE "KbrdSize"
#define KBRD_KEYDATA "KbrdData"

#define CNS_CAPSLOCK   1
#define CNS_NUMLOCK    2
#define CNS_SCROLLLOCK 4

// Информация о клавише клавиатуры
struct stKbrdKey
{
 WORD KbrdKey; // L:модификатор H:виртуальный код
 WORD CNS;     // L:вкл./выкл. CNS  H:статус клавиш CNS
 int  CmdID;   // Идентификатор команды в модуле
};

// Класс списка клавиш клавиатуры
class CKbrdKeys
{
 stKbrdKey *KeysData; // Массив(список) клавиш, исползуемых в модуле
 int KeysCount;       // Число исползуемых клавиш в списке

 public:
  CKbrdKeys();  // Конструктор
  ~CKbrdKeys(); // Деструктор

  void AddKey(stKbrdKey *newkbrdkey);             // Добавляется новый элемент
  bool DelKey(int num);                           // Удаляется элемент с индексом num
  bool ChangeKey(int num, stKbrdKey *newkbrdkey); // Изменение данных элемента под номером num
  bool GetKey(int num, stKbrdKey *getkbrdkey);    // Возвращяет элемент c индексом num
  int  GetKeysCount(void);                        // Возвращает количество элементов в списке

  int  FindByKey(WORD kbrdkey); // Возвращает номер первого элемента по коду клавиши
  int  FindByCmdID(int cmdid);  // Возвращает номер первого элемента по id команды

  void LoadKeys(LPTSTR inifilename, LPTSTR section, LPTSTR keysize, LPTSTR keydata); // Загрузка списка клавиш из ini файла
  void SaveKeys(LPTSTR inifilename, LPTSTR section, LPTSTR keysize, LPTSTR keydata); // Сохранение списка клавиш в ini файл
};

#endif